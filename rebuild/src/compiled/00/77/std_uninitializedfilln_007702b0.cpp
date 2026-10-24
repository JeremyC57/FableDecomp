// std::__uninitialized_fill_n(CWeaponTrailList* first, unsigned int n, const CWeaponTrailList& x, const __false_type&) -- STLport placement fill of n raw std::list<CWeaponTrailEntry> slots with copies of x (each copy: a malloc'd self-linked sentinel, then the range insert of x's entries), returning the end of the filled range
// Leaf: ?__uninitialized_fill_n@std@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007702B0, 83 bytes, /Gr `__fastcall` (first in ecx -> esi, n in edx -> ebx,
//   &x and the empty tag reference on the stack: `ret 8`).
// Callees: malloc @ 0x00BFEA0E (the 0x24-byte sentinel node), list::_M_insert_dispatch(iterator, const_iterator,
//   const_iterator, const __false_type&) @ 0x0076FD40 (`ret 10h`).
// Element: a 4-byte STLport list<CEnginePrimitiveWeaponTrail::CWeaponTrailEntry> (CTCWeaponTrail::Trail is a vector of
//   them); the same list model as the landed sibling __uninitialized_copy 0x007705A0 and CEnginePrimitiveWeaponTrail
//   0x007703B0. Callers: vector<list<CWeaponTrailEntry> > insert / reallocation (0x00770600, 0x00770810, 0x00770990).
// Label correction: catalogued as `_Fill_n<unsigned long*,unsigned int,unsigned long>` (propagated); the body is STLport
//   `__uninitialized_fill_n` (non-POD path).
// Shape: STLport 4.x `for (; n > 0; --n, ++cur) _Construct(&*cur, x); return cur;` (unsigned n: `test edx,edx ; jbe`)
//   with the inline list copy constructor: the alloc proxy is zeroed, the sentinel malloc'd and self-linked, then
//   `insert(begin(), x.begin(), x.end())` -- the by-value `pos` iterator (user copy constructor) is built in its argument
//   slot, the const_iterators are pushed directly and the empty tag is homed in the dead `x` argument slot.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() { } };
namespace FableFill_007702b0 {
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
FableFill_007702b0::CWeaponTrailList* __fastcall __uninitialized_fill_n(FableFill_007702b0::CWeaponTrailList* first, unsigned int n,
    const FableFill_007702b0::CWeaponTrailList& x, const __false_type&)
{
    FableFill_007702b0::CWeaponTrailList* cur = first;
    for (; n > 0; --n, ++cur)
        new ((void*)cur) FableFill_007702b0::CWeaponTrailList(x);
    return cur;
}
}