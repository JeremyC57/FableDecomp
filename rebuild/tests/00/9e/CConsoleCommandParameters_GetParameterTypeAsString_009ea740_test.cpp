// Behaviour test for 009ea740 CConsoleCommandParameters::GetParameterTypeAsString: each type code maps
// to its console name and anything else to "null".
#include <cstdio>
#include <string.h>

struct CName { char text[16]; CName() { text[0] = 0; } CName& operator=(const char* t) { strcpy(text, t); return *this; } };
struct CParams {
    long* m_pTypes;
    CName GetParameterTypeAsString(long index) const
    {
        CName result;
        switch (m_pTypes[index]) {
        case 4: result = "bool"; break;
        case 1: result = "slong"; break;
        case 2: result = "ulong"; break;
        case 3: result = "float"; break;
        case 5: result = "string"; break;
        default: result = "null"; break;
        }
        return result;
    }
};

int main()
{
    long types[7] = { 1, 2, 3, 4, 5, 0, 9 };
    const char* want[7] = { "slong", "ulong", "float", "bool", "string", "null", "null" };
    CParams p; p.m_pTypes = types;
    bool ok = true;
    for (long i = 0; i < 7; ++i) if (strcmp(p.GetParameterTypeAsString(i).text, want[i]) != 0) ok = false;
    if (!ok) { std::printf("009ea740_TEST FAIL\n"); return 1; }
    std::printf("009ea740_TEST PASS\n");
    return 0;
}