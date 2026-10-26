// std::list<CClanMember>::push_back(const CClanMember& x) -- STLport list push_back: insert(end(), x) -- malloc a 0x14-byte node, copy the clan member record (CTCHeroOnlineScoreboard::CClanMember) into it and link it before the sentinel
// Leaf: ?push_back@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00567FC0, 75 bytes, thiscall `ret 4`. Caller: CTCHeroOnlineScoreboard::
//   DownloadMemberListForSelectedClan 0x00561DC0 (landed; it names this address list<CClanMember>::push_back,
//   ClanMembers at +0x40). The catalog label "ClanLogo_CreateNode" is descriptive.
// Callees: malloc @ 0x00BFEA0E, CWideString(const CWideString&) @ 0x0099B720 (x2).
// Layout (Ego_r.pdb CTCHeroOnlineScoreboard::CClanMember, 0x0C): Name +0, MemberID +4, Description +8.
// Shape: STLport `push_back(x) { insert(end(), x); }` with the inline insert of the landed 0x005355C0 shape: end()
//   (the sentinel pointer, `mov ebx,[ecx]`) is read before the node is created, placement new tests
//   &node->_M_data, then the four link stores; the iterator has user ctors.
extern "C" void* __cdecl malloc(unsigned int size);                              // 0x00BFEA0E

inline void* operator new(unsigned int, void* p) { return p; }

class CWideStringData;
class CWideString { public:
    CWideStringData* PStringData;
    CWideString(const CWideString& other);              // 0x0099B720
};

class CTCHeroOnlineScoreboard {
public:
    class CClanMember { public:                                // 0x0C
        CWideString Name;                               // +0x00
        long        MemberID;                           // +0x04
        CWideString Description;                        // +0x08
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

template void list<CTCHeroOnlineScoreboard::CClanMember>::push_back(const CTCHeroOnlineScoreboard::CClanMember&);

}