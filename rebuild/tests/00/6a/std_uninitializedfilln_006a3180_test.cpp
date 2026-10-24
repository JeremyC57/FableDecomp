// Behaviour test for 006a3180: STLport __uninitialized_fill_n over the 9-byte CThingGraphicArgPasser records (model copy).
#include <cstdio>
#include <new>
class CThing { public: int id; };
static int g_copies = 0;
#pragma pack(push, 1)
struct CIntelligentPointer {
    CThing* item;
    CIntelligentPointer(const CIntelligentPointer& o) : item(o.item) { ++g_copies; }
    CIntelligentPointer(CThing* t) : item(t) { }
};
struct CThingGraphicArgPasser {
    CIntelligentPointer thing;
    bool bypassSet;
    CThingGraphicArgPasser(CThing* t, bool b) : thing(t), bypassSet(b) { }
};
#pragma pack(pop)
struct __false_type { };
CThingGraphicArgPasser* uninitialized_fill_n(CThingGraphicArgPasser* first, unsigned int n, const CThingGraphicArgPasser& x, const __false_type&)
{
    CThingGraphicArgPasser* cur = first;
    for (; n > 0; --n, ++cur)
        new ((void*)&*cur) CThingGraphicArgPasser(x);
    return cur;
}
int main()
{
    CThing t; t.id = 3;
    CThingGraphicArgPasser x(&t, true);
    unsigned char raw[6 * sizeof(CThingGraphicArgPasser)];
    CThingGraphicArgPasser* buf = (CThingGraphicArgPasser*)(void*)raw;
    int fail = 0;
    if (sizeof(CThingGraphicArgPasser) != sizeof(CThing*) + 1) fail = 1;
    CThingGraphicArgPasser* end = uninitialized_fill_n(buf, 5, x, __false_type());
    if (end != buf + 5 || g_copies != 5) fail = 1;
    for (int i = 0; i < 5; ++i) if (buf[i].thing.item != &t || !buf[i].bypassSet) fail = 1;
    if (fail) { std::printf("006a3180_TEST FAIL\n"); return 1; }
    std::printf("006a3180_TEST PASS\n");
    return 0;
}