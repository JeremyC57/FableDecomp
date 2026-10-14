// CRenderEffectHolder_00cb0040::SetRenderEffect(IEffect*) -- EH-framed: holds the critical section at +8 through a scoped lock (import enter/leave, EH state 0 while held), adds a reference to the new effect (`__stdcall` slot 1), detaches the pending target at +0x54 from the current effect at +0x60 (slot 6) and releases it (slot 2), stores the new effect and rebuilds out of line (@ 0x00CAF750); returns 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB0040, oracle length 140 bytes. Built under /EHsc.
// Retail bytes:  push -1 ; push offset __ehhandler ; mov eax,fs:[0] ; push eax ; mov fs:[0],esp ; push ecx ; push ebp ; push esi ; push edi ; mov esi,ecx ; lea edi,[esi+8] ; push edi ; mov [esp+10h],edi ; call [__imp_EnterCriticalSection] ; mov ebp,[effect] ; test ebp,ebp ; mov dword ptr [esp+18h],0 ; je noadd ; mov eax,[ebp] ; push ebp ; call [eax+4] ; noadd: mov eax,[esi+60h] ; test eax,eax ; je store ; mov ecx,[esi+54h] ; test ecx,ecx ; je rel ; mov edx,[eax] ; push ecx ; push eax ; call [edx+18h] ; mov dword ptr [esi+54h],0 ; rel: mov eax,[esi+60h] ; mov ecx,[eax] ; push eax ; call [ecx+8] ; store: mov ecx,esi ; mov [esi+60h],ebp ; call Rebuild ; push edi ; call [__imp_LeaveCriticalSection] ; mov ecx,[esp+10h] ; pop edi ; pop esi ; xor eax,eax ; pop ebp ; mov fs:[0],ecx ; add esp,10h ; ret 4
// The scoped lock is a one-word local holding the section's address (parked for the unwind
// funclet); its leave registers no EH state change because the import cannot throw. The
// manifest label is propagated; the class is named by address.
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(void* section);
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(void* section);

struct CSectionLock_00cb0040 {             // scoped critical-section holder
    void* m_pSection;
    CSectionLock_00cb0040(void* section) : m_pSection(section) { EnterCriticalSection(m_pSection); }
    ~CSectionLock_00cb0040() { LeaveCriticalSection(m_pSection); }
};

struct IEffect_00cb0040 {                  // COM-style: `this` is pushed as the first stack argument
    virtual long __stdcall QueryInterface();
    virtual unsigned long __stdcall AddRef();        // slot 1
    virtual unsigned long __stdcall Release();       // slot 2
    virtual void __stdcall v03();
    virtual void __stdcall v04();
    virtual void __stdcall v05();
    virtual long __stdcall Detach(void* target);     // slot 6
};

struct CRenderEffectHolder_00cb0040 {
    unsigned char _pad[8];
    unsigned char m_section[0x18];         // +8
    unsigned char _pad2[0x34];
    void* m_pPendingTarget;                // +0x54
    unsigned char _pad3[8];
    IEffect_00cb0040* m_pEffect;           // +0x60
    void Rebuild();                        // 0x00CAF750
    long SetRenderEffect(IEffect_00cb0040* effect);
};

long CRenderEffectHolder_00cb0040::SetRenderEffect(IEffect_00cb0040* effect)
{
    CSectionLock_00cb0040 lock(m_section);
    if (effect)
        effect->AddRef();
    if (m_pEffect) {
        if (m_pPendingTarget) {
            m_pEffect->Detach(m_pPendingTarget);
            m_pPendingTarget = 0;
        }
        m_pEffect->Release();
    }
    m_pEffect = effect;
    Rebuild();
    return 0;
}
