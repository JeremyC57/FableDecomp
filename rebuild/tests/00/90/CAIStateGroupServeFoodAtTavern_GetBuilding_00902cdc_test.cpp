#include <cstdio>

static void* g_arg = 0;
struct CThingBuilding { int dummy; };
CThingBuilding* __fastcall IntelligentPointer_GetPItem_00a01b50(void* p) { g_arg = p; return (CThingBuilding*)p; }
struct Group { void* vt; char* brain; CThingBuilding* GetBuilding() { char* v = *(char**)(brain + 0x18); return IntelligentPointer_GetPItem_00a01b50(v + 0x170); } };
int main()
{
    char villager[0x200]; char brain[0x20]; *(char**)(brain + 0x18) = villager;
    Group g; g.brain = brain; g.GetBuilding();
    if (g_arg != villager + 0x170) { std::printf("00902cdc_TEST FAIL\n"); return 1; }
    std::printf("00902cdc_TEST PASS\n");
    return 0;
}