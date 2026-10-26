// CGamePlayerInterface::GetGameEvent(CGameEvent& event) const -- hand out the next game event of the current processed
// input, refilling it from the queued inputs or from the input processes when it is used up
// Leaf: ?GetGameEvent@CGamePlayerInterface
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00446A30, length 224 bytes, `ret 4`.
// Callees: CGamePlayerInterface::GetInputProcessEvent(CProcessedInput&) @ 0x00446330,
// GetInputProcessEventFromUpdate(CProcessedInput&) @ 0x00446220, CProcessedInput::GetGameEvent(unsigned long) const
// @ 0x00A0D3C0 (returns a reference), CProcessedInput::Clear @ 0x00A0D300.
// Retail layout (the engine header is all padding; PDB names, offsets from these bytes): QueuedProcessedInputs
// +0x14 (ten 0xB0-byte CProcessedInput), NumQueuedProcessedInputs +0x6F4, Suspended +0x79C (PDB agrees),
// CurrentQueuedProcessedInputNo +0x7DC (PDB +0x7EC: the four PDB 16-byte containers are 12 bytes in retail),
// CurrentProcessedInput +0x7E0 (Type +4, NoGameEvents byte +0xA8), CurrentEventInProcessedInput +0x890.
// Shape: the Suspended test wraps the body (`if (!Suspended) {...} return false;`) so every failure shares the
// final `xor al,al` tail; a const virtual over mutable state; the queued copy is the implicit CProcessedInput assignment
// (`rep movsd` x 0x2C) with the post-increment of the index; GetNoGameEvents is the inline `movzx` byte read
// compared unsigned.

class CGameEvent { public:
    long Type;
    signed char Player;
    unsigned char Data[32];
    unsigned char EndPos;
    bool Valid;
    bool Replacement;
};

enum EProcessedEventType { PROCESSED_EVENT_TYPE_NONE = 0 };

class CProcessedInput {
public:
    unsigned long Player;
    EProcessedEventType Type;
    CGameEvent GameEvents[4];
    unsigned char NoGameEvents;
    long Priority;

    EProcessedEventType GetType() const { return Type; }
    unsigned long GetNoGameEvents() const { return NoGameEvents; }
    const CGameEvent& GetGameEvent(unsigned long index) const;     // 0x00A0D3C0
    void Clear();                                                   // 0x00A0D300
};

class CGamePlayerInterface {
public:
    virtual bool GetGameEvent(CGameEvent& event) const;
private:
    bool GetInputProcessEventFromUpdate(CProcessedInput& input) const;  // 0x00446220
    bool GetInputProcessEvent(CProcessedInput& input) const;            // 0x00446330

    unsigned char _pad_0x04[0x10];
    mutable CProcessedInput QueuedProcessedInputs[10];      // +0x14
    mutable long NumQueuedProcessedInputs;                  // +0x6F4
    unsigned char _pad_0x6f8[0xA4];
    bool Suspended;                                         // +0x79C
    unsigned char _pad_0x79d[0x3F];
    mutable long CurrentQueuedProcessedInputNo;             // +0x7DC
    mutable CProcessedInput CurrentProcessedInput;          // +0x7E0
    mutable unsigned long CurrentEventInProcessedInput;     // +0x890
};

bool CGamePlayerInterface::GetGameEvent(CGameEvent& event) const
{
    if (!Suspended) {
        if (CurrentProcessedInput.GetType() == PROCESSED_EVENT_TYPE_NONE) {
            CurrentEventInProcessedInput = 0;
            if (CurrentQueuedProcessedInputNo >= 0 && CurrentQueuedProcessedInputNo < NumQueuedProcessedInputs) {
                CurrentProcessedInput = QueuedProcessedInputs[CurrentQueuedProcessedInputNo++];
            } else {
                NumQueuedProcessedInputs = 0;
                CurrentQueuedProcessedInputNo = 0;
                if (!GetInputProcessEvent(CurrentProcessedInput) && !GetInputProcessEventFromUpdate(CurrentProcessedInput))
                    return false;
            }
        }

        if (CurrentEventInProcessedInput < CurrentProcessedInput.GetNoGameEvents()) {
            event = CurrentProcessedInput.GetGameEvent(CurrentEventInProcessedInput);
            ++CurrentEventInProcessedInput;
            if (CurrentEventInProcessedInput >= CurrentProcessedInput.GetNoGameEvents())
                CurrentProcessedInput.Clear();
            return true;
        }
    }
    return false;
}