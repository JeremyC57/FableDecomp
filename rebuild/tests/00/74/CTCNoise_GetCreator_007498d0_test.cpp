// Behaviour test for CTCNoise::GetCreator 0x007498D0: a noise's creator is its intelligent pointer's item. The body forwards to the member at +0x48
// (`add ecx,48h ; jmp`), so the call must reach that member object and nothing else.
#include <cstdio>
#include <cstddef>
struct CThing { int id; };
struct CIntelligentPointer { CThing* Item; CThing* GetPItem() const { return Item; } void SetPItem(CThing* t) { Item = t; } };
#pragma pack(push, 4)
struct CTCNoise {                                  // retail is 32-bit: 4-byte packing keeps the +0x48 offset on a 64-bit host
    unsigned char Before[0x48];
    CIntelligentPointer Creator;
    CThing* GetCreator() const { return Creator.GetPItem(); }
};
#pragma pack(pop)
int main()
{
    if ((unsigned int)offsetof(CTCNoise, Creator) != 0x48u) { std::printf("007498d0_TEST FAIL\n"); return 1; }
    CThing t = { 5 };
    CTCNoise o; o.Creator.Item = &t;
    if (o.GetCreator() != &t) { std::printf("007498d0_TEST FAIL\n"); return 1; }
    o.Creator.Item = 0;
    if (o.GetCreator() != 0) { std::printf("007498d0_TEST FAIL\n"); return 1; }
    std::printf("007498d0_TEST PASS\n");
    return 0;
}