// Behaviour test for STLport list<long>::pop_front (0x00A219E0): unlink the first node and free it.
#include <cstdio>
#include <cstdlib>

struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
struct _List_node : _List_node_base { long _M_data; };

static int g_freed = 0;
static void FreeNode(void* p) { ++g_freed; std::free(p); }

struct list {
    _List_node_base* _M_node;
    list() { _M_node = (_List_node_base*)std::malloc(sizeof(_List_node)); _M_node->_M_next = _M_node->_M_prev = _M_node; }
    ~list() { while (_M_node->_M_next != _M_node) pop_front(); std::free(_M_node); }
    void push_back(long v)
    {
        _List_node* n = (_List_node*)std::malloc(sizeof(_List_node));
        n->_M_data = v;
        n->_M_next = _M_node; n->_M_prev = _M_node->_M_prev;
        _M_node->_M_prev->_M_next = n; _M_node->_M_prev = n;
    }
    void erase(_List_node_base* pos)
    {
        _List_node_base* next = pos->_M_next;
        _List_node_base* prev = pos->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        FreeNode(pos);
    }
    void pop_front() { erase(_M_node->_M_next); }
    long front() const { return ((_List_node*)_M_node->_M_next)->_M_data; }
    bool empty() const { return _M_node->_M_next == _M_node; }
};

int main()
{
    list l;
    l.push_back(5); l.push_back(6); l.push_back(7);
    l.pop_front();
    if (g_freed != 1 || l.front() != 6 || l._M_node->_M_next->_M_prev != l._M_node) { std::printf("00a219e0_TEST FAIL\n"); return 1; }
    l.pop_front(); l.pop_front();
    if (!l.empty() || g_freed != 3 || l._M_node->_M_prev != l._M_node) { std::printf("00a219e0_TEST FAIL\n"); return 1; }
    std::printf("00a219e0_TEST PASS\n");
    return 0;
}