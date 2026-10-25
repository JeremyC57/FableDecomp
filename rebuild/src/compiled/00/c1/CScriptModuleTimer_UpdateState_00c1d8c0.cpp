// CScriptModuleTimer::UpdateState(CScriptContext* context, CScriptArgument* argument) -- -1 when the owner has no dispatch target; otherwise the target's slot-1 dispatch with the module's remaining/total seconds, its current frame, the argument's value for the context, the argument and the owner's mode word, mapped to 0 (handled) / -1
// Leaf: UpdateState
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1D8C0, 101 bytes (`ret 8`). Descriptive name (no PDB covers this library): the
//   manifest's "CScriptModuleSelfTerminate_UpdateState" is propagated. Slot 1 of vtable 0x012B32A0 (only reference); the
//   slot-2 sibling 0x00C1D950 forwards the same three timer values to 0x00C1DF70 (landed), whose target dispatch is
//   the same six-argument slot 1.
// Callees: GetRemainingTimeSeconds @ 0x00C1D820, GetTotalTimeSeconds @ 0x00C1D850, GetCurrentFrame @ 0x00C1D810 (all
//   thiscall, no arguments), CScriptArgument::GetValue @ 0x00C26260 (thiscall, the context), the target's slot 1.
// Shape: the dispatch result goes into a named bool and the ternary is a separate statement: only then does VC7.1 load
//   the target and its vtable into edi/ebx before the argument calls (the one-expression `->Dispatch(...) ? 0 : -1`
//   re-reads the target after them). Right-to-left argument evaluation (the mode word and the argument are pushed
//   before any call); `neg al ; sbb ; neg ; dec` is `handled ? 0 : -1`.
struct CScriptContext;

struct CScriptArgument {
    long GetValue(CScriptContext* context); // 0x00C26260
};

struct CDispatchTarget {
    virtual void v00();
    virtual bool Dispatch(unsigned long remaining, unsigned long total, unsigned long frame, long value,
                          CScriptArgument* argument, long mode);   // slot 1 (+4)
    virtual unsigned long GetFramesPerSecond();                    // slot 2 (+8)
};

struct CModeSource {
    unsigned char _pad[0x7b4];
    long m_mode;                            // +0x7B4
};

struct CModeOwner {
    unsigned char _pad[0x38];
    CModeSource* m_pSource;                 // +0x38
};

struct CScriptModuleOwner {
    void* _pad00;
    CDispatchTarget* m_pTarget;             // +0x04
    CModeOwner* m_pModeOwner;               // +0x08
};

struct CScriptModuleTimer {                 // vtable 0x012B32A0
    virtual void v00();
    CScriptModuleOwner* POwner;             // +0x04
    unsigned long GetCurrentFrame();        // 0x00C1D810
    unsigned long GetRemainingTimeSeconds();    // 0x00C1D820
    unsigned long GetTotalTimeSeconds();    // 0x00C1D850
    virtual long UpdateState(CScriptContext* context, CScriptArgument* argument);
};

long CScriptModuleTimer::UpdateState(CScriptContext* context, CScriptArgument* argument)
{
    if (!POwner->m_pTarget)
        return -1;
    bool handled = POwner->m_pTarget->Dispatch(GetRemainingTimeSeconds(), GetTotalTimeSeconds(), GetCurrentFrame(),
                                               argument->GetValue(context), argument,
                                               POwner->m_pModeOwner->m_pSource->m_mode);
    return handled ? 0 : -1;
}