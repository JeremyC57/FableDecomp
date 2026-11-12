// Behaviour test for CCreatureAction_TrollWhackGroundBase::WhackGround (0x008C61C0): a copy of the row's logic over a small model.
// Checks: the effect is created only when the sub-def exists, the action names an effect, the name maps to an index and the action
// has a strike-event helper name; its direction is the normalised Position - OldPosition (zero when the troll has not moved); the
// rumble (medium, short) is always started at the troll's position; the def reference taken by GetSubDef is released once.
#include <cstdio>
#include <cmath>
#include <cstring>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    const C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    const C3DVector operator*(float s) const { return C3DVector(X * s, Y * s, Z * s); }
    float GetAccurateMagnitude() const { float magnitude = X * X + Y * Y + Z * Z; return (float)sqrt(magnitude); }
};
static const float DELTA_NOTIONAL_ZERO = 0.0001f;
enum EQuakeStrength { QUAKE_STRENGTH_WEAK = 0, QUAKE_STRENGTH_MEDIUM = 1 };
enum EQuakeLength { QUAKE_LENGTH_SHORT = 0, QUAKE_LENGTH_MEDIUM = 1 };

struct SpecialEffectsDef {
    long RefCount;
    const char* KnownName;
    long KnownEffect;
    bool GetEffectFromString(const char* name, long& effect) const
    {
        if (KnownName && std::strcmp(KnownName, name) == 0) { effect = KnownEffect; return true; }
        return false;
    }
};
struct DefPointer {
    SpecialEffectsDef* Data;
    int* Released;
    DefPointer(int* released) : Data(0), Released(released) {}
    ~DefPointer() { if (Data && --Data->RefCount == 0) ++*Released; }
};
struct Creature {
    SpecialEffectsDef* SubDef;          // null: the creature def has no CSpecialEffectsDef
    C3DVector Position, OldPosition;
    C3DVector HelperPoint;
    bool GetSubDef(DefPointer* out) const
    {
        if (!SubDef) return false;
        ++SubDef->RefCount;
        out->Data = SubDef;
        return true;
    }
};
struct Log {
    int effects; long effect; C3DVector direction, pos;
    int rumbles; C3DVector rumblePos; EQuakeStrength strength; EQuakeLength length; bool rumbleFlag;
    int released;
};
struct Action {
    Creature* PCreature;
    const char* EffectName;             // null: GetEffectName returns false
    bool HasHelperName;
    Log* Out;
    bool GetEffectName(const char*& name) const { if (!EffectName) return false; name = EffectName; return true; }
    bool IsActionWithStrikeEventHelperName() const { return HasHelperName; }
    void WhackGround()
    {
        DefPointer def(&Out->released);
        const char* effectName = "";
        if (PCreature->GetSubDef(&def) && GetEffectName(effectName)) {
            long effect;
            if (def.Data->GetEffectFromString(effectName, effect) && IsActionWithStrikeEventHelperName()) {
                C3DVector movement = PCreature->Position - PCreature->OldPosition;
                float magnitude = movement.GetAccurateMagnitude();
                C3DVector direction = magnitude > DELTA_NOTIONAL_ZERO ? movement * (1.0f / magnitude) : C3DVector(0.0f, 0.0f, 0.0f);
                C3DVector pos = PCreature->HelperPoint;
                ++Out->effects; Out->effect = effect; Out->direction = direction; Out->pos = pos;
            }
        }
        ++Out->rumbles; Out->rumblePos = PCreature->Position; Out->strength = QUAKE_STRENGTH_MEDIUM;
        Out->length = QUAKE_LENGTH_SHORT; Out->rumbleFlag = false;
    }
};
static bool Near(float a, float b) { return fabs(a - b) < 1e-5f; }
static bool RumbleOk(const Log& log, const Creature& c)
{
    return log.rumbles == 1 && log.rumblePos.X == c.Position.X && log.rumblePos.Y == c.Position.Y && log.rumblePos.Z == c.Position.Z
        && log.strength == QUAKE_STRENGTH_MEDIUM && log.length == QUAKE_LENGTH_SHORT && !log.rumbleFlag;
}
int main()
{
    bool ok = true;
    SpecialEffectsDef sub = { 1, "TROLL_WHACK", 77 };
    Creature troll;
    troll.SubDef = &sub;
    troll.Position = C3DVector(4.0f, 6.0f, 1.0f);
    troll.OldPosition = C3DVector(1.0f, 2.0f, 1.0f);
    troll.HelperPoint = C3DVector(9.0f, 8.0f, 7.0f);
    Log log;
    Action action = { &troll, "TROLL_WHACK", true, &log };

    // 1. everything resolves: the effect faces (3,4,0)/5 at the helper point, then the rumble; the def reference is given back
    std::memset(&log, 0, sizeof(log));
    action.WhackGround();
    if (log.effects != 1 || log.effect != 77 || !Near(log.direction.X, 0.6f) || !Near(log.direction.Y, 0.8f) || !Near(log.direction.Z, 0.0f)
        || log.pos.X != 9.0f || log.pos.Y != 8.0f || log.pos.Z != 7.0f || !RumbleOk(log, troll) || sub.RefCount != 1 || log.released != 0)
        ok = false;

    // 2. the troll has not moved: the direction is the zero vector (no divide)
    troll.OldPosition = troll.Position;
    std::memset(&log, 0, sizeof(log));
    action.WhackGround();
    if (log.effects != 1 || log.direction.X != 0.0f || log.direction.Y != 0.0f || log.direction.Z != 0.0f || !RumbleOk(log, troll))
        ok = false;
    troll.OldPosition = C3DVector(1.0f, 2.0f, 1.0f);

    // 3. no strike-event helper name: no effect, rumble still starts
    action.HasHelperName = false;
    std::memset(&log, 0, sizeof(log));
    action.WhackGround();
    if (log.effects != 0 || !RumbleOk(log, troll) || sub.RefCount != 1) ok = false;
    action.HasHelperName = true;

    // 4. the name is not in the effects map
    action.EffectName = "UNKNOWN";
    std::memset(&log, 0, sizeof(log));
    action.WhackGround();
    if (log.effects != 0 || !RumbleOk(log, troll) || sub.RefCount != 1) ok = false;

    // 5. the action names no effect (the base GetEffectName returns false)
    action.EffectName = 0;
    std::memset(&log, 0, sizeof(log));
    action.WhackGround();
    if (log.effects != 0 || !RumbleOk(log, troll) || sub.RefCount != 1) ok = false;
    action.EffectName = "TROLL_WHACK";

    // 6. the creature def has no special-effects sub-def: nothing to release, rumble still starts
    troll.SubDef = 0;
    std::memset(&log, 0, sizeof(log));
    action.WhackGround();
    if (log.effects != 0 || !RumbleOk(log, troll) || log.released != 0) ok = false;

    // 7. the last reference held only by the action's pointer is released at the end
    troll.SubDef = &sub;
    sub.RefCount = 0;
    std::memset(&log, 0, sizeof(log));
    action.WhackGround();
    if (log.effects != 1 || log.released != 1 || sub.RefCount != 0) ok = false;

    if (!ok) { std::printf("008c61c0_TEST FAIL\n"); return 1; }
    std::printf("008c61c0_TEST PASS\n");
    return 0;
}