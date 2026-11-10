// Behaviour test for CUnprovenReverbParameters::SetRoomHF @ 0x00C53420: the value lands in RoomHF and only bit 0x40 is added to the dirty flags.
#include <cstdio>
class CUnprovenReverbParameters {
public:
    unsigned long DirtyFlags;
    long          Room;
    long          RoomHF;
    long          RoomLF;
    void SetRoomHF(long value);
};
void CUnprovenReverbParameters::SetRoomHF(long value)
{
    RoomHF = value;
    DirtyFlags |= 0x40;
}
int main()
{
    CUnprovenReverbParameters p;
    p.DirtyFlags = 0x1; p.Room = 1; p.RoomHF = 2; p.RoomLF = 3;
    p.SetRoomHF(-1234);
    bool ok = p.RoomHF == -1234 && p.DirtyFlags == (0x1u | 0x40u);
    p.SetRoomHF(-100);
    ok = ok && p.RoomHF == -100 && p.DirtyFlags == (0x1u | 0x40u);
    long others = p.Room + p.RoomHF + p.RoomLF - p.RoomHF;
    long expect = 1 + 2 + 3 - 2;
    ok = ok && others == expect;
    if (!ok) { std::printf("00c53420_TEST FAIL\n"); return 1; }
    std::printf("00c53420_TEST PASS\n");
    return 0;
}