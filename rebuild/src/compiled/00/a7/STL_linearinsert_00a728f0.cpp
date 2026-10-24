// _STL::__linear_insert(pair<long,C3DMeshVolume>* first, pair<long,C3DMeshVolume>* last, pair<long,C3DMeshVolume> val, CKeyPairCompareLess<long,C3DMeshVolume> comp) -- insertion-sort step over the navigation containment volumes: a value whose key is below first's shifts [first, last) up one slot and goes first, anything else goes through the unguarded insert
// Leaf: ??$__linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A728F0, 179 bytes (+13 int3), /Gr `__fastcall` (first in ecx, last in
//   edx; the 16-byte value and the empty comparator on the stack, `ret 14h`).
// Callees: _STL::__copy_backward 0x00A70A10 (first/last in ecx/edx, result, the tag reference and a null distance
//   pointer), C3DMeshVolume::operator= 0x00A32960 (out-of-line COMDAT), CCharString::CCharString(const CCharString&)
//   0x0099EC30, _STL::__unguarded_linear_insert 0x00A703C0 (same chunk), the count block's DeleteFunc, operator
//   delete 0x00BFE9BC, CCharString::~CCharString 0x0099EAE0 (val).
// Identity: the manifest's "Vector_InsertElement_ComplexValue" is descriptive. 16-byte pair<long, C3DMeshVolume>
//   entries of CPhysicsMesh::NavigationContainmentVolumes ordered by the signed key (`jge`).
// Shape: STLport `if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; } else
//   __unguarded_linear_insert(last, val, comp);` -- copy_backward inlined down to the out-of-line SGI __copy_backward
//   with the random-access tag homed in the dead comparator slot; `*first = val` is the pair's implicit assignment
//   (the key word, then the volume's out-of-line operator=); the else arm rebuilds val in the argument area; the
//   callee destroys its by-value val on both paths.
void __cdecl operator delete(void* p);                  // 0x00BFE9BC

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CCPPointerInfo {                                  // Ego_r.pdb, 12 bytes
public:
    long  RefCount;                                     // +0x00
    void (__fastcall* DeleteFunc)(void* data);          // +0x04, Data in ecx
    void* Data;                                         // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CCountedPointer()
    {
        if (Info && --Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            operator delete(Info);
        }
        Data = 0;
        Info = 0;
    }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info != info) {
            if (Info) {
                --Info->RefCount;
                if (Info->RefCount == 0) {
                    Info->DeleteFunc(Info->Data);
                    operator delete(Info);
                }
            }
            Data = data;
            Info = info;
            if (Info)
                ++Info->RefCount;
        }
    }
    CCountedPointer& operator=(const CCountedPointer& other) { ShareData(other.Data, other.Info); return *this; }
};

class CContainmentVolume;

class C3DMeshVolume {                                   // 12 bytes (Ego_r.pdb)
public:
    CCharString                         Name;           // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;   // +0x04
    C3DMeshVolume& operator=(const C3DMeshVolume& other)   // out of line @ 0x00A32960 where not inlined
    {
        Name = other.Name;
        ConvexVolume = other.ConvexVolume;
        return *this;
    }
};

namespace _STL {
template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

namespace _STL {

class __false_type { public: __false_type() { } };
class random_access_iterator_tag { public: random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*);   // 0x00A70A10

template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _BI1, class _BI2>
inline _BI2 copy_backward(_BI1 __first, _BI1 __last, _BI2 __result)
{
    return __copy_backward_ptrs(__first, __last, __result, __false_type());
}

template <class _RandomAccessIter, class _Tp, class _Compare>
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00A703C0

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

typedef pair<long, C3DMeshVolume> TNavEntry;
template void __fastcall __linear_insert<TNavEntry*, TNavEntry, CKeyPairCompareLess<long, C3DMeshVolume> >(TNavEntry*, TNavEntry*, TNavEntry, CKeyPairCompareLess<long, C3DMeshVolume>);

}