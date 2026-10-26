// Behaviour test for 00bddde0 CObjectTypeCollection::RelocateData (copy of the model; primitives are moved by
// copying them into a second buffer, as the movable-resource pool does, then the callback fixes the list).
#include <cstdio>
#include <cstring>
static int g_fixups = 0;
static const void* g_fixed = 0;
class CLocalDetailPrimitiveBase { public:
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {}
    virtual void v04() {} virtual void v05() {} virtual void v06() {}
    virtual void OnPostRelocation() { ++g_fixups; g_fixed = this; }
    CLocalDetailPrimitiveBase* m_pNext;
    int tag;
};
template <class T> inline void RelocatePointer(T*& p, void* oldBase, void* newBase)
{
    if (p)
        p = (T*)((long)p + ((long)newBase - (long)oldBase));
}
struct CObjectTypeCollection {
    virtual ~CObjectTypeCollection() {}
    void* m_pType;
    CLocalDetailPrimitiveBase* m_pFirstPrimitive;
    void RelocateData(long blockType, void* oldBase, void* newBase, long size);
};
void CObjectTypeCollection::RelocateData(long blockType, void* oldBase, void* newBase, long size)
{
    CLocalDetailPrimitiveBase* prev = 0;
    for (CLocalDetailPrimitiveBase* p = m_pFirstPrimitive; p; p = p->m_pNext) {
        if (p == oldBase) {
            RelocatePointer(p, oldBase, newBase);
            p->OnPostRelocation();
            if (prev)
                prev->m_pNext = p;
            else
                m_pFirstPrimitive = p;
            return;
        }
        prev = p;
    }
}
int main()
{
    CLocalDetailPrimitiveBase a, b, c, b2, a2, other;
    a.tag = 1; b.tag = 2; c.tag = 3;
    a.m_pNext = &b; b.m_pNext = &c; c.m_pNext = 0;
    CObjectTypeCollection col; col.m_pFirstPrimitive = &a;
    int ok = 1;
    b2 = b;                                              // move the middle one
    col.RelocateData(0, &b, &b2, sizeof(b));
    if (a.m_pNext != &b2 || b2.m_pNext != &c || g_fixups != 1 || g_fixed != &b2) ok = 0;
    a2 = a;                                              // move the head
    col.RelocateData(0, &a, &a2, sizeof(a));
    if (col.m_pFirstPrimitive != &a2 || a2.m_pNext != &b2 || g_fixups != 2 || g_fixed != &a2) ok = 0;
    col.RelocateData(0, &other, &a, sizeof(a));          // not ours: nothing happens
    if (g_fixups != 2 || col.m_pFirstPrimitive != &a2) ok = 0;
    CObjectTypeCollection empty; empty.m_pFirstPrimitive = 0;
    empty.RelocateData(0, &a, &b, 4);
    if (g_fixups != 2 || empty.m_pFirstPrimitive != 0) ok = 0;
    if (!ok) { std::printf("00bddde0_TEST FAIL\n"); return 1; }
    std::printf("00bddde0_TEST PASS\n");
    return 0;
}