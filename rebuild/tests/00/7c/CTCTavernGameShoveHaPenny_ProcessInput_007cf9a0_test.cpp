// Behaviour test for 0x007CF9A0 CTCTavernGameShoveHaPenny::ProcessInput: a host copy of the row's logic over plain models (the
// game, the held coin, the def's tuning values, the input process answers and the effects), checked state by state.
#include <cstdio>
#include <cmath>

enum { QUIT_MID_GAME = 11, POSITION_COIN = 15, AIM_COIN = 16, WATCH_COINS = 17, OTHER_STATE = 5 };
enum { ACT_YES = 22, ACT_THIRD = 24, ACT_CANCEL_AIM = 58, ACT_MOVE = 79, ACT_ACTION_BUTTON = 81, ACT_QUIT = 83 };

struct Vec2 { float X, Y; };
class Coin { public: Vec2 Pos, Vel; };
struct Def {
    float SpeedPC, Minimum, Maximum, ScalePCX, ScalePCY, TranslateY, MagAbove, Drift;
};
struct Input {                       // what the input process answers for this event
    int Action;                      // the event's game action
    bool Pressed;                    // IsEventButtonPressed
    bool HasMovement;                // GetControlMovementFromGameActionEvent result
    Vec2 Movement;
    bool ActionButtonHeld;           // IsGameActionEventInQueue(ACTION_BUTTON, 2, time)
};
struct Game {
    int State;
    Coin* Held;
    float BoardSizeX;
    Vec2 MousePosEffect, AverageJoystickPos;
    // effects
    int SetStateTo, YesNoNoState, Performed, Sounds, BaseCalls, CoinSoundsStopped;
    bool YesNoAsked;
};

static float Magnitude(const Vec2& v) { return (float)std::sqrt(v.X * v.X + v.Y * v.Y); }
static float MaxF(float a, float b) { return a < b ? b : a; }
static float MinF(float a, float b) { return b < a ? b : a; }
static void SetState(Game& g, int s) { g.SetStateTo = s; g.State = s; }

// The row's logic (same order of tests and effects as 0x007CF9A0).
static void ProcessInput(Game& g, const Def& def, const Input& in, float frame_time)
{
    Vec2 movement = { 0.0f, 0.0f };
    bool moveEvent = in.Action == ACT_MOVE;
    switch (g.State) {
    case POSITION_COIN:
        if (moveEvent && in.HasMovement) {
            movement = in.Movement;
            g.Held->Pos.X += movement.X * def.SpeedPC * frame_time;
            g.Held->Pos.X = MaxF(g.Held->Pos.X, def.Minimum * g.BoardSizeX);
            g.Held->Pos.X = MinF(g.Held->Pos.X, def.Maximum * g.BoardSizeX);
            g.Performed++;
        }
        if (in.Pressed && in.Action == ACT_YES) {
            SetState(g, AIM_COIN);
            g.MousePosEffect.X = 0.0f; g.MousePosEffect.Y = -0.2f;
            g.AverageJoystickPos.X = 0.0f; g.AverageJoystickPos.Y = -0.2f;
            g.Held->Vel.X = -(g.AverageJoystickPos.X * def.ScalePCX);
            g.Held->Vel.Y = -(g.AverageJoystickPos.Y * def.ScalePCY) + def.TranslateY;
            g.Performed++;
        }
        break;
    case AIM_COIN:
        if (in.Pressed && in.Action == ACT_CANCEL_AIM) {
            SetState(g, POSITION_COIN);
            g.Performed++;
        }
        if ((moveEvent && in.HasMovement && (movement = in.Movement, true))
            || (g.AverageJoystickPos.Y != 0.0f && Magnitude(g.AverageJoystickPos) > def.MagAbove)) {
            if (Magnitude(g.AverageJoystickPos) > def.MagAbove) {
                if (!in.ActionButtonHeld) {
                    g.Sounds++;
                    SetState(g, WATCH_COINS);
                } else {
                    g.Held->Vel.X = g.AverageJoystickPos.X * def.ScalePCX;
                    g.Held->Vel.Y = -(g.AverageJoystickPos.Y * def.ScalePCY) + def.TranslateY;
                    g.Held->Vel.Y = MaxF(g.Held->Vel.Y, def.TranslateY);
                }
            } else {
                g.Held->Vel.X = 0.0f;
                g.Held->Vel.Y = 0.0f;
            }
            if (moveEvent && in.ActionButtonHeld) {
                g.MousePosEffect.X += movement.X;
                g.MousePosEffect.Y -= movement.Y;
                float m = Magnitude(g.MousePosEffect);
                if (m > 1.0f) { g.MousePosEffect.X /= m; g.MousePosEffect.Y /= m; }
                movement = g.MousePosEffect;
                g.AverageJoystickPos.X = g.AverageJoystickPos.X * (1.0f - def.Drift) + movement.X * def.Drift;
                g.AverageJoystickPos.Y = g.AverageJoystickPos.Y * (1.0f - def.Drift) + movement.Y * def.Drift;
                g.Performed++;
            }
        }
        if (in.Pressed && in.Action == ACT_YES) {
            if (g.Held->Vel.X * g.Held->Vel.X + g.Held->Vel.Y * g.Held->Vel.Y > 0.03f) {
                g.Held = 0;
                g.Sounds++;
                SetState(g, WATCH_COINS);
            }
            g.Performed++;
        }
        break;
    case WATCH_COINS:
        break;
    default:
        g.BaseCalls++;
        break;
    }
    if (g.State >= POSITION_COIN && g.State <= WATCH_COINS) {
        if (in.Action == ACT_THIRD || in.Action == ACT_QUIT) {
            g.YesNoAsked = true;
            g.YesNoNoState = g.State;
            g.Performed++;
            g.CoinSoundsStopped++;
        }
    }
}

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

static void Reset(Game& g, Coin& c, int state)
{
    c.Pos.X = 1.0f; c.Pos.Y = 0.0f; c.Vel.X = 0.0f; c.Vel.Y = 0.0f;
    g.State = state; g.Held = &c; g.BoardSizeX = 2.0f;
    g.MousePosEffect.X = g.MousePosEffect.Y = 0.0f;
    g.AverageJoystickPos.X = g.AverageJoystickPos.Y = 0.0f;
    g.SetStateTo = g.YesNoNoState = -1; g.Performed = g.Sounds = g.BaseCalls = g.CoinSoundsStopped = 0; g.YesNoAsked = false;
}

static Input MakeInput(int action, bool pressed, bool hasMove, float mx, float my, bool held)
{
    Input in; in.Action = action; in.Pressed = pressed; in.HasMovement = hasMove; in.Movement.X = mx; in.Movement.Y = my;
    in.ActionButtonHeld = held; return in;
}

int main()
{
    Def def = { 4.0f, 0.25f, 0.75f, 3.0f, 2.0f, 0.5f, 0.1f, 0.5f };
    Game g; Coin c; bool ok = true;

    // positioning: the coin slides by movement * speed * frame time, clamped to [Minimum, Maximum] * board width
    Reset(g, c, POSITION_COIN);
    ProcessInput(g, def, MakeInput(ACT_MOVE, false, true, 0.25f, 0.0f, false), 0.25f);
    ok = ok && Near(c.Pos.X, 1.25f) && g.Performed == 1;
    ProcessInput(g, def, MakeInput(ACT_MOVE, false, true, 2.0f, 0.0f, false), 0.25f);   // 1.25 + 2 -> clamped to 1.5
    ok = ok && Near(c.Pos.X, 1.5f);
    ProcessInput(g, def, MakeInput(ACT_MOVE, false, true, -10.0f, 0.0f, false), 0.25f); // clamped to 0.5
    ok = ok && Near(c.Pos.X, 0.5f);
    // yes starts aiming with the joystick reset to (0, -0.2) and the coin's starting velocity
    ProcessInput(g, def, MakeInput(ACT_YES, true, false, 0.0f, 0.0f, false), 0.25f);
    ok = ok && g.State == AIM_COIN && Near(g.AverageJoystickPos.Y, -0.2f) && Near(g.MousePosEffect.Y, -0.2f)
            && Near(c.Vel.X, 0.0f) && Near(c.Vel.Y, 0.4f + 0.5f) && g.Performed == 4;

    // aiming: the action button held with movement accumulates the mouse effect and blends the average
    Reset(g, c, AIM_COIN);
    g.AverageJoystickPos.X = 0.0f; g.AverageJoystickPos.Y = -0.2f;
    ProcessInput(g, def, MakeInput(ACT_MOVE, false, true, 0.2f, 0.1f, true), 0.25f);
    // magnitude 0.2 > 0.1, held: velocity from the old average, clamped up to the translation
    ok = ok && Near(c.Vel.X, 0.0f) && Near(c.Vel.Y, 0.9f);
    ok = ok && Near(g.MousePosEffect.X, 0.2f) && Near(g.MousePosEffect.Y, -0.1f);
    ok = ok && Near(g.AverageJoystickPos.X, 0.1f) && Near(g.AverageJoystickPos.Y, -0.15f) && g.Performed == 1;
    // a big movement is normalised to length 1
    Reset(g, c, AIM_COIN);
    ProcessInput(g, def, MakeInput(ACT_MOVE, false, true, 3.0f, -4.0f, true), 0.25f);
    ok = ok && Near(g.MousePosEffect.X, 0.6f) && Near(g.MousePosEffect.Y, 0.8f) && Near(c.Vel.X, 0.0f) && Near(c.Vel.Y, 0.0f);
    // the action button let go while the joystick is out: flick
    Reset(g, c, AIM_COIN);
    g.AverageJoystickPos.X = 0.3f; g.AverageJoystickPos.Y = 0.4f;
    ProcessInput(g, def, MakeInput(ACT_THIRD + 100, false, false, 0.0f, 0.0f, false), 0.25f);
    ok = ok && g.State == WATCH_COINS && g.Sounds == 1 && g.Held == &c;
    // velocity clamp: a positive y average would go below the translation and is raised to it
    Reset(g, c, AIM_COIN);
    g.AverageJoystickPos.X = 0.3f; g.AverageJoystickPos.Y = 0.4f;
    ProcessInput(g, def, MakeInput(ACT_THIRD + 100, false, false, 0.0f, 0.0f, true), 0.25f);
    ok = ok && Near(c.Vel.X, 0.9f) && Near(c.Vel.Y, 0.5f) && g.State == AIM_COIN;
    // cancel aim goes back to positioning; yes with enough velocity drops the coin and watches
    Reset(g, c, AIM_COIN);
    ProcessInput(g, def, MakeInput(ACT_CANCEL_AIM, true, false, 0.0f, 0.0f, false), 0.25f);
    ok = ok && g.State == POSITION_COIN && g.Performed == 1;
    Reset(g, c, AIM_COIN);
    c.Vel.X = 0.1f; c.Vel.Y = 0.1f;                         // 0.02 <= 0.03: kept
    ProcessInput(g, def, MakeInput(ACT_YES, true, false, 0.0f, 0.0f, false), 0.25f);
    ok = ok && g.Held == &c && g.State == AIM_COIN && g.Performed == 1;
    c.Vel.X = 0.2f;                                         // 0.05 > 0.03: dropped
    ProcessInput(g, def, MakeInput(ACT_YES, true, false, 0.0f, 0.0f, false), 0.25f);
    ok = ok && g.Held == 0 && g.State == WATCH_COINS && g.Sounds == 1 && g.Performed == 2;

    // other states go to the base game; the quit buttons ask only in the coin states
    Reset(g, c, OTHER_STATE);
    ProcessInput(g, def, MakeInput(ACT_QUIT, true, false, 0.0f, 0.0f, false), 0.25f);
    ok = ok && g.BaseCalls == 1 && !g.YesNoAsked;
    Reset(g, c, WATCH_COINS);
    ProcessInput(g, def, MakeInput(ACT_QUIT, true, false, 0.0f, 0.0f, false), 0.25f);
    ok = ok && g.BaseCalls == 0 && g.YesNoAsked && g.YesNoNoState == WATCH_COINS && g.CoinSoundsStopped == 1;

    if (!ok) { std::printf("007cf9a0_TEST FAIL\n"); return 1; }
    std::printf("007cf9a0_TEST PASS\n");
    return 0;
}