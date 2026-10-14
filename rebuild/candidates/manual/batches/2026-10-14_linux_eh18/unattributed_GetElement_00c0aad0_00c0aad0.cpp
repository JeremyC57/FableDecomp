// CCombatAbilityDef_00c0aad0::GetElement(unsigned long) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the float at +0x344 + 4*index when the index is below 8 (parked in the dead argument slot across the lock's destructor), else the pool constant loaded after the unlock
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C0AAD0, oracle length 129 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call CScopedLock::CScopedLock ; mov eax,[index] ; cmp eax,8 ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; jb hit ; call ~CScopedLock ; fld dword ptr [pool] ; pop esi ; mov ecx,[esp+8] ; mov fs:[0],ecx ; add esp,14h ; ret 4 ; hit: fld dword ptr [esi+eax*4+344h] ; fstp dword ptr [index] ; call ~CScopedLock ; mov ecx,[esp+0Ch] ; fld dword ptr [index] ; pop esi ; ...
// The lock's destructor runs on both paths after the state store is hoisted; the array element
// survives it in the dead argument slot. The manifest label is propagated; the class is named by
// address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CCombatAbilityDef_00c0aad0 {
    unsigned char _pad[0x344];
    float m_elements[8];                   // +0x344
    float GetElement(unsigned long index);
};

float CCombatAbilityDef_00c0aad0::GetElement(unsigned long index)
{
    CScopedLock lock(&g_lock_0143bb14);
    if (index >= 8)
        return 100.0f;
    return m_elements[index];
}
