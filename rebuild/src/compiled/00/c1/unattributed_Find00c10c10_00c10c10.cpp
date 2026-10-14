// CEditControlCentre_00c10c10::Find(void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the object at +0x7bc is queried out of line (@ 0x00C1BEC0) with the argument; the result survives the lock's destructor in esi
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C10C10, oracle length 104 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call CScopedLock::CScopedLock ; mov eax,[key] ; mov ecx,[esi+7BCh] ; push eax ; mov dword ptr [esp+18h],0 ; call CTarget::Find ; lea ecx,[esp+4] ; mov esi,eax ; mov dword ptr [esp+14h],-1 ; call ~CScopedLock ; mov ecx,[esp+0Ch] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret 4
// The scoped-lock shape with a forwarded argument and a returned result. The manifest label is
// propagated garbage; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CTarget_00c10c10 {
    void* Find(void* key);                 // 0x00C1BEC0
};

struct CEditControlCentre_00c10c10 {
    unsigned char _pad[0x7bc];
    CTarget_00c10c10* m_pTarget;           // +0x7bc
    void* Find(void* key);
};

void* CEditControlCentre_00c10c10::Find(void* key)
{
    CScopedLock lock(&g_lock_0143bb14);
    return m_pTarget->Find(key);
}