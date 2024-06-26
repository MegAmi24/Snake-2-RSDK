#pragma once

#include "Snake2.hpp"

#define IsLore() (sceneInfo->listPos == 0)

namespace GameLogic
{

struct TextSetup : RSDK::GameObject::Entity {

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
        RSDK::SoundFX sfxSnake;
        int32 stringsSize;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    int32 timer;

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

    static void LoadTextStrings(void);
    static bool32 VideoSkipCB(void);

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(TextSetup);
};
} // namespace GameLogic
