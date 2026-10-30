// CTCGraphicAppearanceSprite_00c15b90::Update() -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the float computed out of line (@ 0x00C15A70) is parked in the frame and passed by value to the object at +8 (@ 0x00C2D1E0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C15B90, oracle length 106 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,0Ch ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+0Ch] ; call PKCProtectionInstance::PKCProtectionInstance ; mov ecx,esi ; mov dword ptr [esp+18h],0 ; call GetAngle ; fstp dword ptr [esp+4] ; mov eax,[esp+4] ; mov ecx,[esi+8] ; push eax ; call CTarget::SetAngle ; lea ecx,[esp+8] ; mov dword ptr [esp+18h],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+10h] ; pop esi ; mov fs:[0],ecx ; add esp,18h ; ret
// The update called by the eh11 setters: a float result passed by value (stored to the frame and
// reloaded as a dword for the push). The manifest label is propagated garbage; the class is named by address.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct CTarget_00c15b90 {
    void SetAngle(float angle);            // 0x00C2D1E0
};

struct CTCGraphicAppearanceSprite_00c15b90 {
    unsigned char _pad[8];
    CTarget_00c15b90* m_pTarget;           // +8
    float GetAngle();                      // 0x00C15A70
    void Update();
};

void CTCGraphicAppearanceSprite_00c15b90::Update()
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    float angle = GetAngle();
    m_pTarget->SetAngle(angle);
}