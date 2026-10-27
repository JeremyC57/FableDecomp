// std::__unguarded_insertion_sort_aux(pair<long, CCountedPointer<NParticleEngine::CParticleEmitter> >* first, pair<long, CCountedPointer<NParticleEngine::CParticleEmitter> >* last, pair<long, CCountedPointer<NParticleEngine::CParticleEmitter> >*, CKeyPairCompareLess comp) -- STLport final-insertion-sort pass over the vector-map entries: insert each element (a counted copy) into the already ordered prefix with the out-of-line __unguarded_linear_insert
// Leaf: ??$__unguarded_insertion_sort_aux@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AE9DB0, 64 bytes, /Gr __fastcall (first ecx, last edx; the _Tp* tag and the 4-byte
//   comparator on the stack), `ret 8`.
// Identity: its only callee is the landed std::__unguarded_linear_insert<pair<long, CCountedPointer<NParticleEngine::CParticleEmitter> >*, ..> 0x00AE9A40 (agent953 J050: CParticleEmitterDatabase::Emitters).
// Callees: std::__unguarded_linear_insert 0x00AE9A40 (`__fastcall`: the element in ecx, the by-value pair built in the
//   argument area by CCountedPointer's copy constructor, the comparator copied ahead of it).
// Shape (family K013, representative 0x006B9ED0): STLport 4.6 _algo.c
//   `for (_RandomAccessIter __i = __first; __i != __last; ++__i) __unguarded_linear_insert(__i, _Tp(*__i), __comp);`
//   with the comparator's operator() declaration-only (GOTCHAS 1474) and the pair's implicit copy constructor calling the
//   counted pointer's inline add-ref copy.
class CCPPointerInfo {                                       // Ego_r.pdb, 12 bytes
public:
    unsigned long RefCount;                                  // +0
    void (__fastcall* DeleteFunc)(void* data);               // +4, Data in ecx
    void* Data;                                              // +8
};
template <class T> class CCountedPointer {
public:
    T*              Data;                                    // +0
    CCPPointerInfo* Info;                                    // +4
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
namespace NParticleEngine { class CParticleEmitter; }
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}   // namespace std
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const;
};

namespace std {
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00AE9A40

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_insertion_sort_aux(_RandomAccessIter __first, _RandomAccessIter __last, _Tp*, _Compare __comp)
{
    for (_RandomAccessIter __i = __first; __i != __last; ++__i)
        __unguarded_linear_insert(__i, _Tp(*__i), __comp);
}
typedef pair<long, CCountedPointer<NParticleEngine::CParticleEmitter> > _Entry;
template void __fastcall __unguarded_insertion_sort_aux<_Entry*, _Entry, CKeyPairCompareLess<long, CCountedPointer<NParticleEngine::CParticleEmitter> > >(_Entry*, _Entry*, _Entry*, CKeyPairCompareLess<long, CCountedPointer<NParticleEngine::CParticleEmitter> >);
}   // namespace std