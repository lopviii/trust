#include "djui.h"
#include "djui_panel.h"
#include "djui_panel_main.h"
#include "djui_panel_language.h"
#include "djui_panel_pause.h"
#include "djui_panel_join.h"
#include "djui_panel_join_message.h"
#include "djui_console.h"
#include "djui_fps_display.h"
#include "../debuglog.h"
#include "pc/cliopts.h"
#include "game/level_update.h"
#include "pc/lua/smlua_hooks.h"
#include "djui_panel_playerlist.h"
#include "djui_hud_utils.h"
#include "engine/math_util.h"
#include "pc/utils/misc.h"

static Gfx* sSavedDisplayListHead = NULL;

struct DjuiRoot* gDjuiRoot = NULL;
static struct DjuiText* sDjuiPauseOptions = NULL;
static struct DjuiText* sDjuiLuaError = NULL;
static u32 sDjuiLuaErrorTimeout = 0;
bool gDjuiInMainMenu = true;
bool gDjuiDisabled = false;
static bool sDjuiInited = false;

bool sDjuiRendered60fps = false;

void reset_djui_text(void);

void reset_djui(void) {
    sSavedDisplayListHead = NULL;
    sDjuiPauseOptions = NULL;
    sDjuiLuaError = NULL;
    sDjuiLuaErrorTimeout = 0;
    if (gDjuiRoot) djui_base_destroy(&gDjuiRoot->base);

    if (gDjuiConsole) djui_base_destroy(&gDjuiConsole->panel->base);
    extern u32 sDjuiConsoleMessages;
    sDjuiConsoleMessages = 0;

    sDjuiInited = false;
}

void patch_djui_before(void) {
    sDjuiRendered60fps = false;
}

void patch_djui_interpolated(UNUSED f32 delta) {
    // reset the head and re-render DJUI
    if (delta >= 0.5f && !sDjuiRendered60fps && (gDjuiInMainMenu || gDjuiPanelPauseCreated)) {
        sDjuiRendered60fps = true;
        if (sSavedDisplayListHead == NULL) { return; }
        gDisplayListHead = sSavedDisplayListHead;
        djui_render();
        gDPFullSync(gDisplayListHead++);
        gSPEndDisplayList(gDisplayListHead++);
    }
}

void djui_init(void) {
    gDjuiRoot = djui_root_create();

    sDjuiPauseOptions = djui_text_create(&gDjuiRoot->base, "R Button - Options");
    djui_base_set_size_type(&sDjuiPauseOptions->base, DJUI_SVT_RELATIVE, DJUI_SVT_ABSOLUTE);
    djui_base_set_size(&sDjuiPauseOptions->base, 1.0f, 32);
    djui_base_set_location(&sDjuiPauseOptions->base, 0, 16);
    djui_text_set_drop_shadow(sDjuiPauseOptions, 0, 0, 0, 255);
    djui_text_set_alignment(sDjuiPauseOptions, DJUI_HALIGN_CENTER, DJUI_VALIGN_CENTER);
    djui_base_set_visible(&sDjuiPauseOptions->base, false);

    sDjuiLuaError = djui_text_create(&gDjuiRoot->base, "");
    djui_base_set_size_type(&sDjuiLuaError->base, DJUI_SVT_RELATIVE, DJUI_SVT_ABSOLUTE);
    djui_base_set_size(&sDjuiLuaError->base, 1.0f, 32);
    djui_base_set_location(&sDjuiLuaError->base, 0, 64);
    djui_text_set_drop_shadow(sDjuiLuaError, 0, 0, 0, 255);
    djui_text_set_alignment(sDjuiLuaError, DJUI_HALIGN_CENTER, DJUI_VALIGN_CENTER);
    djui_base_set_visible(&sDjuiLuaError->base, false);
    djui_base_set_color(&sDjuiLuaError->base, 255, 0, 0, 255);

    djui_panel_playerlist_create(NULL);

    djui_console_create();

    djui_fps_display_create();

    sDjuiInited = true;
}

void djui_init_late(void) {
    if (gCLIOpts.Network != NT_SERVER) {
        djui_panel_main_create(NULL);
        if (configLanguage[0] == '\0') {
            gPanelLanguageOnStartup = true;
            djui_panel_language_create(NULL);
        }
        //djui_panel_debug_create();
    }
    djui_cursor_create();
}

void djui_connect_menu_open(void) {
    djui_panel_shutdown();
    gDjuiInMainMenu = true;
    djui_panel_main_create(NULL);
    djui_panel_join_create(NULL);
    djui_panel_join_message_create(NULL);
}

void djui_lua_error(char* text) {
    if (!sDjuiLuaError) { return; }
    djui_text_set_text(sDjuiLuaError, text);
    djui_base_set_visible(&sDjuiLuaError->base, true);
    sDjuiLuaErrorTimeout = 30 * 5;
}

void djui_reset_hud_params(void) {
    djui_hud_set_resolution(RESOLUTION_DJUI);
    djui_hud_set_font(FONT_NORMAL);
    djui_hud_set_rotation(0, 0, 0);
    djui_hud_reset_color();
}

void djui_render(void) {
    if (!sDjuiInited || gDjuiDisabled) { return; }
    djui_reset_hud_params();

    sSavedDisplayListHead = gDisplayListHead;
    gDjuiHudUtilsZ = 0;

    create_dl_ortho_matrix();
    djui_gfx_displaylist_begin();

    smlua_call_event_on_hud_render(djui_reset_hud_params);

    djui_panel_update();
    djui_popup_update();

    djui_base_set_visible(&sDjuiPauseOptions->base, (sCurrPlayMode == PLAY_MODE_PAUSED));
    djui_base_set_visible(&sDjuiPauseOptions->base, (sCurrPlayMode == PLAY_MODE_PAUSED) && !gDjuiPanelPauseCreated);
    if (gDjuiRoot != NULL) {
        djui_base_render(&gDjuiRoot->base);
    }

    djui_fps_display_render();

    if (sDjuiLuaErrorTimeout > 0) {
        sDjuiLuaErrorTimeout--;
        if (sDjuiLuaErrorTimeout == 0) {
            djui_base_set_visible(&sDjuiLuaError->base, false);
        }
    }

    djui_cursor_update();
    djui_interactable_update();
    djui_gfx_displaylist_end();
}
