// Behaviour test for CTCActionUseBase::CTCActionUseBase @ 0x004DA939: the owner reaches the base, the
// action-use def starts null and the component starts usable.
#include <cstdio>
#include <new>

struct CThing { int unused; };
struct CActionUseDef { int unused; };

struct CTCBase_Calls {
    virtual ~CTCBase_Calls() {}
    CThing* ParentThing;
    bool    ToBeRemoved;
    bool    IsInGlobalUpdateTCs;
    CTCBase_Calls(CThing* thing) : ParentThing(thing), ToBeRemoved(false), IsInGlobalUpdateTCs(false) {}
};

template<class T>
struct CDefPointer {
    const T* m_pDef;
    CDefPointer() : m_pDef(0) { }
};

struct CTCActionUseBase : CTCBase_Calls {
    CDefPointer<CActionUseDef> PActionUseDef;
    bool                       Usable;
    CTCActionUseBase(CThing* owner) : CTCBase_Calls(owner), Usable(true) {}
};

int main()
{
    CThing owner;
    unsigned char storage[sizeof(CTCActionUseBase)];
    for (unsigned i = 0; i < sizeof(storage); ++i) storage[i] = 0xCD;
    CTCActionUseBase* p = new (storage) CTCActionUseBase(&owner);
    bool ok = p->ParentThing == &owner && p->PActionUseDef.m_pDef == 0 && p->Usable;
    p->~CTCActionUseBase();
    if (!ok) { std::printf("004da939_TEST FAIL\n"); return 1; }
    std::printf("004da939_TEST PASS\n");
    return 0;
}