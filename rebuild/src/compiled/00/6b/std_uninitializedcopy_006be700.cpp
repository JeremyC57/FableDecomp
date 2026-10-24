// std::__uninitialized_copy(const SMusicEntry* first, const SMusicEntry* last, SMusicEntry* result, const __false_type&) -- pair<long, CMusicManager::CRegisteredMusicEntry> elements: key, two wide-string copies, gain/offset, flag
// Leaf: ?__uninitialized_copy@std@@YIPAU?$pair@JUCRegisteredMusicEntry@FableCopy_006be700@@@FableCopy_006be700@@PBU23@0PAU23@ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BE700, length 111 bytes, `ret 8`.
// Element: pair<long, CMusicManager::CRegisteredMusicEntry> (24 bytes; Ego_r.pdb CRegisteredMusicEntry: SrcFileName
// +0, DstFileName +4 (CWideString), Gain +8, Offset +0xC (float), CacheToHD +0x10), the element of the landed
// vector<pair<long, CRegisteredMusicEntry> >::_M_insert_overflow 0x006BF1A0 (its only caller). STLport's pair copy
// constructor copies the key, then the entry's implicit copy: two out-of-line CWideString copies, the two floats
// as dwords and the flag. With ebx/ebp/esi/edi all busy (first, cur, and the second pointer pair at +0xC) `last`
// lives in the `push ecx` frame word and is reloaded before the loop test (`mov edx,[esp+10h]`).
// Callees: CWideString::CWideString(const CWideString&) @ 0x0099B720.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` from a /Gr TU (`__fastcall`: first
// in ecx, last in edx, result and the empty tag reference on the stack, `ret 8`): `for (; first != last; ++first,
// ++cur) _Construct(&*cur, *first);` -- a bottom-tested loop behind the `first == last` test, the placement new
// null-tests the destination, the advanced result is returned in eax.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the `ret 8`, the element stride and the placement null test name this body.
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
namespace FableCopy_006be700 {
struct CWideString {                                    // 4 bytes: the wide string-data pointer
    void* PData;
    CWideString(const CWideString& other);              // 0x0099B720 (out of line)
};
struct CRegisteredMusicEntry {                          // Ego_r.pdb CMusicManager::CRegisteredMusicEntry, 20 bytes
    CWideString SrcFileName;                            // +0x00
    CWideString DstFileName;                            // +0x04
    float       Gain;                                   // +0x08
    float       Offset;                                 // +0x0C
    bool        CacheToHD;                              // +0x10
};
template <class T1, class T2>
struct pair {                                           // STLport pair: explicit memberwise copy constructor
    T1 first;
    T2 second;
    pair(const pair& other) : first(other.first), second(other.second) { }
};
typedef pair<long, CRegisteredMusicEntry> SMusicEntry;
}
namespace std {
FableCopy_006be700::SMusicEntry* __fastcall __uninitialized_copy(const FableCopy_006be700::SMusicEntry* first, const FableCopy_006be700::SMusicEntry* last,
    FableCopy_006be700::SMusicEntry* result, const __false_type&)
{
    FableCopy_006be700::SMusicEntry* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) FableCopy_006be700::SMusicEntry(*first);
    return cur;
}
}