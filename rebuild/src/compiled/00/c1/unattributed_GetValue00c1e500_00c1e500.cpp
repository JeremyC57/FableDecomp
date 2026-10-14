// CReactionFrequency_00c1e500::GetValue() -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the float query @ 0x00C2D9A0 on the member at +8 of the object at +4, the result parked in the frame across the lock's destructor
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1E500, oracle length 101 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,0Ch ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+0Ch] ; call CScopedLock::CScopedLock ; mov ecx,[esi+4] ; add ecx,8 ; mov dword ptr [esp+18h],0 ; call CTarget::Get ; fstp dword ptr [esp+4] ; lea ecx,[esp+8] ; mov dword ptr [esp+18h],-1 ; call ~CScopedLock ; fld dword ptr [esp+4] ; mov ecx,[esp+10h] ; pop esi ; mov fs:[0],ecx ; add esp,18h ; ret
// The scoped-lock shape of `2026-10-14_linux_i1117` returning a float: the result is stored to a
// frame temporary before the lock's destructor and reloaded after it. The manifest label is
// propagated garbage; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CTarget_00c1e500 {
    float Get();                           // 0x00C2D9A0
};

struct CTargetOwner_00c1e500 {
    unsigned char _pad[8];
    CTarget_00c1e500 m_target;             // +8
};

struct CReactionFrequency_00c1e500 {
    unsigned char _pad[4];
    CTargetOwner_00c1e500* m_pOwner;       // +4
    float GetValue();
};

float CReactionFrequency_00c1e500::GetValue()
{
    CScopedLock lock(&g_lock_0143bb14);
    return m_pOwner->m_target.Get();
}