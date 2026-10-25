#include <cstdio>
#include <cstring>
// Behaviour test for CConsoleVariable::GetHelp @ 0x009EA3C0 (text assembly): slot 3 is GetTypeAsString ("variable").
static void GetHelp(char* out, const char* typeName, long type, const char* value)
{
    std::strcpy(out, typeName); std::strcat(out, " of type ");
    if (type == 4) std::strcat(out, "bool");
    else if (type == 3) std::strcat(out, "float");
    else if (type == 1) std::strcat(out, "slong");
    else if (type == 5) std::strcat(out, "string");
    std::strcat(out, "; value is currently "); std::strcat(out, value);
}
int main()
{
    char buf[128];
    GetHelp(buf, "variable", 4, "true");
    if (std::strcmp(buf, "variable of type bool; value is currently true") != 0) { std::printf("009ea3c0_TEST FAIL\n"); return 1; }
    GetHelp(buf, "variable", 2, "7");
    if (std::strcmp(buf, "variable of type ; value is currently 7") != 0) { std::printf("009ea3c0_TEST FAIL\n"); return 1; }
    std::printf("009ea3c0_TEST PASS\n");
    return 0;
}