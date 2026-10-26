// Behaviour test for 0x00687E10 CInputProcessMain::AddManualCameraEvent: one game event of type 0x0B reaches the input, built with
// player -1, valid, not a replacement, and carrying exactly one data byte equal to the flag.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#endif

#pragma pack(push, 1)
class CGameEvent { public:
    int           Type;
    signed char   Player;
    unsigned char Data[32];
    unsigned char EndPos;
    bool          Valid;
    bool          Replacement;
};
#pragma pack(pop)

struct CGameEventBuilder : CGameEvent {
    CGameEventBuilder(int type) { Type = type; Player = -1; EndPos = 0; Valid = true; Replacement = false; }
    void Copy(unsigned int length, const void* data)        // models CMemoryBuffer::Copy 0x009F1760
    {
        std::memcpy(Data + EndPos, data, length);
        EndPos = (unsigned char)(EndPos + length);
    }
    void AddUBYTE(unsigned char value) { Copy(1, &value); }
};

class CProcessedInput {
public:
    CGameEvent Last;
    int        Count;
    void AddGameEvent(const CGameEvent& event) { Last = event; ++Count; }   // models 0x00A0D340
};

class CInputProcessMain {
public:
    static void __fastcall AddManualCameraEvent(unsigned char on, CProcessedInput& input);
};

void __fastcall CInputProcessMain::AddManualCameraEvent(unsigned char on, CProcessedInput& input)
{
    CGameEventBuilder event(0x0B);
    event.AddUBYTE(on);
    input.AddGameEvent(event);
}

int main()
{
    if (sizeof(CGameEvent) != 0x28) { std::printf("00687e10_TEST FAIL\n"); return 1; }
    CProcessedInput input;
    input.Count = 0;
    CInputProcessMain::AddManualCameraEvent(1, input);
    if (input.Count != 1 || input.Last.Type != 0x0B || input.Last.Player != -1 || input.Last.EndPos != 1 ||
        input.Last.Data[0] != 1 || !input.Last.Valid || input.Last.Replacement) {
        std::printf("00687e10_TEST FAIL\n"); return 1;
    }
    CInputProcessMain::AddManualCameraEvent(0, input);
    if (input.Count != 2 || input.Last.Data[0] != 0 || input.Last.EndPos != 1) { std::printf("00687e10_TEST FAIL\n"); return 1; }
    std::printf("00687e10_TEST PASS\n");
    return 0;
}