// CTCBase::AddToGlobalTCs() -- register this thing component in the global-update TC list once (push_back(this), then set IsInGlobalUpdateTCs)
// Leaf: ?AddToGlobalTCs@CTCBase@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00686ED0, 56 bytes, thiscall, plain `ret`.
// Identity: the manifest's "std::list::push_back" is propagated; Ego_r.pdb names ?AddToGlobalTCs@CTCBase@@QAEXXZ next to
//   RemoveFromGlobalTCs 0x00686DF0 and the landed UpdateGlobalTCs 0x00686DC0, which walk the same one-word STLport list
//   at 0x013BA8AC. Twenty TC constructors call it (CTCBoastUI, CTCHeroOnlineScoreboard, CTCQuickAccessMenu, ...).
// Callees: malloc 0x00BFEA0E (the STLport list node allocator).
// Shape: `if (!IsInGlobalUpdateTCs) { GlobalUpdateTCs.push_back(this); IsInGlobalUpdateTCs = true; }` with STLport's
//   `push_back(x) { insert(end(), x); }` inline: end() (the sentinel) is read from the global BEFORE the node is
//   created, the placement new null-tests &node->_M_data, then the four link stores.
// Layout: Ego_r.pdb CTCBase (12 bytes): vptr, ParentThing +4, ToBeRemoved +8, IsInGlobalUpdateTCs +9 (as
//   engine/CTCBase.h). The header is data-only and cannot declare the member or the static list, so the class is
//   modelled locally with the same members (as in the landed UpdateGlobalTCs).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }

class CThing;

namespace std {

struct _List_node_base {
    _List_node_base* _M_next;                               // +0x00
    _List_node_base* _M_prev;                               // +0x04
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;                                            // +0x08
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
    _List_node_base* _M_node;                               // the sentinel

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
    void push_back(const _Tp& __x) { insert(end(), __x); }
};

} // namespace std

class CTCBase {
public:
    virtual ~CTCBase();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09

    static std::list<CTCBase*> GlobalUpdateTCs;            // 0x013BA8AC
    void AddToGlobalTCs();
};

void CTCBase::AddToGlobalTCs()
{
    if (!IsInGlobalUpdateTCs) {
        GlobalUpdateTCs.push_back(this);
        IsInGlobalUpdateTCs = true;
    }
}