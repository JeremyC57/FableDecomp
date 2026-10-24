// _STL::vector<CUIStateMap>::push_back(const CUIStateMap& x) -- STLport vector push_back of NUISystem::CList::ScrollingChildrenStates (CUIStateMap = map<unsigned long, NUISystem::CUIState>): copy-construct the map at the end when there is room, else grow through _M_insert_overflow
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0053CFC2, 56 bytes, `ret 4` (thiscall on the vector, &x); size-optimised frame-pointer
//   TU (`push ebp ; mov ebp,esp`, unaligned entry: `optimize("y", off)` + `("s", on)`, as the landed CList rows).
// Callees: map<unsigned long, CUIState> copy constructor 0x0053CB9B (manifest "std::list_construct_from_list": a 0x38-byte
//   header node from malloc, then the tree copy), the vector's _M_insert_overflow(pos, x, const __false_type&, 1, true)
//   0x0053CEF6 (manifest "CActiveFile::Read_CArrayElements").
// Callers: NUISystem::CList 0x0053B964 (Initialise 0x0053B91E), 0x0053BEF4 (AddScrollingChildDynamically 0x0053BEAC,
//   landed), 0x0053C2C1 (AddScrollingChildDynamicallyAndSetOffsets 0x0053BF20), 0x0053C764 (DoRecomputeOffsets 0x0053C332).
// Label correction: catalogued as "UIScrollList_AddChild" (the landed 0x0053BEAC declares it as a CScrollLayout::AddChild
//   at +0x17c); +0x17c is CList::ScrollingChildrenStates (Ego_r.pdb +0x1b4, vector<map<unsigned long, CUIState> >; the
//   retail vectors are 12 bytes) and this is its push_back.
// Layout: STLport vector { start, finish, end of storage }; the map element is 12 bytes { header, count, comparator }.
// Shape: STLport `if (_M_finish != _M_end_of_storage) { _Construct(_M_finish, x); ++_M_finish; } else
//   _M_insert_overflow(_M_finish, x, __false_type(), 1UL, true);` -- the placement copy is null tested, the tag temporary
//   sits in the high byte of the `x` argument slot (`lea eax,[ebp+0Bh]`). An explicit instantiation emits only push_back.
#pragma optimize("y", off)
#pragma optimize("s", on)
inline void* operator new(unsigned int, void* where) { return where; }

namespace NUISystem {
class CUIState;
}

namespace _STL {
struct __false_type {
    __false_type() {}
};

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

class CUIStateMap {                             // map<unsigned long, NUISystem::CUIState> (12 bytes)
public:
    void*         _M_header;                    // +0x00 the header node (0x38 bytes)
    unsigned long _M_node_count;                // +0x04
    unsigned char _M_key_compare;               // +0x08 less<unsigned long>
    CUIStateMap(const CUIStateMap& x);          // 0x0053CB9B
};

template <class T> class vector {
public:
    T* _M_start;                                // +0x00
    T* _M_finish;                               // +0x04
    T* _M_end_of_storage;                       // +0x08
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);   // 0x0053CEF6
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage) {
            _Construct(_M_finish, x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, __false_type(), 1UL, true);
    }
};

template void vector<CUIStateMap>::push_back(const CUIStateMap& x);
}