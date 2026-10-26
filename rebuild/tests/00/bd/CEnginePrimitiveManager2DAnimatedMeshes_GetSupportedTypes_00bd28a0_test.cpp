// behaviour test for 0x00BD28A0 CEnginePrimitiveManager2DAnimatedMeshes::GetSupportedTypes(CEnginePrimitiveTypeList&) const: the manager appends its two supported primitive types (0x25 then 0x26) to the caller's list, after whatever it already holds, growing the list when full
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
        types.Add((EEnginePrimitiveType)0x25);
        types.Add((EEnginePrimitiveType)0x26);
    }
};
int main()
{
    int fail = 0;
    Manager m;
    CEnginePrimitiveTypeList a;
    m.GetSupportedTypes(a);
    if (a.Types.size() != 2u || a.Types[0] != 0x25 || a.Types[1] != 0x26) fail |= 1;
    CEnginePrimitiveTypeList b;
    b.Types.push_back(99);
    m.GetSupportedTypes(b);
    m.GetSupportedTypes(b);
    if (b.Types.size() != 5u || b.Types[0] != 99 || b.Types[1] != 0x25 || b.Types[2] != 0x26 || b.Types[3] != 0x25 || b.Types[4] != 0x26) fail |= 2;
    if (fail) { std::printf("00bd28a0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00bd28a0_TEST PASS\n");
    return 0;
}