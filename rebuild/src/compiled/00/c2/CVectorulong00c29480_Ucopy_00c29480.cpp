// CVector_ulong_00c29480::_Ucopy(unsigned long* first, unsigned long* last, unsigned long* ptr) -- VC7-era Dinkumware vector<4-byte scalar>::_Ucopy: memmove [first, last) to the raw storage at ptr and return ptr advanced by the element count
// Leaf: _Ucopy
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C29480, length 43 bytes (+ int3 padding), `ret 0Ch`.
// Callers: the vector<unsigned long> _Insert_n @ 0x00C295B0 (calls 0x00C29718 / 0x00C29764), the InsertRange of the landed
//   CVector_ulong_00c29b30::push_back @ 0x00C29B30 (its _Ufill neighbour 0x00C29840 carries the PDB label
//   `vector<unsigned int>::_Ufill`).
// Callees: memmove (MSVCR71 import, IAT 0x01440144).
// Shape: Dinkumware `template<class _Iter> pointer _Ucopy(_Iter _First, _Iter _Last, pointer _Ptr)` is a thiscall member
//   that never reads `this` (three stack arguments, `ret 0Ch`); `_Uninitialized_copy` on a scalar pointer range reduces
//   to `_Uninit_copy(..., _Scalar_ptr_iterator_tag)`: `count = _Last - _First; return (T*)memmove(_Ptr, _First, count *
//   sizeof(T)) + count;` -- the byte count is computed once into esi (`sar 2 ; lea esi,[eax*4]`) and re-added to
//   memmove's result.
// Layout: the VC7 vector<unsigned long> of the landed 0x00C29B30 (allocator word +0, first +4, last +8, end +0xC).
// Label correction: the catalog's `_Umove<NGameText::CDataBankEntry**>` is the FableWin (VC8) name of the byte-identical
//   NGameText instance; this one is the vector<unsigned long> of the 0x00C29B30 TU, and the VC7.1 member is `_Ucopy`.
typedef unsigned int size_t;
extern "C" __declspec(dllimport) void* __cdecl memmove(void* dest, const void* src, size_t count);   // IAT 0x01440144 (MSVCR71)

class CVector_ulong_00c29480 {
public:
    long           _Alval;                     // +0x0 (the empty allocator padded to a word)
    unsigned long* _Myfirst;                   // +0x4
    unsigned long* _Mylast;                    // +0x8
    unsigned long* _Myend;                     // +0xC
    unsigned long* _Ucopy(unsigned long* first, unsigned long* last, unsigned long* ptr);
};

unsigned long* CVector_ulong_00c29480::_Ucopy(unsigned long* first, unsigned long* last, unsigned long* ptr)
{
    size_t count = (size_t)(last - first);
    return (unsigned long*)memmove(ptr, first, count * sizeof(*first)) + count;
}