#include <cstdio>
#include <cstring>
struct CThingInfo { const char* TypeName; const char* SubTypeName; };
static const char* g_names[3] = { "?", "OBJECT_A", "OBJECT_B" };
struct CThing {
    unsigned short DefGlobalIndex;
    void GetInfo(CThingInfo* info) const {
        if (DefGlobalIndex == 0) { info->TypeName = "None"; info->SubTypeName = "None"; return; }
        const char* name = g_names[DefGlobalIndex];
        info->SubTypeName = name;
    }
};
int main()
{
    CThingInfo i = { "T", "S" };
    CThing a = { 0 }, b = { 2 };
    a.GetInfo(&i);
    if (std::strcmp(i.TypeName, "None") || std::strcmp(i.SubTypeName, "None")) { std::printf("004c7b00_TEST FAIL\n"); return 1; }
    i.TypeName = "T"; b.GetInfo(&i);
    if (std::strcmp(i.TypeName, "T") || std::strcmp(i.SubTypeName, "OBJECT_B")) { std::printf("004c7b00_TEST FAIL\n"); return 1; }
    std::printf("004c7b00_TEST PASS\n");
    return 0;
}
