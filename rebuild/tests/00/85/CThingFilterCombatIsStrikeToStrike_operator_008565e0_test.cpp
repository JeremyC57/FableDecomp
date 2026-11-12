// Behaviour test for 0x008565E0 CThingFilter_CombatIsStrikeToStrike::operator(): a standalone copy of the model that
// exercises every arm (mode rejections, the facing test, the vulnerability result, the non-creature arm) and the def counts.
#include <cstdio>

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};
static int g_deleted = 0;
struct Def {
    unsigned long IVRefCount;
    bool AlwaysIncludeInObjectStrikeScans;
    bool DeferHitsIfBehindHitter;
    Def() : IVRefCount(1), AlwaysIncludeInObjectStrikeScans(false), DeferHitsIfBehindHitter(false) {}
    void Delete() { ++g_deleted; }
};
struct DefPointer {
    Def* Object;
    DefPointer() : Object(0) {}
    DefPointer(const DefPointer& o) : Object(o.Object) { if (Object) ++Object->IVRefCount; }
    ~DefPointer() { Release(); }
    void Release() { if (Object) { Def* o = Object; if (--o->IVRefCount == 0) o->Delete(); Object = 0; } }
    void SetDynamicCast(const DefPointer& o)
    {
        if (Object != o.Object) { Release(); Object = o.Object; if (Object) ++Object->IVRefCount; }
    }
    Def* operator->() const { return Object; }
    bool IsNull() const { return Object == 0; }
};
struct Thing {
    unsigned long Type;
    unsigned long TypeFlags;
    Def* BaseDef;
    C3DVector Pos, Facing;
    bool EvadeAll, Rolling, Vulnerable;
    Thing() : Type(0), TypeFlags(0), BaseDef(0), Pos(0, 0, 0), Facing(1, 0, 0), EvadeAll(false), Rolling(false), Vulnerable(false) {}
    DefPointer GetPBaseDef() const { DefPointer p; p.Object = BaseDef; if (BaseDef) ++BaseDef->IVRefCount; return p; }
};
static int g_vulnerableCalls = 0;
static bool IsVulnerable(const Thing& thing, const Thing&) { ++g_vulnerableCalls; return thing.Vulnerable; }

struct Filter {
    Thing* ThingStriking;
    bool operator()(const Thing& thing) const
    {
        if (thing.TypeFlags & 8) {
            if (thing.EvadeAll || thing.Rolling)
                return false;
            DefPointer def;
            def.SetDynamicCast(thing.GetPBaseDef());
            if (!def.IsNull() && def->DeferHitsIfBehindHitter) {
                const C3DVector& pos = thing.Pos;
                C3DVector delta = pos - ThingStriking->Pos;
                C3DVector facing = ThingStriking->Facing;
                if (facing.X * delta.X + delta.Y * facing.Y + delta.Z * facing.Z < 0.0f)
                    return false;
            }
            if (!(thing.TypeFlags & 8))
                return true;
            return IsVulnerable(thing, *ThingStriking);
        }
        if (ThingStriking->Type == 2)
            return true;
        if (thing.GetPBaseDef()->AlwaysIncludeInObjectStrikeScans)
            return true;
        return false;
    }
};

int main()
{
    int bad = 0;
    Thing striker; striker.Pos = C3DVector(0, 0, 0); striker.Facing = C3DVector(0, 1, 0);
    Filter f; f.ThingStriking = &striker;
    Def def;
    Thing t; t.BaseDef = &def;

    // non-creature: striker type 2 passes without touching the def
    striker.Type = 2; if (!f(t)) bad |= 1;
    // non-creature: the def flag decides
    striker.Type = 1; if (f(t)) bad |= 2;
    def.AlwaysIncludeInObjectStrikeScans = true; if (!f(t)) bad |= 4;
    if (def.IVRefCount != 1 || g_deleted != 0) bad |= 8;

    // creature: mode rejections come first and never ask about vulnerability
    t.TypeFlags = 8; t.Vulnerable = true;
    t.EvadeAll = true; if (f(t)) bad |= 0x10;
    t.EvadeAll = false; t.Rolling = true; if (f(t)) bad |= 0x20;
    t.Rolling = false; if (g_vulnerableCalls != 0) bad |= 0x40;
    // creature without the defer flag: the vulnerability result
    if (!f(t)) bad |= 0x80;
    t.Vulnerable = false; if (f(t)) bad |= 0x100;
    t.Vulnerable = true;
    // defer flag: in front of the striker (positive dot) passes, behind (negative dot) fails, level (zero) passes
    def.DeferHitsIfBehindHitter = true;
    t.Pos = C3DVector(3, 5, -1); if (!f(t)) bad |= 0x200;
    int calls = g_vulnerableCalls;
    t.Pos = C3DVector(3, -5, 1); if (f(t)) bad |= 0x400;
    if (g_vulnerableCalls != calls) bad |= 0x800;
    t.Pos = C3DVector(7, 0, 2); if (!f(t)) bad |= 0x1000;
    // all three components take part
    striker.Facing = C3DVector(1, 1, 1); t.Pos = C3DVector(1, 1, -3); if (f(t)) bad |= 0x2000;
    t.Pos = C3DVector(1, 1, -1); if (!f(t)) bad |= 0x4000;
    // a creature with no def skips the facing test
    t.BaseDef = 0; t.Pos = C3DVector(-9, -9, -9); if (!f(t)) bad |= 0x8000;
    // every path balanced the def's count
    if (def.IVRefCount != 1 || g_deleted != 0) bad |= 0x10000;

    if (bad) { std::printf("008565e0_TEST FAIL\n"); return 1; }
    std::printf("008565e0_TEST PASS\n");
    return 0;
}