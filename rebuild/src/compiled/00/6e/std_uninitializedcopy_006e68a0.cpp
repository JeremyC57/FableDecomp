// std::__uninitialized_copy(const CConversationLine* first, const CConversationLine* last, CConversationLine* result, const __false_type&) -- CConversationLine elements: two speaker words, the line string, the subtitle flag, return the end
// Leaf: ?__uninitialized_copy@std@@YIPAUCConversationLine@FableCopy_006e68a0@@PBU23@0PAU23@ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006E68A0, length 66 bytes, `ret 8`.
// Element: CConversationLine (Ego_r.pdb, 16 bytes: PersonSpeaking +0, PersonSpokenTo +4, LineToSay (CCharString) +8,
// ShowSubtitle (bool) +0xC -- the only 16-byte PDB record of this shape). Implicit copy constructor: the two words
// by register, the out-of-line string copy, the flag byte.
// Callers: 0x006E6A30 (vector copy).
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Byte twin: 0x006E65A0 (separate COMDAT copy; distinct element namespace keeps one symbol per address).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` from a /Gr TU (`__fastcall`: first
// in ecx, last in edx, result and the empty tag reference on the stack, `ret 8`): `for (; first != last; ++first,
// ++cur) _Construct(&*cur, *first);` -- a bottom-tested loop behind the `first == last` test, the placement new
// null-tests the destination, the advanced result is returned in eax.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the `ret 8`, the element stride and the placement null test name this body.
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
namespace FableCopy_006e68a0 {
struct CCharString {                                    // 4 bytes: the shared string-data pointer
    void* m_pData;
    CCharString(const CCharString& other);              // 0x0099EC30 (out of line)
};
struct CConversationLine {                              // Ego_r.pdb, 16 bytes
    long        PersonSpeaking;                         // +0x00
    long        PersonSpokenTo;                         // +0x04
    CCharString LineToSay;                              // +0x08
    bool        ShowSubtitle;                           // +0x0C
};
}
namespace std {
FableCopy_006e68a0::CConversationLine* __fastcall __uninitialized_copy(const FableCopy_006e68a0::CConversationLine* first, const FableCopy_006e68a0::CConversationLine* last,
    FableCopy_006e68a0::CConversationLine* result, const __false_type&)
{
    FableCopy_006e68a0::CConversationLine* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) FableCopy_006e68a0::CConversationLine(*first);
    return cur;
}
}