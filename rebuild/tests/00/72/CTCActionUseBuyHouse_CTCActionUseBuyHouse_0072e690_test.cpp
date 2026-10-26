// Behaviour test for CTCActionUseBuyHouse::CTCActionUseBuyHouse @ 0x0072E690: base state (null def, usable),
// null buy-house def, no pending confirmation, and the component joins the global update list once.
#include <cstdio>
#include <new>

class CThing { public: int unused; };
class CActionUseDef { public: int unused; };
class CBuyHouseDef { public: int unused; };

static int g_globalAdds = 0;

struct CTCBase_Calls {
    virtual ~CTCBase_Calls() {}
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CTCBase_Calls(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) {}
    void AddToGlobalUpdateTCs() { if (!IsInGlobalUpdateTCs) { IsInGlobalUpdateTCs = true; ++g_globalAdds; } }
};

template<class T>
class CDefPointer { public:
    const T* m_pDef;
    CDefPointer() : m_pDef(0) { }
};

class CTCActionUseBase : public CTCBase_Calls { public:
    CDefPointer<CActionUseDef> PActionUseDef;
    bool                       Usable;
    CTCActionUseBase(CThing* owner) : CTCBase_Calls(owner), Usable(true) { }
};

struct CTCActionUseBuyHouse : CTCActionUseBase {
    CDefPointer<CBuyHouseDef> PDef;
    bool                      WaitingForConfirmation;
    long                      HouseQuestion;
    CTCActionUseBuyHouse(CThing* owner) : CTCActionUseBase(owner), WaitingForConfirmation(false)
    {
        AddToGlobalUpdateTCs();
    }
};

int main()
{
    CThing owner;
    unsigned char storage[sizeof(CTCActionUseBuyHouse)];
    for (unsigned i = 0; i < sizeof(storage); ++i) storage[i] = 0xCD;
    CTCActionUseBuyHouse* p = new (storage) CTCActionUseBuyHouse(&owner);
    bool ok = p->ParentThing == &owner && p->PActionUseDef.m_pDef == 0 && p->Usable && p->PDef.m_pDef == 0
           && !p->WaitingForConfirmation && p->IsInGlobalUpdateTCs && g_globalAdds == 1;
    p->~CTCActionUseBuyHouse();
    if (!ok) { std::printf("0072e690_TEST FAIL\n"); return 1; }
    std::printf("0072e690_TEST PASS\n");
    return 0;
}