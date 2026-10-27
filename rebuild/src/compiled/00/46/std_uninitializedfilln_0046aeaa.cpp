// std::uninitialized_fill_n(CSoundBankEntry* __first, unsigned int __n, const CSoundBankEntry& __x) -- construct n copies of x in raw slots, return the end
// Leaf: ??$uninitialized_fill_n@PAVCSoundBankEntry@@IV1@@std@@YIPAVCSoundBankEntry@@PAV1@IABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046AEAA, 34 bytes, `__fastcall` (first ecx, n edx, x on the stack), `ret 4` (size-optimised module).
// Identity (proven): its only caller is the landed vector<CSoundBankEntry>::_M_fill_insert 0x0046AD59; 20-byte stride = Ego_r.pdb CSoundBankEntry.
// Shape (family M055, representative 0x0046AEAA): STLport `return __uninitialized_fill_n(__first, __n, __x, __false_type());`
//   with the loop `for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x);` inline (`__forceinline`); unsigned n gives `jbe`.
#pragma optimize("s", on)
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
class CDefString { public: long TablePos; };   // Ego_r.pdb { long TablePos; }
class CSoundBankEntry { public: unsigned long ID; CDefString Filename; CDefString SymbolFilename; CDefString LipSyncBankHandle; bool DVD; };   // Ego_r.pdb, 20 bytes
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> __forceinline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _ForwardIter, class _Size, class _Tp>
__forceinline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{
    return __uninitialized_fill_n(__first, __n, __x, __false_type());
}
template CSoundBankEntry* __fastcall uninitialized_fill_n<CSoundBankEntry*, unsigned int, CSoundBankEntry>(CSoundBankEntry*, unsigned int, const CSoundBankEntry&);
}