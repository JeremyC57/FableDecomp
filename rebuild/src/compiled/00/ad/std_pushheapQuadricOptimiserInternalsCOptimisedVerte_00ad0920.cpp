// std::__push_heap<COptimisedVertex::CGroup*, int, CGroup, bool (__fastcall*)(const CGroup&, const CGroup&)> --
// STLport heap sift-up of a vertex's bone groups under the static CGroup::CompareWeight function pointer.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AD0920, 104 bytes, `ret 10h`, no direct callees: /Gr algorithm TU (first ecx,
// hole edx, then top, the 8-byte CGroup by value and the comparator), spelled here as explicit __fastcall. The
// comparator is called `mov ecx,&parent ; lea edx,&value ; call [esp+20h]` (a /Gr function pointer is __fastcall).
// Identity: Ego_r.pdb QuadricOptimiserInternals::COptimisedVertex::CGroup (8 bytes: BoneIndex +0, BoneWeight +4) and
// the PDB family `??$_Push_heap@PAVCGroup@COptimisedVertex@QuadricOptimiserInternals@@HV123@P6I_NABV123@0@Z@std@@...`
// (Dinkumware spelling; retail is STLport, `__push_heap`); sole tail of the CGroup __adjust_heap 0x00AD1DD0.
// Leaf: ??$__push_heap@PAVCGroup@COptimisedVertex@QuadricOptimiserInternals@@HV123@P6I_NABV123@0@Z@std@@YIXPAVCGroup@COptimisedVertex@QuadricOptimiserInternals@@HHV123@P6I_NABV123@2@Z@Z
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
                            _Compare __comp)
{
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __value)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __value;
}

template void __fastcall __push_heap<CGroup*, int, CGroup, PFNGroupCompare>(CGroup*, int, int, CGroup, PFNGroupCompare);

} // namespace std
