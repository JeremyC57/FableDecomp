// Behaviour test for 0x00864F80 NCreatureHitNotification::CFlash::CFlash(const CFlash&) (self-contained model: the list
// copy is modelled with its own range insert)
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
namespace NCreatureHitNotification {
class CFlash {
public:
    CWeaponTrailList FlashGeometry;
    long             CreationFrame;
    CFlash() : CreationFrame(0) { }
    CFlash(const CFlash& other);
};
CFlash::CFlash(const CFlash& other)
    : FlashGeometry(other.FlashGeometry), CreationFrame(other.CreationFrame)
{
}
}
using NCreatureHitNotification::CFlash;
int main()
{
    int ok = 1;
    CFlash a;
    a.CreationFrame = 77;
    a.FlashGeometry.push_back(10);
    a.FlashGeometry.push_back(20);
    CFlash b(a);
    if (b.CreationFrame != 77) ok = 0;
    if (b.FlashGeometry.m_node == a.FlashGeometry.m_node) ok = 0;
    SListNode* n1 = (SListNode*)b.FlashGeometry.m_node->m_next;
    SListNode* n2 = (SListNode*)n1->m_next;
    if (n1 == b.FlashGeometry.m_node || n2 == b.FlashGeometry.m_node) ok = 0;
    else {
        if (n1->m_data.m_data[0] != 10 || n1->m_data.m_data[6] != 16 || n2->m_data.m_data[0] != 20) ok = 0;
        if (n2->m_next != b.FlashGeometry.m_node || b.FlashGeometry.m_node->m_prev != n2) ok = 0;
    }
    printf(ok ? "00864f80_TEST PASS\n" : "00864f80_TEST FAIL\n");
    return ok ? 0 : 1;
}