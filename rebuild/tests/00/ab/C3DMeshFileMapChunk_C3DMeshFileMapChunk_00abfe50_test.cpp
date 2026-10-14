#include <cstdio>

struct Map { const char* type; long MapIndex; const char* Name; const char* Filename; Map() : type("MMAP"), MapIndex(-1), Name(""), Filename("") {} };
int main()
{
    Map m; if (m.type[0] != 'M' || m.MapIndex != -1 || m.Name[0] || m.Filename[0]) { std::printf("00abfe50_TEST FAIL\n"); return 1; }
    std::printf("00abfe50_TEST PASS\n");
    return 0;
}