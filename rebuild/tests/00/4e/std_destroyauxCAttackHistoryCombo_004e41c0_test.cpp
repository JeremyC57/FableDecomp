// Test for 0x004E41C0 std::__destroy_aux<CAttackHistoryCombo*>: the source object must destroy every record of
// [first, last) in order -- each frees its AttackCombo buffer when non-null and the record itself is NOT deleted (flag 0)
// -- and do nothing for an empty range. `free` is recorded here (static storage only).
#include <stdio.h>
#include <string.h>
static void* g_freed[16];
static int g_nfreed;
extern "C" void __cdecl free(void* p) { if (g_nfreed < 16) g_freed[g_nfreed] = p; ++g_nfreed; }
namespace std {
struct __false_type { __false_type() { } };
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);
}
class CAttackHistoryCombo;
static unsigned char g_recs[3][16];
static long g_b0[4], g_b2[2];
int main()
{
    memset(g_recs, 0, sizeof(g_recs));
    void* p = g_b0; memcpy(g_recs[0], &p, 4);
    p = g_b2; memcpy(g_recs[2], &p, 4);
    CAttackHistoryCombo* first = (CAttackHistoryCombo*)g_recs[0];
    CAttackHistoryCombo* last = (CAttackHistoryCombo*)(g_recs[0] + 3 * 16);
    std::__false_type tag;
    g_nfreed = 0;
    std::__destroy_aux(first, last, tag);
    bool ok = g_nfreed == 2 && g_freed[0] == g_b0 && g_freed[1] == g_b2;
    g_nfreed = 0;
    std::__destroy_aux(first, first, tag);
    ok = ok && g_nfreed == 0;
    if (!ok) { puts("004e41c0_TEST FAIL"); return 1; }
    puts("004e41c0_TEST PASS");
    return 0;
}
