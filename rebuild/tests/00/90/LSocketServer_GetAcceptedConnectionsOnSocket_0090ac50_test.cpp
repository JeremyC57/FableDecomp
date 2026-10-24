// Behaviour test for 0x0090AC50 LSocketServer::GetAcceptedConnectionsOnSocket: null when nothing is pending, else the
// first accepted socket moves (every copy of it) from the accepted list to the end of the connected list.
#include <cstdio>
#include <cstdlib>
#include <new>

struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
template <class _Tp> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* x) : _M_node(x) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    bool operator==(const _List_iterator& y) const { return _M_node == y._M_node; }
};

template <class _Tp> class list {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp> iterator;
    _List_node_base* _M_node;
    list()
    {
        _M_node = (_List_node_base*)std::malloc(sizeof(_Node));
        _M_node->_M_next = _M_node;
        _M_node->_M_prev = _M_node;
    }
    iterator begin() { return iterator(_M_node->_M_next); }
    iterator end() { return iterator(_M_node); }
    void push_back(const _Tp& x)
    {
        _Node* tmp = (_Node*)std::malloc(sizeof(_Node));
        new (&tmp->_M_data) _Tp(x);
        _List_node_base* n = _M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
    }
    void remove(const _Tp& value)                  // STLport remove: every node equal to value
    {
        _List_node_base* first = _M_node->_M_next;
        while (first != _M_node) {
            _List_node_base* next = first->_M_next;
            if (((_Node*)first)->_M_data == value) {
                first->_M_prev->_M_next = next;
                next->_M_prev = first->_M_prev;
                std::free(first);
            }
            first = next;
        }
    }
    int count() const
    {
        int c = 0;
        for (_List_node_base* it = _M_node->_M_next; it != _M_node; it = it->_M_next)
            ++c;
        return c;
    }
    _Tp& back() { return ((_Node*)_M_node->_M_prev)->_M_data; }
};

class LSocket { public: int id; };

class LSocketServer {
public:
    list<LSocket*> ListOfAcceptedConnections;
    list<LSocket*> ListOfConnectedSockets;
    LSocket* GetAcceptedConnectionsOnSocket();
};

LSocket* LSocketServer::GetAcceptedConnectionsOnSocket()
{
    list<LSocket*>::iterator it = ListOfAcceptedConnections.begin();
    if (it == ListOfAcceptedConnections.end())
        return 0;
    LSocket* socket = *it;
    ListOfAcceptedConnections.remove(*it);
    ListOfConnectedSockets.push_back(socket);
    return socket;
}

int main()
{
    int ok = 1;
    LSocketServer server;
    if (server.GetAcceptedConnectionsOnSocket() != 0) ok = 0;
    LSocket a, b;
    server.ListOfAcceptedConnections.push_back(&a);
    server.ListOfAcceptedConnections.push_back(&b);
    server.ListOfAcceptedConnections.push_back(&a);
    if (server.GetAcceptedConnectionsOnSocket() != &a) ok = 0;
    if (server.ListOfAcceptedConnections.count() != 1 || server.ListOfConnectedSockets.count() != 1) ok = 0;
    if (server.ListOfConnectedSockets.back() != &a) ok = 0;
    if (server.GetAcceptedConnectionsOnSocket() != &b) ok = 0;
    if (server.ListOfConnectedSockets.count() != 2 || server.ListOfConnectedSockets.back() != &b) ok = 0;
    if (server.GetAcceptedConnectionsOnSocket() != 0) ok = 0;
    if (!ok) { std::printf("0090ac50_TEST FAIL\n"); return 1; }
    std::printf("0090ac50_TEST PASS\n");
    return 0;
}