// std::list<CConsoleCommandLine>::insert(iterator, const CConsoleCommandLine&) -- STLport list insert of a copied command line before the position; returns an iterator to the new node
// Retail: Fable.exe (TLC, VC7.1) @ 0x009EEB00, 92 bytes, `ret 0Ch` (hidden result, the iterator by value, &x).
//   Catalog label "HeroInformationScreens_CreateListNode" is wrong: the node holds a CConsoleCommandLine (vtable
//   0x0129C400), the CConsole::CommandLineHistory element.
// Callees: malloc 0x00BFEA0E (the 0x18-byte node: two links + the 16-byte command line), CBase::CBase 0x0099A2D0,
//   CCharString copy ctor 0x0099EC30.
// Shape: STLport 4.x insert + _M_create_node inlined (placement-new null test, no EH). The element copy runs the
//   out-of-line CBase() DEFAULT constructor, so CConsoleCommandLine has a user copy constructor that does not name
//   its base (an implicit one would only store the vptr); members copied in order, the bool as a byte.
// Leaf: ?insert@?$list@VCConsoleCommandLine@@@std@@QAE?AU?$_List_iterator@VCConsoleCommandLine@@@2@U32@ABVCConsoleCommandLine@@@Z
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);

class CCharString
{
public:
    void* PStringData;
    CCharString(const CCharString& other);         // 0x0099EC30
};

class CBase
{
public:
    CBase();                                       // 0x0099A2D0
    virtual ~CBase();                              // 0x0099A2E0
};

class CConsoleCommandLine : public CBase           // vtable 0x0129C400, 16 bytes
{
public:
    CCharString CurrentEntry;                      // +0x04
    bool        UserEntry;                         // +0x08
    long        CursorPosition;                    // +0x0c
    CConsoleCommandLine(const CConsoleCommandLine& other)
        : CurrentEntry(other.CurrentEntry), UserEntry(other.UserEntry), CursorPosition(other.CursorPosition) {}
};

namespace std {

struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};

template <class _Tp>
struct _List_node : public _List_node_base {
    _Tp _M_data;
};

template <class _Tp>
struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* __x) : _M_node(__x) {}
    _List_iterator(const _List_iterator& __x) : _M_node(__x._M_node) {}
};

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp>
class list {
public:
    typedef _List_iterator<_Tp> iterator;
    typedef _List_node<_Tp> _Node;
    _List_node_base* _M_node;

    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)malloc(sizeof(_Node));
        _Construct(&__p->_M_data, __x);
        return __p;
    }
    iterator insert(iterator __position, const _Tp& __x)
    {
        _Node* __tmp = _M_create_node(__x);
        _List_node_base* __n = __position._M_node;
        _List_node_base* __prev = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __prev;
        __prev->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return iterator(__tmp);
    }
};

template list<CConsoleCommandLine>::iterator list<CConsoleCommandLine>::insert(iterator, const CConsoleCommandLine&);

}