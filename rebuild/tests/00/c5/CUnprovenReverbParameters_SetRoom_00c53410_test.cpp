// Behaviour test for CUnprovenReverbParameters::SetRoom @ 0x00C53410: the value lands in Room and only bit 0x20 is added to the dirty flags.
#include <cstdio>
class CUnprovenReverbParameters {
public:
    unsigned long DirtyFlags;
    long          Room;
    long          RoomHF;
    long          RoomLF;
    void SetRoom(long value);
};
void CUnprovenReverbParameters::SetRoom(long value)
{
    Room = value;
    DirtyFlags |= 0x20;
}
int main()
{
    CUnprovenReverbParameters p;
    p.DirtyFlags = 0x1; p.Room = 1; p.RoomHF = 2; p.RoomLF = 3;
    p.SetRoom(-1234);
    bool ok = p.Room == -1234 && p.DirtyFlags == (0x1u | 0x20u);
    p.SetRoom(-100);
    ok = ok && p.Room == -100 && p.DirtyFlags == (0x1u | 0x20u);
    long others = p.Room + p.RoomHF + p.RoomLF - p.Room;
    long expect = 1 + 2 + 3 - 1;
    ok = ok && others == expect;
    if (!ok) { std::printf("00c53410_TEST FAIL\n"); return 1; }
    std::printf("00c53410_TEST PASS\n");
    return 0;
}