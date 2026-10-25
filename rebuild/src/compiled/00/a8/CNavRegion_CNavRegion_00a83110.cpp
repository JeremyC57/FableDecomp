// CNavRegion::CNavRegion(const CNavRegion& other) -- copy a navigation region: its id, a deep copy of its connection list (fresh sentinel, every counted connection pointer copied) and its most-recent search id
// Leaf: ??0CNavRegion@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A83110, 81 bytes (+ int3 padding), thiscall `ret 4`, returns this.
// Identity: callers 0x00A7A0F8 / 0x00A7A200 (CNavQuadTree 0x00A7A0A0), 0x00A7B09F (CNavQuadTree::LoadFromFile), 0x00A839F5,
//   0x00A840F9, 0x00A84619, 0x00A84BBC -- the CNavQuadTree TU's list<CNavRegion> (Regions) node construction and region
//   copies. Ego_r.pdb CNavRegion = {long RegionID +0; list<CCountedPointer<CNavRegionConnection> > Connections +4; long
//   MostRecentSearchID +0x10}; with retail's 4-byte STLport list the record is 12 bytes and MostRecentSearchID sits at +8
//   (the PDB layout disagrees with the bytes, so the class is modelled locally). The catalog label "List_Construct" is
//   descriptive.
// Callees: malloc 0x00BFEA0E (the list sentinel, 0x10-byte nodes = links + an 8-byte CCountedPointer),
//   list<CCountedPointer<CNavRegionConnection> >::_M_insert_dispatch(pos, first, last, const __false_type&) @ 0x00A80E90.
// Shape: the memberwise copy constructor (as VC generates it): RegionID, STLport's inline list copy constructor (proxy
//   zeroed, sentinel malloc'd and self-linked, `insert(begin(), x.begin(), x.end())` with the position iterator built in
//   its argument slot and the empty tag homed in the dead `other` slot -- same-TU _M_insert_dispatch stand-in), then
//   MostRecentSearchID.
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E
inline void* operator new(unsigned int, void* where) { return where; }

class CNavRegionConnection;
template <class T> class CCountedPointer {             // 8 bytes: object + shared count block
public:
    T*    Data;
    void* Info;
};

namespace std {
struct __false_type { __false_type() { } };

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : _List_node_base {
    _Tp _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) { }
};
template <class _Tp> struct _List_iterator : _List_iterator_base {
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_iterator(const _List_iterator& x) : _List_iterator_base(x._M_node) { }   // copy constructor
};
template <class _Tp> struct _List_const_iterator : _List_iterator_base {
    _List_const_iterator(_List_node_base* x) : _List_iterator_base(x) { }
    _List_const_iterator(const _List_iterator<_Tp>& x) : _List_iterator_base(x._M_node) { }   // converting, not a copy
};
template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int n) { return n != 0 ? (_Tp*)malloc(n * sizeof(_Tp)) : 0; }
};
template <class _Tp> struct _STLP_alloc_proxy : allocator<_Tp> {
    _Tp* _M_data;
    _STLP_alloc_proxy(const allocator<_Tp>& a, _Tp* p) : allocator<_Tp>(a), _M_data(p) { }
};
template <class _Tp> class _List_base {
public:
    _STLP_alloc_proxy<_List_node<_Tp> > _M_node;
    _List_base(const allocator<_List_node<_Tp> >& a) : _M_node(a, (_List_node<_Tp>*)0)
    {
        _List_node<_Tp>* n = _M_node.allocate(1);
        n->_M_next = n;
        n->_M_prev = n;
        _M_node._M_data = n;
    }
};
template <class _Tp> class list : public _List_base<_Tp> {
public:
    typedef _List_iterator<_Tp> iterator;
    typedef _List_const_iterator<_Tp> const_iterator;
    allocator<_List_node<_Tp> > get_allocator() const { return allocator<_List_node<_Tp> >(); }
    iterator begin() { return iterator(this->_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(this->_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(this->_M_node._M_data); }
    __declspec(noinline) void _M_insert_dispatch(iterator pos, const_iterator first, const_iterator last,
                                                 const __false_type&)   // 0x00A80E90 (same-TU body)
    {
        for (; first._M_node != last._M_node; first._M_node = first._M_node->_M_next) {
            _List_node<_Tp>* n = this->_M_node.allocate(1);
            new (&n->_M_data) _Tp(((_List_node<_Tp>*)first._M_node)->_M_data);
            n->_M_next = pos._M_node;
            n->_M_prev = pos._M_node->_M_prev;
            pos._M_node->_M_prev->_M_next = n;
            pos._M_node->_M_prev = n;
        }
    }
    void insert(iterator pos, const_iterator first, const_iterator last) { _M_insert_dispatch(pos, first, last, __false_type()); }
    list(const list& x) : _List_base<_Tp>(x.get_allocator()) { insert(begin(), x.begin(), x.end()); }
};
}

class CNavRegion {                                      // retail 12 bytes
public:
    long                                                   RegionID;             // +0x00
    std::list<CCountedPointer<CNavRegionConnection> >     Connections;          // +0x04
    long                                                   MostRecentSearchID;   // +0x08
    CNavRegion(const CNavRegion& other);
};

CNavRegion::CNavRegion(const CNavRegion& other)
    : RegionID(other.RegionID), Connections(other.Connections), MostRecentSearchID(other.MostRecentSearchID)
{
}