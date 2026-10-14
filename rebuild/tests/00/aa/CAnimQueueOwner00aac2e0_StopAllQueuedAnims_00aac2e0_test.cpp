#include <cstdio>
#include <cstring>
#include <cmath>
#include <cstdlib>

static long g_stopped[4]; static int g_nstop = 0, g_removeInactive = 0, g_cleared = 0;
struct CAnimationPlayer { virtual void StopAnim(long h) { g_stopped[g_nstop++] = h; } };
struct CAnimationOwner { CAnimationPlayer player; virtual CAnimationPlayer* GetAnimationPlayer() { return &player; } };
struct CThing { unsigned char p[0x40]; CAnimationOwner* m_pAnimationOwner; };
struct SAnimEntry { unsigned char p[8]; long m_animHandle; };
struct CRetailEntryList { struct node { node* next; node* prev; SAnimEntry* obj; void* blk; }; struct iterator { node* m_pNode; explicit iterator(node* n) : m_pNode(n) {} SAnimEntry* operator*() const { return m_pNode->obj; } iterator& operator++() { m_pNode = m_pNode->next; return *this; } bool operator!=(const iterator& o) const { return m_pNode != o.m_pNode; } };
    node* head; CRetailEntryList() { head = new node; head->next = head->prev = head; } iterator begin() { return iterator(head->next); } iterator end() { return iterator(head); }
    void add(SAnimEntry* e) { node* n = new node; n->obj = e; n->prev = head->prev; n->next = head; head->prev->next = n; head->prev = n; } void clear() { g_cleared++; head->next = head->prev = head; } };
struct CAnimQueueOwner { unsigned char p[4]; CThing* m_pParentThing; unsigned char q[0x14 - 8]; CRetailEntryList m_animQueue; void RemoveInactiveEntries() { g_removeInactive++; }
    void StopAllQueuedAnims() { if (m_pParentThing->m_pAnimationOwner != 0) { for (CRetailEntryList::iterator it = m_animQueue.begin(); it != m_animQueue.end(); ++it) { SAnimEntry* e = *it; m_pParentThing->m_pAnimationOwner->GetAnimationPlayer()->StopAnim(e->m_animHandle); e->m_animHandle = -1; } RemoveInactiveEntries(); m_animQueue.clear(); } } };
int main() { CAnimationOwner owner; CThing thing; CAnimQueueOwner o; o.m_pParentThing = &thing; SAnimEntry a, b; a.m_animHandle = 11; b.m_animHandle = 22; o.m_animQueue.add(&a); o.m_animQueue.add(&b);
    thing.m_pAnimationOwner = 0; o.StopAllQueuedAnims(); if (g_nstop || g_cleared) { std::printf("00aac2e0_TEST FAIL\n"); return 1; }
    thing.m_pAnimationOwner = &owner; o.StopAllQueuedAnims(); if (g_nstop != 2 || g_stopped[0] != 11 || g_stopped[1] != 22 || a.m_animHandle != -1 || b.m_animHandle != -1 || g_removeInactive != 1 || g_cleared != 1) { std::printf("00aac2e0_TEST FAIL\n"); return 1; }
    std::printf("00aac2e0_TEST PASS\n"); return 0; }