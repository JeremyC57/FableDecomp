// std::__uninitialized_copy(const CTestQuest* first, const CTestQuest* last, CTestQuest* result, const __false_type&) -- CTestQuest elements: six string copies and the colour index, return the end
// Leaf: ?__uninitialized_copy@std@@YIPAUCTestQuest@FableCopy_00623700@@PBU23@0PAU23@ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00623700, length 112 bytes, `ret 8`.
// Element: CTestQuest (Ego_r.pdb, 28 bytes: QuestName +0, HSPName +4, ColourIndex (long) +8, DisplayName +0xC,
// IniFile +0x10, OutiFile +0x14, QuestCardDefName +0x18 -- the only 28-byte PDB record with strings at +0/+4/+0xC..+0x18),
// the element of CWorld::ActiveTestQuests. Implicit copy constructor: six out-of-line string copies and one dword.
// Callers: 0x00623AF0 (landed tree lookup TU) and the vector assign 0x00624A30 (manifest "vector_assign_questpair").
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` from a /Gr TU (`__fastcall`: first
// in ecx, last in edx, result and the empty tag reference on the stack, `ret 8`): `for (; first != last; ++first,
// ++cur) _Construct(&*cur, *first);` -- a bottom-tested loop behind the `first == last` test, the placement new
// null-tests the destination, the advanced result is returned in eax.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the `ret 8`, the element stride and the placement null test name this body.
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
namespace FableCopy_00623700 {
struct CCharString {                                    // 4 bytes: the shared string-data pointer
    void* m_pData;
    CCharString(const CCharString& other);              // 0x0099EC30 (out of line)
};
struct CTestQuest {                                     // Ego_r.pdb, 28 bytes
    CCharString QuestName;                              // +0x00
    CCharString HSPName;                                // +0x04
    long        ColourIndex;                            // +0x08
    CCharString DisplayName;                            // +0x0C
    CCharString IniFile;                                // +0x10
    CCharString OutiFile;                               // +0x14
    CCharString QuestCardDefName;                       // +0x18
};
}
namespace std {
FableCopy_00623700::CTestQuest* __fastcall __uninitialized_copy(const FableCopy_00623700::CTestQuest* first, const FableCopy_00623700::CTestQuest* last,
    FableCopy_00623700::CTestQuest* result, const __false_type&)
{
    FableCopy_00623700::CTestQuest* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) FableCopy_00623700::CTestQuest(*first);
    return cur;
}
}