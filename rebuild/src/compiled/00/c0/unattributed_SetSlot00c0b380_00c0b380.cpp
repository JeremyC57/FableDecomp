// CCombatAbilityDef_00c0b380::SetSlot(unsigned long, void*, void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, when the index is below 16 the 0x34-byte slot at +4 + 0x34*index is set from the two arguments and 0 (@ 0x00C0B570), the display at +0x38 of the owner at +0x3c4 refreshed (@ 0x00C11180) and the optional sprite at +0x60 of the owner updated (@ 0x00C15B90)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0B380, oracle length 150 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call CScopedLock::CScopedLock ; mov eax,[index] ; cmp eax,10h ; mov dword ptr [esp+14h],0 ; jae done ; mov ecx,[b] ; imul eax,eax,34h ; mov edx,[a] ; push ecx ; push 0 ; push edx ; lea ecx,[eax+esi+4] ; call CSlot::Set ; mov eax,[esi+3C4h] ; mov ecx,[eax+38h] ; call CDisplay::Refresh ; mov ecx,[esi+3C4h] ; mov ecx,[ecx+60h] ; test ecx,ecx ; je done ; call CSprite::Update ; done: lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~CScopedLock ; ...
// The manifest label is propagated; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CSlot_00c0b380 {                    // 0x34 bytes
    unsigned char _body[0x34];
    void Set(void* a, long mode, void* b); // 0x00C0B570
};

struct CDisplay_00c0b380 { void Refresh(); };    // 0x00C11180
struct CSprite_00c0b380 { void Update(); };      // 0x00C15B90

struct COwner_00c0b380 {
    unsigned char _pad[0x38];
    CDisplay_00c0b380* m_pDisplay;         // +0x38
    unsigned char _pad2[0x24];
    CSprite_00c0b380* m_pSprite;           // +0x60
};

struct CCombatAbilityDef_00c0b380 {
    unsigned char _pad[4];
    CSlot_00c0b380 m_slots[16];            // +4
    unsigned char _pad2[0x80];
    COwner_00c0b380* m_pOwner;             // +0x3c4
    void SetSlot(unsigned long index, void* a, void* b);
};

void CCombatAbilityDef_00c0b380::SetSlot(unsigned long index, void* a, void* b)
{
    CScopedLock lock(&g_lock_0143bb14);
    if (index < 16) {
        m_slots[index].Set(a, 0, b);
        m_pOwner->m_pDisplay->Refresh();
        if (m_pOwner->m_pSprite)
            m_pOwner->m_pSprite->Update();
    }
}