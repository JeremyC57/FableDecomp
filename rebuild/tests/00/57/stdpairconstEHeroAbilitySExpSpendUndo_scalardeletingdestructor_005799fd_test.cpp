// Behaviour test for std::pair<const EHeroAbility,SExpSpendUndo>::`scalar deleting destructor' (0x005799FD): flag 0 destroys
// the value in place, flag 1 also frees the pair. The source is copied verbatim; the value destructor and operator delete
// are recording stubs.
#include <stdio.h>
#include <stdlib.h>
static int g_valueDtors = 0, g_deletes = 0; static const void* g_lastValue = 0;
// ---- source (verbatim) ----
// std::pair<const EHeroAbility,SExpSpendUndo>::`scalar deleting destructor'(unsigned int flags) -- destroy the SExpSpendUndo value (+4), free the pair when bit 0 is set
// Leaf: ??_G?$pair@$$CBW4EHeroAbility@@USExpSpendUndo@@@std@@QAEPAXI@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005799FD, 31 bytes, thiscall `ret 4`; size-optimised (`pop ecx` after the delete).
// Identity: the map node value of CTCInventoryExperience::UndoableAbilityBuys (struct_layouts_egor:
//   map<EHeroAbility, SExpSpendUndo>). Its only direct caller is 0x00579D55, the UndoableAbilityBuys erase(iterator), reached from CTCInventoryExperience::UndoBuyAbility 0x00576EBE;
//   STLport erase runs `_Destroy(&node->_M_value_field)`, which VC7.1 under optimize("s") compiles as `push 0 ; call ??_G`
//   (a call with flag 0) instead of inlining ~pair. The twin for map<EHeroTrainableStatType, SExpSpendUndo> sits beside it
//   (0x005799DE / 0x005799FD, no identical-code folding). SExpSpendUndo::~SExpSpendUndo is 0x005797FA (Ego_r.pdb
//   ??1SExpSpendUndo@@QAE@XZ). The earlier "CLandscapeBackgroundPatch::`vector_deleting_destructor'" label was propagated.
// Shape: the implicit ~pair is emitted beside it; the explicit _Destroy instantiation below is what makes VC7.1 emit the
//   `??_G` (exactly STLport's `_Destroy(_Tp* p) { p->~_Tp(); }`).
#pragma optimize("s", on)
enum EHeroAbility { EHEROABILITY_FIRST };

struct SExpSpendUndo {
    unsigned char _body[0x34];                  // four STLport list<long> + nine longs (see 0x005797FA)
    ~SExpSpendUndo();                           // 0x005797FA
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                  // +0x00
    _T2 second;                                 // +0x04
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template void _Destroy(pair<const EHeroAbility, SExpSpendUndo>* __pointer);
}

// ---- stubs ----
SExpSpendUndo::~SExpSpendUndo() { ++g_valueDtors; g_lastValue = this; }
void operator delete(void* p) { ++g_deletes; free(p); }
typedef std::pair<const EHeroAbility, SExpSpendUndo> Pair;
int main()
{
    static unsigned char raw[sizeof(Pair)];
    Pair* p = (Pair*)raw;
    std::_Destroy(p);                                   // push 0 ; call ??_G
    int ok = sizeof(Pair) == 0x38 && g_valueDtors == 1 && g_lastValue == raw + 4 && g_deletes == 0;
    Pair* h = (Pair*)malloc(sizeof(Pair));
    delete h;                                           // ??_G with flag 1
    ok = ok && g_valueDtors == 2 && g_lastValue == (unsigned char*)h + 4 && g_deletes == 1;
    printf(ok ? "005799fd_TEST PASS\n" : "005799fd_TEST FAIL\n");
    return ok ? 0 : 1;
}
