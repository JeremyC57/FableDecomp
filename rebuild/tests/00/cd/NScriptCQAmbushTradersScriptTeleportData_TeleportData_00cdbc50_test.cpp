// Behaviour test for NScript::CQ_AmbushTradersScript::TeleportData::~TeleportData (0x00CDBC50): destroy every carried-item
// string, free the vector block (only when allocated), then scriptName, then defName (reverse declaration order).
#include <cstdio>
#include <cstdlib>
static int g_order[16]; static int g_n = 0; static int g_frees = 0;
struct CCharString { int id; ~CCharString() { if (g_n < 16) g_order[g_n++] = id; } };
struct CStringVector {
    CCharString* _M_start; CCharString* _M_finish; CCharString* _M_end_of_storage;
    ~CStringVector()
    {
        for (CCharString* p = _M_start; p != _M_finish; ++p) p->~CCharString();
        if (_M_start) { std::free(_M_start); ++g_frees; }
    }
};
struct TeleportData {
    CCharString defName; unsigned long appearanceSeed; float health; CCharString scriptName; CStringVector itemsCarried;
};
int main()
{
    {
        TeleportData d;
        d.defName.id = 1; d.scriptName.id = 2;
        d.itemsCarried._M_start = (CCharString*)std::malloc(3 * sizeof(CCharString));
        for (int i = 0; i < 3; ++i) d.itemsCarried._M_start[i].id = 10 + i;
        d.itemsCarried._M_finish = d.itemsCarried._M_end_of_storage = d.itemsCarried._M_start + 3;
    }
    bool ok = g_n == 5 && g_order[0] == 10 && g_order[1] == 11 && g_order[2] == 12 && g_order[3] == 2 && g_order[4] == 1 && g_frees == 1;
    g_n = 0;
    {
        TeleportData e;
        e.defName.id = 7; e.scriptName.id = 8;
        e.itemsCarried._M_start = e.itemsCarried._M_finish = e.itemsCarried._M_end_of_storage = 0;
    }
    ok = ok && g_n == 2 && g_order[0] == 8 && g_order[1] == 7 && g_frees == 1;
    if (!ok) { std::printf("00cdbc50_TEST FAIL\n"); return 1; }
    std::printf("00cdbc50_TEST PASS\n");
    return 0;
}