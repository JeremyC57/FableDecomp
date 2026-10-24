// Behaviour test for 0x007702B0 std::__uninitialized_fill_n over std::list<CWeaponTrailEntry>
#include <cstdio>
#include <cstdlib>
#include <new>

struct __false_type { __false_type() { } };
struct CWeaponTrailEntry { unsigned int m_data[7]; };
struct SListNodeBase { SListNodeBase* m_next; SListNodeBase* m_prev; };
struct SListNode : SListNodeBase { CWeaponTrailEntry m_data; };
struct SListIterator {
    SListNodeBase* m_node;
    SListIterator(SListNodeBase* node) : m_node(node) { }
    SListIterator(const SListIterator& other) : m_node(other.m_node) { }
};
struct SListConstIterator { SListNodeBase* m_node; };
struct CWeaponTrailList {
    SListNode* m_node;
    CWeaponTrailList() : m_node(0)
    {
        SListNode* node = (SListNode*)std::malloc(sizeof(SListNode));
        node->m_next = node; node->m_prev = node; m_node = node;
    }
    CWeaponTrailList(const CWeaponTrailList& x) : m_node(0)
    {
        SListNode* node = (SListNode*)std::malloc(sizeof(SListNode));
        node->m_next = node;
        node->m_prev = node;
        m_node = node;
        insert(begin(), x.cbegin(), x.cend());
    }
    SListIterator begin() { return SListIterator(m_node->m_next); }
    SListConstIterator cbegin() const { SListConstIterator it; it.m_node = m_node->m_next; return it; }
    SListConstIterator cend() const { SListConstIterator it; it.m_node = m_node; return it; }
    void InsertDispatch(SListIterator pos, SListConstIterator first, SListConstIterator last, const __false_type&)
    {   // 0x0076FD40: insert(pos, *first) for each element
        for (SListNodeBase* p = first.m_node; p != last.m_node; p = p->m_next) {
            SListNode* tmp = (SListNode*)std::malloc(sizeof(SListNode));
            tmp->m_data = ((SListNode*)p)->m_data;
            SListNodeBase* n = pos.m_node;
            tmp->m_next = n; tmp->m_prev = n->m_prev; n->m_prev->m_next = tmp; n->m_prev = tmp;
        }
    }
    void insert(SListIterator pos, SListConstIterator first, SListConstIterator last) { InsertDispatch(pos, first, last, __false_type()); }
    void push_back(const CWeaponTrailEntry& e)
    {
        SListNode* tmp = (SListNode*)std::malloc(sizeof(SListNode));
        tmp->m_data = e;
        tmp->m_next = m_node; tmp->m_prev = m_node->m_prev; m_node->m_prev->m_next = tmp; m_node->m_prev = tmp;
    }
};

static CWeaponTrailList* UninitializedFillN(CWeaponTrailList* first, unsigned int n, const CWeaponTrailList& x, const __false_type&)
{
    CWeaponTrailList* cur = first;
    for (; n > 0; --n, ++cur)
        new ((void*)cur) CWeaponTrailList(x);
    return cur;
}

int main()
{
    CWeaponTrailList src;
    CWeaponTrailEntry e1, e2;
    for (int i = 0; i < 7; ++i) { e1.m_data[i] = i + 1; e2.m_data[i] = 100 + i; }
    src.push_back(e1);
    src.push_back(e2);
    static unsigned char raw[sizeof(CWeaponTrailList) * 3];
    CWeaponTrailList* slots = (CWeaponTrailList*)raw;
    CWeaponTrailList* end = UninitializedFillN(slots, 3, src, __false_type());
    if (end != slots + 3) { std::printf("007702b0_TEST FAIL\n"); return 1; }
    for (int k = 0; k < 3; ++k) {
        SListNode* a = (SListNode*)slots[k].m_node->m_next;
        SListNode* b = (SListNode*)a->m_next;
        if (a == slots[k].m_node || b == slots[k].m_node || b->m_next != slots[k].m_node
            || a->m_data.m_data[0] != 1 || a->m_data.m_data[6] != 7 || b->m_data.m_data[0] != 100 || b->m_data.m_data[6] != 106
            || slots[k].m_node->m_prev != b) {
            std::printf("007702b0_TEST FAIL\n"); return 1;
        }
    }
    if (UninitializedFillN(slots, 0, src, __false_type()) != slots) { std::printf("007702b0_TEST FAIL\n"); return 1; }
    std::printf("007702b0_TEST PASS\n");
    return 0;
}