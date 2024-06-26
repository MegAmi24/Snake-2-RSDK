// ---------------------------------------------------------------------
// RSDK Project: Snake 2
// Object Description: Food Object
// Object Author: MegAmi
// ---------------------------------------------------------------------

#include "Food.hpp"
#include "Snake.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(Food);

void Food::Update(void)
{
    if (globals->dlcActivated) {
        if (++this->timer >= 5)
            this->foodColor = Math::Rand(0, 256) | Math::Rand(0, 256) << 8 | Math::Rand(0, 256) << 16;
    }

    this->position.x += this->velocity.x;
    this->position.y += this->velocity.y;

    // Handle Top/Bottom bounds collision
    if (FROM_FIXED(this->position.y) + sVars->hitbox.top < 0) {
        this->position.y = TO_FIXED(sVars->hitbox.bottom);
        this->velocity.y *= -1;
    }
    else if (FROM_FIXED(this->position.y) + sVars->hitbox.bottom > screenInfo->size.y) {
        this->position.y = TO_FIXED(screenInfo->size.y + sVars->hitbox.top);
        this->velocity.y *= -1;
    }

    // Handle Left/Right bounds collision
    if (FROM_FIXED(this->position.x) + sVars->hitbox.left < 0) {
        this->position.x = TO_FIXED(sVars->hitbox.right);
        this->velocity.x *= -1;
    }
    else if (FROM_FIXED(this->position.x) + sVars->hitbox.right > screenInfo->size.x) {
        this->position.x = TO_FIXED(screenInfo->size.x + sVars->hitbox.left);
        this->velocity.x *= -1;
    }
}

void Food::LateUpdate(void)
{
    // Handle Snake collision
    foreach_active(Snake, snake)
    {
        if (!snake->segmentID && this->CheckCollisionTouchBox(&sVars->hitbox, snake, &Snake::sVars->hitbox)) {
            Snake::EatFood(snake);
            NewPosition();
        }
    }
}

void Food::StaticUpdate(void) {}

void Food::Draw(void)
{
    Graphics::DrawRect(FROM_FIXED(this->position.x) - 2, FROM_FIXED(this->position.y) - 2, 4, 4, this->foodColor, 0xFF, INK_NONE, true);
}

void Food::Create(void *data)
{
    if (!sceneInfo->inEditor) {
        this->active     = ACTIVE_NORMAL;
        this->visible    = true;
        this->drawGroup  = 3;
        this->velocity.x = TO_FIXED(2 + globals->dlcActivated);
        this->velocity.y = TO_FIXED(2 + globals->dlcActivated);
        this->foodColor  = 0x00FF00;
        NewPosition();
    }
}

void Food::StageLoad(void)
{
    sVars->hitbox.left   = -2;
    sVars->hitbox.top    = -2;
    sVars->hitbox.right  = 2;
    sVars->hitbox.bottom = 2;

    foreach_all(Food, spawn)
    {
        Food *food = RSDK_GET_ENTITY(SLOT_FOOD, Food);
        spawn->Copy(food, true);
    }
}

void Food::NewPosition(void)
{
    this->position.x = TO_FIXED(Math::Rand(2, screenInfo->size.x - 1));
    this->position.y = TO_FIXED(Math::Rand(2, screenInfo->size.y - 1));
}

#if GAME_INCLUDE_EDITOR
void Food::EditorDraw(void)
{
    Graphics::DrawRect(FROM_FIXED(this->position.x) - 2, FROM_FIXED(this->position.y) - 2, 4, 4, 0x00FF00, 0xFF, INK_NONE, true);
}

void Food::EditorLoad(void) {}
#endif

#if RETRO_REV0U
void Food::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(Food); }
#endif

void Food::Serialize(void) {}

} // namespace GameLogic
