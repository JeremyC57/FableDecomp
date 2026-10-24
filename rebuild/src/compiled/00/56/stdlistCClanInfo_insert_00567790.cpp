// std::list<CClanInfo>::insert(iterator pos, const CClanInfo& x) -- STLport list insert: malloc a 0x1C-byte node, copy-construct the clan record (CTCHeroOnlineScoreboard::CClanInfo) into it, link it in front of pos and return an iterator to it
// Leaf: ?insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00567790, 99 bytes, thiscall `ret 0Ch` with `this` unused (hidden iterator
//   result, the by-value iterator, the element reference). Callers: CTCHeroOnlineScoreboard::DownloadListOfMyClans
//   0x005615C0 (landed; it names this address list<CClanInfo>::insert, push_back on MyClans) and 0x00567F71. The
//   catalog label "ClanMember_CreateNode" is descriptive (the element is CClanInfo, not CClanMember).
// Callees: malloc @ 0x00BFEA0E (node allocator, no out-of-memory check), CWideString(const CWideString&)
//   @ 0x0099B720 (x2), CCharString(const CCharString&) @ 0x0099EC30.
// Layout (Ego_r.pdb CTCHeroOnlineScoreboard::CClanInfo, 0x14): Name +0, ClanID +4, Description +8, HomePageURL
//   +0xC, NumMembers +0x10; STLport list node {_M_next, _M_prev, _M_data}.
// Shape: the landed STLport insert (0x005355C0): `_Node* tmp = _M_create_node(x)` (placement new, null test on
//   &tmp->_M_data), link before pos, `return iterator(tmp)` through the hidden pointer (iterator has user ctors).
//   The record's memberwise copy is inline in source member order.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }

struct CWideStringData;
struct CWideString {
    CWideStringData* PStringData;
    CWideString(const CWideString& other);              // 0x0099B720
};

struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
};

class CTCHeroOnlineScoreboard {
public:
    struct CClanInfo {                                  // 0x14
        CWideString Name;                               // +0x00
        long        ClanID;                             // +0x04
        CWideString Description;                        // +0x08
        CCharString HomePageURL;                        // +0x0C
        int         NumMembers;                         // +0x10
    };
};

namespace std {

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

    iterator insert(iterator __position, const _Tp& __x);
};

template <class _Tp>
typename list<_Tp>::iterator list<_Tp>::insert(iterator __position, const _Tp& __x)
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

template class list<CTCHeroOnlineScoreboard::CClanInfo>;

}