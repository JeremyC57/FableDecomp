// Behaviour test for 0x00864F80 CWeaponTrailListRecord::CWeaponTrailListRecord(const CWeaponTrailListRecord&)
#include <stdio.h>
#include <stdlib.h>
struct CWeaponTrailEntry { unsigned long m_data[7]; };
struct SListNodeBase { SListNodeBase* m_next; SListNodeBase* m_prev; };
struct SListNode : SListNodeBase { CWeaponTrailEntry m_data; };
struct CWeaponTrailList {
    SListNode* m_node;
    CWeaponTrailList() : m_node(0)
    {
        SListNode* node = (SListNode*)malloc(sizeof(SListNode));
        node->m_next = node; node->m_prev = node; m_node = node;
    }
    CWeaponTrailList(const CWeaponTrailList& x) : m_node(0)
    {
        SListNode* node = (SListNode*)malloc(sizeof(SListNode));
        node->m_next = node;
        node->m_prev = node;
        m_node = node;
        insert(m_node->m_next, x.m_node->m_next, x.m_node);
    }
    void insert(SListNodeBase* pos, SListNodeBase* first, SListNodeBase* last)
    {
        for (; first != last; first = first->m_next) {
            SListNode* n = (SListNode*)malloc(sizeof(SListNode));
            n->m_data = ((SListNode*)first)->m_data;
            n->m_next = pos; n->m_prev = pos->m_prev;
            pos->m_prev->m_next = n; pos->m_prev = n;
        }
    }
    void push_back(unsigned long v)
    {
        CWeaponTrailEntry e;
        int i = 0;
        for (i = 0; i < 7; ++i) e.m_data[i] = v + i;
        SListNode tmp; tmp.m_data = e;
        tmp.m_next = m_node;
        insert(m_node, &tmp, m_node);
    }
};
struct CWeaponTrailListRecord {
    CWeaponTrailList Entries;
    unsigned long    Extra;
    CWeaponTrailListRecord() : Extra(0) { }
    CWeaponTrailListRecord(const CWeaponTrailListRecord& other);
};
CWeaponTrailListRecord::CWeaponTrailListRecord(const CWeaponTrailListRecord& other)
    : Entries(other.Entries), Extra(other.Extra)
{
}
int main()
{
    int ok = 1;
    CWeaponTrailListRecord a;
    a.Extra = 77;
    a.Entries.push_back(10);
    a.Entries.push_back(20);
    CWeaponTrailListRecord b(a);
    if (b.Extra != 77) ok = 0;
    if (b.Entries.m_node == a.Entries.m_node) ok = 0;
    SListNode* n1 = (SListNode*)b.Entries.m_node->m_next;
    SListNode* n2 = (SListNode*)n1->m_next;
    if (n1 == b.Entries.m_node || n2 == b.Entries.m_node) ok = 0;
    else {
        if (n1->m_data.m_data[0] != 10 || n1->m_data.m_data[6] != 16 || n2->m_data.m_data[0] != 20) ok = 0;
        if (n2->m_next != b.Entries.m_node || b.Entries.m_node->m_prev != n2) ok = 0;
    }
    printf(ok ? "00864f80_TEST PASS\n" : "00864f80_TEST FAIL\n");
    return ok ? 0 : 1;
}