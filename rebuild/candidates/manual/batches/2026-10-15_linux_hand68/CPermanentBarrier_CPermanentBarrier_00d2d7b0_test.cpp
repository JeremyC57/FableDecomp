// Behaviour test for CPermanentBarrier's constructor (0x00D2D7B0): quest interface from the parent, the thing copied,
// parent and master data stored, both thing vectors empty.
#include <cstdio>
static int g_thingCopies = 0;
struct CScriptThing { long id; CScriptThing() : id(0) { } CScriptThing(const CScriptThing& o) : id(o.id) { ++g_thingCopies; } };
struct CGameScriptInterface { int x; };
struct CParent { CGameScriptInterface* GSI; };
struct CMasterData { int y; };
struct CRoot { CGameScriptInterface* PQuest; CRoot(CGameScriptInterface* q) : PQuest(q) { } virtual ~CRoot() { } };
struct CEntityBase : CRoot { CScriptThing Thing; CEntityBase(CParent* p, const CScriptThing& t) : CRoot(p->GSI), Thing(t) { } };
struct CVec { void* a; void* b; void* c; CVec() : a(0), b(0), c(0) { } };
struct CPermanentBarrier : CEntityBase {
    CParent* ParentClass; CMasterData* PMasterData; CVec particle_effectVector; CVec MyMarkerVector;
    CPermanentBarrier(CParent* parent, CMasterData* masterData, const CScriptThing& thing) : CEntityBase(parent, thing), ParentClass(parent)
    {
        PMasterData = masterData;
    }
};
int main()
{
    CGameScriptInterface gsi; CParent parent; parent.GSI = &gsi; CMasterData md; CScriptThing thing; thing.id = 42;
    CPermanentBarrier b(&parent, &md, thing);
    bool ok = b.PQuest == &gsi && b.Thing.id == 42 && g_thingCopies == 1 && b.ParentClass == &parent && b.PMasterData == &md
           && b.particle_effectVector.a == 0 && b.particle_effectVector.c == 0 && b.MyMarkerVector.a == 0 && b.MyMarkerVector.c == 0;
    if (!ok) { std::printf("00d2d7b0_TEST FAIL\n"); return 1; }
    std::printf("00d2d7b0_TEST PASS\n");
    return 0;
}
