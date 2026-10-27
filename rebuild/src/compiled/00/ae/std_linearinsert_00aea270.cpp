// std::__linear_insert(pair<long, CCountedPointer<NParticleEngine::CParticleEmitter> >* first, pair* last, pair val, CKeyPairCompareLess comp) -- STLport insertion-sort step: a value with a smaller key than the first shifts the run up and goes first, otherwise it sinks unguarded
// Flags: /Gr
// Leaf: ??$__linear_insert@PAU?$pair@JV?$CCountedPointer@VCParticleEmitter@NParticleEngine@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AEA270, 113 bytes, /Gr __fastcall (first ecx, last edx; the 12-byte value and the empty
//   comparator on the stack), `ret 10h`.
// Identity (proven): reloc_bindings names this ??$__linear_insert@PAU?$pair@JV?$CCountedPointer@VCParticleEmitter@NParticleEngine@@@@@std@@U12@V?$CKeyPairCompareLess@...;
//   its caller is 0x00AEA530 (its __insertion_sort).
// Callees: __copy_backward 0x00AE9C00 (the iterator tag homed over the comparator slot), pair's implicit operator= 0x00AE98E0 (VC7.1 emits
//   it out of line and calls it; a declared-only operator= makes VC reload the value's Info), __unguarded_linear_insert 0x00AE9A40 (the value copy-constructed in its argument area, `inc [Info]`), the value's
//   destructor (DeleteFunc through the count block, operator delete 0x00BFE9BC).
// Shape (family K087, representative 0x00448E90): STLport 4.x `if (comp(val, *first)) { copy_backward(first, last, last + 1);
//   *first = val; } else __unguarded_linear_insert(last, val, comp);` -- the landed 0x00ADBC50 model.
void __cdecl operator delete(void* p);                  // 0x00BFE9BC
class CCPPointerInfo {                                   // CCountedPointer's shared block
public:
    unsigned long RefCount;                             // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4, Data in ecx
    void* Data;                                         // +8
};
template <class T> class CCountedPointer {              // agent950's landed model (0x00A99110: ReleaseData / ShareData)
public:
    T*              Data;                               // +0
    CCPPointerInfo* Info;                               // +4
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { ReleaseData(); Data = 0; Info = 0; }
    CCountedPointer& operator=(const CCountedPointer& other) { ShareData(other.Data, other.Info); return *this; }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            ReleaseData();
            Data = data;
            Info = info;
            if (info)
                ++info->RefCount;
        }
    }
    void ReleaseData()
    {
        if (Info) {
            if (--Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
};
namespace NParticleEngine { class CParticleEmitter; }
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x0
    _T2 second;                                         // +0x4
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }   // STLport 4.6 _pair.h
};
struct random_access_iterator_tag { random_access_iterator_tag() { } };
}
template <class K, class V> class CKeyPairCompareLess {  // Ego_r.pdb: orders vector-map entries by key
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};
namespace std {
#pragma auto_inline(off)
// Same-TU stand-in for the out-of-line __copy_backward (retail 0x00AE9C00): it never touches the tag, which lets VC7.1 home the
// caller's empty tag on a dead argument slot as retail does.
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
__declspec(noinline) _BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                                                   _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
#pragma auto_inline(on)
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last, _BidirectionalIter2 __result)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00AE9A40
template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __linear_insert(_RandomAccessIter __first, _RandomAccessIter __last, _Tp __val, _Compare __comp)
{
    if (__comp(__val, *__first)) {
        copy_backward(__first, __last, __last + 1);
        *__first = __val;
    }
    else
        __unguarded_linear_insert(__last, __val, __comp);
}
typedef pair<long, CCountedPointer<NParticleEngine::CParticleEmitter> > _Elem;
typedef CKeyPairCompareLess<long, CCountedPointer<NParticleEngine::CParticleEmitter> > _Cmp;
template void __fastcall __linear_insert<_Elem*, _Elem, _Cmp>(_Elem*, _Elem*, _Elem, _Cmp);
} // namespace std