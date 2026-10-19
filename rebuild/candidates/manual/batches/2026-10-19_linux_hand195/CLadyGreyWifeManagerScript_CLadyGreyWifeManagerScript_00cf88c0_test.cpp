#include <cstdio>
#include <cstring>
#include <new>
static int g_base = 0, g_bc = 0, g_str = 0;
struct CGameScriptInterface { int x; };
struct CScriptBase { virtual ~CScriptBase() {} unsigned char _body[0x3c]; CScriptBase() { ++g_base; } };
struct CQuestScriptObject : CScriptBase {
    CGameScriptInterface* Game; void* Data;
    CQuestScriptObject(void* data, CGameScriptInterface* game) : CScriptBase(), Game(game), Data(data) { }
};
struct CCharString { void* PStringData; CCharString() : PStringData(0) { ++g_str; } };
struct CBaseClass { virtual ~CBaseClass() {} CBaseClass() { ++g_bc; } };
template<class T> struct CCountedPointer { T* Data; void* Info; CCountedPointer() : Data(0), Info(0) { } };
struct CScriptThing : CBaseClass { CCountedPointer<CScriptThing> PImp; CScriptThing() : CBaseClass(), PImp() { } };
struct CLadyGreyWifeManagerScript : CQuestScriptObject {
    CScriptThing LadyGrey, BowerstoneNorthVillage, MayorsBedroomVillage, MayorsBedroom, AppearPoint;
    long CurrentTime; bool NewRegionEntered;
    CCharString InteriorRegion, ExteriorRegion;
    CLadyGreyWifeManagerScript(void* data, CGameScriptInterface* game);
};
CLadyGreyWifeManagerScript::CLadyGreyWifeManagerScript(void* data, CGameScriptInterface* game)
    : CQuestScriptObject(data, game)
{
}
int main()
{
    static unsigned char buf[sizeof(CLadyGreyWifeManagerScript)];
    std::memset(buf, 0xcd, sizeof(buf));
    CGameScriptInterface gsi; int data = 5;
    CLadyGreyWifeManagerScript* s = new (buf) CLadyGreyWifeManagerScript(&data, &gsi);
    if (g_base != 1 || g_bc != 5 || g_str != 2 || s->Data != &data || s->Game != &gsi) { std::printf("00cf88c0_TEST FAIL\n"); return 1; }
    if (s->LadyGrey.PImp.Data != 0 || s->AppearPoint.PImp.Info != 0 || s->MayorsBedroom.PImp.Data != 0 || s->ExteriorRegion.PStringData != 0) { std::printf("00cf88c0_TEST FAIL\n"); return 1; }
    std::printf("00cf88c0_TEST PASS\n");
    return 0;
}
