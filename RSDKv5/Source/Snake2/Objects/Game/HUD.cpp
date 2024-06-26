// ---------------------------------------------------------------------
// RSDK Project: Snake 2
// Object Description: HUD Object
// Object Author: MegAmi
// ---------------------------------------------------------------------

#include "HUD.hpp"
#include "Snake.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(HUD);

void HUD::Update(void) {}
void HUD::LateUpdate(void) {}
void HUD::StaticUpdate(void) {}

void HUD::Draw(void)
{
    Snake *snake = RSDK_GET_ENTITY(SLOT_SNAKE, Snake);

    RSDK::Vector2 drawPos;
    drawPos.x = TO_FIXED(6);
    drawPos.y = TO_FIXED(8);

    this->inkEffect = INK_NONE;

    // Draw Score Text
    RSDK::String string;
    string.Set("Score:");
    string.SetSpriteString(sVars->aniFrames, 0);
    this->animator.DrawString(&drawPos, &string, 0, string.length, ALIGN_LEFT, 0, nullptr, true);

    // Draw Score Number
    drawPos.x += TO_FIXED(string.GetWidth(sVars->aniFrames, 0, 0));
    DrawNumbers(&drawPos, snake->score);

    // Draw Level Text
    drawPos.x = TO_FIXED(6);
    drawPos.y += TO_FIXED(STRING_SPACING);
    string.Set("Level:");
    string.SetSpriteString(sVars->aniFrames, 0);
    this->animator.DrawString(&drawPos, &string, 0, string.length, ALIGN_LEFT, 0, nullptr, true);

    // Draw Level Number
    drawPos.x += TO_FIXED(string.GetWidth(sVars->aniFrames, 0, 0));
    DrawNumbers(&drawPos, snake->level);

    // Draw XP Text
    drawPos.x = TO_FIXED(6);
    drawPos.y += TO_FIXED(STRING_SPACING);
    string.Set("XP:");
    string.SetSpriteString(sVars->aniFrames, 0);
    this->animator.DrawString(&drawPos, &string, 0, string.length, ALIGN_LEFT, 0, nullptr, true);

    // Draw Current and Required XP Numbers
    drawPos.x += TO_FIXED(string.GetWidth(sVars->aniFrames, 0, 0));
    DrawNumbers(&drawPos, snake->xp);
    drawPos.x += TO_FIXED(8);
    this->animator.frameID = 15; // /
    this->animator.DrawSprite(&drawPos, true);
    drawPos.x += TO_FIXED(2);
    DrawNumbers(&drawPos, snake->xpNeeded);

#ifdef __ANDROID__
    // Initialize DPad Position
    drawPos.x = TO_FIXED(DPAD_X);
    drawPos.y = TO_FIXED(DPAD_Y);

    this->inkEffect = INK_BLEND;

    // Draw DPad Base
    this->dpadAnim.frameID = 0;
    this->dpadAnim.DrawSprite(&drawPos, true);

    // Draw Up DPad Button
    this->dpadAnim.frameID = snake->up + 1;
    this->dpadAnim.DrawSprite(&drawPos, true);

    // Draw Down DPad Button
    this->dpadAnim.frameID = snake->down + 3;
    this->dpadAnim.DrawSprite(&drawPos, true);

    // Draw Left DPad Button
    this->dpadAnim.frameID = snake->left + 5;
    this->dpadAnim.DrawSprite(&drawPos, true);

    // Draw Right DPad Button
    this->dpadAnim.frameID = snake->right + 7;
    this->dpadAnim.DrawSprite(&drawPos, true);
#endif
}

void HUD::Create(void *data)
{
    if (!sceneInfo->inEditor) {
        this->active    = ACTIVE_NORMAL;
        this->visible   = true;
        this->drawGroup = DRAWGROUP_COUNT - 1;
        this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
        this->dpadAnim.SetAnimation(sVars->dpadFrames, 0, true, 0);
    }
}

void HUD::StageLoad(void)
{
    sVars->aniFrames.Load("UI/SmallFont.bin", SCOPE_STAGE);
    sVars->dpadFrames.Load("Game/DPad.bin", SCOPE_STAGE);

    foreach_all(HUD, spawn)
    {
        HUD *hud = RSDK_GET_ENTITY(SLOT_HUD, HUD);
        spawn->Copy(hud, true);
    }
}

void HUD::DrawNumbers(RSDK::Vector2 *drawPos, int32 value)
{
    int32 digitCount = 1, checkValue = value;
    while (checkValue >= 10) {
        digitCount++;
        checkValue /= 10;
    }

    drawPos->x += TO_FIXED(digitCount * 8);
    RSDK::Vector2 numPos = *drawPos;

    int32 digit = 1;
    while (digitCount--) {
        this->animator.frameID = (value / digit % 10) + 16;
        this->animator.DrawSprite(&numPos, true);
        digit *= 10;
        numPos.x -= TO_FIXED(8);
    }
}

#if GAME_INCLUDE_EDITOR
void HUD::EditorDraw(void) {}

void HUD::EditorLoad(void) {}
#endif

#if RETRO_REV0U
void HUD::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(HUD); }
#endif

void HUD::Serialize(void) {}

} // namespace GameLogic
