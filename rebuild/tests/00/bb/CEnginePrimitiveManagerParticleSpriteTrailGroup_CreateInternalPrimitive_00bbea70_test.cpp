// Behaviour test for 0x00BBEA70 (see src/00bbea70.cpp): CreateInternalPrimitive building an empty sprite-trail group.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("00bbea70_TEST FAIL\n"); return 1; } } while (0)

static int g_mallocs = 0;
static void* Malloc(unsigned int n) { ++g_mallocs; return std::malloc(n); }
struct CEngineInternalPrimitiveBase { unsigned int Type; unsigned int Flags; CEngineInternalPrimitiveBase(unsigned int t, unsigned int f) : Type(t), Flags(f) { } virtual ~CEngineInternalPrimitiveBase() { } };
template<class T> struct CRetailList {
    struct SNode { SNode* m_pNext; SNode* m_pPrev; T m_value; };
    SNode* m_pNode;
    CRetailList() : m_pNode(0) { SNode* node = (SNode*)Malloc(sizeof(SNode)); node->m_pNext = node; node->m_pPrev = node; m_pNode = node; }
    ~CRetailList() { std::free(m_pNode); }
    bool empty() const { return m_pNode->m_pNext == m_pNode && m_pNode->m_pPrev == m_pNode; }
};
struct CIteratorArray { void* m_pFirst; void* m_pLast; void* m_pEnd; CIteratorArray() : m_pFirst(0), m_pLast(0), m_pEnd(0) { } };
struct C3DBoundingSphere { unsigned int x, y, z, r; C3DBoundingSphere() : x(0), y(0), z(0), r(0) { } };
struct CEnginePrimitiveHandle { void* a; void* b; };
struct CEngineInternalPrimitiveParticleGroup : CEngineInternalPrimitiveBase {
    CRetailList<CEnginePrimitiveHandle> Primitives; CIteratorArray PrimitiveIDToIterator; CRetailList<int> FreePrimitiveIDs; C3DBoundingSphere BSphere;
    CEngineInternalPrimitiveParticleGroup(unsigned int type, unsigned int flags) : CEngineInternalPrimitiveBase(type, flags) { }
};
static bool GroupEmpty(const CEngineInternalPrimitiveParticleGroup& g)
{
    return g.Primitives.empty() && g.FreePrimitiveIDs.empty() && g.PrimitiveIDToIterator.m_pFirst == 0 && g.PrimitiveIDToIterator.m_pLast == 0
        && g.PrimitiveIDToIterator.m_pEnd == 0 && g.BSphere.x == 0 && g.BSphere.y == 0 && g.BSphere.z == 0 && g.BSphere.r == 0;
}

struct CEngineInternalPrimitiveParticleSpriteTrailGroup : CEngineInternalPrimitiveParticleGroup {
    unsigned char _trail[0x10];
    CEngineInternalPrimitiveParticleSpriteTrailGroup() : CEngineInternalPrimitiveParticleGroup(0x1d, 0x88000) { }
};
struct CEnginePrimitiveManagerParticleSpriteTrailGroup {
    CEngineInternalPrimitiveBase* CreateInternalPrimitive(int) { return new CEngineInternalPrimitiveParticleSpriteTrailGroup; }
};
int main()
{
    CEnginePrimitiveManagerParticleSpriteTrailGroup m;
    CEngineInternalPrimitiveParticleSpriteTrailGroup* g = (CEngineInternalPrimitiveParticleSpriteTrailGroup*)m.CreateInternalPrimitive(0);
    CHECK(g != 0 && g->Type == 0x1d && g->Flags == 0x88000 && GroupEmpty(*g) && g_mallocs == 2);
    delete g;
    std::printf("00bbea70_TEST PASS\n");
    return 0;
}