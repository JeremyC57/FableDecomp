// FindScriptThingByUid_00556930(long uid, CThingList* list, long map) -- walk the list's nodes and return the first thing whose identity record (+0x19c -> +0 -> +8) carries `uid` and whose map word (+0x1a0) is `map`; null otherwise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00556930, length 80 bytes.
// Callees: none. Shape: static `__stdcall` (no `this`, `ret 0Ch`), the empty-list test before the registers are saved, the two keys kept in esi/edi, the found thing
// re-read from the node at the exit. The chunk label "CGameScriptThingManager::HasScriptThing" is propagated; named by address.
struct SIdentity_00556930 {
    unsigned char _pad[8];
    long          m_uid;                           // +0x08
};
struct SIdentityHolder_00556930 {
    SIdentity_00556930* m_pIdentity;               // +0x00
};
struct CThing_00556930 {
    unsigned char           _pad[0x19c];
    SIdentityHolder_00556930* m_pHolder;           // +0x19c
    long                    m_map;                 // +0x1a0
};
struct SNode_00556930 {
    SNode_00556930*  m_pNext;
    SNode_00556930*  m_pPrev;
    CThing_00556930* m_pThing;                     // +0x08
};
struct CThingList_00556930 {
    SNode_00556930* m_pHead;                       // the sentinel node
};

CThing_00556930* __stdcall FindScriptThingByUid_00556930(long uid, CThingList_00556930* list, long map)
{
    SNode_00556930* head = list->m_pHead;
    for (SNode_00556930* node = head->m_pNext; node != head; node = node->m_pNext) {
        CThing_00556930* thing = node->m_pThing;
        if (thing->m_pHolder->m_pIdentity->m_uid == uid && thing->m_map == map)
            return node->m_pThing;
    }
    return 0;
}
