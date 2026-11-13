// Behaviour test for 0x00665860 CThingCreatureBase::UpdateAnimation: a host copy of the row's logic over plain models (the creature's
// movement, the animator's main / combination animations, the components the row consults and the effects), checked path by path.
#include <cstdio>
#include <cstring>
#include <cmath>

static const float DELTA = 1e-4f;
static const unsigned long HOVER = 3;

struct Animator {
    int MainType; bool MainFinished, MainFinishedSet, Interruptable; unsigned long MainFlags;
    bool HasType25, Type25Finished; float Type25Alpha, Type25Change;
    bool HasFace;                                        // a type-20 combination exists
    int Cleared, MainSetType, MainSetBlend, MainSetDelay; float MainSetSpeed; bool MainSetLooping;
    int FacesAdded, Removed[32], Updates; float LastTime, LastDistance;
};
struct Creature {
    bool HasAnimation, HasModes, HasNav, HasLook, LookInRange, HasExpression, HasTalk, DemonDoor, Oracle, Blinking, FaceExists;
    unsigned long NavType; float MoveX, MoveY, MoveZ, Multiplier; long IdleCounter;
    bool HasAction, ActionFinished; long ActionAnim; bool ActionAnimIsCombination;
    int ModeTypeQueries, ModeAnimUpdates, ExpressionUpdates, TalkUpdates, ActionAnimUpdates;
};

static const long FPS = 30;
static const float RATE = 1.0f;                          // the 0x013B8774 global (0) + 1

// The row's logic (same order of tests and effects as 0x00665860).
static void UpdateAnimation(Creature& c, Animator& a, float time)
{
    if (!c.HasAnimation) return;
    float speed_squared = c.MoveX * c.MoveX + c.MoveY * c.MoveY;
    float frame_time = 1.0f / FPS;
    float distance = (float)std::sqrt(speed_squared) * c.Multiplier;
    float update_time = c.Multiplier * frame_time;
    c.IdleCounter = 0;
    c.ModeTypeQueries++;
    if (RATE * 9999980.0f < time) {
        c.Blinking = false;
        a.Cleared++;
        a.MainSetType = 6; a.MainSetSpeed = 0.0f; a.MainSetBlend = 1; a.MainSetDelay = 0; a.MainSetLooping = false;
        a.Interruptable = true;
    } else {
        bool update_main = true;
        if (a.MainFinished || a.Interruptable) {
            if (c.HasAction && !c.ActionFinished) {
                if (c.ActionAnim) {
                    if (!c.ActionAnimIsCombination) update_main = false;
                    if (a.HasType25 && !a.Type25Finished) { a.Type25Finished = true; a.Type25Change = a.Type25Alpha / 3.0f; }
                }
            } else if (a.MainType == 7 && !a.MainFinishedSet) {
                update_main = false;
            }
        }
        if (update_main) {
            if (c.HasNav && c.NavType == HOVER && distance > DELTA) {
                if (-DELTA <= c.MoveZ) { if (a.MainType != 9) { a.MainSetType = 9; a.MainSetLooping = true; a.MainType = 9; } }
                else if (a.MainType != 10) { a.MainSetType = 10; a.MainSetLooping = true; a.MainType = 10; }
            } else {
                c.ModeAnimUpdates++;
            }
        }
        if ((a.MainFlags & 4) && RATE * 12.0f > time) {
            if (c.FaceExists && !a.HasFace) { a.FacesAdded++; a.HasFace = true; }
        } else {
            a.Removed[20]++;
        }
        if (c.HasLook && !c.LookInRange) { a.Removed[11]++; a.Removed[12]++; }
    }
    if (!(RATE * 12.0f > time) && !c.DemonDoor && !c.Oracle) a.Removed[18]++;
    else if (c.HasExpression) c.ExpressionUpdates++;
    if (!(RATE * 8.0f > time) && !c.DemonDoor && !c.Oracle) a.Removed[19]++;
    else if (c.HasTalk) c.TalkUpdates++;
    if (c.HasAction && !c.ActionFinished && c.ActionAnim) c.ActionAnimUpdates++;
    a.Updates++; a.LastTime = update_time; a.LastDistance = distance;
}

static bool Near(float x, float y) { return std::fabs(x - y) < 1e-4f; }

static void Reset(Creature& c, Animator& a)
{
    std::memset(&c, 0, sizeof(c)); std::memset(&a, 0, sizeof(a));
    c.HasAnimation = c.HasModes = c.HasExpression = c.HasTalk = true; c.Multiplier = 2.0f; c.IdleCounter = 9; c.Blinking = true;
    a.MainSetType = -1;
}

int main()
{
    Creature c; Animator a; bool ok = true;

    // no animation component: nothing at all
    Reset(c, a); c.HasAnimation = false;
    UpdateAnimation(c, a, 0.5f);
    ok = ok && a.Updates == 0 && c.IdleCounter == 9;

    // ordinary frame: the mode drives the main animation, the update carries time and xy distance
    Reset(c, a); c.MoveX = 3.0f; c.MoveY = 4.0f; c.MoveZ = 7.0f;
    UpdateAnimation(c, a, 0.5f);
    ok = ok && c.ModeAnimUpdates == 1 && a.Updates == 1 && Near(a.LastDistance, 10.0f) && Near(a.LastTime, 2.0f / 30.0f)
            && c.IdleCounter == 0 && c.ExpressionUpdates == 1 && c.TalkUpdates == 1 && a.Removed[20] == 1;

    // a long gap resets to the DEFAULT pose (speed 0) and skips the main-anim logic
    Reset(c, a);
    UpdateAnimation(c, a, 2.0e7f);
    ok = ok && a.Cleared == 1 && a.MainSetType == 6 && Near(a.MainSetSpeed, 0.0f) && a.Interruptable && !c.Blinking
            && c.ModeAnimUpdates == 0 && a.Removed[18] == 1 && a.Removed[19] == 1 && c.ExpressionUpdates == 0;

    // a hovering creature flies when level, glides when sinking
    Reset(c, a); c.HasNav = true; c.NavType = HOVER; c.MoveX = 1.0f;
    UpdateAnimation(c, a, 0.5f);
    ok = ok && a.MainSetType == 9 && a.MainSetLooping && c.ModeAnimUpdates == 0;
    c.MoveZ = -1.0f; a.MainSetType = -1;
    UpdateAnimation(c, a, 0.5f);
    ok = ok && a.MainSetType == 10;

    // the action's own non-combination animation keeps the main animation; a type-25 combination fades over 3 frames
    Reset(c, a); a.MainFinished = true; c.HasAction = true; c.ActionAnim = 5; a.HasType25 = true; a.Type25Alpha = 0.9f;
    UpdateAnimation(c, a, 0.5f);
    ok = ok && c.ModeAnimUpdates == 0 && a.Type25Finished && Near(a.Type25Change, 0.3f) && c.ActionAnimUpdates == 1;

    // the default face is added once while the main animation allows it; the look out of range drops the head layers
    Reset(c, a); a.MainFlags = 4; c.FaceExists = true; c.HasLook = true;
    UpdateAnimation(c, a, 0.5f);
    UpdateAnimation(c, a, 0.5f);
    ok = ok && a.FacesAdded == 1 && a.Removed[20] == 0 && a.Removed[11] == 2 && a.Removed[12] == 2;

    // demon-door / oracle creatures keep their expression and talk layers whatever the gap
    Reset(c, a); c.DemonDoor = true;
    UpdateAnimation(c, a, 100.0f);
    ok = ok && a.Removed[18] == 0 && a.Removed[19] == 0 && c.ExpressionUpdates == 1 && c.TalkUpdates == 1;

    if (!ok) { std::printf("00665860_TEST FAIL\n"); return 1; }
    std::printf("00665860_TEST PASS\n");
    return 0;
}