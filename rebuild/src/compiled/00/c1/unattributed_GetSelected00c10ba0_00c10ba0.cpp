// CEditControlCentre_00c10ba0::GetSelected() -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the object at +0x7bc is queried out of line (@ 0x00C1C030) and the result returned
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C10BA0, oracle length 97 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; mov ecx,[esi+7BCh] ; mov dword ptr [esp+14h],0 ; call CTarget::Get ; lea ecx,[esp+4] ; mov esi,eax ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+0Ch] ; mov eax,esi ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret
// The scoped-lock shape of `2026-10-14_linux_i1117` with a returned result kept in esi across the
// lock's destructor. The manifest label is propagated; the class is named by address.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct CTarget_00c10ba0 {
    void* Get();                           // 0x00C1C030
};

struct CEditControlCentre_00c10ba0 {
    unsigned char _pad[0x7bc];
    CTarget_00c10ba0* m_pTarget;           // +0x7bc
    void* GetSelected();
};

void* CEditControlCentre_00c10ba0::GetSelected()
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    return m_pTarget->Get();
}