// CTCInventoryMagic_00c1c6a0::Update() -- EH-framed: under a scoped lock over the global @ 0x0143BAFC, the out-of-line step @ 0x00C1C460 on `this`
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1C6A0, oracle length 89 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; mov ecx,esi ; mov dword ptr [esp+14h],0 ; call Step ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+0Ch] ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret
// The scoped-lock shape of `2026-10-14_linux_i1117` with the body a call on `this`. The manifest
// label (`OpenInventory`) is propagated; the class is named by address.
class PKCCriticalSection;
extern PKCCriticalSection g_soundLock;        // 0x0143BAFC

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct CTCInventoryMagic_00c1c6a0 {
    void Step();                           // 0x00C1C460
    void Update();
};

void CTCInventoryMagic_00c1c6a0::Update()
{
    PKCProtectionInstance lock(&g_soundLock);
    Step();
}