// CParticleProgram_00c149e0::Add(void*) -- EH-framed: under a scoped lock over the global @ 0x0143BB14, the object at +8 receives the argument out of line (@ 0x00C2A6D0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C149E0, oracle length 97 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; sub esp,8 ; push esi ; mov esi,ecx ; push offset g_lock ; lea ecx,[esp+8] ; call PKCProtectionInstance::PKCProtectionInstance ; mov eax,[value] ; mov ecx,[esi+8] ; push eax ; mov dword ptr [esp+18h],0 ; call CTarget::Add ; lea ecx,[esp+4] ; mov dword ptr [esp+14h],-1 ; call ~PKCProtectionInstance ; mov ecx,[esp+0Ch] ; pop esi ; mov fs:[0],ecx ; add esp,14h ; ret 4
// The scoped-lock shape of `2026-10-14_linux_i1117` with a forwarded argument. The manifest label
// is propagated; the class is named by address.
class PKCCriticalSection;
extern PKCCriticalSection g_lock_0143bb14;        // 0x0143BB14

class PKCProtectionInstance { public:                       // 8-byte scoped lock
    PKCCriticalSection* m_pTarget;
    long m_state;
    PKCProtectionInstance(PKCCriticalSection* target);   // 0x00C0FFC0
    ~PKCProtectionInstance();                        // 0x00C0FFE0
};

struct CTarget_00c149e0 {
    void Add(void* value);                 // 0x00C2A6D0
};

struct CParticleProgram_00c149e0 {
    unsigned char _pad[8];
    CTarget_00c149e0* m_pTarget;           // +8
    void Add(void* value);
};

void CParticleProgram_00c149e0::Add(void* value)
{
    PKCProtectionInstance lock(&g_lock_0143bb14);
    m_pTarget->Add(value);
}