// std::__linear_insert(pair<unsigned long, C3DMeshVolume>* first, pair<unsigned long, C3DMeshVolume>* last, pair<unsigned long, C3DMeshVolume> val, CKeyPairCompareLess comp) -- insertion-sort step: a value smaller than the first entry shifts the run up one slot and goes first, anything else goes through the unguarded insert; the by-value val is destroyed
// Leaf: __linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A34820, 179 bytes, `ret 14h` (__fastcall: first in ecx, last in edx, the
//   16-byte value and the empty comparator on the stack).
// Identity: called by __insertion_sort 0x00A34D10; the catalog's "Deque_Insert_At_WithRefCount" is descriptive. Callees:
//   __copy_backward(first, last, result, const random_access_iterator_tag&, int*) @ 0x00A33050, the volume assignment
//   @ 0x00A32960, CCharString::CCharString(const CCharString&) @ 0x0099EC30, __unguarded_linear_insert @ 0x00A33250, the
//   count block's DeleteFunc, operator delete @ 0x00BFE9BC, CCharString::~CCharString @ 0x0099EAE0.
// Element: the 16-byte C3DMeshStats volume-table entry pair<unsigned long, C3DMeshVolume> (Ego_r.pdb C3DMeshVolume, 12 bytes:
//   CCharString Name +0, CCountedPointer<CContainmentVolume> ConvexVolume +4; the landed swap 0x00A331B0 names it) -- the
//   model of the landed 0x00A333A0 / 0x00A34420 (copy = key, string copy call, the pointer's two words and add-ref; the
//   by-value parameter's destruction = the pointer's release, its zero stores, the string's destructor); ordered by the
//   unsigned key (`jae`/`jb`).
// Shape: the landed 0x00A33780 / 0x00A33800 template: copy_backward inlined down to __copy_backward with the user-constructed
//   iterator tag homed in the dead comparator slot and a null distance; `*first = val` is the key store plus the volume's
//   out-of-line assignment; the else arm rebuilds val in the argument area (pair copy constructor) after pushing comp;
//   val's destructor closes both paths.
void __cdecl operator delete(void* p);                  // 0x00BFE9BC

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

class CCPPointerInfo { public:
    long  RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4, Data in ecx
    void* Data;                                         // +8
};

template <class T> class CCountedPointer { public:
    T*              Data;                               // +0
    CCPPointerInfo* Info;                               // +4
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

class C3DMeshVolume { public:                                        // 12 bytes (Ego_r.pdb)
    CCharString                         Name;                   // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;           // +0x04
    C3DMeshVolume& operator=(const C3DMeshVolume& other)        // out of line @ 0x00A32960 where not inlined
    {
        Name = other.Name;
        ConvexVolume = other.ConvexVolume;
        return *this;
    }
};

namespace std {

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& p) : first(p.first), second(p.second) { }
};

typedef pair<unsigned long, C3DMeshVolume> TEntry;      // 0x10 bytes

struct CKeyPairCompareLess {                            // CKeyPairCompareLess<unsigned long, ...>
    bool operator()(const TEntry& a, const TEntry& b) const { return a.first < b.first; }
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

}

namespace std {

template <class BidirectionalIter1, class BidirectionalIter2, class Distance>
BidirectionalIter2 __fastcall __copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result,
                                              const random_access_iterator_tag&, Distance*);   // 0x00A33050

template <class RandomAccessIter, class T, class Compare>
void __fastcall __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp);   // 0x00A33250

template <class BidirectionalIter1, class BidirectionalIter2>
inline BidirectionalIter2 copy_backward(BidirectionalIter1 first, BidirectionalIter1 last, BidirectionalIter2 result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

template <class RandomAccessIter, class T, class Compare>
void __fastcall __linear_insert(RandomAccessIter first, RandomAccessIter last, T val, Compare comp)
{
    if (comp(val, *first)) {
        copy_backward(first, last, last + 1);
        *first = val;
    }
    else
        __unguarded_linear_insert(last, val, comp);
}

template void __fastcall __linear_insert<TEntry*, TEntry, CKeyPairCompareLess>(TEntry*, TEntry*, TEntry, CKeyPairCompareLess);

}