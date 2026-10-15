// CInputEventProcessor_00c219f0::ProcessEvent(CInputEvent*) -- runs the two event stages on the input event and returns 0 when the processor's state word reached 6 (finished), else -1
// Leaf: ProcessEvent
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C219F0, 41 bytes (+7 int3 padding). Callees (both __fastcall on `this`
// with the event): 0x00C208E0 (manifest "CScrollableList::OnDeactivate", a propagated name) and 0x00C21780 (the
// EH-framed input handler that reads CInputEvent::GetKey). Caller 0x00C10D2A loops while the result is >= 0
// (`test eax,eax ; jl`), so the result is a signed long.
// Label: "CSpellContainerList::AddChild" is propagated over byte-identical two-call forwarders. Shape: `xor eax,eax ;
// cmp word ; sete al ; dec eax` is `(state == 6) - 1` on a long result, the ternary `== 6 ? 0 : -1`.
class CInputEvent;

struct CInputEventProcessor_00c219f0 {
    unsigned char _pad00[0xAC];
    short         m_state;                       // +0xAC
    void BeginEvent(CInputEvent* event);          // 0x00C208E0
    void HandleEvent(CInputEvent* event);         // 0x00C21780
    long ProcessEvent(CInputEvent* event);
};

long CInputEventProcessor_00c219f0::ProcessEvent(CInputEvent* event)
{
    BeginEvent(event);
    HandleEvent(event);
    return m_state == 6 ? 0 : -1;
}