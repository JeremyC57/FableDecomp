// CEditControlCentre_00c10d40::SetEnabled(void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the object at +0x7bc is told out of line (@ 0x00C1C620) whether the argument is null; returns 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C10D40, oracle length 107 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call CScopedLock::CScopedLock ; mov edx,[value] ; mov ecx,[esi+7BCh] ; test edx,edx ; sete al ; mov dword ptr [esp+14h],0 ; push eax ; call CTarget::SetHidden ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~CScopedLock ; mov ecx,[esp+0Ch] ; xor eax,eax ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret 4
// A bool argument computed as `value == 0` (`sete al`, pushed as a dword). The manifest label is
// propagated garbage; the class is named by address (the edit control centre of 0x00C10BA0).
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CTarget_00c10d40 {
    void SetHidden(bool hidden);           // 0x00C1C620
};

struct CEditControlCentre_00c10d40 {
    unsigned char _pad[0x7bc];
    CTarget_00c10d40* m_pTarget;           // +0x7bc
    long SetEnabled(void* value);
};

long CEditControlCentre_00c10d40::SetEnabled(void* value)
{
    CScopedLock lock(&g_lock_0143bb14);
    m_pTarget->SetHidden(value == 0);
    return 0;
}