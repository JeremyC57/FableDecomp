// _STL::__linear_insert(pair<CCharString,C3DMeshVolume>* first, pair<CCharString,C3DMeshVolume>* last, pair<CCharString,C3DMeshVolume> val, CKeyPairCompareLess<CCharString,C3DMeshVolume> comp) -- insertion-sort step over the named containment volumes: a value whose name sorts before first's shifts [first, last) up one slot and goes first, anything else goes through the unguarded insert
// Leaf: ??$__linear_insert@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A729B0, 226 bytes (+14 int3), /Gr `__fastcall` (first in ecx, last in
//   edx; the 16-byte value and the empty comparator on the stack, `ret 14h`).
// Callees: CBasicString<char>::operator< 0x00429950 (behind the inline CCharString::operator<), _STL::__copy_backward
//   0x00A70AB0, CCharString::operator= 0x0099EFB0, C3DMeshVolume::operator= 0x00A32960 (out-of-line COMDAT),
//   CCharString::CCharString(const CCharString&) 0x0099EC30 (x2), _STL::__unguarded_linear_insert 0x00A70540 (same
//   chunk), the count block's DeleteFunc, operator delete 0x00BFE9BC, CCharString::~CCharString 0x0099EAE0 (x2).
// Identity: the manifest's "Vector_InsertElement_StringPair" is descriptive. 16-byte pair<CCharString, C3DMeshVolume>
//   entries of CPhysicsMesh::ContainmentVolumes ordered by the CCharString key.
// Shape: STLport `if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; } else
//   __unguarded_linear_insert(last, val, comp);` -- comp is the inline CCharString::operator< (equal -> false, null
//   left -> true, null right -> false, else the data compare); copy_backward inlined down to the out-of-line SGI
//   __copy_backward with the random-access tag homed in the dead comparator slot; `*first = val` is the pair's
//   implicit assignment (string operator=, then the volume's out-of-line operator=); the callee destroys its by-value
//   val on both paths.
void __cdecl operator delete(void* p);                  // 0x00BFE9BC

template <class C> class CBasicString {
public:
    bool operator<(const CBasicString& other) const;    // 0x00429950
};

class CCharString {
public:
    CBasicString<char>* PData;                          // +0x00
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    bool operator<(const CCharString& other) const
    {
        if (other.PData == PData)
            return false;
        if (PData == 0)
            return true;
        if (other.PData == 0)
            return false;
        return *PData < *other.PData;
    }
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
                                              _BidirectionalIter __result, const random_access_iterator_tag&, _Distance*);   // 0x00A70AB0

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
void __fastcall __unguarded_linear_insert(_RandomAccessIter __last, _Tp __val, _Compare __comp);   // 0x00A70540

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

typedef pair<CCharString, C3DMeshVolume> TVolumeEntry;
template void __fastcall __linear_insert<TVolumeEntry*, TVolumeEntry, CKeyPairCompareLess<CCharString, C3DMeshVolume> >(TVolumeEntry*, TVolumeEntry*, TVolumeEntry, CKeyPairCompareLess<CCharString, C3DMeshVolume>);

}