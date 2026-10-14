// CInputProcessJumpingAndRolling::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- rolling first, then jumping, each given the event and the processed-input record
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A4700, length 35 bytes. Vtable 0x01238050 slot 4.
// Callees: CInputProcessJumpingAndRolling::ProcessRolling @ 0x006A40D0, CInputProcessJumpingAndRolling::ProcessJumping @ 0x006A4040.
// Shape: both references are kept in callee-saved registers (ebx, edi) across the first call and pushed again for the second, `ret 8`.
struct CInputEvent;
struct CProcessedInput;
struct CInputProcessJumpingAndRolling {
    void ProcessRolling(const CInputEvent& event, CProcessedInput& processed);   // 0x006A40D0
    void ProcessJumping(const CInputEvent& event, CProcessedInput& processed);   // 0x006A4040
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessJumpingAndRolling::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    ProcessRolling(event, processed);
    ProcessJumping(event, processed);
}
