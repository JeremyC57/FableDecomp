// std::_Sort_heap(std::pair<float,CCharString>* first, std::pair<float,CCharString>* last, CKeyPairCompareLess<float,CCharString> pred) -- order a heap of (score, name) pairs by repeatedly popping the front to the back
// Flags: /Gr
// Leaf: _Sort_heap
// Retail: Fable.exe @ 0x004D1600, 160 bytes (/O2). Callees: STLport __adjust_heap<pair<float,CCharString>*,
// int,pair<float,CCharString>,CKeyPairCompareLess<float,CCharString>> 0x004D0590 (`__fastcall`: first ecx,
// hole edx, bottom + the 8-byte value + the comparator on the stack), CCharString copy ctor 0x0099EC30,
// operator= 0x0099EFB0, dtor 0x0099EAE0.
// Shape: STLport `sort_heap` (the 0x00777EE0 source): `for (; 1 < _Last - _First; --_Last)` popping
// through the inline __pop_heap_aux -> __pop_heap with __adjust_heap out of line from hole 0. std::pair's implicit
// members: the popped value `_Ty(*(_Last - 1))` is a frame local (float by dword, CCharString copy ctor),
// `*_Dest = *_First` is the float store + CCharString::operator=, the by-value argument is built in the
// argument area (float + CCharString copy ctor) and the local's string is destroyed after the call. The 8-byte
// pair local gives VC7.1's `and esp,-8` frame.
class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;                         // +0x0
    CCharString(const CCharString& other);                // 0x0099EC30
    ~CCharString();                                       // 0x0099EAE0
    CCharString& operator=(const CCharString& other);     // 0x0099EFB0
};
template <class K, class V> class CKeyPairCompareLess { public: };
namespace std {
template <class _Ty1, class _Ty2> struct pair {
    _Ty1 first;
    _Ty2 second;
};
template <class _RanIt, class _Diff, class _Ty, class _Pr>
void __fastcall _Adjust_heap(_RanIt _First, _Diff _Hole, _Diff _Bottom, _Ty _Val, _Pr _Pred);   // 0x004D0590
template <class _RanIt, class _Diff, class _Ty, class _Pr> __forceinline
void _Pop_heap(_RanIt _First, _RanIt _Last, _RanIt _Dest, _Ty _Val, _Pr _Pred, _Diff*)
{
    *_Dest = *_First;
    _Adjust_heap(_First, _Diff(0), _Diff(_Last - _First), _Val, _Pred);
}
template <class _RanIt, class _Ty, class _Pr> __forceinline
void _Pop_heap_0(_RanIt _First, _RanIt _Last, _Pr _Pred, _Ty*)
{
    _Pop_heap(_First, _Last - 1, _Last - 1, _Ty(*(_Last - 1)), _Pred, (int*)0);
}
template <class _RanIt, class _Pr>
void __fastcall _Sort_heap(_RanIt _First, _RanIt _Last, _Pr _Pred)
{
    for (; 1 < _Last - _First; --_Last)
        _Pop_heap_0(_First, _Last, _Pred, _First);
}
typedef pair<float, CCharString> Elem;
template void __fastcall _Sort_heap<Elem*, CKeyPairCompareLess<float, CCharString> >(Elem*, Elem*, CKeyPairCompareLess<float, CCharString>);
}