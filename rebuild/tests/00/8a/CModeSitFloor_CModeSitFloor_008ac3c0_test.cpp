#include <cstdio>
#include <stddef.h>
class CThing { public: int id; };
class CCharString { public: void* PStringData; };
static const CCharString* g_filterName = 0; static CThing* g_baseThing = 0;
class CAnimFilterOfGroup { public: virtual void v00() { } CCharString Group; unsigned long GroupCRC; CAnimFilterOfGroup(const CCharString& name) : Group(name), GroupCRC(77) { g_filterName = &name; } };
struct CModeSitBase { virtual void v00() { } unsigned char _body[0x28]; CModeSitBase(CThing* thing) { g_baseThing = thing; } };
struct CModeSitFloor : CModeSitBase { CAnimFilterOfGroup Filter; virtual void v00() { } CModeSitFloor(CThing* thing, const CCharString& group) : CModeSitBase(thing), Filter(group) { } };
int main()
{
    CThing thing = { 1 }; CCharString group = { (void*)0x1234 };
    CModeSitFloor mode(&thing, group);
    if (g_baseThing != &thing || g_filterName != &group || mode.Filter.Group.PStringData != (void*)0x1234 || mode.Filter.GroupCRC != 77
        || offsetof(CModeSitFloor, Filter) != 0x2c || sizeof(mode) != 0x38) { std::printf("008ac3c0_TEST FAIL\n"); return 1; }
    std::printf("008ac3c0_TEST PASS\n"); return 0;
}