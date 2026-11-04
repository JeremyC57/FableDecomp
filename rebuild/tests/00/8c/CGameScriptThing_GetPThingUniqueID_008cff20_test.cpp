// Behaviour test for CGameScriptThing::GetPThingUniqueID 0x008CFF20: the virtual returns the handle's stored 64-bit ThingUID, both
// halves intact (retail returns it in edx:eax).
#include <cstdio>
struct CScriptThing { virtual ~CScriptThing() {} virtual unsigned long long GetPThingUniqueID() const { return 0; } };
struct CGameScriptThing : CScriptThing {
    unsigned long long ThingUID;
    unsigned long long GetPThingUniqueID() const { return ThingUID; }
};
int main()
{
    CGameScriptThing t; t.ThingUID = 0x0123456789ABCDEFull;
    const CScriptThing& s = t;
    if (s.GetPThingUniqueID() != 0x0123456789ABCDEFull) { std::printf("008cff20_TEST FAIL\n"); return 1; }
    t.ThingUID = 0xFFFFFFFF00000001ull;
    if (s.GetPThingUniqueID() != 0xFFFFFFFF00000001ull) { std::printf("008cff20_TEST FAIL\n"); return 1; }
    std::printf("008cff20_TEST PASS\n");
    return 0;
}