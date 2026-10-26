// Behaviour test for CTCSummonableCreature::CTCSummonableCreature (0x0079C240): members reset, then the summonable
// sub-def fetched from the parent thing's def into PDef.
#include <cstdio>

class CThing;
static CThing* g_baseParent = 0;
static int g_ipCtors = 0;
class CSummonableCreatureDef { public: int id; };
static CSummonableCreatureDef g_subDef;

class CTCBase { public:
    CThing* ParentThing;
    virtual ~CTCBase() { }
    CTCBase(CThing* parent) : ParentThing(parent) { g_baseParent = parent; }
};

struct CDefPointerSummonable {
    const CSummonableCreatureDef* Object;
    CDefPointerSummonable() : Object(0) { }
};

struct CThingDef {
    bool m_has;
    bool GetSubDef(CDefPointerSummonable* out) const { if (m_has) out->Object = &g_subDef; return m_has; }
};

class CThing { public:
    CThingDef* PDef;
};

struct CBaseIntelligentPointer {
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { ++g_ipCtors; }
};

struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() : CBaseIntelligentPointer() { }
};

class CTCSummonableCreature : public CTCBase { public:
    CDefPointerSummonable PDef;
    bool                  HasBeenSummoned;
    CIntelligentPointer   Summoner;
    CTCSummonableCreature(CThing* parent);
};

CTCSummonableCreature::CTCSummonableCreature(CThing* parent)
    : CTCBase(parent)
    , HasBeenSummoned(false)
{
    ParentThing->PDef->GetSubDef(&PDef);
}

int main()
{
    CThingDef def; def.m_has = true;
    CThing thing; thing.PDef = &def;
    CTCSummonableCreature c(&thing);
    if (g_baseParent != &thing || c.PDef.Object != &g_subDef || c.HasBeenSummoned || g_ipCtors != 1 || c.Summoner.m_pItem) {
        std::printf("0079c240_TEST FAIL\n"); return 1;
    }
    def.m_has = false;
    CTCSummonableCreature d(&thing);
    if (d.PDef.Object != 0) { std::printf("0079c240_TEST FAIL\n"); return 1; }
    std::printf("0079c240_TEST PASS\n");
    return 0;
}