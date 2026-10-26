#include <cstdio>
class CCharString { public: const char* text; };
static long g_loaded = -1;
class CDefPointeeBase { public: virtual void v00() { } long RefCount; };
struct CDefPointer { CDefPointeeBase* Object; CDefPointer() : Object(0) { } CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) Object->RefCount = Object->RefCount + 1; } };
struct CInstantiatedDefInfo { long ClassName, InstantiationName; unsigned long ClassIndex; CDefPointer PDef; };
class CDefinitionManager { public:
    unsigned char _pad[0xa4]; CInstantiatedDefInfo** InstantiatedDefs;
    long GetDefGlobalIndexFromName(const CCharString& n) { return n.text[0] == 'x' ? 2 : -1; }
    void LoadDef(unsigned long i) { g_loaded = (long)i; }
    CDefPointer GetPDefFromInstantiationName(const CCharString& n) { long index = GetDefGlobalIndexFromName(n); if (index > 0) { LoadDef(index); return InstantiatedDefs[index]->PDef; } return CDefPointer(); }
};
int main()
{
    CDefPointeeBase obj; obj.RefCount = 1; CInstantiatedDefInfo info[3]; info[2].PDef.Object = &obj; CInstantiatedDefInfo* defs[3] = { &info[0], &info[1], &info[2] };
    CDefinitionManager m; m.InstantiatedDefs = defs;
    CCharString good = { "x" }, bad = { "y" };
    CDefPointer p = m.GetPDefFromInstantiationName(good);
    if (p.Object != &obj || obj.RefCount != 2 || g_loaded != 2 || (char*)&m.InstantiatedDefs - (char*)&m != 0xa4) { std::printf("009ada90_TEST FAIL\n"); return 1; }
    CDefPointer q = m.GetPDefFromInstantiationName(bad);
    if (q.Object != 0 || g_loaded != 2) { std::printf("009ada90_TEST FAIL\n"); return 1; }
    std::printf("009ada90_TEST PASS\n"); return 0;
}