// std::__uninitialized_copy(const SThingEntry* first, const SThingEntry* last, SThingEntry* result, const __false_type&) -- pair<long, CIntelligentPointer<CThing> > elements: key, then the intelligent-pointer copy, return the end
// Leaf: ?__uninitialized_copy@std@@YIPAU?$pair@JU?$CIntelligentPointer@VCThing@FableCopy_006a31d0@@@FableCopy_006a31d0@@@FableCopy_006a31d0@@PBU23@0PAU23@ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A31D0, length 78 bytes, `ret 8`.
// Element: pair<long, CIntelligentPointer<CThing> > (12 bytes; Ego_r.pdb `_Pair_base<long,CIntelligentPointer<CThing>>`).
// STLport's pair copy constructor copies the key, then CIntelligentPointer<CThing>'s copy constructor at +4 (base
// constructor 0x00A01B10, the vtable 0x01238C6C store, SetPItem(other.GetPItem())), through a second dst pointer.
// Callers: 0x006A3A70 (vector fill, reached from the landed CTCCarrying::OnCreate 0x006A2540).
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
namespace FableCopy_006a31d0 {
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
FableCopy_006a31d0::SThingEntry* __fastcall __uninitialized_copy(const FableCopy_006a31d0::SThingEntry* first, const FableCopy_006a31d0::SThingEntry* last,
    FableCopy_006a31d0::SThingEntry* result, const __false_type&)
{
    FableCopy_006a31d0::SThingEntry* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) FableCopy_006a31d0::SThingEntry(*first);
    return cur;
}
}