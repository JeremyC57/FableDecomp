// Test for 0x004F963D std::vector<CAttackHistoryCombo>::_M_clear: the source object must hand [start, finish) to
// __destroy_aux once (stubbed here), then free the block; a null start frees nothing. `free` is recorded here.
#include <stdio.h>
static void* g_freed[8];
static int g_nfreed, g_aux;
static const void* g_first; static const void* g_last;
extern "C" void __cdecl free(void* p) { if (g_nfreed < 8) g_freed[g_nfreed] = p; ++g_nfreed; }
class CAttackHistoryCombo { public: unsigned char _body[16]; };
namespace std {
struct __false_type { __false_type() { } };
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);
template <>
void __fastcall __destroy_aux(CAttackHistoryCombo* __first, CAttackHistoryCombo* __last, const __false_type&)
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
static CAttackHistoryCombo g_recs[4];
int main()
{
    std::vector<CAttackHistoryCombo> v;
    v._M_start = g_recs; v._M_finish = g_recs + 3; v._M_end_of_storage._M_data = g_recs + 4;
    g_nfreed = 0;
    v._M_clear();
    bool ok = g_aux == 1 && g_first == g_recs && g_last == g_recs + 3 && g_nfreed == 1 && g_freed[0] == g_recs;
    std::vector<CAttackHistoryCombo> e; e._M_start = e._M_finish = e._M_end_of_storage._M_data = 0;
    g_nfreed = 0;
    e._M_clear();
    ok = ok && g_aux == 2 && g_nfreed == 0;
    if (!ok) { puts("004f963d_TEST FAIL"); return 1; }
    puts("004f963d_TEST PASS");
    return 0;
}
