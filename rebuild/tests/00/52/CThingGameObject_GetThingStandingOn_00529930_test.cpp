// Behaviour test for CThingGameObject::GetThingStandingOn 0x00529930: the thing a game object stands on is its intelligent pointer's item. The body forwards to the member at +0xC0
// (`add ecx,C0h ; jmp`), so the call must reach that member object and nothing else.
#include <cstdio>
#include <cstddef>
struct CThing { int id; };
struct CIntelligentPointer { CThing* Item; CThing* GetPItem() const { return Item; } void SetPItem(CThing* t) { Item = t; } };
#pragma pack(push, 4)
struct CThingGameObject {                                  // retail is 32-bit: 4-byte packing keeps the +0xC0 offset on a 64-bit host
    unsigned char Before[0xC0];
    CIntelligentPointer ThingStandingOn;
    CThing* GetThingStandingOn() const { return ThingStandingOn.GetPItem(); }
};
#pragma pack(pop)
int main()
{
    if ((unsigned int)offsetof(CThingGameObject, ThingStandingOn) != 0xC0u) { std::printf("00529930_TEST FAIL\n"); return 1; }
    CThing t = { 5 };
    CThingGameObject o; o.ThingStandingOn.Item = &t;
    if (o.GetThingStandingOn() != &t) { std::printf("00529930_TEST FAIL\n"); return 1; }
    o.ThingStandingOn.Item = 0;
    if (o.GetThingStandingOn() != 0) { std::printf("00529930_TEST FAIL\n"); return 1; }
    std::printf("00529930_TEST PASS\n");
    return 0;
}