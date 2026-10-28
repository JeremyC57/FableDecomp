// std::__adjust_heap(pair<unsigned long, CAnimationEntry>* first, int holeIndex, int len, pair<unsigned long, CAnimationEntry> value, CKeyPairCompareLess<unsigned long, CAnimationEntry> comp) -- STLport heap sift-down: move the larger-keyed child up into the hole until the bottom, take the lone last child, then __push_heap the value back up from there
// Grade: DIFFER(256v256) -- register swap only: retail keeps secondChild in ebx and the source element pointer in edi, ours the reverse (same instructions); tried the verbatim STLport loop, CIVCountedPointeeBase::Release as long-result / early-return forms, /Gr
// Leaf: ??$__adjust_heap@PAU?$pair@KVCAnimationEntry@@@std@@HU12@V?$CKeyPairCompareLess@KVCAnimationEntry@@@@@std@@YIXPAU?$pair@KVCAnimationEntry@@@0@HHU10@V?$CKeyPairCompareLess@KVCAnimationEntry@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E1CB0, 256 bytes, `__fastcall` (first in ecx, holeIndex in edx; len,
//   the 0x14-byte pair by value and the empty comparator on the stack), `ret 1Ch`.
// Identity (proven): STLport 4.6 _heap.c `__adjust_heap<_RandomAccessIterator, _Distance, _Tp, _Compare>` over
//   pair<unsigned long, CAnimationEntry> (0x14 bytes: the key, then Ego_r.pdb CAnimationEntry {BankIndex, AnimName,
//   GroupName, CIVCountedPointer<CAnimComponentMap> Components}) with CKeyPairCompareLess<unsigned long, CAnimationEntry>
//   (unsigned key compare, `jae`); the Ego_r.pdb name set holds the Dinkumware twin
//   ??$_Adjust_heap@PAU?$pair@KVCAnimationEntry@@@std@@HU12@V?$CKeyPairCompareLess@KVCAnimationEntry@@@@@std@@...
//   (a CVectorMap<unsigned long, CAnimationEntry> sort); retail's STL is STLport, so the STLport spelling.
// Callees: pair<unsigned long, CAnimationEntry>::operator= 0x005E0E20 (the implicit assignment, out of line for the
//   bottom case; inlined in the loop), __push_heap<...> 0x005E1310 (__fastcall), the components' virtual Delete
//   (slot 1) on the last release (value's own destructor, and the assignment's release).
// Shape: STLport 4.6 _heap.c verbatim; `value` is destroyed by this callee after __push_heap (MSVC callee-destroys).
class CAnimComponentMap;
class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                            // slot 1
    mutable unsigned long IVRefCount;                       // +0x04
    void AddRef() const { ++IVRefCount; }
    long Release() const
    {
        if (--IVRefCount == 0) {
            Delete();
            return 0;
        }
        return (long)IVRefCount;
    }
};
template <class T> class CIVCountedPointer {
public:
    CIVCountedPointeeBase* Object;
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CIVCountedPointer() { Release(); }
    void Release()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            Release();
            Object = other.Object;
            if (Object)
                Object->AddRef();
        }
        return *this;
    }
};
class CDefString {
public:
    long TablePos;
};
class CAnimationEntry {                                     // Ego_r.pdb, 0x10 bytes
public:
    long                                  BankIndex;        // +0x00
    CDefString                            AnimName;         // +0x04
    CDefString                            GroupName;        // +0x08
    CIVCountedPointer<CAnimComponentMap>  Components;       // +0x0C
};
namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
}
template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const std::pair<K, V>& a, const std::pair<K, V>& b) const { return a.first < b.first; }
};

namespace std {
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __val,
                            _Compare __comp);               // 0x005E1310

template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __adjust_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __len, _Tp __val,
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
    __push_heap(__first, __holeIndex, __topIndex, __val, __comp);
}

template void __fastcall __adjust_heap<pair<unsigned long, CAnimationEntry>*, int, pair<unsigned long, CAnimationEntry>,
                                       CKeyPairCompareLess<unsigned long, CAnimationEntry> >(
    pair<unsigned long, CAnimationEntry>*, int, int, pair<unsigned long, CAnimationEntry>,
    CKeyPairCompareLess<unsigned long, CAnimationEntry>);
}   // namespace std
