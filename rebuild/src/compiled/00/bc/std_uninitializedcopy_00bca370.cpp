// std::__uninitialized_copy(CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry* first, CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry* last, CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry* result, const __false_type&) -- copy-construct [first, last) into raw slots at result, return the end
// Leaf: ??$__uninitialized_copy@PAVCSpriteTrailEntry@CEngineInternalPrimitiveSpriteTrail@@PAV12@@std@@YIPAVCSpriteTrailEntry@CEngineInternalPrimitiveSpriteTrail@@PAV12@00ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BCA370, 56 bytes, `__fastcall` (first ecx, last edx, result and the tag on the stack), `ret 8`.
// Identity (proven): the catalog binding of 0x00BCA370 is this symbol (recorded by its caller 0x00BCBF80 (vector<CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry>::_M_insert_overflow, landed) and the fill insert 0x00BCA3DA site).
// Callees: none (placement copy inlined behind its null test).
// Shape (family M010, representative 0x0040F5E0): STLport 4.6 `__uninitialized_copy(__false_type)` -- `cur = result`, the
//   first == last early test, `_Construct(&*cur, *first)` (null test, four dword moves through esi: the memberwise copy),
//   `++first, ++cur`; returns cur in eax.
inline void* operator new(unsigned int, void* p) { return p; }
class C3DVector {                                         // Ego_r.pdb, 12 bytes
public:
    float X;
    float Y;
    float Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // PDB ??0C3DVector@@QAE@ABV0@@Z
};
class CEngineInternalPrimitiveSpriteTrail {
public:
    class CSpriteTrailEntry {                             // Ego_r.pdb, 16 bytes
    public:
        C3DVector Position;                               // +0x0
        float     Width;                                  // +0xC
        CSpriteTrailEntry(const CSpriteTrailEntry& other) // PDB ??0CSpriteTrailEntry@CEngineInternalPrimitiveSpriteTrail@@QAE@ABV01@@Z
            : Position(other.Position), Width(other.Width) { }
    };
};
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry* __fastcall __uninitialized_copy<CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry*, CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry*>(CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry*, CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry*, CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry*, const __false_type&);
}