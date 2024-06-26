#pragma once

#include "Snake2.hpp"

#define STRING_SPACING (16)
#define SFX_COUNT      5

namespace GameLogic
{

struct Microtransactions : RSDK::GameObject::Entity {

    // ==============================
    // ENUMS
    // ==============================

    enum Strings {
        STR_MICROTRANSACTIONS,
        STR_SUPERSPEED,
        STR_EXTRAXP,
        STR_MEGAGROWTH,
        STR_BONUSLIFE,
        STR_GLOB,
        STR_COUNT,
    };

    // ==============================
    // STRUCTS
    // ==============================

    // ==============================
    // STATIC VARS
    // ==============================

    struct Static : RSDK::GameObject::Static {
        RSDK::SpriteAnimation aniFrames;
        RSDK::String stringList[STR_COUNT];
        RSDK::String microString;
        RSDK::String speedString;
        RSDK::String xpString;
        RSDK::String growthString;
        RSDK::String lifeString;
        RSDK::String globString;
        RSDK::SoundFX sfxItems[SFX_COUNT];
        uint16 maxLength;
    };

    // ==============================
    // INSTANCE VARS
    // ==============================
    RSDK::Animator animator;

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

    static void SetupString(RSDK::String *string, uint8 id);

    // ==============================
    // DECLARATION
    // ==============================

    RSDK_DECLARE(Microtransactions);
};
} // namespace GameLogic
