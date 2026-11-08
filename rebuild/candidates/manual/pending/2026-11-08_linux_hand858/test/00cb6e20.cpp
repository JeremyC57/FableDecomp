// Behaviour test for 0x00CB6E20 vector<pair<CCharString, CScriptInfo> >::~vector: each element's Name then first destroyed, block freed.
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_order[16]; static int g_n = 0; static int g_freed = 0;
struct CCharString { int id; ~CCharString() { g_order[g_n++] = id; } };
struct CScriptInfo { void* AllocFunc; void* MasterScriptDataAlloc; CCharString Name; long ID; bool MasterScript; };
struct Pair { CCharString first; CScriptInfo second; };
struct Vec {
    Pair* _M_start; Pair* _M_finish; Pair* _M_end_of_storage;
    ~Vec()
    {
        for (Pair* p = _M_start; p != _M_finish; ++p) p->~Pair();
        if (_M_start != 0) { std::free(_M_start); ++g_freed; }
    }
};
int main()
{
    if (sizeof(Pair) != 24 && sizeof(void*) == 4) { std::printf("00cb6e20_TEST FAIL\n"); return 1; }
    {
        Vec v; v._M_start = (Pair*)std::malloc(3 * sizeof(Pair)); v._M_finish = v._M_start + 2; v._M_end_of_storage = v._M_start + 3;
        for (int i = 0; i < 2; ++i) { v._M_start[i].first.id = 10 * i + 1; v._M_start[i].second.Name.id = 10 * i + 2; }
    }
    { Vec e; e._M_start = e._M_finish = e._M_end_of_storage = 0; }
    if (g_n != 4 || g_order[0] != 2 || g_order[1] != 1 || g_order[2] != 12 || g_order[3] != 11 || g_freed != 1)
    { std::printf("00cb6e20_TEST FAIL\n"); return 1; }
    std::printf("00cb6e20_TEST PASS\n");
    return 0;
}
