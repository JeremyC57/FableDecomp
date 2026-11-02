// test: the atexit destructor of GetSectionDesc's static 'desc' -- the miss path returns one shared static description (Loaded false, Size 0); at exit its strings die LongName first, then ShortName
#include <stdio.h>
#include <stdlib.h>
#include <map>
static int order[2]; static int nDtor = 0;
class CCharString { public: int tag; CCharString() : tag(0) { } ~CCharString() { if (tag && nDtor < 2) order[nDtor++] = tag; } };
struct CSectionDesc { CCharString ShortName; CCharString LongName; bool Loaded; long Size; };
class CCodeSectionManager { public:
    std::map<unsigned long, CSectionDesc> SectionDesc;
    CSectionDesc& GetSectionDesc(unsigned long handle) {
        std::map<unsigned long, CSectionDesc>::iterator it = SectionDesc.find(handle);
        if (it == SectionDesc.end()) { static CSectionDesc desc; desc.Loaded = false; desc.Size = 0; return desc; }
        return it->second;
    }
};
static void check() {
    if (nDtor == 2 && order[0] == 2 && order[1] == 1) printf("0122a940_TEST PASS\n");
    else printf("0122a940_TEST FAIL\n");
}
int main() {
    atexit(check);                                       // registered first, so it runs after the static's destructor
    CCodeSectionManager m;
    m.SectionDesc[5].Size = 77; m.SectionDesc[5].Loaded = true;
    CSectionDesc& a = m.GetSectionDesc(9);
    a.Loaded = true; a.Size = 3;
    CSectionDesc& b = m.GetSectionDesc(10);
    if (&a != &b || b.Loaded || b.Size != 0 || m.GetSectionDesc(5).Size != 77) { printf("0122a940_TEST FAIL\n"); return 1; }
    b.ShortName.tag = 1; b.LongName.tag = 2;
    return 0;
}