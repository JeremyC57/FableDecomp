#include <cstdio>
#include <stddef.h>
static int g_nameCopies = 0; static long g_paramType = -1;
struct CCharString { void* PStringData; CCharString(void* p) : PStringData(p) { } CCharString(const CCharString& other) : PStringData(other.PStringData) { ++g_nameCopies; } };
struct CConsoleInputBase { virtual void v00() { } CCharString Name; CConsoleInputBase(const CCharString& name) : Name(name) { } };
struct CConsoleCommandParameters_Retail { long* first; long* last; long* end_of_storage; CConsoleCommandParameters_Retail(long type) : first(0), last(0), end_of_storage(0) { g_paramType = type; } };
struct CConsoleCommandBase_Retail : CConsoleInputBase { CConsoleCommandParameters_Retail ParamTypes; virtual void v00() { }
    CConsoleCommandBase_Retail(const CCharString& name, const long& type) : CConsoleInputBase(name), ParamTypes(type) { } };
int main()
{
    CCharString name((void*)0x55); long type = 3;
    CConsoleCommandBase_Retail cmd(name, type);
    if (g_nameCopies != 1 || cmd.Name.PStringData != (void*)0x55 || g_paramType != 3 || offsetof(CConsoleCommandBase_Retail, ParamTypes) != 8 || sizeof(cmd) != 0x14) { std::printf("00414f30_TEST FAIL\n"); return 1; }
    std::printf("00414f30_TEST PASS\n"); return 0;
}