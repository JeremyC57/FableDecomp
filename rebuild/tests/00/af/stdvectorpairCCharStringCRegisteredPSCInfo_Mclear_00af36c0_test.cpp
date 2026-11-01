// Behaviour test for 0x00AF36C0 std::vector<std::pair<CCharString, NParticleEngine::CRegisteredPSCInfo> >::_M_clear:
// every pair is destroyed in order (its GUIName first, then the key), then the block is freed once (none when empty).
#include <cstdio>
#include <cstdlib>
#include <new>
static int g_order[16];
static int g_count = 0;
static int g_frees = 0;
static void counted_free(void* p) { ++g_frees; std::free(p); }
class CCharString {
public:
    int id;
    CCharString(int i) : id(i) {}
    ~CCharString() { g_order[g_count++] = id; }
};
namespace NParticleEngine {
class CPSCBase;
enum EPSCType { PSC_NONE };
class CRegisteredPSCInfo {
public:
    CRegisteredPSCInfo(int name) : AllocFunc(0), ComponentType(PSC_NONE), GUIName(name) {}
    CPSCBase*   (*AllocFunc)();
    EPSCType    ComponentType;
    CCharString GUIName;
};
}
struct Pair {
    Pair(int k, int n) : first(k), second(n) {}
    CCharString                       first;
    NParticleEngine::CRegisteredPSCInfo second;
};
template <class _Tp> inline void _Destroy(_Tp* p) { p->~_Tp(); }
template <class _Tp> inline void _Destroy(_Tp* first, _Tp* last) { for (; first != last; ++first) _Destroy(first); }
struct Vector {
    Pair* _M_start;
    Pair* _M_finish;
    Pair* _M_end_of_storage;
    void deallocate(Pair* p) { if (p != 0) counted_free(p); }
    void _M_clear() { _Destroy(_M_start, _M_finish); deallocate(_M_start); }
    void destroy() { _Destroy(_M_start, _M_finish); if (_M_start != 0) deallocate(_M_start); }   // ~vector + ~_Vector_base
};
int main()
{
    Vector v;
    v._M_start = (Pair*)std::malloc(3 * sizeof(Pair));
    new (v._M_start + 0) Pair(10, 11);
    new (v._M_start + 1) Pair(20, 21);
    v._M_finish = v._M_start + 2;
    v._M_end_of_storage = v._M_start + 3;
    v._M_clear();
    if (g_count != 4 || g_order[0] != 11 || g_order[1] != 10 || g_order[2] != 21 || g_order[3] != 20 || g_frees != 1) {
        std::printf("00af36c0_TEST FAIL\n"); return 1;
    }
    Vector e; e._M_start = e._M_finish = e._M_end_of_storage = 0;
    e._M_clear();
    if (g_count != 4 || g_frees != 1) { std::printf("00af36c0_TEST FAIL\n"); return 1; }
    std::printf("00af36c0_TEST PASS\n");
    return 0;
}