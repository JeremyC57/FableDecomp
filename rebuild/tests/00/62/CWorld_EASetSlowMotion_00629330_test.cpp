// Behaviour test (0x00629330): CWorld::EASetSlowMotion reads the event bool payload into the SlowMotion flag
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
struct CGameEvent {
    long          Type;
    signed char   Player;
    unsigned char Data[32];
    unsigned char EndPos;
    long GetData(CMemoryBuffer& buffer) const { std::memcpy(buffer.Data, Data, EndPos); buffer.Length = EndPos; return EndPos; }
};
struct CWorld {
    bool SlowMotion;
    void EASetSlowMotion(const CGameEvent* event)
    {
        CMemoryBuffer buffer;
        event->GetData(buffer);
        bool slowMotion = buffer.ReadBool();
        SlowMotion = (slowMotion == true);
    }
};
int main()
{
    CGameEvent e; e.Type = 1; e.Player = 0; e.EndPos = 1;
    bool on = true; std::memcpy(e.Data, &on, 1);
    CWorld w; w.SlowMotion = false;
    w.EASetSlowMotion(&e);
    if (!w.SlowMotion) { std::printf("00629330_TEST FAIL\n"); return 1; }
    bool off = false; std::memcpy(e.Data, &off, 1);
    w.EASetSlowMotion(&e);
    if (w.SlowMotion) { std::printf("00629330_TEST FAIL\n"); return 1; }
    std::printf("00629330_TEST PASS\n");
    return 0;
}