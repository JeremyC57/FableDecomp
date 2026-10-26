// Behaviour test for 0x009b9890 CParsedItem::GetInteger() const: the integer value (the +4 union).
#include <stdio.h>
class CCharString { public: void* PStringData; };
class CParsedItem {
public:
    int Type;
    union { long Integer; float Float; };
    CCharString String;
    long GetInteger() const { return Integer; }
};
int main()
{
    CParsedItem it; it.Type = 1; it.String.PStringData = 0;
    int ok;
    it.Integer = -123456; ok = it.GetInteger() == -123456;
    ok = ok && sizeof(CParsedItem) == 12;
    printf(ok ? "009b9890_TEST PASS\n" : "009b9890_TEST FAIL\n");
    return ok ? 0 : 1;
}
