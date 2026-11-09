// CSoundEventManager::SendRecordToEntry(long key, const SEventRecordWords& words) -- descriptive
// Grade: DIFFER(163v163) -- one store order: retail `mov [esp+10h],ecx` before `push eax`, ours after
// Chunk 894 (11 grades): rebased on the vendor-V1 names (libpk/PKCProtectionInstance.h, SEventRecord ctor 0x00C20390, CLookupOwner_00c21e20::FindInChildThenSelf); 5 copy orders, flag-after-copy, same-TU noinline ctor (2 forms), reference / pointer params, named record pointer: inert. The name SendRecordToEntry is descriptive (vtable 0x012B2748 slot 27); catalog `Inventory_ProcessNavigationItem` is unproven. Next: c2prio on the record copy.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C11520, 163 bytes, __thiscall `ret 8`.
// Leaf: ?SendRecordToEntry@CSoundEventManager@@QAEXJABUSEventRecordWords@@@Z
// Flags: /EHsc
#include "libpk/PKCProtectionInstance.h"
extern PKCCriticalSection g_lock_0143bb14;   // 0x0143BB14

struct SEventRecordWords {                   // the three words the record carries (copied as a block)
    void* m_a;
    void* m_b;
    void* m_c;
};
struct SEventRecord {                        // 16 bytes: three words and a flag byte
    void* m_a;                               // +0
    void* m_b;                               // +4
    void* m_c;                               // +8
    unsigned char m_flag;                    // +0xc
    SEventRecord();                          // 0x00C20390
};
struct CSoundEventManager;
struct CObject {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void ReceiveRecord(CSoundEventManager* source, SEventRecord* record);   // slot 5 (+0x14)
    virtual void v06(); virtual void v07();
    virtual bool AcceptsRecordFrom(CSoundEventManager* source);                     // slot 8 (+0x20)
};
struct CLookupOwner_00c21e20 {
    CObject* FindInChildThenSelf(long key);  // 0x00C21E20
};
struct CSoundEventManager {
    unsigned char _pad[0x2c];
    CLookupOwner_00c21e20 m_eventSet;        // +0x2c
    void SendRecordToEntry(long key, const SEventRecordWords& words);
};

void CSoundEventManager::SendRecordToEntry(long key, const SEventRecordWords& words)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    CObject* entry = m_eventSet.FindInChildThenSelf(key);
    if (entry) {
        SEventRecord record;
        record.m_flag = entry->AcceptsRecordFrom(this);
        record.m_a = words.m_a;
        record.m_b = words.m_b;
        record.m_c = words.m_c;
        entry->ReceiveRecord(this, &record);
    }
}
