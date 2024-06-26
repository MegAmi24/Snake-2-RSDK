// ---------------------------------------------------------------------
// RSDK Project: Snake 2
// Object Description: Microtransactions Object
// Object Author: MegAmi
// ---------------------------------------------------------------------

#include "Microtransactions.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(Microtransactions);

void Microtransactions::Update(void)
{
    bool32 touched = false;

    for (int32 t = 0; t < touchInfo->count; ++t) {
        int32 tx = (int32)(touchInfo->x[t] * screenInfo->size.x);
        int32 ty = (int32)(touchInfo->y[t] * screenInfo->size.y);
        if (touchInfo->down[t]) {
            if (tx >= 45 && ty >= 44 && tx <= (64 + sVars->maxLength) && ty <= 149) {
                touched = true;
                break;
            }
        }
    }

    if ((API::GetConfirmButtonFlip() ? controllerInfo[Input::CONT_P1].keyA.press : controllerInfo[Input::CONT_P1].keyB.press) || touched) {
        this->Destroy();
        sVars->sfxItems[Math::Rand(0, SFX_COUNT)].Play();
        Stage::SetEngineState(ENGINESTATE_REGULAR);
    }
}

void Microtransactions::LateUpdate(void) {}
void Microtransactions::StaticUpdate(void) {}

void Microtransactions::Draw(void)
{
    Graphics::DrawRect(45, 44, 19 + sVars->maxLength, 105, 0x808080, 0xFF, INK_NONE, true);
    Graphics::DrawRect(49, 49, 11 + sVars->maxLength, 96, 0x000000, 0xFF, INK_NONE, true);

    RSDK::Vector2 drawPos;
    drawPos.x = TO_FIXED(51);
    drawPos.y = TO_FIXED(56);
    this->animator.DrawString(&drawPos, &sVars->microString, 0, sVars->microString.length, ALIGN_LEFT, 0, nullptr, true);
    drawPos.y += TO_FIXED(STRING_SPACING);
    this->animator.DrawString(&drawPos, &sVars->speedString, 0, sVars->speedString.length, ALIGN_LEFT, 0, nullptr, true);
    drawPos.y += TO_FIXED(STRING_SPACING);
    this->animator.DrawString(&drawPos, &sVars->xpString, 0, sVars->xpString.length, ALIGN_LEFT, 0, nullptr, true);
    drawPos.y += TO_FIXED(STRING_SPACING);
    this->animator.DrawString(&drawPos, &sVars->growthString, 0, sVars->growthString.length, ALIGN_LEFT, 0, nullptr, true);
    drawPos.y += TO_FIXED(STRING_SPACING);
    this->animator.DrawString(&drawPos, &sVars->lifeString, 0, sVars->lifeString.length, ALIGN_LEFT, 0, nullptr, true);
    drawPos.y += TO_FIXED(STRING_SPACING);
    this->animator.DrawString(&drawPos, &sVars->globString, 0, sVars->globString.length, ALIGN_LEFT, 0, nullptr, true);
}

void Microtransactions::Create(void *data)
{
    if (!sceneInfo->inEditor) {
        this->active    = ACTIVE_ALWAYS;
        this->visible   = true;
        this->drawGroup = DRAWGROUP_COUNT - 1;
        this->animator.SetAnimation(&sVars->aniFrames, 0, true, 0);
        Stage::SetEngineState(ENGINESTATE_FROZEN);
    }
}

void Microtransactions::StageLoad(void)
{
    sVars->aniFrames.Load("UI/SmallFont.bin", SCOPE_STAGE);

    RSDK::String string;
    string.Init("", 0);
    string.LoadStrings("Microtransactions.txt");
    string.Split(sVars->stringList, 0, STR_COUNT);
    sVars->maxLength = 0;

    SetupString(&sVars->microString, STR_MICROTRANSACTIONS);
    SetupString(&sVars->speedString, STR_SUPERSPEED);
    SetupString(&sVars->xpString, STR_EXTRAXP);
    SetupString(&sVars->growthString, STR_MEGAGROWTH);
    SetupString(&sVars->lifeString, STR_BONUSLIFE);
    SetupString(&sVars->globString, STR_GLOB);

    sVars->sfxItems[0].Get("Buying Super Speed.wav");
    sVars->sfxItems[1].Get("Buying Extra XP.wav");
    sVars->sfxItems[2].Get("Buying Mega Growth.wav");
    sVars->sfxItems[3].Get("Buying Bonus Life.wav");
    sVars->sfxItems[4].Get("glob.wav");
}

void Microtransactions::SetupString(RSDK::String *string, uint8 id)
{
    memset(string, 0, sizeof(RSDK::String));
    string->Init("", 0);
    RSDK::String::Copy(string, &sVars->stringList[id]);
    if (string->GetWidth(sVars->aniFrames, 0, 0) > sVars->maxLength)
        sVars->maxLength = string->GetWidth(sVars->aniFrames, 0, 0);
    string->SetSpriteString(sVars->aniFrames, 0);
}

#if GAME_INCLUDE_EDITOR
void Microtransactions::EditorDraw(void) {}

void Microtransactions::EditorLoad(void) {}
#endif

#if RETRO_REV0U
void Microtransactions::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(Microtransactions); }
#endif

void Microtransactions::Serialize(void) {}

} // namespace GameLogic
