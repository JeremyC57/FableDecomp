// CInputManagerDX_00c088d0::ReleaseDevice() -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the device at +0x64 is released through its virtual slot 17 with 1 and cleared when present
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C088D0, oracle length 103 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call CScopedLock::CScopedLock ; mov ecx,[esi+64h] ; test ecx,ecx ; mov dword ptr [esp+14h],0 ; je skip ; mov eax,[ecx] ; push 1 ; call [eax+44h] ; mov dword ptr [esi+64h],0 ; skip: lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~CScopedLock ; mov ecx,[esp+0Ch] ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret
// The scoped-lock shape of `2026-10-14_linux_i1117`. The manifest label (a destructor name) is
// propagated garbage for this body; the class is named by address.
struct CGlobalLockTarget;
extern CGlobalLockTarget g_lock_0143bb14;        // 0x0143BB14

struct CScopedLock {                       // 8-byte scoped lock
    CGlobalLockTarget* m_pTarget;
    long m_state;
    CScopedLock(CGlobalLockTarget* target);   // 0x00C0FFC0
    ~CScopedLock();                        // 0x00C0FFE0
};

struct IInputDevice_00c088d0 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16();
    virtual void Release(long flags);      // slot 17
};

struct CInputManagerDX_00c088d0 {
    unsigned char _pad[0x64];
    IInputDevice_00c088d0* m_pDevice;      // +0x64
    void ReleaseDevice();
};

void CInputManagerDX_00c088d0::ReleaseDevice()
{
    CScopedLock lock(&g_lock_0143bb14);
    if (m_pDevice) {
        m_pDevice->Release(1);
        m_pDevice = 0;
    }
}