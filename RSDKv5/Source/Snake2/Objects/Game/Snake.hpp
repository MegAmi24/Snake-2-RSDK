#pragma once

#include "Snake2.hpp"

#define PLAYER_SCALE    (0x20)
#define SEGMENT_SPACING (4)
#define IGNORE_SEGMENTS (7)
#define SEGMENTS_START  (0x100)
#define MOMENTUM        (0x1000)

#define DPAD_X (56)
#define DPAD_Y (184)

namespace GameLogic
{

struct Snake : RSDK::GameObject::Entity {

    // ==============================
    // ENUMS
    // ==============================

    // ==============================
    // STRUCTS
    // ==============================

    // ==============================
    // STATIC VARS
    // ==============================

    struct Static : RSDK::GameObject::Static {
        RSDK::SpriteAnimation fieriFrames;
        RSDK::Hitbox hitbox;
        RSDK::SoundFX sfxLevelUp;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    int32 score;
    int32 level;
    int32 xp;
    int32 xpNeeded;
    int32 length;
    int32 size;
    int32 speed;
    RSDK::Vector2 momentum;
    RSDK::Vector2 prevPositions[SEGMENT_SPACING];
    int32 segmentID;
    RSDK::Animator animator;
    bool32 up;
    bool32 down;
    bool32 left;
    bool32 right;

    // ==============================
    // EVENTS
    // ==============================

    void Create(void *data);
    void Draw();
    void Update();
    void LateUpdate();

    static void StaticUpdate();
    static void StageLoad();
#if RETRO_REV0U
    static void StaticLoad(Static *sVars);
#endif
    static void Serialize();

#if GAME_INCLUDE_EDITOR
    static void EditorLoad();
    void EditorDraw();
#endif

    // ==============================
    // FUNCTIONS
    // ==============================

    void UpdatePosition(void);
    static void EatFood(Snake *snake);
    static void GameOver(void);

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(Snake);
};
} // namespace GameLogic
