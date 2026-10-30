// CTCGraphicAppearanceSprite_00c15e30::SetRotationAngle(long) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the member at +0x10 is set and the out-of-line update @ 0x00C15B90 runs
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C15E30, oracle length 98 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; mov eax,[angle] ; mov ecx,esi ; mov dword ptr [esp+14h],0 ; mov [esi+0x10],eax ; call Update ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+0Ch] ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret 4
// The scoped-lock shape of `2026-10-14_linux_i1117` with a member store before the call. The
// manifest label is propagated; the class is named by address.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct CTCGraphicAppearanceSprite_00c15e30 {
    unsigned char _pad[0x10];
    long m_angle;                          // +0x10
    void Update();                         // 0x00C15B90
    void SetRotationAngle(long angle);
};

void CTCGraphicAppearanceSprite_00c15e30::SetRotationAngle(long angle)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    m_angle = angle;
    Update();
}