// Behaviour test for 00bddee0 CObjectCacheGroupCollection::RelocateData (copy of the model; the type
// collection's OnPostRelocation is a recording stand-in).
#include <cstdio>
static int g_posts = 0;
static const void* g_post = 0;
template <class T> inline void RelocatePointer(T*& p, void* oldBase, void* newBase)
{
    if (p)
        p = (T*)((long)p + ((long)newBase - (long)oldBase));
}
struct CObjectTypeCollection {
    virtual ~CObjectTypeCollection() {}
    void* m_pType;
    void* m_pFirstPrimitive;
    CObjectTypeCollection* Next;
    void OnPostRelocation() { ++g_posts; g_post = this; }
};
struct CObjectCacheGroupCollection {
    virtual ~CObjectCacheGroupCollection() {}
    CObjectCacheGroupCollection* Next;
    unsigned char BoundingSphere[0x10];
    float MaxFadeDistance;
    unsigned long PrimitiveTypeMask;
    CObjectTypeCollection* ObjectTypeCollections;
    void RelocateData(long blockType, void* oldBase, void* newBase, long size);
};
void CObjectCacheGroupCollection::RelocateData(long blockType, void* oldBase, void* newBase, long size)
{
    CObjectTypeCollection* prev = 0;
    for (CObjectTypeCollection* p = ObjectTypeCollections; p; p = p->Next) {
        if (p == oldBase) {
            RelocatePointer(p, oldBase, newBase);
            p->OnPostRelocation();
            if (prev)
                prev->Next = p;
            else
                ObjectTypeCollections = p;
            return;
        }
        prev = p;
    }
}
int main()
{
    CObjectTypeCollection t1, t2, t3, moved;
    t1.Next = &t2; t2.Next = &t3; t3.Next = 0;
    CObjectCacheGroupCollection g; g.ObjectTypeCollections = &t1;
    int ok = 1;
    moved = t3;
    g.RelocateData(0, &t3, &moved, sizeof(t3));          // tail
    if (t2.Next != &moved || moved.Next != 0 || g_posts != 1 || g_post != &moved) ok = 0;
    CObjectTypeCollection head = t1;
    g.RelocateData(0, &t1, &head, sizeof(t1));           // head
    if (g.ObjectTypeCollections != &head || head.Next != &t2 || g_posts != 2) ok = 0;
    g.RelocateData(0, &t1, &head, sizeof(t1));           // stale address: not found
    if (g_posts != 2) ok = 0;
    if (!ok) { std::printf("00bddee0_TEST FAIL\n"); return 1; }
    std::printf("00bddee0_TEST PASS\n");
    return 0;
}