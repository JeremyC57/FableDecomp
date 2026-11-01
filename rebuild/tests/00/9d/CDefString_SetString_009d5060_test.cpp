// Behaviour test for 0x009D5060 CDefString::SetString(const CCharString&): TablePos becomes the position GDefStringTable.AddString returns for the string
#include <stdio.h>
class CCharString { public: void* PStringData; };
class CDefStringTable { public:
    const CCharString* lastString; int calls;
    long AddString(const CCharString& string);
};
CDefStringTable GDefStringTable;
long CDefStringTable::AddString(const CCharString& string) { lastString = &string; ++calls; return 0x1234; }
class CDefString { public:
    long TablePos;
    void SetString(const CCharString& string);
};
int main()
{
    CCharString s; s.PStringData = 0;
    CDefString d; d.TablePos = -1;
    d.SetString(s);
    if (d.TablePos == 0x1234 && GDefStringTable.lastString == &s && GDefStringTable.calls == 1) { printf("009d5060_TEST PASS\n"); return 0; }
    printf("009d5060_TEST FAIL\n"); return 1;
}
