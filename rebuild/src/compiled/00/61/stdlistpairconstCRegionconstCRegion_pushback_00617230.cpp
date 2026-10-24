// std::list<pair<const CRegion*, const CRegion*> >::push_back(const std::pair<const CRegion*, const CRegion*>& x) -- STLport list push_back: insert(end(), x) -- malloc a 0x10-byte node, copy the region pair into it and link it before the sentinel
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00617230, 53 bytes (+11 int3), thiscall `ret 4`.
// Caller: the CTCInventoryMap persist routine 0x00606300 (call at 0x0060696D): it looks two regions up
//   (0x004FB490 twice), stores them as {second call, first call} into a stack pair and pushes it -- the
//   PDB's CTCInventoryMap::RegionsCorrespondances, list<pair<const CRegion*, const CRegion*> > (Ego_r.pdb).
// Callee: malloc 0x00BFEA0E (the node: next +0, prev +4, the pair +8).
// Label correction: the catalog `CList_PushBack` is behavioural; the body is STLport `push_back(x) { insert(end(), x); }`.
// Shape: speed TU (no pragma), the landed 0x00567FC0 list push_back: end() (the sentinel pointer, `mov esi,[ecx]`) is
//   read before the node is created, placement new tests &node->_M_data, the pair is copied through edi/edx, then
//   the four link stores.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }

class CRegion;

namespace std {

template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair& __p) : first(__p.first), second(__p.second) { }
};

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) { }
};

template <class _Tp> class list {
public:
    typedef _List_node<_Tp>     _Node;
    typedef _List_iterator<_Tp> iterator;

    _List_node_base* _M_node;                           // the sentinel (retail list = one pointer)

    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)malloc(sizeof(_Node));
        new (&__p->_M_data) _Tp(__x);
        return __p;
    }
    iterator end() { return iterator(_M_node); }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __p = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __p;
        __p->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
    void push_back(const _Tp& __x);
};

template <class _Tp>
void list<_Tp>::push_back(const _Tp& __x)
{
    insert(end(), __x);
}

typedef pair<const CRegion*, const CRegion*> TRegionPair;
template void list<TRegionPair>::push_back(const TRegionPair&);

}