// Behaviour test for 0x00A3EED0 CSoundManagerLH::vfunc_40 (links the source object): both name references reach
// FindByNamePair unchanged; absent -> false, else slot 1 gets (a, b) and the result is true.
#include <cstdio>
struct CSoundEntry_00a3eed0 { virtual void v00(); virtual void Set(long a, long b); long x, y; };
void CSoundEntry_00a3eed0::v00() { }
void CSoundEntry_00a3eed0::Set(long a, long b) { x = a; y = b; }
class CCharString { public: void* PStringData; };
struct CSoundManagerLH {
    virtual void v00();
    CSoundEntry_00a3eed0* FindByNamePair(const CCharString& first, const CCharString& second);
    virtual bool vfunc_40(const CCharString& first, const CCharString& second, long a, long b);
};
static CSoundEntry_00a3eed0* g_entry = 0;
static const CCharString* g_first = 0; static const CCharString* g_second = 0; static const CCharString* g_hit = 0;
void CSoundManagerLH::v00() { }
CSoundEntry_00a3eed0* CSoundManagerLH::FindByNamePair(const CCharString& first, const CCharString& second)
{ g_first = &first; g_second = &second; return &first == g_hit ? g_entry : 0; }
int main()
{
    CSoundEntry_00a3eed0 e; e.x = e.y = 0;
    CSoundManagerLH m; CCharString n1, n2, n3; n1.PStringData = n2.PStringData = n3.PStringData = 0;
    g_entry = &e; g_hit = &n1;
    bool ok = !m.vfunc_40(n3, n2, 1, 2) && e.x == 0 && g_first == &n3 && g_second == &n2;
    ok = ok && m.vfunc_40(n1, n2, 7, 8) && e.x == 7 && e.y == 8 && g_first == &n1 && g_second == &n2;
    if (!ok) { std::printf("00a3eed0_TEST FAIL\n"); return 1; }
    std::printf("00a3eed0_TEST PASS\n");
    return 0;
}
