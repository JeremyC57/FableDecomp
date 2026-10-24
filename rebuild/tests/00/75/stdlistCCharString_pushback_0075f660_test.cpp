// Behaviour test for 0x0075F660 list<CCharString>::push_back: nodes are appended before the sentinel in order,
// each holding its own copy of the string.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

class CCharString {
public:
    char* PStringData;
    CCharString(const char* s) { PStringData = new char[std::strlen(s) + 1]; std::strcpy(PStringData, s); }
    CCharString(const CCharString& o) { PStringData = new char[std::strlen(o.PStringData) + 1]; std::strcpy(PStringData, o.PStringData); }
    ~CCharString() { delete[] PStringData; }
};

struct NodeBase { NodeBase* _M_next; NodeBase* _M_prev; };
struct Node : NodeBase { CCharString _M_data; };

struct List {
    NodeBase* _M_node;
    void push_back(const CCharString& x)
    {
        Node* tmp = (Node*)std::malloc(sizeof(Node));
        new (&tmp->_M_data) CCharString(x);
        NodeBase* n = _M_node;
        NodeBase* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
    }
};

int main()
{
    NodeBase sentinel; sentinel._M_next = &sentinel; sentinel._M_prev = &sentinel;
    List l; l._M_node = &sentinel;
    CCharString a("first"), b("second"), c("third");
    l.push_back(a); l.push_back(b); l.push_back(c);
    const char* want[3] = { "first", "second", "third" };
    int ok = 1, i = 0;
    for (NodeBase* n = sentinel._M_next; n != &sentinel; n = n->_M_next, ++i) {
        Node* node = (Node*)n;
        if (i >= 3 || std::strcmp(node->_M_data.PStringData, want[i]) || node->_M_next->_M_prev != n) ok = 0;
    }
    if (i != 3 || ((Node*)sentinel._M_prev)->_M_data.PStringData == c.PStringData) ok = 0;
    for (NodeBase* n = sentinel._M_next; n != &sentinel; ) { NodeBase* next = n->_M_next; ((Node*)n)->_M_data.~CCharString(); std::free(n); n = next; }
    if (!ok) { std::printf("0075f660_TEST FAIL\n"); return 1; }
    std::printf("0075f660_TEST PASS\n");
    return 0;
}