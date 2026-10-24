// std::__uninitialized_copy(const CWeaponTrailList* first, const CWeaponTrailList* last, CWeaponTrailList* result, const __false_type&) -- std::list<CWeaponTrailEntry> elements: build each list copy (sentinel node + range insert), return the end
// Leaf: ?__uninitialized_copy@std@@YIPAUCWeaponTrailList@FableCopy_007705a0@@PBU23@0PAU23@ABU__false_type@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007705A0, length 87 bytes, `ret 8`.
// Element: a 4-byte STLport list<CWeaponTrailEntry> (the alloc proxy's node pointer only; 0x24-byte nodes = links +
// a 28-byte entry), the list type of the landed CEnginePrimitiveWeaponTrail constructor 0x007703B0 (same model). The
// inline STLport list copy constructor: the proxy is zeroed, the sentinel malloc'd and self-linked, then
// `insert(begin(), x.begin(), x.end())` -- begin()'s node is loaded before the pushes, the by-value `pos` iterator
// (user copy constructor) is built in its argument slot, the const_iterators (implicit copy) are pushed directly, and
// the empty __false_type dispatch tag is homed in the dead `result` argument slot (`lea ebp,[esp+14h]`).
// Callers: 0x00770810 / 0x00770990 (vector<list<CWeaponTrailEntry> > insert / reallocation).
// Callees: malloc @ 0x00BFEA0E, list::_M_insert_dispatch(iterator, const_iterator, const_iterator, const __false_type&)
// @ 0x0076FD40 (`ret 10h`).
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, const __false_type&)` from a /Gr TU (`__fastcall`: first
// in ecx, last in edx, result and the empty tag reference on the stack, `ret 8`): `for (; first != last; ++first,
// ++cur) _Construct(&*cur, *first);` -- a bottom-tested loop behind the `first == last` test, the placement new
// null-tests the destination, the advanced result is returned in eax.
// Label correction: the catalog's `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,
// unsigned int*,allocator<unsigned int> >` is a Dinkumware name propagated over all 40 rows of this chunk; retail's STL
// is STLport 4.x and the `ret 8`, the element stride and the placement null test name this body.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
namespace FableCopy_007705a0 {
struct CWeaponTrailEntry { unsigned long m_data[7]; };   // 28 bytes (the range insert copies 7 dwords)
struct SListNodeBase { SListNodeBase* m_next; SListNodeBase* m_prev; };
struct SListNode : SListNodeBase { CWeaponTrailEntry m_data; };   // 0x24 bytes
struct SListIterator {                                  // STLport _List_iterator<T, _Nonconst_traits>: user copy constructor
    SListNodeBase* m_node;
    SListIterator(SListNodeBase* node) : m_node(node) { }
    SListIterator(const SListIterator& other) : m_node(other.m_node) { }
};
struct SListConstIterator { SListNodeBase* m_node; };   // _List_iterator<T, _Const_traits>: implicit copy
struct CWeaponTrailList {                               // STLport list<CWeaponTrailEntry>: 4 bytes
    SListNode* m_node;
    CWeaponTrailList(const CWeaponTrailList& x) : m_node(0)
    {
        SListNode* node = (SListNode*)malloc(sizeof(SListNode));
        node->m_next = node;
        node->m_prev = node;
        m_node = node;
        insert(begin(), x.cbegin(), x.cend());
    }
    SListIterator begin() { return SListIterator(m_node->m_next); }
    SListConstIterator cbegin() const { SListConstIterator it; it.m_node = m_node->m_next; return it; }
    SListConstIterator cend() const { SListConstIterator it; it.m_node = m_node; return it; }
    void InsertDispatch(SListIterator pos, SListConstIterator first, SListConstIterator last, const __false_type& tag);   // 0x0076FD40
    void insert(SListIterator pos, SListConstIterator first, SListConstIterator last)
    {
        InsertDispatch(pos, first, last, __false_type());
    }
};
}
namespace std {
FableCopy_007705a0::CWeaponTrailList* __fastcall __uninitialized_copy(const FableCopy_007705a0::CWeaponTrailList* first, const FableCopy_007705a0::CWeaponTrailList* last,
    FableCopy_007705a0::CWeaponTrailList* result, const __false_type&)
{
    FableCopy_007705a0::CWeaponTrailList* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) FableCopy_007705a0::CWeaponTrailList(*first);
    return cur;
}
}