// std::__uninitialized_copy(const CThingGraphicArgPasser* first, const CThingGraphicArgPasser* last, CThingGraphicArgPasser* result, const __false_type&) -- NScreenEffect::CScreenEffectPriorityStack::CThingGraphicArgPasser elements: thing pointer copy + bypass flag
// Leaf: ?__uninitialized_copy@std@@YIPAUCThingGraphicArgPasser@FableCopy_006a3130@@PBU23@0PAU23@ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A3130, length 72 bytes, `ret 8`.
// Element: CThingGraphicArgPasser (9 bytes, byte-packed: CIntelligentPointer<CThing> thing +0, bool bypassSet +8 --
// one of the two 9-byte {CIntelligentPointer<CThing>, bool} records in Ego_r.pdb; its constructor is the landed
// neighbour 0x006A2F00 of this TU). The implicit copy runs CIntelligentPointer<CThing>'s copy constructor (base
// constructor 0x00A01B10, the vtable 0x01238C6C store, SetPItem(other.GetPItem())) then the flag byte.
// Callers: 0x006A3950 (vector insert; reached from CGameScriptInterface::ScreenFilterFadeTo 0x008A9880).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, GetPItem @ 0x00A01B50, SetPItem @ 0x00A01B90.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` from a /Gr TU (`__fastcall`: first
// in ecx, last in edx, result and the empty tag reference on the stack, `ret 8`): `for (; first != last; ++first,
// ++cur) _Construct(&*cur, *first);` -- a bottom-tested loop behind the `first == last` test, the placement new
// null-tests the destination, the advanced result is returned in eax.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the `ret 8`, the element stride and the placement null test name this body.
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
namespace FableCopy_006a3130 {
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
FableCopy_006a3130::CThingGraphicArgPasser* __fastcall __uninitialized_copy(const FableCopy_006a3130::CThingGraphicArgPasser* first, const FableCopy_006a3130::CThingGraphicArgPasser* last,
    FableCopy_006a3130::CThingGraphicArgPasser* result, const __false_type&)
{
    FableCopy_006a3130::CThingGraphicArgPasser* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) FableCopy_006a3130::CThingGraphicArgPasser(*first);
    return cur;
}
}