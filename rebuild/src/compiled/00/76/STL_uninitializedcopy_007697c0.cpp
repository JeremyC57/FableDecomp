// std::__uninitialized_copy(ParticleAndEndPoints* first, ParticleAndEndPoints* last, ParticleAndEndPoints* result, const __false_type&) -- copy-construct [first, last) into raw storage at result, return the end of the constructed range
// Leaf: ??$__uninitialized_copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007697C0, 76 bytes (+int3 pad to 80), `ret 8`.
// Callers: 0x0076B850 (vector<ParticleAndEndPoints>::_M_insert_overflow, twice). Callee: CCharString copy constructor 0x0099EC30 (twice per element).
// Twin: 0x00769930 is the byte-identical <const T*, T*> instantiation (VC7.1 keeps
//   `__uninitialized_copy<const T*>` and `<T*>` as separate COMDATs); the iterator constness decides which.
// Shape: STLport `__uninitialized_copy(first, last, result, __false_type)` from an /Gr algorithm TU (__fastcall:
//   first in ecx, last in edx, result and the tag reference on the stack). `_Construct` is placement new, so each
//   element is null-tested (`test edi,edi`) before ParticleAndEndPoints' implicit copy constructor runs: two
//   out-of-line CCharString copies, then three dword moves. The `nop` before the loop is VC7.1's loop alignment.
// Element: Ego_r.pdb ParticleAndEndPoints is 0x14 bytes (PeriodicFrames +0x10, the rest unnamed); the bytes show two
//   CCharStrings and three dwords (CTCObjectAugmentations::ParticleEffects is a vector<vector<ParticleAndEndPoints> >).
class CCharString {
public:
    void* PStringData;                            // +0x00
    CCharString(const CCharString& other);        // 0x0099EC30
};

struct ParticleAndEndPoints {                     // 0x14 (Ego_r.pdb: PeriodicFrames +0x10)
    CCharString StartDummy;                       // +0x00 (descriptive, as the landed AttackParticleAndEndPoints 0x00769040)
    CCharString EndDummy;                         // +0x04
    long        ParticleType;                     // +0x08
    long        ParticleUID;                      // +0x0c (descriptive)
    long        PeriodicFrames;                   // +0x10
};

inline void* operator new(unsigned int, void* where) { return where; }

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template ParticleAndEndPoints* __fastcall __uninitialized_copy<ParticleAndEndPoints*, ParticleAndEndPoints*>(
    ParticleAndEndPoints*, ParticleAndEndPoints*, ParticleAndEndPoints*, const __false_type&);

}