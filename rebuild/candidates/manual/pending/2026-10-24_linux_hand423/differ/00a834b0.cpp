// _STL::__linear_insert(SConnectorEntry* first, SConnectorEntry* last, SConnectorEntry value, CKeyPairCompareLess comp) -- SGI insertion-sort step over the connector map's (long id, counted node) pairs: a value below the front shifts the run up one slot and lands at the front, otherwise the unguarded backward scan places it
// Grade: DIFFER(111v113) -- register allocation only: retail keeps `first` in esi and loads value's count block per branch (after the pair operator= call in the copy_backward arm, before the copy in the scan arm); ours hoists that load to the entry (esi) and puts `first` in edi (extra push/pop edi)
// Leaf: __linear_insert
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A834B0, 113 bytes, `ret 10h`: /Gr `__fastcall` (first in ecx -> esi, last
//   in edx; the 12-byte value and the empty comparator on the stack).
// Identity: the only caller 0x00A83D57 is the __insertion_sort of CVectorMap<long, CCountedPointer<CNavConnectorNode> >
//   (the CNavQuadTree::ConnectorNodes map, see DeallocateMemory 0x00A83FD0); the callees are the landed
//   __copy_backward 0x00A802A0 and pair operator= 0x00A7FAF0 of the same 12-byte pair. The twin of the line-list map's
//   __linear_insert 0x00A843F0. The catalog's "Vector_InsertOrErase_CountedPointer" is descriptive.
// Callees: __copy_backward(first, last, result, random_access_iterator_tag, int*) 0x00A802A0, pair::operator= 0x00A7FAF0,
//   __unguarded_linear_insert(last, value, comp) 0x00A7FF40 (`__fastcall`), operator delete 0x00BFE9BC.
// Shape: SGI `if (comp(val, *first)) { copy_backward(first, last, last + 1); *first = val; } else
//   __unguarded_linear_insert(last, val, comp);` -- the signed key compare inline, copy_backward's empty tag homed in the
//   dead comparator slot, the pair assignment out of line (a same-TU body: retail keeps `value`'s count block in esi
//   across the unguarded-scan call, so `value` is not address-exposed), the by-value copy for the unguarded scan built inline in its argument slot (key, object, block, count
//   bump), then `value`'s counted pointer released on both exits (`if (Info && --Info->RefCount == 0)`: the decrement's
//   flags, the destroy hook, operator delete).
void __cdecl operator delete(void* block);              // 0x00BFE9BC

struct CCPPointerInfo {
    long  RefCount;                                     // +0x00
    void (__fastcall* DeleteFunc)(void* data);          // +0x04
    void* Data;                                         // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
    CCountedPointer(const CCountedPointer& p) : Data(p.Data), Info(p.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        if (Info && --Info->RefCount == 0) {
            Info->DeleteFunc(Info->Data);
            operator delete(Info);
        }
    }
};

class CNavConnectorNode;

struct SConnectorEntry {                                // pair<long, CCountedPointer<CNavConnectorNode> >, 12 bytes
    long                               first;           // +0x00
    CCountedPointer<CNavConnectorNode> second;          // +0x04
    SConnectorEntry(const SConnectorEntry& p) : first(p.first), second(p.second) { }
    SConnectorEntry& operator=(const SConnectorEntry& p);   // 0x00A7FAF0 (same-TU body below)
};

// Same-TU stand-in for the pair assignment 0x00A7FAF0 (retail compiled it in this TU; this is its landed shape): with a
// declaration only, VC7.1 treats `value` as address-exposed for the whole function and re-reads its count block around
// every call and decrement; with a visible body that does not let the address escape it keeps the block in esi.
#pragma auto_inline(off)
__declspec(noinline) SConnectorEntry& SConnectorEntry::operator=(const SConnectorEntry& p)
{
    first = p.first;
    CCPPointerInfo* info = p.second.Info;
    CNavConnectorNode* data = p.second.Data;
    if (second.Info != info) {
        if (second.Info && --second.Info->RefCount == 0) {
            second.Info->DeleteFunc(second.Info->Data);
            operator delete(second.Info);
        }
        second.Data = data;
        second.Info = info;
        if (info)
            ++info->RefCount;
    }
    return *this;
}
#pragma auto_inline(on)

struct CKeyPairCompareLess {                            // empty, passed by value
    bool operator()(const SConnectorEntry& a, const SConnectorEntry& b) const { return a.first < b.first; }
};

struct random_access_iterator_tag { random_access_iterator_tag() { } };

SConnectorEntry* __fastcall __copy_backward(SConnectorEntry* first, SConnectorEntry* last, SConnectorEntry* result,
                                            const random_access_iterator_tag&, int*);              // 0x00A802A0
void __fastcall __unguarded_linear_insert(SConnectorEntry* last, SConnectorEntry value, CKeyPairCompareLess comp);   // 0x00A7FF40

inline SConnectorEntry* copy_backward(SConnectorEntry* first, SConnectorEntry* last, SConnectorEntry* result)
{
    return __copy_backward(first, last, result, random_access_iterator_tag(), (int*)0);
}

void __fastcall __linear_insert(SConnectorEntry* first, SConnectorEntry* last, SConnectorEntry value, CKeyPairCompareLess comp)
{
    if (comp(value, *first)) {
        copy_backward(first, last, last + 1);
        *first = value;
    } else
        __unguarded_linear_insert(last, value, comp);
}
