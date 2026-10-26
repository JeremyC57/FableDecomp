// Behaviour test for 0x009b98a0 CParsedItem::GetFloat() const: the float value (the +4 union).
#include <stdio.h>
class CCharString { public: void* PStringData; };
class CParsedItem {
public:
    int Type;
    union { long Integer; float Float; };
    CCharString String;
    float GetFloat() const { return Float; }
};
int main()
{
    CParsedItem it; it.Type = 1; it.String.PStringData = 0;
    int ok;
    it.Float = 2.5f; ok = it.GetFloat() == 2.5f;
    ok = ok && sizeof(CParsedItem) == 12;
    printf(ok ? "009b98a0_TEST PASS\n" : "009b98a0_TEST FAIL\n");
    return ok ? 0 : 1;
}
