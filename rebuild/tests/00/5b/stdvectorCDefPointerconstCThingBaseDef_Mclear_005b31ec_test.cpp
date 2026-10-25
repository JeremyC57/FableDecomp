// Test for 0x005B31EC std::vector<CDefPointer<const CThingBaseDef> >::_M_clear: the source object must hand
// [start, finish) to __destroy_aux once (stubbed here), then free the block; a null start frees nothing.
// `free` is recorded here (the counter is reset before each call: the CRT frees before main).
#include <stdio.h>
static void* g_freed[8];
static int g_nfreed, g_aux;
static const void* g_first; static const void* g_last;
extern "C" void __cdecl free(void* p) { if (g_nfreed < 8) g_freed[g_nfreed] = p; ++g_nfreed; }
struct CThingBaseDef;
class CDefPointeeBase;
template <class T> class CDefPointer { public: CDefPointeeBase* Object; };
typedef CDefPointer<const CThingBaseDef> CThingDefPointer;
namespace std {
struct __false_type { __false_type() { } };
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);
template <>
void __fastcall __destroy_aux(CThingDefPointer* __first, CThingDefPointer* __last, const __false_type&)
{
    ++g_aux; g_first = __first; g_last = __last;
}
template <class _Tp> class allocator { };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
template <class _Tp> class vector {
public:
    _Tp* _M_start; _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, allocator<_Tp> > _M_end_of_storage;
    void _M_clear();
};
}
static CThingDefPointer g_recs[4];
int main()
{
    std::vector<CThingDefPointer> v;
    v._M_start = g_recs; v._M_finish = g_recs + 3; v._M_end_of_storage._M_data = g_recs + 4;
    g_nfreed = 0;
    v._M_clear();
    bool ok = g_aux == 1 && g_first == g_recs && g_last == g_recs + 3 && g_nfreed == 1 && g_freed[0] == g_recs;
    std::vector<CThingDefPointer> e; e._M_start = e._M_finish = e._M_end_of_storage._M_data = 0;
    g_nfreed = 0;
    e._M_clear();
    ok = ok && g_aux == 2 && g_nfreed == 0;
    if (!ok) { puts("005b31ec_TEST FAIL"); return 1; }
    puts("005b31ec_TEST PASS");
    return 0;
}
