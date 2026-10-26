// Behaviour test (0x006292d0): CWorld::EASetPauseMode reads the event's bool payload and pauses / unpauses the world
#include <cstdio>
#include <cstring>
struct CMemoryBuffer {
    unsigned char Data[0x20];
    long          Length;
    long          ReadPos;
    long          WritePos;
    CMemoryBuffer() : Length(0), ReadPos(0), WritePos(0) { }
    void Read(unsigned long length, void* data) { std::memcpy(data, Data + ReadPos, length); ReadPos += (long)length; }
    bool ReadBool() { bool value; Read(1, &value); return value; }
};
class CGameEvent { public:
    long          Type;
    signed char   Player;
    unsigned char Data[32];
    unsigned char EndPos;
    long GetData(CMemoryBuffer& buffer) const { std::memcpy(buffer.Data, Data, EndPos); buffer.Length = EndPos; return EndPos; }
};
class CWorld { public:
    bool Paused;
    int  Calls;
    void SetAsPaused(bool paused) { ++Calls; Paused = paused; }
    void EASetPauseMode(const CGameEvent* event)
    {
        CMemoryBuffer buffer;
        event->GetData(buffer);
        bool paused = buffer.ReadBool();
        SetAsPaused(paused == true);
    }
};
int main()
{
    CGameEvent e; e.Type = 1; e.Player = 0; e.EndPos = 1;
    bool on = true; std::memcpy(e.Data, &on, 1);
    CWorld w; w.Paused = false; w.Calls = 0;
    w.EASetPauseMode(&e);
    if (w.Calls != 1 || !w.Paused) { std::printf("006292d0_TEST FAIL\n"); return 1; }
    bool off = false; std::memcpy(e.Data, &off, 1);
    w.EASetPauseMode(&e);
    if (w.Calls != 2 || w.Paused) { std::printf("006292d0_TEST FAIL\n"); return 1; }
    std::printf("006292d0_TEST PASS\n");
    return 0;
}