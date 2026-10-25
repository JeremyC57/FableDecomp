// _STL::__unguarded_partition(const CAnimationEntry** first, const CAnimationEntry** last, const CAnimationEntry* pivot, CTurningAnimCompare comp) -- introsort partition step for the turning animations: walk both ends towards the pivot under the rotation comparator and swap the out-of-place pairs
// Leaf: ??$__unguarded_partition@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087DFA0, 172 bytes, `ret 8` (/Gr algorithm TU: first in ecx, last in
//   edx, the pivot value and the 4-byte comparator by value on the stack).
// Identity: called by the __introsort_loop of the same instantiation @ 0x0087E530 (`push comp ; push *median ;
//   call`), so this is STLport `__unguarded_partition(first, last, _Tp(__median(...)), comp)` over the
//   vector<const CAnimationEntry*> of the CCreatureAction_InterruptableMidAttackAutoTurn TU, comparator
//   `?A0x37766ae9::CTurningAnimCompare` (Ego_r.pdb). The catalog label "introsort_3way_partition_CAnimationEntry"
//   is descriptive. Landed relatives of the TU: __median 0x0087DA90, __unguarded_linear_insert 0x0087DC30,
//   __push_heap 0x0087DCA0, __adjust_heap 0x0087DE50, __linear_insert 0x0087E250.
// Callee: CAnimationEntry::GetRotationInAnimation(float*) const @ 0x005DBFD0 (landed).
// Shape: the in-class comparator is inlined at both calls (*first held in edi for its two rotation fetches, the
//   pivot in ebp for the second loop); the first loop's rotation is homed in the pivot's dead argument slot, the
//   other three in the 12-byte frame. Each comparator reads its LEFT operand twice (the retail quirk shared by the
//   whole TU, see the landed relatives), so `*last` is never loaded; the iter_swap of `first`/`last` is inline.
extern "C" double __cdecl fabs(double x);
#pragma intrinsic(fabs)

class CAnimationEntry {
public:
    void GetRotationInAnimation(float* rotation) const; // 0x005DBFD0
};

namespace {
class CTurningAnimCompare {                             // Ego_r.pdb, 4 bytes (a class: mangled V)
public:
    float RotationAngle;                                // +0x00
    bool operator()(const CAnimationEntry* a, const CAnimationEntry* b) const    // out of line @ 0x0087C660
    {
        float rotation_a;
        float rotation_b;
        a->GetRotationInAnimation(&rotation_a);
        a->GetRotationInAnimation(&rotation_b);
        float delta_a = (float)fabs(RotationAngle - rotation_a);
        float delta_b = (float)fabs(RotationAngle - rotation_b);
        if (delta_a < delta_b)
            return true;
        return false;
    }
};
}

namespace _STL {
template <class _ForwardIter1, class _ForwardIter2>
inline void iter_swap(_ForwardIter1 __i1, _ForwardIter2 __i2)
{
    const CAnimationEntry* __tmp = *__i1;
    *__i1 = *__i2;
    *__i2 = __tmp;
}

template <class _RandomAccessIter, class _Tp, class _Compare>
_RandomAccessIter __fastcall __unguarded_partition(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __pivot,
                                                   _Compare __comp)
{
    for (;;) {
        while (__comp(*__first, __pivot))
            ++__first;
        --__last;
        while (__comp(__pivot, *__last))
            --__last;
        if (!(__first < __last))
            return __first;
        iter_swap(__first, __last);
        ++__first;
    }
}
template const CAnimationEntry** __fastcall __unguarded_partition<const CAnimationEntry**, const CAnimationEntry*,
                                                                  CTurningAnimCompare>(
    const CAnimationEntry**, const CAnimationEntry**, const CAnimationEntry*, CTurningAnimCompare);
}