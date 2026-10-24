// _STL::__unguarded_insertion_sort_aux(pair<unsigned long, C3DMeshVolume>* first, pair<unsigned long, C3DMeshVolume>* last, pair<unsigned long, C3DMeshVolume>*, CKeyPairCompareLess comp) -- insertion-sort pass with no lower-bound test over the mesh-volume table, each entry handed by value to __unguarded_linear_insert
// Leaf: __unguarded_insertion_sort_aux
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A33870, 79 bytes (+ int3 padding), `ret 8` (__fastcall: first in ecx, last
//   in edx, the value-type tag pointer and the empty comparator on the stack).
// Identity: the __final_insertion_sort tail of the 16-byte C3DMeshStats sort; the catalog's
//   "_Copy_impl<CComplexNumber*,CComplexNumber*>" is propagated. Callees: CCharString::CCharString(const CCharString&)
//   @ 0x0099EC30, __unguarded_linear_insert(last, val, comp) @ 0x00A33250 (out of line: it owns and destroys val).
// Element: the 16-byte C3DMeshStats volume-table entry pair<unsigned long, C3DMeshVolume> (Ego_r.pdb C3DMeshVolume, 12 bytes:
//   CCharString Name +0, CCountedPointer<CContainmentVolume> ConvexVolume +4; the landed swap 0x00A331B0 names it) -- the
//   model of the landed 0x00A333A0 / 0x00A34420: the volume's assignment is an inline member whose out-of-line COMDAT is
//   0x00A32960, the counted pointer's assignment is ShareData's `Info != info` test, release, two stores and add-ref;
//   ordered by the unsigned key (`jae`/`jb`).
// Shape: STLport `for (i = first; i != last; ++i) __unguarded_linear_insert(i, T(*i), comp);` -- comp pushed first, then
//   T(*i) built in the argument area by STLport's pair copy constructor (key, the string copy call, the counted pointer's
//   two words and add-ref).
void __cdecl operator delete(void* p);                  // 0x00BFE9BC

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct CCPPointerInfo {
    long  RefCount;                                     // +0
    void (__fastcall* DeleteFunc)(void* data);          // +4, Data in ecx
    void* Data;                                         // +8
};

template <class T> struct CCountedPointer {
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

struct C3DMeshVolume {                                        // 12 bytes (Ego_r.pdb)
    CCharString                         Name;                   // +0x00
    CCountedPointer<CContainmentVolume> ConvexVolume;           // +0x04
    C3DMeshVolume& operator=(const C3DMeshVolume& other)        // out of line @ 0x00A32960 where not inlined
    {
        Name = other.Name;
        ConvexVolume = other.ConvexVolume;
        return *this;
    }
};

namespace _STL {

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

namespace _STL {

template <class RandomAccessIter, class T, class Compare>
void __fastcall __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp);   // 0x00A33250

template <class RandomAccessIter, class T, class Compare>
void __fastcall __unguarded_insertion_sort_aux(RandomAccessIter first, RandomAccessIter last, T*, Compare comp)
{
    for (RandomAccessIter i = first; i != last; ++i)
        __unguarded_linear_insert(i, T(*i), comp);
}

template void __fastcall __unguarded_insertion_sort_aux<TEntry*, TEntry, CKeyPairCompareLess>(TEntry*, TEntry*, TEntry*, CKeyPairCompareLess);

}