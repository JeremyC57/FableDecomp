// std::__uninitialized_fill_n(SThingEntry* first, unsigned int n, const SThingEntry& x, const __false_type&) -- STLport fill-construct n (long, CIntelligentPointer<CThing>) pairs from x; returns the end
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A3220, length 74 bytes, `ret 8`.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, ::GetPItem @ 0x00A01B50, ::SetPItem
//   @ 0x00A01B90; CIntelligentPointer<CThing> vtable 0x01238C6C.
// Identity: the catalog's "Vector_CopyConstructor" is a descriptive auto label; the `ret 8` (x and the empty
//   __false_type tag reference), the unsigned count (`jbe`), the 12-byte stride and the placement null test are
//   STLport 4.x `__uninitialized_fill_n(__first, __n, __x, const __false_type&)`: `for (; __n > 0; --__n, ++__cur)
//   _Construct(&*__cur, __x); return __cur;`.
// Element: pair<long, CIntelligentPointer<CThing> > with STLport's declared memberwise copy constructor (key,
//   then CIntelligentPointer's copy constructor: base ctor, vtable store, SetPItem(other.GetPItem())) -- the landed
//   __uninitialized_copy 0x006A31D0 model, including the second destination pointer for the pointer member.
// Shape: /Gr TU (__fastcall: first ecx, n edx).
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
namespace FableFill_006a3220 {
class CThing;
class CBaseObject;
struct CBaseIntelligentPointer {                        // 8 bytes: vtable + item
    void* PData;                                        // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;                      // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template <class T>
struct CIntelligentPointer : CBaseIntelligentPointer {  // vtable 0x01238C6C for CIntelligentPointer<CThing>
    __forceinline CIntelligentPointer(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};
template <class T1, class T2>
struct pair {                                           // STLport pair: explicit memberwise copy constructor
    T1 first;
    T2 second;
    pair(const pair& other) : first(other.first), second(other.second) { }
};
typedef pair<long, CIntelligentPointer<CThing> > SThingEntry;
}
namespace std {
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        new ((void*)&*__cur) _Tp(__x);
    return __cur;
}
template FableFill_006a3220::SThingEntry* __fastcall __uninitialized_fill_n<FableFill_006a3220::SThingEntry*, unsigned int, FableFill_006a3220::SThingEntry>(
    FableFill_006a3220::SThingEntry*, unsigned int, const FableFill_006a3220::SThingEntry&, const __false_type&);
}