// ---------------------------------------------------------------------
// RSDK Project: Snake 2
// Object Description: Snake Object
// Object Author: MegAmi
// ---------------------------------------------------------------------

#include "Snake.hpp"
#include "Microtransactions.hpp"

using namespace RSDK;

namespace GameLogic
{
RSDK_REGISTER_OBJECT(Snake);

void Snake::Update(void)
{
    // Store previous positions
    this->prevPositions[3] = this->prevPositions[2];
    this->prevPositions[2] = this->prevPositions[1];
    this->prevPositions[1] = this->prevPositions[0];
    this->prevPositions[0] = this->position;

    if (!this->segmentID) {
        // Process controls
        ControllerState *controller = &controllerInfo[Input::CONT_P1];
        AnalogState *stick          = &analogStickInfoL[Input::CONT_P1];

        this->up    = controller->keyUp.down;
        this->down  = controller->keyDown.down;
        this->left  = controller->keyLeft.down;
        this->right = controller->keyRight.down;

        this->up |= stick->keyUp.down;
        this->down |= stick->keyDown.down;
        this->left |= stick->keyLeft.down;
        this->right |= stick->keyRight.down;

        this->up |= stick->vDelta > 0.3;
        this->down |= stick->vDelta < -0.3;
        this->left |= stick->hDelta < -0.3;
        this->right |= stick->hDelta > 0.3;

        int32 touched = -1, fx = 0, fy = 0;

        for (int32 t = 0; t < touchInfo->count; ++t) {
            int32 tx = (int32)(touchInfo->x[t] * screenInfo->size.x);
            int32 ty = (int32)(touchInfo->y[t] * screenInfo->size.y);
            if (touchInfo->down[t]) {
                if (tx >= 0 && ty >= 96 && tx <= screenInfo->center.x && ty <= screenInfo->size.y) {
                    fx      = tx;
                    fy      = ty;
                    touched = t;
                    break;
                }
            }
        }

        if (touched >= 0) {
            // Initialize D-Pad position
            fx -= DPAD_X;
            fy -= DPAD_Y;

            switch (((Math::ATan2(fx, fy) + 32) & 0xFF) >> 6) { // Check where the touch is, if it's there
                case 0: this->right |= true; break;
                case 1: this->down |= true; break;
                case 2: this->left |= true; break;
                case 3: this->up |= true; break;
            }
        }

        if (this->up && this->down) {
            this->up   = false;
            this->down = false;
        }

        if (this->left && this->right) {
            this->left  = false;
            this->right = false;
        }

        // Check for inputs and set momentum
        if (this->up && this->momentum.y != this->size) {
            this->momentum.x = 0;
            this->momentum.y = -this->size;
        }
        else if (this->down && this->momentum.y != -this->size) {
            this->momentum.x = 0;
            this->momentum.y = this->size;
        }
        else if (this->left && this->momentum.x != this->size) {
            this->momentum.x = -this->size;
            this->momentum.y = 0;
        }
        else if (this->right && this->momentum.x != -this->size) {
            this->momentum.x = this->size;
            this->momentum.y = 0;
        }

        UpdatePosition();

        // Check collision with boundaries
        if (FROM_FIXED(this->position.x) < 0 || FROM_FIXED(this->position.x) > screenInfo->size.x || FROM_FIXED(this->position.y) < 0
            || FROM_FIXED(this->position.y) > screenInfo->size.y)
            GameOver();
    }
    else {
        Snake *prevSegment = RSDK_GET_ENTITY(this->segmentID == 1 ? SLOT_SNAKE : (this->Slot() - 1), Snake);
        this->position     = prevSegment->prevPositions[SEGMENT_SPACING - 1];

        if (this->segmentID >= IGNORE_SEGMENTS) {
            if (this->CheckCollisionTouchBox(&sVars->hitbox, RSDK_GET_ENTITY(SLOT_SNAKE, Snake), &sVars->hitbox))
                GameOver();
        }
    }
}

void Snake::LateUpdate(void)
{
    // Segment order sorting (basically reverse the regular order)
    // There's probably a better way to do this but just doing whatever works is the true DougDoug way
    if (this->length > 1 && !this->segmentID) {
        foreach_all(Snake, segment)
        {
            foreach_all(Snake, segment2)
            {
                if (segment->segmentID < segment2->segmentID)
                    Graphics::SwapDrawListEntries(this->drawGroup, segment->Slot(), segment2->Slot(), 0);
            }
        }
        Graphics::SwapDrawListEntries(this->drawGroup, SEGMENTS_START + this->length - 1, SEGMENTS_START + this->length - 2,
                                      this->length); // dumb hack
    }
}

void Snake::StaticUpdate(void) {}

void Snake::Draw(void)
{
    paletteBank[1].SetActivePalette(0, screenInfo->size.y);
    this->animator.DrawSprite(&this->position, false);
    paletteBank[0].SetActivePalette(0, screenInfo->size.y);
}

void Snake::Create(void *data)
{
    this->scale.x = PLAYER_SCALE;
    this->scale.y = PLAYER_SCALE;
    this->drawFX |= FX_SCALE;

    if (!sceneInfo->inEditor) {
        this->active    = ACTIVE_NORMAL;
        this->visible   = true;
        this->drawGroup = 3;

        this->animator.SetAnimation(sVars->fieriFrames, 0, true, 0);
        sVars->hitbox = *this->animator.GetHitbox(0);

        if (!data) {
            this->position.x = TO_FIXED(screenInfo->center.x);
            this->position.y = TO_FIXED(screenInfo->center.y);

            for (int32 p = 0; p < SEGMENT_SPACING; p++) this->prevPositions[p] = this->position;

            this->speed     = TO_FIXED(2 + globals->dlcActivated);
            this->score     = 0;
            this->level     = 1;
            this->xp        = 0;
            this->xpNeeded  = 10;
            this->length    = 1;
            this->size      = TO_FIXED(2);
            this->segmentID = 0;
        }
        else {
            this->segmentID = VOID_TO_INT(data);

            Snake *prevSegment = RSDK_GET_ENTITY(this->segmentID == 1 ? SLOT_SNAKE : (this->Slot() - 1), Snake);
            this->position     = prevSegment->prevPositions[SEGMENT_SPACING - 1];
            for (int32 p = 0; p < SEGMENT_SPACING; p++) this->prevPositions[p] = this->position;
        }
    }
}

void Snake::StageLoad(void)
{
    sVars->fieriFrames.Load("Game/GuyFieri.bin", SCOPE_STAGE);
    sVars->sfxLevelUp.Get("Minecraft Level Up.wav");

    foreach_all(Snake, spawn)
    {
        Snake *snake = RSDK_GET_ENTITY(SLOT_SNAKE, Snake);
        spawn->Copy(snake, true);
    }
}

void Snake::UpdatePosition(void)
{
    // Modify current snake velocity based on momentum
    this->velocity.x += FROM_FIXED((this->momentum.x - this->velocity.x) * MOMENTUM);
    this->velocity.y += FROM_FIXED((this->momentum.y - this->velocity.y) * MOMENTUM);

    // Ensure that x velocity does not exceed max speed
    if (this->velocity.x < -this->speed)
        this->velocity.x = -this->speed;
    else if (this->velocity.x > this->speed)
        this->velocity.x = this->speed;

    // Ensure that y velocity does not exceed max speed
    if (this->velocity.y < -this->speed)
        this->velocity.y = -this->speed;
    else if (this->velocity.y > this->speed)
        this->velocity.y = this->speed;

    // Update snake position
    this->position.x += this->velocity.x;
    this->position.y += this->velocity.y;
}

void Snake::EatFood(Snake *snake)
{
    snake->score++;
    snake->xp++;

    if (snake->xp >= snake->xpNeeded) {
        snake->level++;
        snake->size += TO_FIXED(1);
        snake->speed += 0x2000;
        snake->xp = 0;
        snake->xpNeeded *= 2;
        sVars->sfxLevelUp.Play();
    }

    int32 segmentID = SEGMENTS_START + snake->length - 1;
    if (segmentID < TEMPENTITY_START) {
        GameObject::Reset(segmentID, sVars->classID, INT_TO_VOID(snake->length));
        snake->length++;
    }

    CREATE_ENTITY(Microtransactions, nullptr, 0, 0);
}

void Snake::GameOver(void)
{
    Stage::SetScene("Presentation", "Credits");
    Stage::LoadScene();
}

#if GAME_INCLUDE_EDITOR
void Snake::EditorDraw(void)
{
    this->animator.SetAnimation(sVars->fieriFrames, 0, true, 0);
    this->animator.DrawSprite(nullptr, false);
}

void Snake::EditorLoad(void) { sVars->fieriFrames.Load("Game/GuyFieri.bin", SCOPE_STAGE); }
#endif

#if RETRO_REV0U
void Snake::StaticLoad(Static *sVars) { RSDK_INIT_STATIC_VARS(Snake); }
#endif

void Snake::Serialize(void) {}

} // namespace GameLogic
