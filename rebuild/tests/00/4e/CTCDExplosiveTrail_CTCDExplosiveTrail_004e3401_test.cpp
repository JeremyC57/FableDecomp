#include <cstdio>
#include <cstdlib>
struct CThing { int id; };
struct CTCDriverBase { CThing* m_pThing; long m_baseWord; CTCDriverBase(CThing* o) : m_pThing(o), m_baseWord(0) { } virtual ~CTCDriverBase() { } };
struct CBaseIntelligentPointer { void* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { } virtual ~CBaseIntelligentPointer() { } };
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer { CIntelligentPointer() { } };
struct STrapAllocator { STrapAllocator() { } };
struct STrapNode { STrapNode* next; STrapNode* prev; long value[3]; };
struct CTrapList {
    STrapNode* _Head;
    explicit CTrapList(const STrapAllocator& = STrapAllocator()) { _Head = 0; STrapNode* n = (STrapNode*)std::malloc(sizeof(STrapNode)); n->next = n; n->prev = n; _Head = n; }
    ~CTrapList() { std::free(_Head); }
};
struct CThingPointerVector { void* _M_start; void* _M_finish; void* _M_end_of_storage; CThingPointerVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { } };
struct CExplosiveTrailDefPointer { const void* m_pDef; CExplosiveTrailDefPointer() : m_pDef(0) { } };
struct CTCDExplosiveTrail : CTCDriverBase {
    CIntelligentPointer<const CThing> PTarget; long Timer; bool Terminated; CTrapList ComponentTraps; CThingPointerVector ThingsToIgnore; CExplosiveTrailDefPointer PDef;
    CTCDExplosiveTrail(CThing* owner) : CTCDriverBase(owner), Timer(-1), Terminated(false) { }
};
int main()
{
    CThing t; t.id = 9;
    CTCDExplosiveTrail* e = new CTCDExplosiveTrail(&t);
    if (e->m_pThing != &t || e->PTarget.m_pItem || e->Timer != -1 || e->Terminated || !e->ComponentTraps._Head || e->ComponentTraps._Head->next != e->ComponentTraps._Head
        || e->ThingsToIgnore._M_start || e->ThingsToIgnore._M_end_of_storage || e->PDef.m_pDef) { std::printf("004e3401_TEST FAIL\n"); return 1; }
    delete e;
    std::printf("004e3401_TEST PASS\n");
    return 0;
}