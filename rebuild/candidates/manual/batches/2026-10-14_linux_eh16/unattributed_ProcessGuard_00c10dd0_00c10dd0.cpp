// CEditControlCentre_00c10dd0::ProcessGuard(void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, a `__stdcall` helper takes the argument (@ 0x00C1BD80), bits 1 and 2 of the flags at +0x7c8 are set, and the optional object at +0x64 of the object at +4 is poked through virtual slot 0; returns 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C10DD0, oracle length 119 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call CScopedLock::CScopedLock ; mov eax,[arg] ; push eax ; mov dword ptr [esp+18h],0 ; call Helper ; or dword ptr [esi+7C8h],6 ; mov esi,[esi+4] ; mov eax,[esi+64h] ; test eax,eax ; je skip ; mov ecx,eax ; mov edx,[ecx] ; call [edx] ; skip: lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~CScopedLock ; mov ecx,[esp+0Ch] ; xor eax,eax ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret 4
// The helper takes its argument on the stack and cleans it (no `add esp` after the call), so it
// is `__stdcall`. The manifest label is propagated; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

void __stdcall Helper_00c1bd80(void* argument);   // 0x00C1BD80

struct CListener_00c10dd0 {
    virtual void Poke();                   // slot 0
};

struct COwner_00c10dd0 {
    unsigned char _pad[0x64];
    CListener_00c10dd0* m_pListener;       // +0x64
    __forceinline void PokeListener() { if (m_pListener) m_pListener->Poke(); }
};

struct CEditControlCentre_00c10dd0 {
    unsigned char _pad[4];
    COwner_00c10dd0* m_pOwner;             // +4
    unsigned char _pad2[0x7c0];
    unsigned long m_flags;                 // +0x7c8
    long ProcessGuard(void* argument);
};

long CEditControlCentre_00c10dd0::ProcessGuard(void* argument)
{
    CScopedLock lock(&g_lock_0143bb14);
    Helper_00c1bd80(argument);
    m_flags |= 6;
    m_pOwner->PokeListener();
    return 0;
}
