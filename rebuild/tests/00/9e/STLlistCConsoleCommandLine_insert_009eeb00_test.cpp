// Behaviour test for 0x009EEB00 list<CConsoleCommandLine>::insert(iterator, const T&): the copy is linked in before
// the position and the returned iterator names it.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

static int gBaseCtors = 0;

class CBase { public: CBase() { ++gBaseCtors; } virtual ~CBase() {} };

class CConsoleCommandLine : public CBase
{
public:
    const char* CurrentEntry;
    bool        UserEntry;
    long        CursorPosition;
    CConsoleCommandLine(const char* e, bool u, long c) : CurrentEntry(e), UserEntry(u), CursorPosition(c) {}
    CConsoleCommandLine(const CConsoleCommandLine& other)
        : CurrentEntry(other.CurrentEntry), UserEntry(other.UserEntry), CursorPosition(other.CursorPosition) {}
};

struct NodeBase { NodeBase* _M_next; NodeBase* _M_prev; };
struct Node : NodeBase { CConsoleCommandLine _M_data; };
struct Iterator { NodeBase* _M_node; Iterator(NodeBase* n) : _M_node(n) {} };

struct List
{
    NodeBase* _M_node;
    Iterator insert(Iterator __position, const CConsoleCommandLine& __x)
    {
        Node* __tmp = (Node*)std::malloc(sizeof(Node));
        new (&__tmp->_M_data) CConsoleCommandLine(__x);
        NodeBase* __n = __position._M_node;
        NodeBase* __prev = __n->_M_prev;
        __tmp->_M_next = __n;
        __tmp->_M_prev = __prev;
        __prev->_M_next = __tmp;
        __n->_M_prev = __tmp;
        return Iterator(__tmp);
    }
};

int main()
{
    NodeBase head; head._M_next = &head; head._M_prev = &head;
    List l; l._M_node = &head;
    CConsoleCommandLine a("first", true, 5), b("second", false, 0);
    gBaseCtors = 0;
    Iterator ia = l.insert(Iterator(&head), a);            // push_back
    Iterator ib = l.insert(Iterator(head._M_next), b);     // push_front
    Node* na = (Node*)ia._M_node; Node* nb = (Node*)ib._M_node;
    bool ok = gBaseCtors == 2 && head._M_next == nb && nb->_M_next == na && na->_M_next == &head
           && head._M_prev == na && na->_M_prev == nb && nb->_M_prev == &head
           && std::strcmp(na->_M_data.CurrentEntry, "first") == 0 && na->_M_data.UserEntry && na->_M_data.CursorPosition == 5
           && std::strcmp(nb->_M_data.CurrentEntry, "second") == 0 && !nb->_M_data.UserEntry;
    std::free(na); std::free(nb);
    if (!ok) { std::printf("009eeb00_TEST FAIL\n"); return 1; }
    std::printf("009eeb00_TEST PASS\n");
    return 0;
}