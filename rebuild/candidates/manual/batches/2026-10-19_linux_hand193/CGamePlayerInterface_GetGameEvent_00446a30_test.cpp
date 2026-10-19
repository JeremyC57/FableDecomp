// Behaviour test for 0x00446A30 CGamePlayerInterface::GetGameEvent: suspended, queued inputs, refill, exhaustion.
#include <cstdio>
#include <cstring>
static int g_clears = 0, g_update = 0, g_process = 0, g_fill = 0;
struct CGameEvent {
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
    const CGameEvent& GetGameEvent(unsigned long index) const { return GameEvents[index]; }
    void Clear() { Type = PROCESSED_EVENT_TYPE_NONE; NoGameEvents = 0; ++g_clears; }
};

class CGamePlayerInterface {
public:
    virtual bool GetGameEvent(CGameEvent& event) const;
public:
    bool GetInputProcessEventFromUpdate(CProcessedInput& input) const { ++g_update; if (!g_fill) return false; input.Type = (EProcessedEventType)1; input.NoGameEvents = 1; input.GameEvents[0].Type = 77; return true; }
    bool GetInputProcessEvent(CProcessedInput& input) const { ++g_process; return false; }
public:

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

int main()
{
    static CGamePlayerInterface gpi;
    std::memset(&gpi, 0, sizeof(gpi));
    CGameEvent ev;
    int fail = 0;
    gpi.Suspended = true;
    if (gpi.GetGameEvent(ev)) fail |= 1;
    gpi.Suspended = false;
    // two queued inputs: first holds two events, second one
    gpi.NumQueuedProcessedInputs = 2;
    gpi.QueuedProcessedInputs[0].Type = (EProcessedEventType)1; gpi.QueuedProcessedInputs[0].NoGameEvents = 2;
    gpi.QueuedProcessedInputs[0].GameEvents[0].Type = 10; gpi.QueuedProcessedInputs[0].GameEvents[1].Type = 11;
    gpi.QueuedProcessedInputs[1].Type = (EProcessedEventType)1; gpi.QueuedProcessedInputs[1].NoGameEvents = 1;
    gpi.QueuedProcessedInputs[1].GameEvents[0].Type = 20;
    if (!gpi.GetGameEvent(ev) || ev.Type != 10) fail |= 2;
    if (!gpi.GetGameEvent(ev) || ev.Type != 11 || g_clears != 1) fail |= 4;
    if (!gpi.GetGameEvent(ev) || ev.Type != 20 || gpi.CurrentQueuedProcessedInputNo != 2) fail |= 8;
    // queue exhausted, both processes empty
    if (gpi.GetGameEvent(ev) || gpi.NumQueuedProcessedInputs != 0 || g_process != 1 || g_update != 1) fail |= 16;
    g_fill = 1;
    if (!gpi.GetGameEvent(ev) || ev.Type != 77 || g_clears != 3) fail |= 32;
    if (fail) { std::printf("00446a30_TEST FAIL\n"); return 1; }
    std::printf("00446a30_TEST PASS\n");
    return 0;
}
