// _STL::__insertion_sort(pair<unsigned long, C3DMeshVolume>* first, pair<unsigned long, C3DMeshVolume>* last, CKeyPairCompareLess comp) -- straight insertion sort of the mesh-volume table: hand each entry after the first by value to __linear_insert
// Leaf: __insertion_sort
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A34D10, 109 bytes (+ int3 padding), `ret 4` (__fastcall: first in ecx, last
//   in edx, the empty comparator on the stack).
// Identity: the __final_insertion_sort head of the 16-byte C3DMeshStats sort; the catalog's "Vector_Destruct_Range" is
//   descriptive. Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30, __linear_insert(first, i, val, comp)
//   @ 0x00A34820 (it owns and destroys val).
// Element: the 16-byte C3DMeshStats volume-table entry pair<unsigned long, C3DMeshVolume> (Ego_r.pdb C3DMeshVolume, 12 bytes:
//   CCharString Name +0, CCountedPointer<CContainmentVolume> ConvexVolume +4; the landed swap 0x00A331B0 names it) -- the
//   model of the landed 0x00A333A0 / 0x00A34420 (copy = key, string copy call, the pointer's two words and add-ref; the
//   by-value parameter's destruction = the pointer's release, its zero stores, the string's destructor); ordered by the
//   unsigned key (`jae`/`jb`).
// Shape: SGI `if (first == last) return; for (i = first + 1; i != last; ++i) __linear_insert(first, i, T(*i), comp);` --
//   first and last spilled to the frame, comp pushed before the pair copy is built in the argument area.
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
void __fastcall __linear_insert(RandomAccessIter first, RandomAccessIter last, T val, Compare comp);   // 0x00A34820

template <class RandomAccessIter, class Compare>
void __fastcall __insertion_sort(RandomAccessIter first, RandomAccessIter last, Compare comp)
{
    if (first == last)
        return;
    for (RandomAccessIter i = first + 1; i != last; ++i)
        __linear_insert(first, i, TEntry(*i), comp);
}

template void __fastcall __insertion_sort<TEntry*, CKeyPairCompareLess>(TEntry*, TEntry*, CKeyPairCompareLess);

}