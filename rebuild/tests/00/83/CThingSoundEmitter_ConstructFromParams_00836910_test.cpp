// Behaviour test for CThingSoundEmitter::ConstructFromParams (0x00836910).
#include <cstdio>
struct C3DVector { float x, y, z; };
class CThing;

static long g_baseArgs[4];
static bool g_baseResult = true;
static const C3DVector* g_queried = 0;

struct CWorldMap { long GetMapNumberAt(const C3DVector& pos) const { g_queried = &pos; return 42; } };
struct CThingManager { CWorldMap* WorldMap; };
static CWorldMap g_map;
static CThingManager g_mgr = { &g_map };
static CThingManager* g_pThingManager = &g_mgr;

struct CBaseIntelligentPointer {
    CThing* PItem;
    CBaseIntelligentPointer() : PItem(0) {}
    CThing* GetPItem() const { return PItem; }
    void SetPItem(CThing* item) { PItem = item; }
};
struct CThingPointer : CBaseIntelligentPointer {
    CThingPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CThingPointer& operator=(const CThingPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class CThing {
public:
    int dummy;
protected:
    bool ConstructFromParams(long defIndex, long player, long map, bool levelPersistent)
    {
        g_baseArgs[0] = defIndex; g_baseArgs[1] = player; g_baseArgs[2] = map; g_baseArgs[3] = levelPersistent;
        return g_baseResult;
    }
};

class CThingSoundEmitter : public CThing {
public:
    CThingSoundEmitter() : AttachedToThing(false), PAttachedThing(0), ContinuePlayingAfterAttachedThingDeath(false) { Pos.x = Pos.y = Pos.z = 0; }
    bool           AttachedToThing;
    CThingPointer  PAttachedThing;
    bool           ContinuePlayingAfterAttachedThingDeath;
    C3DVector      Pos;
    bool Construct(const C3DVector& pos, const CThing* attachedThing, bool continuePlaying)
    {
        if (CThing::ConstructFromParams(0, 4, g_pThingManager->WorldMap->GetMapNumberAt(pos), false)) {
            if (attachedThing) {
                PAttachedThing = (CThing*)attachedThing;
                AttachedToThing = true;
            } else {
                PAttachedThing = 0;
                AttachedToThing = false;
            }
            ContinuePlayingAfterAttachedThingDeath = continuePlaying;
            Pos = pos;
            return true;
        }
        return false;
    }
};

int main()
{
    C3DVector p = { 1.0f, 2.0f, 3.0f };
    CThingSoundEmitter other;
    CThingSoundEmitter e;
    if (!e.Construct(p, &other, true)) { std::printf("00836910_TEST FAIL\n"); return 1; }
    if (g_queried != &p || g_baseArgs[0] != 0 || g_baseArgs[1] != 4 || g_baseArgs[2] != 42 || g_baseArgs[3] != 0) { std::printf("00836910_TEST FAIL\n"); return 1; }
    if (!e.AttachedToThing || e.PAttachedThing.GetPItem() != (CThing*)&other || !e.ContinuePlayingAfterAttachedThingDeath) { std::printf("00836910_TEST FAIL\n"); return 1; }
    if (e.Pos.x != 1.0f || e.Pos.y != 2.0f || e.Pos.z != 3.0f) { std::printf("00836910_TEST FAIL\n"); return 1; }
    C3DVector q = { 4.0f, 5.0f, 6.0f };
    if (!e.Construct(q, 0, false) || e.AttachedToThing || e.PAttachedThing.GetPItem() != 0 || e.ContinuePlayingAfterAttachedThingDeath || e.Pos.z != 6.0f) { std::printf("00836910_TEST FAIL\n"); return 1; }
    g_baseResult = false;
    C3DVector r = { 7.0f, 8.0f, 9.0f };
    if (e.Construct(r, &other, true) || e.AttachedToThing || e.Pos.x != 4.0f) { std::printf("00836910_TEST FAIL\n"); return 1; }
    std::printf("00836910_TEST PASS\n");
    return 0;
}