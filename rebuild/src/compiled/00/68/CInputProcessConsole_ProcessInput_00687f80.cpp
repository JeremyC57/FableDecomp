// CInputProcessConsole::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- make the global console on first use (counted pointer at 0x013CAA40, a 0x9c-byte CConsole), hand it the event and mark the processed input handled when it took it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00687F80, length 80 bytes. Vtable 0x01237F24 slot 4 (CInputProcessConsole); `this` is unused.
// Callees: operator new @ 0x00BFEA1A (__cdecl, `add esp,4`), CConsole::CConsole @ 0x009ECD80 (vtable 0x0129C600; null-checked
// placement), CCountedPointer<CConsole>::Reset @ 0x00413520, CConsole::ProcessInput(const CInputEvent&) @ 0x009EA530 (the stub
// `xor al,al ; ret 4`; the label "CThingFilter_IsUnconscious::operator()" is propagated), CProcessedInput::SetHandled @ 0x00A0D390
// (NoGameEvents = 0, Type = 2; labelled "CWomanToAttract::Init"). Shape: the global's object word is tested, the `new` result
// null-tested around the constructor, then `Reset`; the console pointer is re-read into ecx after the event is loaded.
class CInputEvent;
class CProcessedInput { public:
    void SetHandled();                              // 0x00A0D390
};
class CConsole { public:                                   // vtable 0x0129C600, 0x9c bytes
    virtual void v00();
    unsigned char _body[0x98];
    CConsole();                                     // 0x009ECD80
    bool ProcessInput(const CInputEvent& event);    // 0x009EA530
};
struct CCountedPointer_CConsole {                   // {object, count block}
    CConsole* m_pObject;
    void*     m_pCount;
    void Reset(CConsole* object);                   // 0x00413520
};
extern CCountedPointer_CConsole g_pConsole;         // 0x013CAA40

class CInputProcessConsole { public:
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessConsole::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (g_pConsole.m_pObject == 0)
        g_pConsole.Reset(new CConsole());
    if (g_pConsole.m_pObject->ProcessInput(event))
        processed.SetHandled();
}