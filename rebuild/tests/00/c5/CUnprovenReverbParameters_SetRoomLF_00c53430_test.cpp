// Behaviour test for CUnprovenReverbParameters::SetRoomLF @ 0x00C53430: the value lands in RoomLF and only bit 0x80 is added to the dirty flags.
#include <cstdio>
class CUnprovenReverbParameters {
public:
    unsigned long DirtyFlags;
    long          Room;
    long          RoomHF;
    long          RoomLF;
    void SetRoomLF(long value);
};
void CUnprovenReverbParameters::SetRoomLF(long value)
{
    RoomLF = value;
    DirtyFlags |= 0x80;
}
int main()
{
    CUnprovenReverbParameters p;
    p.DirtyFlags = 0x1; p.Room = 1; p.RoomHF = 2; p.RoomLF = 3;
    p.SetRoomLF(-1234);
    bool ok = p.RoomLF == -1234 && p.DirtyFlags == (0x1u | 0x80u);
    p.SetRoomLF(-100);
    ok = ok && p.RoomLF == -100 && p.DirtyFlags == (0x1u | 0x80u);
    long others = p.Room + p.RoomHF + p.RoomLF - p.RoomLF;
    long expect = 1 + 2 + 3 - 3;
    ok = ok && others == expect;
    if (!ok) { std::printf("00c53430_TEST FAIL\n"); return 1; }
    std::printf("00c53430_TEST PASS\n");
    return 0;
}