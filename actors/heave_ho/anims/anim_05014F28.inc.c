// 0x05014DD0
static const s16 heave_ho_seg5_animvalue_05014DD0[] = {
    0x0000, 0x0030, 0xFF92, 0xC001, 0x3FFF, 0xC001, 0x0000, 0xF5C3,
    0xEB86, 0xE148, 0xD70B, 0xCCCE, 0xC290, 0xB853, 0xAE16, 0xA3D8,
    0x999B, 0x8F5E, 0x8520, 0x7AE0, 0x70A2, 0x6665, 0x5C28, 0x51EA,
    0x47AD, 0x3D70, 0x3332, 0x28F5, 0x1EB8, 0x147A, 0x0A3D, 0x2170,
    0x8001, 0x8001, 0x0000, 0xF5C3, 0xEB86, 0xE148, 0xD70B, 0xCCCE,
    0xC290, 0xB853, 0xAE16, 0xA3D8, 0x999B, 0x8F5E, 0x8520, 0x7AE0,
    0x70A2, 0x6665, 0x5C28, 0x51EA, 0x47AD, 0x3D70, 0x3332, 0x28F5,
    0x1EB8, 0x147A, 0x0A3D, 0x2170, 0x8001, 0x8001, 0x8001, 0x8001,
    0x3FFF, 0xC001, 0x3FFF, 0x8001, 0x8001, 0x8001,
};

// 0x05014E5C
static const u16 heave_ho_seg5_animindex_05014E5C[] = {
    0x0001, 0x0000, 0x0001, 0x0001, 0x0001, 0x0002, 0x0001, 0x0003, 0x0001, 0x0004, 0x0001, 0x0005,
    0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x0045,
    0x0001, 0x0043, 0x0001, 0x0044, 0x0001, 0x0000,
    0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000,
    0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000,
    0x0019, 0x0022, 0x0001, 0x0000, 0x0001, 0x003B,
    0x0019, 0x0006, 0x0001, 0x0000, 0x0001, 0x001F,
    0x0001, 0x0040, 0x0001, 0x0041, 0x0001, 0x0042,
    0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000,
    0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000,
    0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x0000,
    0x0001, 0x003E, 0x0001, 0x003F, 0x0001, 0x0000,
    0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x003D,
    0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x003C,
    0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x0020,
    0x0001, 0x0000, 0x0001, 0x0000, 0x0001, 0x0021,
};

// 0x05014F28
static const struct Animation heave_ho_seg5_anim_05014F28 = {
    0,
    0,
    0,
    0,
    0x18,
    ANIMINDEX_NUMPARTS(heave_ho_seg5_animindex_05014E5C),
    heave_ho_seg5_animvalue_05014DD0,
    heave_ho_seg5_animindex_05014E5C,
    0,
    ANIM_FIELD_LENGTH(heave_ho_seg5_animvalue_05014DD0),
    ANIM_FIELD_LENGTH(heave_ho_seg5_animindex_05014E5C),
};
