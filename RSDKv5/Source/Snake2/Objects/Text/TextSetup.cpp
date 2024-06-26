// ---------------------------------------------------------------------
// RSDK Project: Snake 2
// Object Description: TextSetup Object
// Object Author: MegAmi
// ---------------------------------------------------------------------

#include "TextSetup.hpp"
#include "TextString.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(TextSetup);

void TextSetup::Update(void)
{
    if (!this->timer) {
        LoadTextStrings();
        if (!IsLore())
            sVars->sfxSnake.Play();
    }

    if (++this->timer == 600) {
        if (IsLore()) {
            Stage::SetScene("Game", "Game");
            Stage::LoadScene();
        }
        else {
            channels[0].PlayStream("DLC_TOTK.ogg", 0, 0, false);
            Video::LoadVideo("DLC_TOTK.ogv", 0, VideoSkipCB);
        }
    }
    else if (this->timer == 601)
        API::ExitGame();
}

void TextSetup::LateUpdate(void) {}
void TextSetup::StaticUpdate(void) {}

void TextSetup::Draw(void) {}

void TextSetup::Create(void *data)
{
    if (!sceneInfo->inEditor) {
        this->active    = ACTIVE_NORMAL;
        this->visible   = true;
        this->drawGroup = 3;
        this->timer     = 0;
    }
}

void TextSetup::StageLoad(void)
{
    sVars->sfxSnake.Get("Snake Snake Snake.wav");
    GameObject::Reset(0, sVars->classID, nullptr);
}

void TextSetup::LoadTextStrings(void)
{
    RSDK::String string;
    RSDK::String stringList;

    stringList.LoadStrings(IsLore() ? "Lore.txt" : "Credits.txt");
    string.Init("", 0x80);

    int32 offset = TO_FIXED(5);

    if (!IsLore()) {
        int32 lineCount = 0;
        for (int32 i = 0; stringList.Split(&string, i, 1); ++i) lineCount++;
        offset = TO_FIXED(screenInfo->center.y - lineCount * 8);
    }

    for (int32 i = 0; stringList.Split(&string, i, 1); ++i) {
        TextString *text = RSDK_GET_ENTITY(i + 1, TextString);
        GameObject::Reset(text->Slot(), TextString::sVars->classID, nullptr);
        TextString::SetText(text, &string);
        text->position.x = TO_FIXED(IsLore() ? 0 : screenInfo->center.x);
        if (!IsLore())
            text->position.x -= TO_FIXED(text->text.GetWidth(TextString::sVars->aniFrames, 0, 0, text->text.length, 0) / 2);
        text->position.y = offset;
        offset += TO_FIXED(16);
    }

    sVars->stringsSize = offset + (screenInfo->size.y << 15);
}

bool32 TextSetup::VideoSkipCB(void) { return false; }

#if GAME_INCLUDE_EDITOR
void TextSetup::EditorDraw(void) { Draw(); }

void TextSetup::EditorLoad(void) {}
#endif

#if RETRO_REV0U
void TextSetup::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(TextSetup); }
#endif

void TextSetup::Serialize(void) {}

} // namespace GameLogic
