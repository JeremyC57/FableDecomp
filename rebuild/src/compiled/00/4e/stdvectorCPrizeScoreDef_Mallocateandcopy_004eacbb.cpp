// std::vector<CPrizeScoreDef>::_M_allocate_and_copy(unsigned int n, const CPrizeScoreDef* first, const CPrizeScoreDef* last) -- allocate n elements and copy-construct [first, last) into them
// Leaf: ?_M_allocate_and_copy@?$vector@VCPrizeScoreDef@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EACBB, 71 bytes, thiscall `ret 0Ch`, returns the new block.
// Identity (probable): its _Construct 0x004EAC30 is bound ??$_Construct@VCPrizeScoreDef@@V1@@std@@... (0x30 stride) and its caller is the landed vector<CPrizeScoreDef>::operator= 0x004EAAD0.
// Callees: malloc 0x00BFEA0E (allocate: `n ? malloc(n * 0x30) : 0`), _Construct(T*, const T&) 0x004EAC30 (__fastcall, per element).
// Shape (family K123, representative 0x004E9C63): STLport 4.6 `_M_allocate_and_copy` with allocate and __uninitialized_copy inline
//   (size TU: both __forceinline) and the element _Construct out of line; the vector model is the landed operator= rows' one
//   (one template parameter, matching the bound names).
#pragma optimize("s", on)
extern "C" void* __cdecl malloc(unsigned int bytes);                                   // 0x00BFEA0E
class CPrizeScoreDef {                                   // retail 0x30 bytes (Ego_r.pdb); opaque here
public:
    unsigned char _data[0x30];
};
namespace std {
struct __false_type { };
template <class T> void __fastcall _Construct(T* p, const T& val);                     // 0x004EAC30
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
template CPrizeScoreDef* vector<CPrizeScoreDef>::_M_allocate_and_copy(unsigned int, const CPrizeScoreDef*, const CPrizeScoreDef*);
}