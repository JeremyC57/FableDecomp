#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SIdentity { unsigned char _pad[8]; long m_uid; }; struct SIdentityHolder { SIdentity* m_pIdentity; };
struct CThing { unsigned char _pad[0x19c]; SIdentityHolder* m_pHolder; long m_map; };
struct SNode { SNode* m_pNext; SNode* m_pPrev; CThing* m_pThing; }; struct CThingList { SNode* m_pHead; };
static CThing* FindScriptThingByUid_00556930(long uid, CThingList* list, long map) { SNode* head = list->m_pHead; for (SNode* node = head->m_pNext; node != head; node = node->m_pNext) { CThing* thing = node->m_pThing; if (thing->m_pHolder->m_pIdentity->m_uid == uid && thing->m_map == map) return node->m_pThing; } return 0; }

int main() {
    SIdentity i1, i2; i1.m_uid = 10; i2.m_uid = 20; SIdentityHolder h1, h2; h1.m_pIdentity = &i1; h2.m_pIdentity = &i2;
    CThing t1, t2; t1.m_pHolder = &h1; t1.m_map = 1; t2.m_pHolder = &h2; t2.m_map = 2; SNode head, n1, n2; head.m_pNext = &n1; n1.m_pNext = &n2; n2.m_pNext = &head; n1.m_pThing = &t1; n2.m_pThing = &t2; CThingList list; list.m_pHead = &head;
    if (FindScriptThingByUid_00556930(20, &list, 2) != &t2 || FindScriptThingByUid_00556930(10, &list, 1) != &t1) { std::printf("00556930_TEST FAIL\n"); return 1; }
    if (FindScriptThingByUid_00556930(10, &list, 2) != 0 || FindScriptThingByUid_00556930(30, &list, 1) != 0) { std::printf("00556930_TEST FAIL\n"); return 1; } head.m_pNext = &head; if (FindScriptThingByUid_00556930(10, &list, 1) != 0) { std::printf("00556930_TEST FAIL\n"); return 1; }
    std::printf("00556930_TEST PASS\n"); return 0;
}