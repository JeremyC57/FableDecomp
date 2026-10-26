// Behaviour test for 00bde0c0 CQuadTreeElement::RelocateData (copy of the model; the manager and the child's
// OnPostRelocation are recording stand-ins).
#include <cstdio>
static int g_rebinds = 0, g_childPosts = 0;
static const void* g_lastBlock = 0;
static const void* g_lastOwner = 0;
static const void* g_lastChild = 0;
class CMovableResource { public: virtual ~CMovableResource() {} };
class CEngineLandscapeMemoryManager { public:
    void RebindBlockOwner(void* block, CMovableResource* owner) { ++g_rebinds; g_lastBlock = block; g_lastOwner = owner; }
};
struct SEngine_01436ea8 { unsigned char _pad[0x6b0]; CEngineLandscapeMemoryManager* m_pLandscapeMemory; };
SEngine_01436ea8* g_pEngine_01436ea8;
template <class T> inline void RelocatePointer(T*& p, void* oldBase, void* newBase)
{
    if (p)
        p = (T*)((long)p + ((long)newBase - (long)oldBase));
}
struct CObjectTypeCollection { void* vt; void* m_pType; void* m_pFirstPrimitive; CObjectTypeCollection* Next; };
struct CObjectCacheGroupCollection : CMovableResource {
    CObjectCacheGroupCollection* Next;
    unsigned char BoundingSphere[0x10];
    float MaxFadeDistance;
    unsigned long PrimitiveTypeMask;
    CObjectTypeCollection* ObjectTypeCollections;
    void OnPostRelocation()
    {
        CEngineLandscapeMemoryManager* manager = g_pEngine_01436ea8->m_pLandscapeMemory;
        for (CObjectTypeCollection* collection = ObjectTypeCollections; collection; collection = collection->Next)
            manager->RebindBlockOwner(collection, this);
    }
};
struct CQuadTreeElement : CMovableResource {
    CObjectCacheGroupCollection* ObjectCacheGroupCollections;
    CQuadTreeElement* Children[4];
    void OnPostRelocation() { ++g_childPosts; g_lastChild = this; }
    void RelocateData(long blockType, void* oldBase, void* newBase, long size);
};
void CQuadTreeElement::RelocateData(long blockType, void* oldBase, void* newBase, long size)
{
    for (long i = 0; i < 4; ++i) {
        if (oldBase == Children[i]) {
            RelocatePointer(Children[i], oldBase, newBase);
            Children[i]->OnPostRelocation();
            return;
        }
    }
    CObjectCacheGroupCollection* prev = 0;
    for (CObjectCacheGroupCollection* p = ObjectCacheGroupCollections; p; p = p->Next) {
        if (p == oldBase) {
            RelocatePointer(p, oldBase, newBase);
            if (prev)
                prev->Next = p;
            else
                ObjectCacheGroupCollections = p;
            p->OnPostRelocation();
            return;
        }
        prev = p;
    }
}
int main()
{
    static SEngine_01436ea8 engine; static CEngineLandscapeMemoryManager mgr;
    engine.m_pLandscapeMemory = &mgr; g_pEngine_01436ea8 = &engine;
    CQuadTreeElement root, c0, c2, c2moved;
    root.Children[0] = &c0; root.Children[1] = 0; root.Children[2] = &c2; root.Children[3] = 0;
    CObjectTypeCollection tA, tB; tA.Next = &tB; tB.Next = 0;
    CObjectCacheGroupCollection g1, g2, g2moved;
    g1.Next = &g2; g2.Next = 0; g1.ObjectTypeCollections = 0; g2.ObjectTypeCollections = &tA;
    root.ObjectCacheGroupCollections = &g1;
    int ok = 1;
    root.RelocateData(0, &c2, &c2moved, sizeof(c2));                   // a child
    if (root.Children[2] != &c2moved || g_childPosts != 1 || g_lastChild != &c2moved || g_rebinds != 0) ok = 0;
    g2moved.Next = 0; g2moved.ObjectTypeCollections = &tA;
    root.RelocateData(0, &g2, &g2moved, sizeof(g2));                   // a group (second in the chain)
    if (g1.Next != &g2moved || g_rebinds != 2 || g_lastBlock != &tB || g_lastOwner != (CMovableResource*)&g2moved) ok = 0;
    root.RelocateData(0, &tA, &tB, 4);                                 // unknown block: no effect
    if (g_rebinds != 2 || g_childPosts != 1) ok = 0;
    if (!ok) { std::printf("00bde0c0_TEST FAIL\n"); return 1; }
    std::printf("00bde0c0_TEST PASS\n");
    return 0;
}