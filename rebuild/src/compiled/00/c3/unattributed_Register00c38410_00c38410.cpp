// CParticleProgram_00c38410::Register() -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the object at +4 registers `this` out of line (@ 0x00C2C8F0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C38410, oracle length 91 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call CScopedLock::CScopedLock ; mov ecx,[esi+4] ; push esi ; mov dword ptr [esp+18h],0 ; call CTarget::Add ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~CScopedLock ; mov ecx,[esp+0Ch] ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret
// The scoped-lock shape of `2026-10-14_linux_i1117` with the body a call on the member pointer at
// +4 taking `this`. The manifest label is propagated; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CParticleProgram_00c38410;
struct CTarget_00c38410 {
    void Add(CParticleProgram_00c38410* program);   // 0x00C2C8F0
};

struct CParticleProgram_00c38410 {
    unsigned char _pad[4];
    CTarget_00c38410* m_pTarget;                // +4
    void Register();
};

void CParticleProgram_00c38410::Register()
{
    CScopedLock lock(&g_lock_0143bb14);
    m_pTarget->Add(this);
}