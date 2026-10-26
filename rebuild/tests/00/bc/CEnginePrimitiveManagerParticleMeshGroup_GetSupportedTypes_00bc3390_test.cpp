// behaviour test for 0x00BC3390 CEnginePrimitiveManagerParticleMeshGroup::GetSupportedTypes(CEnginePrimitiveTypeList&) const: the manager appends its two supported primitive types (0x1C then 0x17) to the caller's list, after whatever it already holds, growing the list when full
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
        types.Add((EEnginePrimitiveType)0x1C);
        types.Add((EEnginePrimitiveType)0x17);
    }
};
int main()
{
    int fail = 0;
    Manager m;
    CEnginePrimitiveTypeList a;
    m.GetSupportedTypes(a);
    if (a.Types.size() != 2u || a.Types[0] != 0x1C || a.Types[1] != 0x17) fail |= 1;
    CEnginePrimitiveTypeList b;
    b.Types.push_back(99);
    m.GetSupportedTypes(b);
    m.GetSupportedTypes(b);
    if (b.Types.size() != 5u || b.Types[0] != 99 || b.Types[1] != 0x1C || b.Types[2] != 0x17 || b.Types[3] != 0x1C || b.Types[4] != 0x17) fail |= 2;
    if (fail) { std::printf("00bc3390_TEST FAIL %d\n", fail); return 1; }
    std::printf("00bc3390_TEST PASS\n");
    return 0;
}