// ---------------------------------------------------------------------
// RSDK Project: Snake 2
// Object Description: TextString Object
// Object Author: MegAmi
// ---------------------------------------------------------------------

#include "TextString.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(TextString);

void TextString::Update(void) {}
void TextString::LateUpdate(void) {}
void TextString::StaticUpdate(void) {}

void TextString::Draw(void) { this->animator.DrawString(&this->position, &this->text, 0, this->text.length, ALIGN_LEFT, 0, nullptr, true); }

void TextString::Create(void *data)
{
    this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);

    if (!sceneInfo->inEditor) {
        this->active        = ACTIVE_NORMAL;
        this->visible       = true;
        this->updateRange.x = 0x800000;
        this->updateRange.y = 0x800000;
        this->drawGroup     = 3;

        if (!this->text.chars)
            this->text.Init("UNTITLED", 0);
    }
}

void TextString::StageLoad(void) { sVars->aniFrames.Load("UI/SmallFont.bin", SCOPE_STAGE); }

void TextString::SetText(TextString *label, RSDK::String *text)
{
    label->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
    RSDK::String::Copy(&label->text, text);
    label->text.SetSpriteString(sVars->aniFrames, 0);
    SpriteFrame *frame = sVars->aniFrames.GetFrame(0, 0);
}

#if GAME_INCLUDE_EDITOR
void TextString::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->aniFrames, 0, true, 0);
    Draw();
}

void TextString::EditorLoad(void) { sVars->aniFrames.Load("UI/SmallFont.bin", SCOPE_STAGE); }
#endif

#if RETRO_REV0U
void TextString::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(TextString); }
#endif

void TextString::Serialize(void) {}

} // namespace GameLogic
