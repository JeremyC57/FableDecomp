// std::__uninitialized_copy(const SAnimEventsEntry* first, const SAnimEventsEntry* last, SAnimEventsEntry* result, const __false_type&) -- pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32> > elements: key, then the small vector copy
// Leaf: ?__uninitialized_copy@std@@YIPAU?$pair@KUCSmallVectorEvents@FableCopy_006fbae0@@@FableCopy_006fbae0@@PBU23@0PAU23@ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FBAE0, length 67 bytes, `ret 8`.
// Element: pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32> > (12 bytes; the only such pair in Ego_r.pdb).
// STLport's pair copy constructor copies the key, then CSmallVector's copy constructor: Start and Size zeroed
// (init list), then the protected Copy(other) (landed 0x006FB5C0) -- its argument is pushed before the zero stores.
// Callers: 0x006FBB30 (fill), 0x006FC2D0 / 0x006FC6A0 (reallocation), 0x006FC7A0 / 0x006FC950 (insert).
// Callees: CSmallVector<NAnimationEvents::CEvent,32>::Copy @ 0x006FB5C0.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` from a /Gr TU (`__fastcall`: first
// in ecx, last in edx, result and the empty tag reference on the stack, `ret 8`): `for (; first != last; ++first,
// ++cur) _Construct(&*cur, *first);` -- a bottom-tested loop behind the `first == last` test, the placement new
// null-tests the destination, the advanced result is returned in eax.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the `ret 8`, the element stride and the placement null test name this body.
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
namespace FableCopy_006fbae0 {
namespace NAnimationEvents { class CEvent; }
struct CSmallVectorEvents {                             // CSmallVector<NAnimationEvents::CEvent,32>: 8 bytes
    NAnimationEvents::CEvent* Start;                    // +0x00
    int                       Size;                     // +0x04
    CSmallVectorEvents(const CSmallVectorEvents& other) : Start(0), Size(0) { Copy(other); }
    void Copy(const CSmallVectorEvents& other);         // 0x006FB5C0
};
template <class T1, class T2>
struct pair {                                           // STLport pair: explicit memberwise copy constructor
    T1 first;
    T2 second;
    pair(const pair& other) : first(other.first), second(other.second) { }
};
typedef pair<unsigned long, CSmallVectorEvents> SAnimEventsEntry;
}
namespace std {
FableCopy_006fbae0::SAnimEventsEntry* __fastcall __uninitialized_copy(const FableCopy_006fbae0::SAnimEventsEntry* first, const FableCopy_006fbae0::SAnimEventsEntry* last,
    FableCopy_006fbae0::SAnimEventsEntry* result, const __false_type&)
{
    FableCopy_006fbae0::SAnimEventsEntry* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) FableCopy_006fbae0::SAnimEventsEntry(*first);
    return cur;
}
}