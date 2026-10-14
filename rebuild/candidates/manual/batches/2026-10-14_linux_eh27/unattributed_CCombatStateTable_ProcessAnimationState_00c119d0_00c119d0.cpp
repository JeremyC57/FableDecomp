// CCombatStateTable_00c119d0::ProcessAnimationState(tag, entry) -- EH-framed under the module's profiling lock (0x0143BB14; ctor @ 0x00C0FFC0, dtor @ 0x00C0FFE0, state 0): -1 without an entry; otherwise a 0x10-byte lookup key is built on the frame (ctor @ 0x00C25C50, state 1) from the entry's +4 word, the entry and the entry's +8 table resolved by the tag (@ 0x00C1BAC0; set into the key's value part at +4 @ 0x00C25C70), the state map embedded at +0x1f4 is searched (slot 0x44), the key is cleared (@ 0x00C25CB0) and destroyed (@ 0x00C25CA0), and the hit's +0xc state (read before the destructors) or -1 is returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C119D0, oracle length 238 bytes. Built under /EHsc.
// Retail bytes:  ... mov edi,ecx ; push offset g_section ; lea ecx,[esp+0Ch] ; call CProfileLock::CProfileLock ; mov esi,[esp+34h] ; test esi,esi ; mov dword ptr [esp+28h],0 ; je none ; lea ecx,[esp+10h] ; call CKey::CKey ; mov eax,[esp+30h] ; push eax ; lea ecx,[esi+8] ; mov byte ptr [esp+2Ch],1 ; call CTable::Lookup ; mov ecx,[esi+4] ; push eax ; push esi ; push ecx ; lea ecx,[esp+20h] ; call CKey::Set ; mov edx,[edi+1F4h] ; lea ecx,[edi+1F4h] ; lea eax,[esp+10h] ; push eax ; call [edx+44h] ; lea ecx,[esp+10h] ; mov esi,eax ; call CKey::Clear ; test esi,esi ; lea ecx,[esp+10h] ; mov byte ptr [esp+28h],0 ; je miss ; mov esi,[esi+0Ch] ; call CKey::~CKey ; lea ecx,[esp+8] ; mov dword ptr [esp+28h],-1 ; call ~CProfileLock ; pop edi ; mov eax,esi ; ... ret 8 ; miss: call CKey::~CKey ; none: lea ecx,[esp+8] ; mov dword ptr [esp+28h],-1 ; call ~CProfileLock ; ... or eax,-1 ; ... ret 8
// The manifest label is propagated; the classes are named by address.
extern unsigned char g_section_0143bb14[];  // 0x0143BB14: the module's critical section

struct CProfileLock_00c119d0 {             // 8 bytes: section + entry tick
    void* m_pSection;
    unsigned long m_tick;
    CProfileLock_00c119d0(void* section);  // 0x00C0FFC0
    ~CProfileLock_00c119d0();              // 0x00C0FFE0
};

struct CTable_00c119d0 { void* Lookup(void* tag); };   // 0x00C1BAC0

struct CEntry_00c119d0 {
    long m_0;
    void* m_pOwner;                        // +4
    CTable_00c119d0 m_table;               // +8
};

struct CKeyFields_00c119d0 {               // the key's value part at +4
    unsigned char _body[0xc];
    void Set(void* owner, CEntry_00c119d0* entry, void* resolved);   // 0x00C25C70
};

struct CKey_00c119d0 {                     // 0x10-byte lookup key
    long m_0;
    CKeyFields_00c119d0 m_fields;          // +4
    CKey_00c119d0();                       // 0x00C25C50
    void Clear();                          // 0x00C25CB0
    ~CKey_00c119d0();                      // 0x00C25CA0
};

struct CStateHit_00c119d0 {
    unsigned char _pad[0xc];
    long m_state;                          // +0xc
};

struct CStateMap_00c119d0 {                // embedded at +0x1f4
    virtual void Slot0();
    virtual void Slot4();
    virtual void Slot8();
    virtual void SlotC();
    virtual void Slot10();
    virtual void Slot14();
    virtual void Slot18();
    virtual void Slot1C();
    virtual void Slot20();
    virtual void Slot24();
    virtual void Slot28();
    virtual void Slot2C();
    virtual void Slot30();
    virtual void Slot34();
    virtual void Slot38();
    virtual void Slot3C();
    virtual void Slot40();
    virtual CStateHit_00c119d0* Find(CKey_00c119d0* key);   // slot 0x44
};

struct CCombatStateTable_00c119d0 {
    unsigned char _pad[0x1f4];
    CStateMap_00c119d0 m_states;           // +0x1f4
    long ProcessAnimationState(void* tag, CEntry_00c119d0* entry);
};

long CCombatStateTable_00c119d0::ProcessAnimationState(void* tag, CEntry_00c119d0* entry)
{
    CProfileLock_00c119d0 lock(g_section_0143bb14);
    if (entry) {
        CKey_00c119d0 key;
        key.m_fields.Set(entry->m_pOwner, entry, entry->m_table.Lookup(tag));
        CStateHit_00c119d0* hit = m_states.Find(&key);
        key.Clear();
        if (hit)
            return hit->m_state;
    }
    return -1;
}
