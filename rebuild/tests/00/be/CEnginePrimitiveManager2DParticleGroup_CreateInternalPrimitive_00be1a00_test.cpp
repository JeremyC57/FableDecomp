// Behaviour test for CEnginePrimitiveManager2DParticleGroup::CreateInternalPrimitive @ 0x00BE1A00.
#include <cstdio>
#include <cstdlib>
class CEngineInternalPrimitiveBase { public:
    long m_type; unsigned int m_flags;
    CEngineInternalPrimitiveBase(long t, unsigned int f) : m_type(t), m_flags(f) { }
    virtual int Kind() { return 0; }
    virtual ~CEngineInternalPrimitiveBase() { }
};
template <class T> struct CRetailSTLportList {
    struct SNode { SNode* next; SNode* prev; T value; };
    SNode* m_node;
    CRetailSTLportList() { m_node = 0; SNode* n = (SNode*)std::malloc(sizeof(SNode)); n->next = n; n->prev = n; m_node = n; }
    ~CRetailSTLportList() { std::free(m_node); }
};
template <class T> class CArray { public: T* f; T* l; T* e; CArray() : f(0), l(0), e(0) { } };
struct CEngineInternalPrimitiveParticleGroup : CEngineInternalPrimitiveBase {
    CRetailSTLportList<long long> Primitives; CArray<int> PrimitiveIDToIterator; CRetailSTLportList<long> FreePrimitiveIDs;
    virtual int Kind() { return 1; }
    CEngineInternalPrimitiveParticleGroup() : CEngineInternalPrimitiveBase(0x20, 0x80000000u) { }
};
struct CEngineInternalPrimitive2DParticleGroup : CEngineInternalPrimitiveParticleGroup {
    long m_5c, m_60, m_64, m_68;
    virtual int Kind() { return 2; }
    CEngineInternalPrimitive2DParticleGroup() : m_5c(0), m_60(0), m_64(0), m_68(0) { }
};
class CEnginePrimitiveManager2DParticleGroup { public:
    CEngineInternalPrimitiveBase* CreateInternalPrimitive(long) { return new CEngineInternalPrimitive2DParticleGroup; }
};
int main()
{
    CEnginePrimitiveManager2DParticleGroup m;
    CEngineInternalPrimitive2DParticleGroup* g = (CEngineInternalPrimitive2DParticleGroup*)m.CreateInternalPrimitive(5);
    int ok = g && g->Kind() == 2 && g->m_type == 0x20 && g->m_flags == 0x80000000u
          && g->Primitives.m_node && g->Primitives.m_node->next == g->Primitives.m_node && g->Primitives.m_node->prev == g->Primitives.m_node
          && g->FreePrimitiveIDs.m_node->next == g->FreePrimitiveIDs.m_node && g->PrimitiveIDToIterator.f == 0 && g->PrimitiveIDToIterator.e == 0
          && g->m_5c == 0 && g->m_60 == 0 && g->m_64 == 0 && g->m_68 == 0;
    delete g;
    if (!ok) { std::printf("00be1a00_TEST FAIL\n"); return 1; }
    std::printf("00be1a00_TEST PASS\n");
    return 0;
}