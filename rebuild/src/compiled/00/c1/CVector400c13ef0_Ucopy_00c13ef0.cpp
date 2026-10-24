// CVector4_00c13ef0::_Ucopy(unsigned long* first, unsigned long* last, unsigned long* ptr) -- VC7-era Dinkumware vector<4-byte scalar>::_Ucopy: memmove [first, last) to the raw storage at ptr and return ptr advanced by the element count
// Leaf: _Ucopy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C13EF0, length 43 bytes, `ret 0Ch`.
// Callers: the second vector's _Insert_n @ 0x00C14270 (calls 0x00C143D8 / 0x00C14424), called from the editor TU
//   (CEditControlCentre 0x00C26CD0, the 0x00C14490 buffer family). Byte-identical to its neighbour 0x00C13EC0: the same
//   member template instantiated for another 4-byte element type of the same TU.
// Callees: memmove (MSVCR71 import, IAT 0x01440144).
// Shape: Dinkumware `template<class _Iter> pointer _Ucopy(_Iter _First, _Iter _Last, pointer _Ptr)` is a thiscall member
//   that never reads `this` (three stack arguments, `ret 0Ch`); `_Uninitialized_copy` on a scalar pointer range reduces
//   to `_Uninit_copy(..., _Scalar_ptr_iterator_tag)`: `count = _Last - _First; return (T*)memmove(_Ptr, _First, count *
//   sizeof(T)) + count;` -- the byte count is computed once into esi (`sar 2 ; lea esi,[eax*4]`) and re-added to
//   memmove's result.
// Layout: the VC7 vector (allocator word +0, first +4, last +8, end +0xC; see the landed 0x00C29B30 / 0x00C14490); the
//   4-byte element type is not identifiable from this body.
// Label correction: the catalog's `_Umove<NGameText::CDataBankEntry**>` is the FableWin (VC8) name of the byte-identical
//   NGameText instance; this one sits in the editor TU, and the VC7.1 member is `_Ucopy`.
typedef unsigned int size_t;
extern "C" __declspec(dllimport) void* __cdecl memmove(void* dest, const void* src, size_t count);   // IAT 0x01440144 (MSVCR71)

class CVector4_00c13ef0 {
public:
    long           _Alval;                     // +0x0 (the empty allocator padded to a word)
    unsigned long* _Myfirst;                   // +0x4
    unsigned long* _Mylast;                    // +0x8
    unsigned long* _Myend;                     // +0xC
    unsigned long* _Ucopy(unsigned long* first, unsigned long* last, unsigned long* ptr);
};

unsigned long* CVector4_00c13ef0::_Ucopy(unsigned long* first, unsigned long* last, unsigned long* ptr)
{
    size_t count = (size_t)(last - first);
    return (unsigned long*)memmove(ptr, first, count * sizeof(*first)) + count;
}