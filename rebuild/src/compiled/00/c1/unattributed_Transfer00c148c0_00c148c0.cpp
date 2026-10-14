// CPersistContext_00c148c0::Transfer(void*, void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the object at +8 receives both arguments out of line (@ 0x00C2A300)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C148C0, oracle length 102 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call CScopedLock::CScopedLock ; mov eax,[b] ; mov ecx,[a] ; push eax ; push ecx ; mov ecx,[esi+8] ; mov dword ptr [esp+1Ch],0 ; call CTarget::Transfer ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~CScopedLock ; mov ecx,[esp+0Ch] ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret 8
// The scoped-lock shape with two forwarded arguments. The manifest label is propagated; the
// class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CTarget_00c148c0 {
    void Transfer(void* a, void* b);       // 0x00C2A300
};

struct CPersistContext_00c148c0 {
    unsigned char _pad[8];
    CTarget_00c148c0* m_pTarget;           // +8
    void Transfer(void* a, void* b);
};

void CPersistContext_00c148c0::Transfer(void* a, void* b)
{
    CScopedLock lock(&g_lock_0143bb14);
    m_pTarget->Transfer(a, b);
}