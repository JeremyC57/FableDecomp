// Behaviour test for 0x009b9880 CParsedItem::GetString() const: the string text (the String member at +8).
#include <stdio.h>
class CCharString { public: void* PStringData; };
class CParsedItem {
public:
    int Type;
    union { long Integer; float Float; };
    CCharString String;
    const CCharString& GetString() const { return String; }
};
int main()
{
    CParsedItem it; it.Type = 1; it.String.PStringData = 0;
    int ok;
    const CCharString* r = &it.GetString(); ok = r == &it.String;
    ok = ok && sizeof(CParsedItem) == 12;
    printf(ok ? "009b9880_TEST PASS\n" : "009b9880_TEST FAIL\n");
    return ok ? 0 : 1;
}
