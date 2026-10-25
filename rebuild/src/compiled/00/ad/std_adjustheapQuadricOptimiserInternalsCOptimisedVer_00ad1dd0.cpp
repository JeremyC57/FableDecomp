// std::__adjust_heap<COptimisedVertex::CGroup*, int, CGroup, bool (__fastcall*)(const CGroup&, const CGroup&)> --
// STLport heap sift-down of a vertex's bone groups under the static CGroup::CompareWeight function pointer, then
// __push_heap @ 0x00AD0920 (same instantiation).
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD1DD0, 128 bytes, `ret 10h`: /Gr algorithm TU (first ecx, hole edx, then len,
// the 8-byte CGroup by value as two dwords, and the comparator); the comparator is called as
// `call [esp+24h]` with both element addresses in ecx/edx (a /Gr function pointer is __fastcall).
// Identity: Ego_r.pdb QuadricOptimiserInternals::COptimisedVertex::CGroup (8 bytes: BoneIndex +0, BoneWeight +4)
// with `?CompareWeight@CGroup@COptimisedVertex@QuadricOptimiserInternals@@SI_NABV123@0@Z` and the PDB family
// `??$_Adjust_heap@PAVCGroup@COptimisedVertex@QuadricOptimiserInternals@@HV123@P6I_NABV123@0@Z@std@@...`; callers are
// the CGroup sort_heap 0x00AD4960 / __introsort_loop TU (make_heap 0x00AD2CE0); its callee 0x00AD0920 carries the
// CGroup manifest label. The landed CSpeechAnalyser::CMatchProbability model was propagated (same 8-byte shape).
// Leaf: ??$__adjust_heap@PAVCGroup@COptimisedVertex@QuadricOptimiserInternals@@HV123@P6I_NABV123@0@Z@std@@YIXPAVCGroup@COptimisedVertex@QuadricOptimiserInternals@@HHV123@P6I_NABV123@2@Z@Z
namespace QuadricOptimiserInternals {
class COptimisedVertex {
public:
    class CGroup {                           // Ego_r.pdb COptimisedVertex::CGroup, 8 bytes
    public:
        unsigned long BoneIndex;             // +0x0
        float         BoneWeight;            // +0x4
        static bool __fastcall CompareWeight(const CGroup& a, const CGroup& b);
    };
};
}
typedef QuadricOptimiserInternals::COptimisedVertex::CGroup CGroup;
typedef bool (__fastcall* PFNGroupCompare)(const CGroup&, const CGroup&);

namespace std {

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __value,
                            _Compare __comp);   // 0x00AD0920

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __value,
                              _Compare __comp)
{
    _Distance __topIndex = __holeIndex;
    _Distance __secondChild = 2 * __holeIndex + 2;
    while (__secondChild < __len) {
        if (__comp(*(__first + __secondChild), *(__first + (__secondChild - 1))))
            __secondChild--;
        *(__first + __holeIndex) = *(__first + __secondChild);
        __holeIndex = __secondChild;
        __secondChild = 2 * (__secondChild + 1);
    }
    if (__secondChild == __len) {
        *(__first + __holeIndex) = *(__first + (__secondChild - 1));
        __holeIndex = __secondChild - 1;
    }
    __push_heap(__first, __holeIndex, __topIndex, __value, __comp);
}

template void __fastcall __adjust_heap<CGroup*, int, CGroup, PFNGroupCompare>(CGroup*, int, int, CGroup, PFNGroupCompare);

} // namespace std
