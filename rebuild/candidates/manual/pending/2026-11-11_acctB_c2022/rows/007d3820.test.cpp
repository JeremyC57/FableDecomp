// Behaviour test for 0x007D3820 CTCTavernGameCoinGolf::ProcessInput: a host model of the coin-golf input handler (camera stick,
// aim reset, alternate-button zero, held-movement blend, release flick, quit prompt, base fallback) with the callees mocked.
#include <cstdio>
#include <cmath>

struct V2 { float X, Y; };
struct V3 { float X, Y, Z; };

static float Mag(const V2& v) { return (float)sqrt(v.X * v.X + v.Y * v.Y); }
static float AngleXY(const V3& v)
{
    float angle = (float)atan2(v.X, v.Y) * 0.15915494f;
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f) angle += 1.0f;
    }
    return angle;
}
static const float& Max(const float& a, const float& b) { return a < b ? b : a; }
static const float& Min(const float& a, const float& b) { return b < a ? b : a; }

enum { AIM = 16, WATCH = 17, QUIT_MID_GAME = 11 };
enum { A_THIRD = 24, A_MOVEMENT = 79, A_CAMERA = 80, A_ACTION = 81, A_ALTERNATE = 82, A_QUIT = 83 };

struct Def { float JoystickScaleX, JoystickScaleY, JoystickMagBelow, AverageDrift, MaxPower; };

struct Input {                                  // mocked CInputProcessTavernGame + the event it is asked about
    bool held; int action; bool pressed; bool has_movement; V2 movement; bool camera_manager;
    bool IsGameActionEventInQueue() const { return held; }
    bool IsEventGameAction(int a) const { return action == a; }
    bool GetControlMovement(int a, V2& m) const { if (action == a && has_movement) { m = movement; return true; } return false; }
    bool IsEventButtonPressed() const { return pressed; }
};

struct Game {
    int State; V2 MousePosEffect; V2 AverageJoystickPos; V2 HeldVel; V3 BoardForward; float ZAngle, XAngle;
    V3 CamPos, LookAt;                          // what the mocked GetCamera returns
    int performed, base_calls, sounds, quit_prompts, coin_sound_updates; float last_rotate; int rotates;
    void GetCamera(V3& c, V3& l) { c = CamPos; l = LookAt; }
    void Rotate(V2& v, float a) { last_rotate = a; ++rotates; (void)v; }
    void ProcessInput(const Input& in, const Def& def)
    {
        V2 movement = { 0.0f, 0.0f };
        bool action_held = in.IsGameActionEventInQueue();
        if (!action_held && in.IsEventGameAction(A_CAMERA) && in.GetControlMovement(A_CAMERA, movement)
            && Mag(movement) > 0.04) {
            if (in.camera_manager) {
                float magnitude = Mag(movement);
                movement.X *= magnitude; movement.Y *= magnitude;
                ZAngle += movement.X * 0.04f;
                XAngle -= movement.Y * 0.08f;
                XAngle = Max(XAngle, 0.0f);
                XAngle = Min(XAngle, 1.0f);
                ++performed;
            }
        }
        switch (State) {
        case AIM:
            if (in.IsEventGameAction(A_ACTION) && in.IsEventButtonPressed()) {
                MousePosEffect.X = AverageJoystickPos.X = 0.0f;
                MousePosEffect.Y = AverageJoystickPos.Y = -0.2f;
                HeldVel.X = AverageJoystickPos.X * def.JoystickScaleX;
                HeldVel.Y = AverageJoystickPos.Y * def.JoystickScaleY;
                V3 c, l; GetCamera(c, l);
                l.X -= c.X; l.Y -= c.Y; l.Z = 0.0f;
                Rotate(HeldVel, AngleXY(l) - AngleXY(BoardForward) + 0.5f);
            }
            if (in.IsEventGameAction(A_ALTERNATE) && in.IsEventButtonPressed()) {
                AverageJoystickPos.X = 0.0f; AverageJoystickPos.Y = 0.0f;
                HeldVel.X = HeldVel.Y = 0.0f;
            }
            if ((action_held && in.IsEventGameAction(A_MOVEMENT) && in.GetControlMovement(A_MOVEMENT, movement))
                || (!action_held && Mag(HeldVel) >= def.JoystickMagBelow)) {
                if (!action_held && Mag(HeldVel) >= def.JoystickMagBelow) {
                    if (MousePosEffect.X == 0.0f && MousePosEffect.Y == -0.2f) MousePosEffect.Y = 0.0f;
                    ++sounds;
                    State = WATCH;
                } else {
                    MousePosEffect.X -= movement.X; MousePosEffect.Y -= movement.Y;
                    if (Mag(MousePosEffect) > def.MaxPower) {
                        float m = Mag(MousePosEffect);
                        MousePosEffect.X = MousePosEffect.X / m * def.MaxPower;
                        MousePosEffect.Y = MousePosEffect.Y / m * def.MaxPower;
                    }
                    movement = MousePosEffect;
                    if (Mag(movement) > 0.001) {
                        float m = Mag(movement);
                        float s = Max((float)fabs(movement.X / m), (float)fabs(movement.Y / m));
                        movement.X *= s; movement.Y *= s;
                    }
                    AverageJoystickPos.X = AverageJoystickPos.X * (1.0f - def.AverageDrift) + movement.X * def.AverageDrift;
                    AverageJoystickPos.Y = AverageJoystickPos.Y * (1.0f - def.AverageDrift) + movement.Y * def.AverageDrift;
                    HeldVel.X = AverageJoystickPos.X * def.JoystickScaleX;
                    HeldVel.Y = AverageJoystickPos.Y * def.JoystickScaleY;
                    V3 c, l; GetCamera(c, l);
                    l.X -= c.X; l.Y -= c.Y; l.Z = 0.0f;
                    Rotate(HeldVel, AngleXY(l) - AngleXY(BoardForward) + 0.5f);
                }
                ++performed;
            }
            break;
        case WATCH:
            break;
        default:
            ++base_calls;
            break;
        }
        if (State >= AIM && State <= WATCH) {
            if (in.IsEventGameAction(A_THIRD) || in.IsEventGameAction(A_QUIT)) {
                ++quit_prompts; ++performed; ++coin_sound_updates;
            }
        }
    }
};

static bool Near(float a, float b) { return fabs(a - b) < 1e-4f; }

static Game Fresh()
{
    Game g;
    g.State = AIM; g.MousePosEffect.X = 0.3f; g.MousePosEffect.Y = 0.1f; g.AverageJoystickPos.X = 0.1f; g.AverageJoystickPos.Y = 0.2f;
    g.HeldVel.X = 0.0f; g.HeldVel.Y = 0.0f; g.BoardForward.X = 0.0f; g.BoardForward.Y = 1.0f; g.BoardForward.Z = 0.0f;
    g.ZAngle = 0.25f; g.XAngle = 0.5f;
    g.CamPos.X = 0.0f; g.CamPos.Y = 0.0f; g.CamPos.Z = 2.0f; g.LookAt.X = 1.0f; g.LookAt.Y = 0.0f; g.LookAt.Z = 0.0f;
    g.performed = g.base_calls = g.sounds = g.quit_prompts = g.coin_sound_updates = g.rotates = 0; g.last_rotate = -1.0f;
    return g;
}
static Input In(bool held, int action, bool pressed, float mx, float my)
{
    Input i; i.held = held; i.action = action; i.pressed = pressed; i.has_movement = true; i.movement.X = mx; i.movement.Y = my;
    i.camera_manager = true; return i;
}

int main()
{
    Def def = { 2.0f, 3.0f, 0.5f, 0.25f, 0.8f };
    int bad = 0;
    // camera stick: magnitude-squared response, XAngle clamped into [0, 1]
    Game g = Fresh(); g.State = 5;
    g.ProcessInput(In(false, A_CAMERA, false, 0.6f, -0.8f), def);      // |m| = 1
    if (!Near(g.ZAngle, 0.25f + 0.6f * 0.04f) || !Near(g.XAngle, 0.5f + 0.8f * 0.08f) || g.performed != 1 || g.base_calls != 1) bad |= 1;
    g = Fresh(); g.XAngle = 0.99f; g.ProcessInput(In(false, A_CAMERA, false, 0.0f, -1.0f), def);
    if (!Near(g.XAngle, 1.0f)) bad |= 2;
    g = Fresh(); g.XAngle = 0.01f; g.ProcessInput(In(false, A_CAMERA, false, 0.0f, 1.0f), def);
    if (!Near(g.XAngle, 0.0f)) bad |= 4;
    g = Fresh(); g.ProcessInput(In(false, A_CAMERA, false, 0.01f, 0.01f), def);    // under the 0.04 dead zone
    if (!Near(g.ZAngle, 0.25f) || g.performed != 0) bad |= 8;
    // aim reset: joystick average and mouse effect to (0, -0.2), velocity scaled and turned by camera heading - board heading + 0.5
    g = Fresh(); g.ProcessInput(In(true, A_ACTION, true, 0.0f, 0.0f), def);
    if (!Near(g.AverageJoystickPos.Y, -0.2f) || !Near(g.MousePosEffect.Y, -0.2f) || !Near(g.HeldVel.Y, -0.6f) || g.rotates != 1
        || !Near(g.last_rotate, 0.25f - 0.0f + 0.5f)) bad |= 16;
    // alternate button zeroes the aim
    g = Fresh(); g.HeldVel.X = 0.1f; g.HeldVel.Y = 0.1f; g.ProcessInput(In(true, A_ALTERNATE, true, 0.0f, 0.0f), def);
    if (g.AverageJoystickPos.X != 0.0f || g.AverageJoystickPos.Y != 0.0f || g.HeldVel.X != 0.0f || g.HeldVel.Y != 0.0f) bad |= 32;
    // held movement: mouse effect -= movement, clamped to MaxPower, squared off, blended into the average
    g = Fresh(); g.ProcessInput(In(true, A_MOVEMENT, false, 0.3f, 0.1f), def);
    if (!Near(g.MousePosEffect.X, 0.0f) || !Near(g.MousePosEffect.Y, 0.0f) || !Near(g.AverageJoystickPos.X, 0.075f)
        || !Near(g.HeldVel.X, 0.15f) || g.performed != 1 || g.rotates != 1) bad |= 64;
    g = Fresh(); g.MousePosEffect.X = 0.0f; g.MousePosEffect.Y = 0.0f; g.ProcessInput(In(true, A_MOVEMENT, false, -1.0f, 0.0f), def);
    if (!Near(g.MousePosEffect.X, 0.8f) || !Near(g.AverageJoystickPos.X, 0.1f * 0.75f + 0.8f * 0.25f)) bad |= 128;
    // release with enough velocity: flick (an untouched aim's mouse effect Y is cleared), WATCH state
    g = Fresh(); g.MousePosEffect.X = 0.0f; g.MousePosEffect.Y = -0.2f; g.HeldVel.Y = -0.6f;
    g.ProcessInput(In(false, 0, false, 0.0f, 0.0f), def);
    if (g.State != WATCH || g.sounds != 1 || g.MousePosEffect.Y != 0.0f || g.performed != 1) bad |= 256;
    // release with too little velocity: nothing
    g = Fresh(); g.HeldVel.Y = -0.1f; g.ProcessInput(In(false, 0, false, 0.0f, 0.0f), def);
    if (g.State != AIM || g.sounds != 0 || g.performed != 0) bad |= 512;
    // quit while aiming or watching; not in other states
    g = Fresh(); g.State = WATCH; g.ProcessInput(In(true, A_QUIT, true, 0.0f, 0.0f), def);
    if (g.quit_prompts != 1 || g.coin_sound_updates != 1 || g.base_calls != 0) bad |= 1024;
    g = Fresh(); g.State = 15; g.ProcessInput(In(true, A_THIRD, true, 0.0f, 0.0f), def);
    if (g.quit_prompts != 0 || g.base_calls != 1) bad |= 2048;
    if (bad) { std::printf("007d3820_TEST FAIL %d\n", bad); return 1; }
    std::printf("007d3820_TEST PASS\n");
    return 0;
}
