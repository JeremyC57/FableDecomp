// std::__uninitialized_fill_n(CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry* first, unsigned int n, const CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry& x, const __false_type&) -- copy-construct x into n raw slots, return the end
// Leaf: ??$__uninitialized_fill_n@PAVCSpriteTrailEntry@CEngineInternalPrimitiveSpriteTrail@@IV12@@std@@YIPAVCSpriteTrailEntry@CEngineInternalPrimitiveSpriteTrail@@PAV12@IABV12@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BC9EB0, 52 bytes, `__fastcall` (first ecx, n edx, x and the tag on the stack), `ret 8`.
// Identity (proven): the catalog binding of 0x00BC9EB0 is this symbol (recorded by its caller 0x00BCBF80 (vector<CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry>::_M_insert_overflow, landed) and the fill insert 0x00BCA3DA site); the element's 16-byte
//   memberwise copy matches the PDB copy constructor. The manifest label is propagated/behavioural.
// Callees: none (placement copy inlined behind its null test).
// Shape (family M007, representative 0x0040DE00): STLport 4.6 `__uninitialized_fill_n(__false_type)` -- `cur` walks from
//   first, `_Construct(&*cur, x)` (placement new's null test, then four dword moves through esi), `--n, ++cur`; VC7.1 turns the
//   `n > 0` count into the `test edx,edx ; jbe` guard plus a `dec edx ; jne` loop with an aligned head.
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
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry* __fastcall __uninitialized_fill_n<CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry*, unsigned int, CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry>(CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry*, unsigned int, const CEngineInternalPrimitiveSpriteTrail::CSpriteTrailEntry&, const __false_type&);
}