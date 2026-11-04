// Behaviour test for CTCPerceiveThing::GetPerceivedThing 0x007446A0: the perceived thing is the intelligent pointer's item. The body forwards to the member at +0xC
// (`add ecx,Ch ; jmp`), so the call must reach that member object and nothing else.
#include <cstdio>
#include <cstddef>
struct CThing { int id; };
struct CIntelligentPointer { CThing* Item; CThing* GetPItem() const { return Item; } void SetPItem(CThing* t) { Item = t; } };
#pragma pack(push, 4)
struct CTCPerceiveThing {                                  // retail is 32-bit: 4-byte packing keeps the +0xC offset on a 64-bit host
    unsigned char Before[0xC];
    CIntelligentPointer PerceivedThing;
    CThing* GetPerceivedThing() const { return PerceivedThing.GetPItem(); }
};
#pragma pack(pop)
int main()
{
    if ((unsigned int)offsetof(CTCPerceiveThing, PerceivedThing) != 0xCu) { std::printf("007446a0_TEST FAIL\n"); return 1; }
    CThing t = { 5 };
    CTCPerceiveThing o; o.PerceivedThing.Item = &t;
    if (o.GetPerceivedThing() != &t) { std::printf("007446a0_TEST FAIL\n"); return 1; }
    o.PerceivedThing.Item = 0;
    if (o.GetPerceivedThing() != 0) { std::printf("007446a0_TEST FAIL\n"); return 1; }
    std::printf("007446a0_TEST PASS\n");
    return 0;
}