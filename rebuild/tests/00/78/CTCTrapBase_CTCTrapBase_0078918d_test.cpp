#include <cstdio>
#include <new>
class CThing { public: int id; };
class CTCBase { public: CThing* Thing; CTCBase(CThing* t) : Thing(t) { } virtual ~CTCBase() { } };
struct CIntelligentPointer { void* m_pItem; CIntelligentPointer() : m_pItem(0) { } virtual ~CIntelligentPointer() { } };
struct CRetailAllocator { CRetailAllocator() { } };
struct CRetailAllocProxy : CRetailAllocator { void* _M_data; CRetailAllocProxy(const CRetailAllocator& a, void* p) : CRetailAllocator(a), _M_data(p) { } };
struct CRetailVectorBase {
    void* _M_start; void* _M_finish; CRetailAllocProxy _M_end_of_storage;
    CRetailVectorBase(const CRetailAllocator& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { }
};
struct CRetailVector : CRetailVectorBase { explicit CRetailVector(const CRetailAllocator& a = CRetailAllocator()) : CRetailVectorBase(a) { } };
static int g_fades = 0;
class CFadeInFadeOutBase { public: int state; CFadeInFadeOutBase() : state(5) { ++g_fades; } };
struct CDefPointerTrapDef { const void* Object; CDefPointerTrapDef() : Object(0) { } };
class CTCTrapBase : public CTCBase { public:
    CRetailVector HitThings, ImmuneThings;
    CIntelligentPointer PObstruction;
    CRetailVector TriggerThings;
    CFadeInFadeOutBase TrapOpener;
    CDefPointerTrapDef PTrapDef;
    bool Active, Triggered, LastTriggerOnProximity;
    long TrapType;
    CTCTrapBase(CThing* thing) : CTCBase(thing), Triggered(false), LastTriggerOnProximity(false) { Active = true; }
};
int main()
{
    CThing owner; owner.id = 3;
    unsigned char raw[sizeof(CTCTrapBase)];
    for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0xAB;
    CTCTrapBase* t = new (raw) CTCTrapBase(&owner);
    if (t->Thing != &owner || t->HitThings._M_start || t->HitThings._M_finish || t->HitThings._M_end_of_storage._M_data
        || t->ImmuneThings._M_start || t->ImmuneThings._M_end_of_storage._M_data || t->TriggerThings._M_finish
        || t->PObstruction.m_pItem || t->PTrapDef.Object || !t->Active || t->Triggered || t->LastTriggerOnProximity
        || g_fades != 1 || t->TrapOpener.state != 5) { std::printf("0078918d_TEST FAIL\n"); return 1; }
    std::printf("0078918d_TEST PASS\n");
    return 0;
}