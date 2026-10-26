// Behaviour test for CAIStateGroup_MinionBehaviourSad::FoundDeadSpouse (0x0096F180): model and body verbatim from
// final.cpp with the out-of-line callees stubbed; checks the 10x10 box, the filter creature, the out pointer and
// that PDeadSpouse is set only on a hit.
#include <stdio.h>
#ifndef _MSC_VER
#define _snprintf snprintf
#define __forceinline inline
#endif
class CTCBase;
class CThing;
class CWorld;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    unsigned char _pad1[0x45];
    unsigned char m_flags91;
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase() {}
    virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {}
    virtual bool IsStillValid() { return true; }
    virtual void v06() {} virtual void v07() {} virtual void v08() {}
    virtual void Cleanup() {}
    virtual void OnReplace() {}
    virtual bool IsRunnable() { return true; }
    virtual void Update() {}
    virtual void InitStateGroup() {}
    CAIBrain* PBrain;
    long ScriptAIPriority;
    void* Name_PStringData;
    unsigned char Terminated;
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
};

static int g_ctor = 0, g_dtor = 0, g_set = 0;
struct CBaseIntelligentPointerCtor {
    virtual void v00() {}
    CThing* m_pItem;
    CBaseIntelligentPointerCtor() : m_pItem(0) { g_ctor++; }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; g_set++; }
    ~CBaseIntelligentPointerCtor() { g_dtor++; m_pItem = 0; }
};
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    CIntelligentPointerThing() {}
    __forceinline CIntelligentPointerThing(CThing* const& item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

class C3DVector { public: float X, Y, Z; };
class C2DBoxI { public: long MinX, MinY, MaxX, MaxY; };

static C3DVector g_pos;
struct CThingCreaturePos {
    const C3DVector& GetPosition() { return g_pos; }
};
struct CThingFilter_IsDeadSpouse {
    CThing* PCreature;
};

static C2DBoxI g_box; static CThing* g_filterCreature = 0; static CThing* g_hit = 0; static int g_search = 0;
class CThingSearchTools { public:
    bool GetPFirstThingInRoughArea(const C2DBoxI& box, const CThingFilter_IsDeadSpouse& filter, CThing** ppThing)
    {
        g_search++; g_box = box; g_filterCreature = filter.PCreature;
        if (g_hit) { *ppThing = g_hit; return true; }
        return false;
    }
};
class CWorld { public:
    unsigned char _pad[0x20];
    CThingSearchTools* PThingSearchTools;
};
static CWorld g_world; static CThingSearchTools g_tools;

struct CAIStateGroup_MinionBehaviourSad : CAIStateGroupBase {
    unsigned char _pad_0x14[0x20];
    CIntelligentPointerThing PDeadSpouse;
    CWorld* GetPWorld() { return &g_world; }
    bool FoundDeadSpouse();
};

bool CAIStateGroup_MinionBehaviourSad::FoundDeadSpouse()
{
    CThing* spouse;
    bool found;
    {
        const C3DVector& pos = ((CThingCreaturePos*)PBrain->PCreature)->GetPosition();
        long x = (long)pos.X;
        long y = (long)pos.Y;
        C2DBoxI box;
        box.MinX = x - 5;
        box.MinY = y - 5;
        box.MaxX = x + 5;
        box.MaxY = y + 5;
        CThingFilter_IsDeadSpouse filter;
        filter.PCreature = PBrain->PCreature;
        CThingSearchTools* tools = GetPWorld()->PThingSearchTools;
        found = tools->GetPFirstThingInRoughArea(box, filter, &spouse);
    }
    if (found) {
        PDeadSpouse = spouse;
        return true;
    }
    return false;
}

static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail++; printf("FAIL line %d: %s\n", __LINE__, #c); } } while (0)
int main()
{
    static CThing creature, corpse;
    CAIBrain brain; brain.PCreature = &creature;
    g_world.PThingSearchTools = &g_tools;
    g_pos.X = 12.7f; g_pos.Y = -3.2f; g_pos.Z = 1.0f;
    CAIStateGroup_MinionBehaviourSad group; group.PBrain = &brain;
    int ctor0 = g_ctor, dtor0 = g_dtor;
    g_hit = 0;
    CHECK(group.FoundDeadSpouse() == false);
    CHECK(g_search == 1 && g_box.MinX == 7 && g_box.MinY == -8 && g_box.MaxX == 17 && g_box.MaxY == 2);
    CHECK(g_filterCreature == &creature && group.PDeadSpouse.GetPItem() == 0);
    CHECK(g_ctor == ctor0 && g_dtor == dtor0);        // no temporary on a miss
    g_hit = &corpse;
    CHECK(group.FoundDeadSpouse() == true);
    CHECK(g_search == 2 && group.PDeadSpouse.GetPItem() == &corpse);
    CHECK(g_ctor == ctor0 + 1 && g_dtor == dtor0 + 1); // one converting temporary, built and destroyed
    char line[64];
    _snprintf(line, sizeof(line), "0096f180_TEST %s", g_fail ? "FAIL" : "PASS");
    puts(line);
    return g_fail ? 1 : 0;
}