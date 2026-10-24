// _STL::__unguarded_linear_insert(pair<unsigned long, C3DMeshVolume>* last, pair<unsigned long, C3DMeshVolume> val, CKeyPairCompareLess comp) -- insertion-sort inner step without a lower bound check for the mesh-volume table: shift every larger entry up one slot, drop the value into the gap, destroy the by-value copy
// Leaf: __unguarded_linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A33250, 200 bytes, `ret 14h` (__fastcall: last in ecx, the 16-byte value and
//   the empty comparator on the stack).
// Identity: called by __unguarded_insertion_sort_aux 0x00A33870 and __linear_insert 0x00A34820; the catalog's
//   "std_deque_insert" is descriptive. Callees: CCharString::operator= @ 0x0099EFB0, the record assignment @ 0x00A32960,
//   the count block's DeleteFunc, operator delete @ 0x00BFE9BC, CCharString::~CCharString @ 0x0099EAE0.
// Element: the 16-byte C3DMeshStats volume-table entry pair<unsigned long, C3DMeshVolume> (Ego_r.pdb C3DMeshVolume, 12 bytes:
//   CCharString Name +0, CCountedPointer<CContainmentVolume> ConvexVolume +4; the landed swap 0x00A331B0 names it) -- the
//   model of the landed 0x00A333A0 / 0x00A34420: the volume's assignment is an inline member whose out-of-line COMDAT is
//   0x00A32960, the counted pointer's assignment is ShareData's `Info != info` test, release, two stores and add-ref;
//   ordered by the unsigned key (`jae`/`jb`).
// Shape: SGI `next = last; --next; while (comp(val, *next)) { *last = *next; last = next; --next; } *last = val;` --
//   the loop's pair assignment inlines the record assignment (string operator= call + the counted-pointer ShareData) while
//   the final `*last = val` calls the record's out-of-line COMDAT (VC7.1 inlines the member once), then val dies: the
//   counted pointer's release (`xor esi,esi` zero, block re-read) and zero stores, and the string's destructor.
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
void __fastcall __unguarded_linear_insert(RandomAccessIter last, T val, Compare comp)
{
    RandomAccessIter next = last;
    --next;
    while (comp(val, *next)) {
        *last = *next;
        last = next;
        --next;
    }
    *last = val;
}

template void __fastcall __unguarded_linear_insert<TEntry*, TEntry, CKeyPairCompareLess>(TEntry*, TEntry, CKeyPairCompareLess);

}