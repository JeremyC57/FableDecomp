// CEditControlCentre_00c10a90::SetMode(bool) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the flag byte at +0x885 is set from the argument and the object at +0x7bc updated out of line (@ 0x00C1C460)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C10A90, oracle length 105 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call CScopedLock::CScopedLock ; mov al,[mode] ; mov ecx,[esi+7BCh] ; mov dword ptr [esp+14h],0 ; mov [esi+885h],al ; call CTarget::Update ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~CScopedLock ; mov ecx,[esp+0Ch] ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret 4
// The scoped-lock shape with a byte store before the call. The manifest label (`OnDie`) is
// propagated garbage; the class is named by address (the edit control centre of 0x00C10BA0).
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct CTarget_00c10a90 {
    void Update();                         // 0x00C1C460
};

struct CEditControlCentre_00c10a90 {
    unsigned char _pad[0x7bc];
    CTarget_00c10a90* m_pTarget;           // +0x7bc
    unsigned char _pad2[0xc5];
    bool m_bMode;                          // +0x885
    void SetMode(bool mode);
};

void CEditControlCentre_00c10a90::SetMode(bool mode)
{
    CScopedLock lock(&g_lock_0143bb14);
    m_bMode = mode;
    m_pTarget->Update();
}