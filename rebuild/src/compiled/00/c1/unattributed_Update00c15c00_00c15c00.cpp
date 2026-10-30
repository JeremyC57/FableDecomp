// CTCGraphicAppearanceSprite_00c15c00::Update() -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the object at +0x34 of the object at +4 is called out of line (@ 0x00C0B130) with the pointer at +0x14 and the 16-bit member at +0xe; the float result is dropped
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C15C00, oracle length 106 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; mov ecx,[esi+14h] ; mov edx,[esi+4] ; xor eax,eax ; mov ax,[esi+0Eh] ; mov dword ptr [esp+14h],0 ; push eax ; push ecx ; mov ecx,[edx+34h] ; call CTarget::Apply ; fstp st(0) ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+0Ch] ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret
// The 16-bit member is passed to an `unsigned short` parameter (`xor eax,eax ; mov ax`); the
// callee's float result is popped. The manifest label is propagated garbage; the class is named by address.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct CTarget_00c15c00 {
    float Apply(void* value, unsigned short index);   // 0x00C0B130
};

struct COwner_00c15c00 {
    unsigned char _pad[0x34];
    CTarget_00c15c00* m_pTarget;           // +0x34
};

struct CTCGraphicAppearanceSprite_00c15c00 {
    unsigned char _pad[4];
    COwner_00c15c00* m_pOwner;             // +4
    unsigned char _pad2[6];
    unsigned short m_index;                // +0xe
    unsigned char _pad3[4];
    void* m_pValue;                        // +0x14
    void Update();
};

void CTCGraphicAppearanceSprite_00c15c00::Update()
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    m_pOwner->m_pTarget->Apply(m_pValue, m_index);
}