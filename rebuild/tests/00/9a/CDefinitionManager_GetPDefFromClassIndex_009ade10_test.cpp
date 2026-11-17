#include <cstdio>
static long g_loaded = -1; static unsigned long g_key = 0;
class CDefPointeeBase { public: virtual void v00() { } long RefCount; void AddRef() { ++RefCount; } };
struct CDefPointer { CDefPointeeBase* Object; CDefPointer() : Object(0) { } CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) Object->AddRef(); } };
struct CInstantiatedDefInfo { long ClassName, InstantiationName; unsigned long ClassIndex; CDefPointer PDef; };
struct CDefClassInfo { unsigned char _pad[8]; unsigned long* GlobalIndices; };
static unsigned long g_indices[2] = { 7, 1 }; static CDefClassInfo g_classInfo = { { 0 }, g_indices };
class CDefinitionManager { public:
    unsigned char _pad[0xa4]; CInstantiatedDefInfo** InstantiatedDefs;
    CDefClassInfo* FindClassInfo(const unsigned long& k) { g_key = k; return &g_classInfo; }
    void LoadDef(unsigned long i) { g_loaded = (long)i; }
    CDefPointer GetPDefFromGlobalIndex(const unsigned long& classKey, long index) { unsigned long g = FindClassInfo(classKey)->GlobalIndices[index]; LoadDef(g); return InstantiatedDefs[g]->PDef; }
};
int main()
{
    CDefPointeeBase obj; obj.RefCount = 1; CInstantiatedDefInfo info[8]; info[1].PDef.Object = &obj; CInstantiatedDefInfo* defs[8] = { &info[0], &info[1], &info[2], &info[3], &info[4], &info[5], &info[6], &info[7] };
    CDefinitionManager m; m.InstantiatedDefs = defs; unsigned long key = 0x1234;
    CDefPointer p = m.GetPDefFromGlobalIndex(key, 1);
    if (p.Object != &obj || obj.RefCount != 2 || g_loaded != 1 || g_key != 0x1234) { std::printf("009ade10_TEST FAIL\n"); return 1; }
    CDefPointer q = m.GetPDefFromGlobalIndex(key, 0);   // global index 7 -> an info without a def: the null copy does not touch a count
    if (g_loaded != 7) { std::printf("009ade10_TEST FAIL\n"); return 1; }
    std::printf("009ade10_TEST PASS\n"); return 0;
}