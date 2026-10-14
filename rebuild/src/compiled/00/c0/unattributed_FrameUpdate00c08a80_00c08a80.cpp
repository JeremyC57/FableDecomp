// CCreatureActionHeroJump_00c08a80::FrameUpdate() -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the object at +0x60 is updated out of line (@ 0x00C15AA0) when present
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C08A80, oracle length 93 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call CScopedLock::CScopedLock ; mov ecx,[esi+0x60] ; test ecx,ecx ; mov dword ptr [esp+14h],0 ; pop esi ; je skip ; call CTarget::Update ; skip: lea ecx,[esp] ; mov dword ptr [esp+10h],-1 ; call ~CScopedLock ; mov ecx,[esp+8] ; mov fs:[0],ecx ; add esp,14h ; ret
// The scoped-lock shape of `2026-10-14_linux_i1117` with a null-checked call on the member
// pointer (esi is popped before the branch since `this` is no longer needed). The manifest label
// is propagated; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CTarget_00c08a80 {
    void Update();                         // 0x00C15AA0
};

struct CCreatureActionHeroJump_00c08a80 {
    unsigned char _pad[0x60];
    CTarget_00c08a80* m_pTarget;                // +0x60
    void FrameUpdate();
};

void CCreatureActionHeroJump_00c08a80::FrameUpdate()
{
    CScopedLock lock(&g_lock_0143bb14);
    if (m_pTarget)
        m_pTarget->Update();
}