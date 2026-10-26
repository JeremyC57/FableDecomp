// CThingComponentSet::HasDriver() -- whether any entry names a driver component (an SGI find_if over the entries)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00870980, 62 bytes (+ int3 padding).
// Callees: __find_if<CEntry*, CIsDriverEntry>(first, last, pred, const random_access_iterator_tag&) @ 0x00870530
//          (the SGI algorithm TU is /Gr: first in ecx, last in edx, the by-value predicate and the tag reference on
//          the stack; it is unrolled by four; manifest "CWorldDriver_FindClosestMeshVolume" is propagated).
// Shape: the inline SGI find_if front end evaluates the iterators first (before the stack arguments); the empty predicate is value-initialised (`mov byte ptr [..],0` then its dword pushed), the tag is an empty
//   class with a user constructor bound to the const reference (its frame byte left untouched); the result is compared
//   with end() re-computed after the call (a separate statement: in one expression the end is CSE'd with the
//   argument) and returned through `return a != b;` (`xor edx,edx ; setne dl ; mov al,dl`).
#pragma pack(push, 1)
class CDefString { public:
    long TablePos;
    CDefString() : TablePos(-1) { }
};

struct CEntry {                         // CThingComponentSet::CEntry, 9 bytes (PDB: packed)
    CDefString Name;                    // +0x00
    unsigned long DriverType;           // +0x04
    bool Override;                      // +0x08
    CEntry() : DriverType(0), Override(false) { }
};
#pragma pack(pop)

struct random_access_iterator_tag {
    random_access_iterator_tag() { }
};

struct CIsDriverEntry {                 // empty predicate (the def lookup is inlined in the find)
    bool operator()(const CEntry& entry) const;
};

CEntry* __fastcall find_if_entries(CEntry* first, CEntry* last, CIsDriverEntry pred,
                                   const random_access_iterator_tag& tag);     // 0x00870530

inline CEntry* find_if(CEntry* first, CEntry* last, CIsDriverEntry pred)    // SGI front end: adds the category tag
{
    return find_if_entries(first, last, pred, random_access_iterator_tag());
}

template<class T, int N> class CSmallVector { public:
    T* Start;                           // +0x00
    signed char Size;                   // +0x04
    T* begin() const { return Start; }
    T* end() const { return Start + Size; }
};

class CThingComponentSet { public:
    CSmallVector<CEntry, 8> Entries;    // +0x00
    bool HasDriver() const;
};

bool CThingComponentSet::HasDriver() const
{
    CEntry* found = find_if(Entries.begin(), Entries.end(), CIsDriverEntry());
    return found != Entries.end();
}