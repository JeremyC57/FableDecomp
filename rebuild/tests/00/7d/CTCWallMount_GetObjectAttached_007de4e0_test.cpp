// Behaviour test for CTCWallMount::GetObjectAttached 0x007DE4E0: the attached object is the trophy pointer's item. The body forwards to the member at +0x10
// (`add ecx,10h ; jmp`), so the call must reach that member object and nothing else.
#include <cstdio>
#include <cstddef>
struct CThing { int id; };
struct CIntelligentPointer { CThing* Item; CThing* GetPItem() const { return Item; } void SetPItem(CThing* t) { Item = t; } };
#pragma pack(push, 4)
struct CTCWallMount {                                  // retail is 32-bit: 4-byte packing keeps the +0x10 offset on a 64-bit host
    unsigned char Before[0x10];
    CIntelligentPointer Trophy;
    CThing* GetObjectAttached() { return Trophy.GetPItem(); }
};
#pragma pack(pop)
int main()
{
    if ((unsigned int)offsetof(CTCWallMount, Trophy) != 0x10u) { std::printf("007de4e0_TEST FAIL\n"); return 1; }
    CThing t = { 5 };
    CTCWallMount o; o.Trophy.Item = &t;
    if (o.GetObjectAttached() != &t) { std::printf("007de4e0_TEST FAIL\n"); return 1; }
    o.Trophy.Item = 0;
    if (o.GetObjectAttached() != 0) { std::printf("007de4e0_TEST FAIL\n"); return 1; }
    std::printf("007de4e0_TEST PASS\n");
    return 0;
}