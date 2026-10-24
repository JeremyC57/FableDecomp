// _global::__push_heap(const CAnimationEntry** first, int holeIndex, int topIndex, const CAnimationEntry* val, CTurningAnimCompare comp) -- SGI push_heap core for a heap of animation entries ordered by how close their rotation is to a target angle: sift `val` up from holeIndex towards topIndex
// Leaf: __push_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087DCA0, length 166 bytes, `ret 0Ch` (topIndex, the value
//   and the 4-byte comparator on the stack; first/holeIndex in ecx/edx -- the SGI algorithm TUs are /Gr). Caller: the
//   push_heap front end at 0x0087DEE3.
// Identity: the row label `_Push_heap<CAnimationEntry const**,int,CAnimationEntry const*,_anon_37766AE9::CTurningAnimCompare>`
//   is right; Ego_r.pdb CTurningAnimCompare = { float RotationAngle } (an anonymous-namespace functor).
// Callee: CAnimationEntry::GetRotationInAnimation(float*) const @ 0x005DBFD0 (thiscall, `ret 4`; stores the negated
//   rotation of the entry's animation).
// Shape: STLport
//     _Distance __parent = (__holeIndex - 1) / 2;
//     while (__holeIndex > __topIndex && __comp(*(__first + __parent), __val)) {
//       *(__first + __holeIndex) = *(__first + __parent); __holeIndex = __parent; __parent = (__holeIndex - 1) / 2; }
//     *(__first + __holeIndex) = __val;
//   with the functor inlined; the final store is tail-duplicated into each of the three exits. The functor rounds each
//   distance into a float local (`(float)fabs(...)`): that is what leaves the `fxch st(1) ; fxch st(1)` pair before `fcompp`.
// Retail quirk: both GetRotationInAnimation calls are made on the LEFT operand (`mov ecx,ebp` twice, ebp = first[parent]);
//   the right operand is never read, so the comparison is |R - x| < |R - x| -- always false -- and the sift never moves.
//   The functor below reproduces that (it is how the bytes read, not a reconstruction error).
extern "C" double __cdecl fabs(double x);
#pragma intrinsic(fabs)

class CAnimationEntry {
public:
    void GetRotationInAnimation(float* rotation) const; // 0x005DBFD0
};

namespace {
struct CTurningAnimCompare {                            // Ego_r.pdb, 4 bytes
    float RotationAngle;                                // +0x00
    bool operator()(const CAnimationEntry* a, const CAnimationEntry* b) const
    {
        float rotation_a;
        float rotation_b;
        a->GetRotationInAnimation(&rotation_a);
        a->GetRotationInAnimation(&rotation_b);
        float delta_a = (float)fabs(RotationAngle - rotation_a);
        float delta_b = (float)fabs(RotationAngle - rotation_b);
        return delta_a < delta_b;
    }
};
}

void __fastcall __push_heap(const CAnimationEntry** first, int holeIndex, int topIndex, const CAnimationEntry* val,
                            CTurningAnimCompare comp)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), val)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = val;
}
