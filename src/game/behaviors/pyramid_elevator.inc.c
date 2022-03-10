/**
 * Behaviors for bhvPyramidElevator and bhvPyramidElevatorTrajectoryMarkerBall.
 *
 * This controls the elevator that descends Shifting Sand Land's pyramid, as
 * well as the small marker balls that demarcate its trajactory.
 */

u8 bhv_pyramid_elevator_ignore_if_true(void) {
    return (o->oAction != PYRAMID_ELEVATOR_IDLE);
}

/**
 * Generate the ten trajectory marker balls that indicate where the elevator
 * moves.
 */
void bhv_pyramid_elevator_init(void) {
    s32 i;
    struct Object *ball;

    for (i = 0; i < 10; i++) {
        ball = spawn_object(o, MODEL_TRAJECTORY_MARKER_BALL, bhvPyramidElevatorTrajectoryMarkerBall);
        if (ball == NULL) { continue; }
        ball->oPosY = 4600 - i * 460;
    }

    struct SyncObject* so = network_init_object(o, SYNC_DISTANCE_ONLY_EVENTS);
    if (so) {
        so->ignore_if_true = bhv_pyramid_elevator_ignore_if_true;
        network_init_object_field(o, &o->oAction);
        network_init_object_field(o, &o->oPrevAction);
        network_init_object_field(o, &o->oTimer);
        network_init_object_field(o, &o->oPosY);
    }
}

void bhv_pyramid_elevator_loop(void) {
    switch (o->oAction) {
        /**
         * Do not move until Mario stands on the elevator. When he does,
         * transition to the starting state.
         */
        case PYRAMID_ELEVATOR_IDLE:
            if (cur_obj_is_any_player_on_platform()) {
                o->oAction = PYRAMID_ELEVATOR_START_MOVING;
                network_send_object(o);
            }
            break;

        /**
         * Use a sine wave to start the elevator's movement with a small jolt.
         * After a certain amount of time, transition to a constant-velocity state.
         */
        case PYRAMID_ELEVATOR_START_MOVING:
            o->oPosY = o->oHomeY - sins(o->oTimer * 0x1000) * 10.0f;
            if (o->oTimer == 8)
                o->oAction = PYRAMID_ELEVATOR_CONSTANT_VELOCITY;
            break;

        /**
         * Move downwards with constant velocity. Once at the bottom of the
         * track, transition to the final state.
         */
        case PYRAMID_ELEVATOR_CONSTANT_VELOCITY:
            o->oVelY = -10.0f;
            o->oPosY += o->oVelY;
            if (o->oPosY < 128.0f) {
                o->oPosY = 128.0f;
                o->oAction = PYRAMID_ELEVATOR_AT_BOTTOM;
            }
            break;

        /**
         * Use a sine wave to stop the elevator's movement with a small jolt.
         * Then, remain at the bottom of the track.
         */
        case PYRAMID_ELEVATOR_AT_BOTTOM:
            o->oPosY = sins(o->oTimer * 0x1000) * 10.0f + 128.0f;
            if (o->oTimer >= 8) {
                o->oVelY = 0;
                o->oPosY = 128.0f;
            }
            break;
    }
}

/**
 * Deactivate the trajectory marker balls if the elevator is not moving.
 * Otherwise, set their scale.
 */
void bhv_pyramid_elevator_trajectory_marker_ball_loop(void) {
    struct Object *elevator;

    cur_obj_scale(0.15f);
    elevator = cur_obj_nearest_object_with_behavior(bhvPyramidElevator);

    if (elevator != NULL) {
        if (elevator->oAction != PYRAMID_ELEVATOR_IDLE) {
            o->activeFlags = ACTIVE_FLAG_DEACTIVATED;
        }
    }
}
