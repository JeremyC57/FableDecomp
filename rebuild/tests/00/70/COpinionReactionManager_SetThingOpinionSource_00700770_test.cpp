// Behaviour test for COpinionReactionManager::SetThingOpinionSource 0x00700770: setting the opinion source stores it in the intelligent pointer. The body forwards to the member at +0x714
// (`add ecx,714h ; jmp`), so the call must reach that member object and nothing else.
#include <cstdio>
#include <cstddef>
struct CThing { int id; };
struct CIntelligentPointer { CThing* Item; CThing* GetPItem() const { return Item; } void SetPItem(CThing* t) { Item = t; } };
#pragma pack(push, 4)
struct COpinionReactionManager {                                  // retail is 32-bit: 4-byte packing keeps the +0x714 offset on a 64-bit host
    unsigned char Before[0x714];
    CIntelligentPointer PThingOpinionSource;
    void SetThingOpinionSource(CThing* thing) { PThingOpinionSource.SetPItem(thing); }
};
#pragma pack(pop)
int main()
{
    if ((unsigned int)offsetof(COpinionReactionManager, PThingOpinionSource) != 0x714u) { std::printf("00700770_TEST FAIL\n"); return 1; }
    CThing t = { 9 };
    COpinionReactionManager m; m.PThingOpinionSource.Item = 0;
    m.SetThingOpinionSource(&t);
    if (m.PThingOpinionSource.Item != &t) { std::printf("004c7bc0_TEST FAIL\n"); return 1; }
    m.SetThingOpinionSource(0);
    if (m.PThingOpinionSource.Item != 0) { std::printf("004c7bc0_TEST FAIL\n"); return 1; }
    std::printf("00700770_TEST PASS\n");
    return 0;
}