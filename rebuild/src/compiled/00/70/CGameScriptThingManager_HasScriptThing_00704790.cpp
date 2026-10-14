// CGameScriptThingManager::HasScriptThing(const CScriptThingRecord& record) -- whether the UID list (sentinel at +0) holds the record's 64-bit UID (+0x18)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00704790, length 64 bytes.
// Callees: none. Shape: the empty-list test before the registers are saved, the UID's two words loaded once into esi/edx, each node's two words compared, `mov al,1` /
// `xor al,al`, `ret 4`. The chunk label is propagated onto the twin at 0x00556930.
struct CScriptThingRecord_00704790 {
    unsigned char _pad[0x18];
    __int64       m_uid;                           // +0x18 (the 64-bit UID: compared half by half through a register)
};
struct SUidNode_00704790 {
    SUidNode_00704790* m_pNext;
    SUidNode_00704790* m_pPrev;
    __int64            m_uid;                      // +0x08
};

struct CGameScriptThingManager {
    SUidNode_00704790* m_pHead;                    // +0x00 (the sentinel)
    bool HasScriptThing(const CScriptThingRecord_00704790& record);
};

bool CGameScriptThingManager::HasScriptThing(const CScriptThingRecord_00704790& record)
{
    SUidNode_00704790* head = m_pHead;
    for (SUidNode_00704790* node = head->m_pNext; node != head; node = node->m_pNext) {
        if (node->m_uid == record.m_uid)
            return true;
    }
    return false;
}