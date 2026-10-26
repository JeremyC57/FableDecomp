#include <cstdio>

class CThing { public: int id; };
class CTCBase { public:
    virtual ~CTCBase() { }
    CThing* ParentThing; bool ToBeRemoved; bool IsInGlobalUpdateTCs;
    CTCBase(CThing* owner) : ParentThing(owner), ToBeRemoved(false), IsInGlobalUpdateTCs(false) { }
};
struct CBaseIntelligentPointer {
    virtual ~CBaseIntelligentPointer() { }
    virtual int Kind() const { return 0; }
    void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
};
struct CIntelligentPointer_CThingConst : CBaseIntelligentPointer {
    virtual int Kind() const { return 3; }
};
class CTCFlourishTarget : public CTCBase { public:
    CIntelligentPointer_CThingConst PAttacker;
    bool CullNegativeXAxisMovement;
    bool CullAllYAxisMovement;
    CTCFlourishTarget(CThing* owner)
        : CTCBase(owner), PAttacker(), CullNegativeXAxisMovement(true), CullAllYAxisMovement(true) { }
};

int main()
{
    CThing owner = { 1 };
    CTCFlourishTarget f(&owner);
    if (f.ParentThing != &owner || f.PAttacker.m_pItem != 0 || f.PAttacker.Kind() != 3 ||
        !f.CullNegativeXAxisMovement || !f.CullAllYAxisMovement)
        { std::printf("004d966f_TEST FAIL\n"); return 1; }
    std::printf("004d966f_TEST PASS\n");
    return 0;
}