// CInstanceOwner_00c30840::CreateNewInstance() -- allocates a 12-byte instance record {active = 1, owner = the global debug buffer @ 0x0139C900, id = g_nextInstanceId++ @ 0x0143BB44}, registers it through virtual slot 9, clears the dirty byte at +0x30 and returns the record's entry part (+4)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C30840, length 92 bytes.
// Callees: operator new @ 0x00BFEA1A; this->vtable slot 9 (`call [eax+0x24]`, one argument).
// The manifest label (C3DAnimFileXSequenceChunk::CreateNewInstance) does not fit (that class is
// 0x20 bytes, this one writes +0x30), so the class is named by what the body does. `new` is
// null-checked around the inlined constructor and VC duplicates the tail into both paths; the
// returned pointer is the address of the embedded entry (no null check: `lea eax,[esi+4]`).
// The id post-increment reloads the global after the store (the fresh object may alias it).
extern void* g_pDebugBuffer;               // 0x0139C900
extern unsigned long g_nextInstanceId;     // 0x0143BB44

struct SInstanceEntry {
    void* m_pOwner;                        // +0
    unsigned long m_id;                    // +4
    SInstanceEntry() : m_pOwner(g_pDebugBuffer), m_id(g_nextInstanceId++) { }
};

struct SInstanceRecord {
    unsigned char m_active;                // +0
    SInstanceEntry m_entry;                // +4
    SInstanceRecord() : m_active(1) { }
};

struct CInstanceOwner_00c30840 {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void Register(SInstanceRecord* record);   // slot 9
    unsigned char _pad[0x2c];
    unsigned char m_dirty;                 // +0x30
    SInstanceEntry* CreateNewInstance();
};

SInstanceEntry* CInstanceOwner_00c30840::CreateNewInstance()
{
    SInstanceRecord* record = new SInstanceRecord;
    Register(record);
    m_dirty = 0;
    return &record->m_entry;
}