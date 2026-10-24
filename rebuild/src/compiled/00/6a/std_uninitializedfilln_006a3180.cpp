// std::__uninitialized_fill_n(CThingGraphicArgPasser* first, unsigned int n, const CThingGraphicArgPasser& x, const __false_type&) -- STLport fill-construct n screen-effect thing records (intelligent thing pointer + bypass flag) from x
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A3180, length 68 bytes (+ int3 padding), `ret 8`.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, ::GetPItem @ 0x00A01B50, ::SetPItem
//   @ 0x00A01B90; CIntelligentPointer<CThing> vtable 0x01238C6C.
// Identity: the catalog's `_Fill_n<unsigned long*,unsigned int,unsigned long>` is a propagated Dinkumware name; the
//   9-byte stride (`add esi,9`), the placement null test, the unsigned count and `ret 8` (x + the __false_type tag
//   reference) are STLport 4.x `__uninitialized_fill_n` over NScreenEffect::CScreenEffectPriorityStack::
//   CThingGraphicArgPasser -- the record of the landed __uninitialized_copy 0x006A3130 (same TU, ctor 0x006A2F00).
// Element: CThingGraphicArgPasser (byte-packed, 9 bytes: CIntelligentPointer<CThing> thing +0, bool bypassSet +8);
//   its implicit copy runs CIntelligentPointer's copy constructor (base ctor, vtable store, Get/SetPItem) then the
//   flag byte (`mov al,[edi+8] ; mov [esi+8],al`).
// Shape: /Gr TU (__fastcall: first ecx, n edx); the loop head is 16-byte aligned (the `nop`).
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
namespace FableFill_006a3180 {
#pragma pack(push, 1)
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
struct CThingGraphicArgPasser {                         // 9 bytes (byte-packed)
    CIntelligentPointer<CThing> thing;                  // +0x00
    bool                        bypassSet;              // +0x08
};
#pragma pack(pop)
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
template FableFill_006a3180::CThingGraphicArgPasser* __fastcall __uninitialized_fill_n<FableFill_006a3180::CThingGraphicArgPasser*, unsigned int, FableFill_006a3180::CThingGraphicArgPasser>(
    FableFill_006a3180::CThingGraphicArgPasser*, unsigned int, const FableFill_006a3180::CThingGraphicArgPasser&, const __false_type&);
}