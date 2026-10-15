#include <cstdio>
#include <string.h>
#define FAIL { std::printf("0099e520_TEST FAIL\n"); return 1; }
struct CCharStringData { char* PData; };
struct CCharString {
    CCharStringData* PStringData;
    bool operator==(char character) const
    {
        if (PStringData != 0) { char text[2]; text[0] = character; text[1] = 0; return strcmp(PStringData->PData, text) == 0 ? true : false; }
        return false;
    }
    bool operator!=(char character) const { return !(*this == character); }
};
int main()
{
    char a[] = "A", ab[] = "AB", empty[] = "";
    CCharStringData d; CCharString s; s.PStringData = &d;
    d.PData = a; if (s != 'A' || !(s != 'B')) FAIL;
    d.PData = ab; if (!(s != 'A')) FAIL;
    d.PData = empty; if (!(s != 'A') || s != 0) FAIL;
    s.PStringData = 0; if (!(s != 'A') || !(s != 0)) FAIL;
    std::printf("0099e520_TEST PASS\n");
    return 0;
}
