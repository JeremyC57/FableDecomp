#include <cstdio>
class CThing { public: int id; };
class CTCBase { public: CThing* m_pThing; long m_baseWord; CTCBase(CThing* o) : m_pThing(o), m_baseWord(0) { } virtual ~CTCBase() { } };
struct CBaseIntelligentPointer { void* m_pItem; CBaseIntelligentPointer() : m_pItem(0) { } virtual ~CBaseIntelligentPointer() { } };
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public: CIntelligentPointer() { } };
struct CLightningDefPointer { const void* m_pDef; CLightningDefPointer() : m_pDef(0) { } };
struct CLightningBranchVector { void* _M_start; void* _M_finish; void* _M_end_of_storage; CLightningBranchVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { } };
class CTCLightningNode : public CTCBase { public:
    CIntelligentPointer<CThing> PParent; CIntelligentPointer<const CThing> PSourceOfFaction; CIntelligentPointer<CThing> PSourceOfHit;
    CLightningDefPointer PDef; long Timer; CLightningBranchVector LightningBranches;
    CTCLightningNode(CThing* owner) : CTCBase(owner), Timer(0) { }
};
int main()
{
    CThing t; t.id = 1;
    CTCLightningNode* n = new CTCLightningNode(&t);
    if (n->m_pThing != &t || n->PParent.m_pItem || n->PSourceOfFaction.m_pItem || n->PSourceOfHit.m_pItem || n->PDef.m_pDef || n->Timer != 0
        || n->LightningBranches._M_start || n->LightningBranches._M_finish || n->LightningBranches._M_end_of_storage) { std::printf("004e6d17_TEST FAIL\n"); return 1; }
    delete n;
    std::printf("004e6d17_TEST PASS\n");
    return 0;
}