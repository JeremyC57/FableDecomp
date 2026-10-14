// CCombatAbilityDef_00c0ab60::SetElement(unsigned long, float) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, when the index is below 8 and the float at +0x344 + 4*index differs from the value it is stored, the display at +0x38 of the owner at +0x3c4 refreshed (@ 0x00C111C0) and the optional sprite at +0x60 of the owner updated (@ 0x00C15C00)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0AB60, oracle length 157 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call CScopedLock::CScopedLock ; mov ecx,[index] ; cmp ecx,8 ; mov dword ptr [esp+14h],0 ; jae done ; fld dword ptr [value] ; fld dword ptr [esi+ecx*4+344h] ; fucompp ; fnstsw ax ; test ah,44h ; jnp done ; fld dword ptr [value] ; fstp dword ptr [esi+ecx*4+344h] ; mov eax,[esi+3C4h] ; mov ecx,[eax+38h] ; call CDisplay::Refresh ; mov ecx,[esi+3C4h] ; mov ecx,[ecx+60h] ; test ecx,ecx ; je done ; call CSprite::Update ; done: lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~CScopedLock ; ...
// The setter counterpart of the getter landed at 0x00C0AAD0. The manifest label is propagated;
// the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CDisplay_00c0ab60 { void Refresh(); };    // 0x00C111C0
struct CSprite_00c0ab60 { void Update(); };      // 0x00C15C00

struct COwner_00c0ab60 {
    unsigned char _pad[0x38];
    CDisplay_00c0ab60* m_pDisplay;         // +0x38
    unsigned char _pad2[0x24];
    CSprite_00c0ab60* m_pSprite;           // +0x60
};

struct CCombatAbilityDef_00c0ab60 {
    unsigned char _pad[0x344];
    float m_elements[8];                   // +0x344
    unsigned char _pad2[0x60];
    COwner_00c0ab60* m_pOwner;             // +0x3c4
    void SetElement(unsigned long index, float value);
};

void CCombatAbilityDef_00c0ab60::SetElement(unsigned long index, float value)
{
    CScopedLock lock(&g_lock_0143bb14);
    if (index < 8 && m_elements[index] != value) {
        m_elements[index] = value;
        m_pOwner->m_pDisplay->Refresh();
        if (m_pOwner->m_pSprite)
            m_pOwner->m_pSprite->Update();
    }
}
