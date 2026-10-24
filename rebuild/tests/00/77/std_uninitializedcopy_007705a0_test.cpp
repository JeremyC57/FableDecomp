#include <cstdio>
#include <new>
struct __false_type { __false_type() { } };
#include <stdlib.h>
struct CWeaponTrailEntry { unsigned long m_data[7]; };
struct SListNodeBase { SListNodeBase* m_next; SListNodeBase* m_prev; };
struct SListNode : SListNodeBase { CWeaponTrailEntry m_data; };
struct SListIterator { SListNodeBase* m_node; SListIterator(SListNodeBase* node) : m_node(node) { } SListIterator(const SListIterator& other) : m_node(other.m_node) { } };
struct SListConstIterator { SListNodeBase* m_node; };
struct CWeaponTrailList {
    SListNode* m_node;
    CWeaponTrailList() : m_node(0) { SListNode* node = (SListNode*)malloc(sizeof(SListNode)); node->m_next = node; node->m_prev = node; m_node = node; }
    CWeaponTrailList(const CWeaponTrailList& x) : m_node(0)
    {
        SListNode* node = (SListNode*)malloc(sizeof(SListNode));
        node->m_next = node; node->m_prev = node; m_node = node;
        insert(begin(), x.cbegin(), x.cend());
    }
    SListIterator begin() { return SListIterator(m_node->m_next); }
    SListConstIterator cbegin() const { SListConstIterator it; it.m_node = m_node->m_next; return it; }
    SListConstIterator cend() const { SListConstIterator it; it.m_node = m_node; return it; }
    void InsertDispatch(SListIterator pos, SListConstIterator first, SListConstIterator last, const __false_type&)
    {
        for (SListNodeBase* n = first.m_node; n != last.m_node; n = n->m_next) {   // 0x0076FD40: insert(pos, *first) per node
            SListNode* node = (SListNode*)malloc(sizeof(SListNode));
            node->m_data = ((SListNode*)n)->m_data;
            node->m_next = pos.m_node; node->m_prev = pos.m_node->m_prev;
            pos.m_node->m_prev->m_next = node; pos.m_node->m_prev = node;
        }
    }
    void insert(SListIterator pos, SListConstIterator first, SListConstIterator last) { InsertDispatch(pos, first, last, __false_type()); }
    void PushBack(unsigned long v) { SListConstIterator it; SListNode tmp; tmp.m_data.m_data[0] = v; tmp.m_next = m_node; it.m_node = &tmp; SListConstIterator e; e.m_node = m_node; insert(SListIterator(m_node), it, e); }
    int Count() const { int c = 0; for (SListNodeBase* n = m_node->m_next; n != m_node; n = n->m_next) ++c; return c; }
};
typedef CWeaponTrailList TElem;
static void Fill(TElem& e, int i)
{
    for (int k = 0; k < i; ++k) e.PushBack(100u * i + k);
}
static bool Check(const TElem& e, int i)
{
    return e.Count() == i && ((SListNode*)e.m_node->m_next)->m_data.m_data[0] == 100u * i && e.m_node->m_prev->m_next == e.m_node && e.m_node->m_next->m_prev == e.m_node;
}
static TElem* UninitCopy(const TElem* first, const TElem* last, TElem* result, const __false_type&)
{
    TElem* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) TElem(*first);
    return cur;
}
int main()
{
    static double srcStorage[4 * sizeof(TElem) / sizeof(double) + 2];
    static double dstStorage[4 * sizeof(TElem) / sizeof(double) + 2];
    TElem* src = (TElem*)srcStorage;
    TElem* dst = (TElem*)dstStorage;
    int i;
    for (i = 0; i < 3; ++i) { new ((void*)(src + i)) TElem(); Fill(src[i], i + 1); }
    __false_type tag;
    TElem* end = UninitCopy(src, src + 3, dst, tag);
    bool ok = end == dst + 3;
    for (i = 0; i < 3; ++i) if (!Check(dst[i], i + 1)) ok = false;
    if (UninitCopy(src, src, dst + 3, tag) != dst + 3) ok = false;
    for (i = 0; i < 3; ++i) if (dst[i].m_node == src[i].m_node) ok = false;
    if (!ok) { std::printf("007705a0_TEST FAIL\n"); return 1; }
    std::printf("007705a0_TEST PASS\n");
    return 0;
}