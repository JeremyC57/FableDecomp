#include <cstdio>
#include <string.h>
class CThing { public: int id; };
static CThing* g_mainPlayer = 0;
class CPlayerManager { public: bool IsThingMainPlayer(const CThing* thing) const { return thing == g_mainPlayer; } };
static CPlayerManager g_pm;
static bool g_bDebugHelpers = false;
inline void* CopyTrivial(const void* first, const void* last, void* result)
{
    if (first == last)
        return result;
    size_t n = (const char*)last - (const char*)first;
    return (char*)memmove(result, first, n) + n;
}
struct CThingVector {
    CThing** _M_start; CThing** _M_finish; CThing** _M_end_of_storage;
    CThing** begin() { return _M_start; }
    CThing** end() { return _M_finish; }
    CThing** erase(CThing** first, CThing** last) { CThing** i = (CThing**)CopyTrivial(last, _M_finish, first); _M_finish = i; return first; }
};
struct CTCPhysicsControlled {
    CThing* owner;
    CPlayerManager* GetPlayerManager() { return &g_pm; }
    void AdjustThingListForDebugHelpers(CThingVector& things)
    {
        if (GetPlayerManager()->IsThingMainPlayer(owner) && !g_bDebugHelpers)
            things.erase(things.begin(), things.end());
    }
};
int main()
{
    CThing a, b, hero; CThing* store[4] = { &a, &b, 0, 0 };
    CThingVector v; v._M_start = store; v._M_finish = store + 2; v._M_end_of_storage = store + 4;
    CTCPhysicsControlled c; c.owner = &a; g_mainPlayer = &hero;
    c.AdjustThingListForDebugHelpers(v);
    if (v._M_finish != store + 2) { std::printf("007254b0_TEST FAIL\n"); return 1; }
    c.owner = &hero; g_bDebugHelpers = true; c.AdjustThingListForDebugHelpers(v);
    if (v._M_finish != store + 2) { std::printf("007254b0_TEST FAIL\n"); return 1; }
    g_bDebugHelpers = false; c.AdjustThingListForDebugHelpers(v);
    if (v._M_finish != store || v._M_start != store || store[0] != &a) { std::printf("007254b0_TEST FAIL\n"); return 1; }
    std::printf("007254b0_TEST PASS\n");
    return 0;
}