// behaviour test for 0x00B48220 CEngineLightingManager::GetSupportedTypes(CEnginePrimitiveTypeList&) const: the manager appends its two supported primitive types (0xF then 0x10) to the caller's list, after whatever it already holds, growing the list when full
#include <cstdio>
#include <vector>
enum EEnginePrimitiveType { };
class CEnginePrimitiveTypeList { public:
    std::vector<long> Types;
    void Add(EEnginePrimitiveType type) { Types.push_back(type); }
};
struct Manager {
    void GetSupportedTypes(CEnginePrimitiveTypeList& types) const     // the function under test
    {
        types.Add((EEnginePrimitiveType)0xF);
        types.Add((EEnginePrimitiveType)0x10);
    }
};
int main()
{
    int fail = 0;
    Manager m;
    CEnginePrimitiveTypeList a;
    m.GetSupportedTypes(a);
    if (a.Types.size() != 2u || a.Types[0] != 0xF || a.Types[1] != 0x10) fail |= 1;
    CEnginePrimitiveTypeList b;
    b.Types.push_back(99);
    m.GetSupportedTypes(b);
    m.GetSupportedTypes(b);
    if (b.Types.size() != 5u || b.Types[0] != 99 || b.Types[1] != 0xF || b.Types[2] != 0x10 || b.Types[3] != 0xF || b.Types[4] != 0x10) fail |= 2;
    if (fail) { std::printf("00b48220_TEST FAIL %d\n", fail); return 1; }
    std::printf("00b48220_TEST PASS\n");
    return 0;
}