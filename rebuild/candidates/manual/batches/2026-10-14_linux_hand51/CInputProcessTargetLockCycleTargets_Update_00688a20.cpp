// CInputProcessTargetLockCycleTargets::Update(CProcessedInput& processed) -- build a scratch CInputEvent (0x00A04410 / destroyed @ 0x00A03B30); unless the (1, 2) button query (0x00687D50) holds, clear the cycle index (+0x28) and set the target (+0x30) to -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00688A20, length 64 bytes.
// The query label "CPersistContext::Transfer<CEngineGraphic>" is propagated. Shape: a 0x34-byte frame for the event, `push 2 ; push 1`, `ret 4`.
struct CInputEvent {                               // 0x34 bytes
    unsigned char _body[0x34];
    CInputEvent();                                 // 0x00A04410 (labelled "CDefaultEngineMemoryInit")
    ~CInputEvent();                                // 0x00A03B30 (an empty `ret`, labelled "$E2")
};
struct CProcessedInput;
struct CInputProcessTargetLockCycleTargets {
    virtual void v00();
    unsigned char _pad04[0x24];
    long          m_cycleIndex;                    // +0x28
    long          m_2c;
    long          m_target;                        // +0x30
    bool IsButtonHeld(long a, long b);             // 0x00687D50
    void Update(CProcessedInput& processed);
};

void CInputProcessTargetLockCycleTargets::Update(CProcessedInput& processed)
{
    CInputEvent event;
    if (!IsButtonHeld(1, 2)) {
        m_cycleIndex = 0;
        m_target = -1;
    }
}
