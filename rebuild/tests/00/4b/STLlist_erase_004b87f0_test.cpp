// Behaviour test for STLport list<CQuestManager::CBoast>::erase(iterator) (0x004B87F0): unlink, destroy the strings, free, return next.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_names = 0, g_wides = 0, g_freed = 0;
class CCharString { public: void* PData; CCharString() : PData(0) {} ~CCharString() { ++g_names; } };
class CWideString { public: void* PData; CWideString() : PData(0) {} ~CWideString() { ++g_wides; } };

class CBoast {
public:
    long ID, Cost, Reward;
    CCharString ScriptName;
    CWideString Text, Title;
    bool Evil, Failed, Completed;
    long GroupIndex;
    CBoast(long id) : ID(id), Cost(0), Reward(0), Evil(false), Failed(false), Completed(false), GroupIndex(0) {}
};

struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
struct _List_node : _List_node_base { CBoast _M_data; };
struct iterator { _List_node_base* _M_node; iterator(_List_node_base* n) : _M_node(n) {} iterator(const iterator& o) : _M_node(o._M_node) {} };

struct list {
    _List_node_base* _M_node;
    list() { _M_node = (_List_node_base*)std::malloc(sizeof(_List_node_base)); _M_node->_M_next = _M_node->_M_prev = _M_node; }
    void push_back(long id)
    {
        _List_node* n = (_List_node*)std::malloc(sizeof(_List_node));
        new (&n->_M_data) CBoast(id);
        n->_M_next = _M_node; n->_M_prev = _M_node->_M_prev;
        _M_node->_M_prev->_M_next = n; _M_node->_M_prev = n;
    }
    iterator erase(iterator pos)
    {
        _List_node_base* next = pos._M_node->_M_next;
        _List_node_base* prev = pos._M_node->_M_prev;
        _List_node* n = (_List_node*)pos._M_node;
        prev->_M_next = next;
        next->_M_prev = prev;
        n->_M_data.~CBoast();
        ++g_freed;
        std::free(n);
        return iterator(next);
    }
};

int main()
{
    list l;
    l.push_back(10); l.push_back(20);
    iterator it = l.erase(iterator(l._M_node->_M_next));
    if (((_List_node*)it._M_node)->_M_data.ID != 20 || g_names != 1 || g_wides != 2 || g_freed != 1) { std::printf("004b87f0_TEST FAIL\n"); return 1; }
    it = l.erase(it);
    if (it._M_node != l._M_node || l._M_node->_M_prev != l._M_node || g_names != 2 || g_wides != 4) { std::printf("004b87f0_TEST FAIL\n"); return 1; }
    std::free(l._M_node);
    std::printf("004b87f0_TEST PASS\n");
    return 0;
}