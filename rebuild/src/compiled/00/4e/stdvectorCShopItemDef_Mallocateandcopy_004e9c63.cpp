// std::vector<CShopItemDef>::_M_allocate_and_copy(unsigned int n, const CShopItemDef* first, const CShopItemDef* last) -- allocate n elements and copy-construct [first, last) into them
// Leaf: ?_M_allocate_and_copy@?$vector@VCShopItemDef@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E9C63, 71 bytes, thiscall `ret 0Ch`, returns the new block.
// Identity (proven): reloc_bindings ?_M_allocate_and_copy@?$vector@VCShopItemDef@@@std@@QAEPAVCShopItemDef@@IPBV3@0@Z; its _Construct 0x004E9C39 is bound ??$_Construct@VCShopItemDef@@@std@@ and its caller is the landed vector<CShopItemDef>::operator= 0x004E9AD3.
// Callees: malloc 0x00BFEA0E (allocate: `n ? malloc(n * 0x48) : 0`), _Construct(T*, const T&) 0x004E9C39 (__fastcall, per element).
// Shape (family K123, representative 0x004E9C63): STLport 4.6 `_M_allocate_and_copy` with allocate and __uninitialized_copy inline
//   (size TU: both __forceinline) and the element _Construct out of line; the vector model is the landed operator= rows' one
//   (one template parameter, matching the bound names).
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int bytes);                                   // 0x00BFEA0E
#include "engine/CShopItemDef.h"
namespace std {
struct __false_type { };
template <class T> void __fastcall _Construct(T* p, const T& val);                     // 0x004E9C39
template <class T> __forceinline T* __uninitialized_copy(const T* first, const T* last, T* result, const __false_type&)
{
    T* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(cur, *first);
    return cur;
}
template <class T> class vector {
public:
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    static __forceinline T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    T* _M_allocate_and_copy(unsigned int n, const T* first, const T* last)
    {
        T* result = allocate(n);
        __uninitialized_copy(first, last, result, __false_type());
        return result;
    }
};
template CShopItemDef* vector<CShopItemDef>::_M_allocate_and_copy(unsigned int, const CShopItemDef*, const CShopItemDef*);
}