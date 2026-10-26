// Behaviour test for 0x009b9870 CParsedItem::GetSymbol() const: the symbol text (the String member at +8).
#include <stdio.h>
class CCharString { public: void* PStringData; };
class CParsedItem {
public:
    int Type;
    union { long Integer; float Float; };
    CCharString String;
    const CCharString& GetSymbol() const { return String; }
};
int main()
{
    CParsedItem it; it.Type = 1; it.String.PStringData = 0;
    int ok;
    const CCharString* r = &it.GetSymbol(); ok = r == &it.String;
    ok = ok && sizeof(CParsedItem) == 12;
    printf(ok ? "009b9870_TEST PASS\n" : "009b9870_TEST FAIL\n");
    return ok ? 0 : 1;
}
