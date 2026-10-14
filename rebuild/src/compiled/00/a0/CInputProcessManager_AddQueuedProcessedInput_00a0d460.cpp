// CInputProcessManager::AddQueuedProcessedInput(const CProcessedInput& input) -- when the queue index (+0x6f4) is within the ten slots, block-copy the 0xb0-byte record into QueuedProcessedInputs[index] (+0x14) and bump the index
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A0D460, length 55 bytes (+ int3 padding). No callees.
// Layout from Ego_r.pdb CInputProcessManager (1784 bytes): InputProcessList +4 (CLinkedList, 16 bytes), QueuedProcessedInputs +0x14
// (CProcessedInput[10], 0xb0 each: Player, Type, CGameEvent[4], NoGameEvents, Priority), NoQueuedProcessedInputs +0x6f4.
// Shape: the count is read once for the signed range test (`jl` / `cmp eax,0Ah ; jge`), the slot address is `imul eax,0B0h` plus
// `this`+0x14, the record copied with a 0x2c-dword `rep movsd`, and the count re-read for the increment (the copy may alias), `ret 4`.
struct CProcessedInput {                            // 0xb0 bytes (engine/CProcessedInput.h)
    long          Player;                           // +0x00
    long          Type;                             // +0x04 EProcessedEventType
    unsigned char GameEvents[0xa0];                 // +0x08 CGameEvent[4]
    unsigned char NoGameEvents;                     // +0xa8
    unsigned char _pad_0xa9[3];
    long          Priority;                         // +0xac EGameEventPriority
};
struct CInputProcessManager {
    virtual void v00();
    unsigned char   InputProcessList[0x10];         // +0x04 CLinkedList<CAInputProcess>
    CProcessedInput QueuedProcessedInputs[10];      // +0x14
    long            NoQueuedProcessedInputs;        // +0x6f4
    void AddQueuedProcessedInput(const CProcessedInput& input);
};

void CInputProcessManager::AddQueuedProcessedInput(const CProcessedInput& input)
{
    if (NoQueuedProcessedInputs >= 0 && NoQueuedProcessedInputs < 10) {
        QueuedProcessedInputs[NoQueuedProcessedInputs] = input;
        ++NoQueuedProcessedInputs;
    }
}