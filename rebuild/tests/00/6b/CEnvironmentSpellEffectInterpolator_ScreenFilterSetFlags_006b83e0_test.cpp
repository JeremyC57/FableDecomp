// Behaviour test for CEnvironmentSpellEffectInterpolator::ScreenFilterSetFlags 0x006B83E0: the flags are set on the interpolator's screen-effect priority stack with the same things and handle. The body forwards to the member at +0x90
// (`add ecx,90h ; jmp`), so the call must reach that member object and nothing else.
#include <cstdio>
#include <cstddef>
#include <vector>
struct CThingGraphicArgPasser { int thing; };
struct CScreenEffectPriorityStack { void* LastHandle; unsigned int LastCount;
    void ScreenFilterSetFlags(const std::vector<CThingGraphicArgPasser>& things, void* handle) { LastHandle = handle; LastCount = (unsigned int)things.size(); } };
#pragma pack(push, 4)
struct CEnvironmentSpellEffectInterpolator {                                  // retail is 32-bit: 4-byte packing keeps the +0x90 offset on a 64-bit host
    unsigned char Before[0x90];
    CScreenEffectPriorityStack ScreenEffectPriorityStack;
    void ScreenFilterSetFlags(const std::vector<CThingGraphicArgPasser>& things, void* handle) { ScreenEffectPriorityStack.ScreenFilterSetFlags(things, handle); }
};
#pragma pack(pop)
int main()
{
    if ((unsigned int)offsetof(CEnvironmentSpellEffectInterpolator, ScreenEffectPriorityStack) != 0x90u) { std::printf("006b83e0_TEST FAIL\n"); return 1; }
    CEnvironmentSpellEffectInterpolator i; i.ScreenEffectPriorityStack.LastHandle = 0; i.ScreenEffectPriorityStack.LastCount = 0;
    std::vector<CThingGraphicArgPasser> things(2); int handle = 0;
    i.ScreenFilterSetFlags(things, &handle);
    if (i.ScreenEffectPriorityStack.LastHandle != &handle || i.ScreenEffectPriorityStack.LastCount != 2u) { std::printf("0069e1c0_TEST FAIL\n"); return 1; }
    std::printf("006b83e0_TEST PASS\n");
    return 0;
}