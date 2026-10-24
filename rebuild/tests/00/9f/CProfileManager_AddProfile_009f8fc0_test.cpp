// Behaviour test for 0x009F8FC0 CProfileManager::AddProfile: the profile goes to the FRONT of ProfileList and its Manager
// points back at the manager.
#include <cstdio>
#include <cstdlib>
#include <new>

struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class T> struct _List_node : _List_node_base { T _M_data; };

template <class T> struct list {
    _List_node<T>* _M_node;
    list() { _M_node = (_List_node<T>*)std::malloc(sizeof(_List_node<T>)); _M_node->_M_next = _M_node->_M_prev = _M_node; }
    void insert(_List_node_base* n, const T& x)
    {
        _List_node<T>* tmp = (_List_node<T>*)std::malloc(sizeof(_List_node<T>));
        new (&tmp->_M_data) T(x);
        _List_node_base* prev = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = prev;
        prev->_M_next = tmp;
        n->_M_prev = tmp;
    }
    void push_front(const T& x) { insert(_M_node->_M_next, x); }
};

class CProfileManager;
class CProfile { public: CProfileManager* Manager; };

class CProfileManager {
public:
    list<CProfile*> ProfileList;
    void AddProfile(CProfile& profile)
    {
        ProfileList.push_front(&profile);
        profile.Manager = this;
    }
};

int main()
{
    CProfileManager m;
    CProfile a, b;
    a.Manager = 0; b.Manager = 0;
    m.AddProfile(a);
    m.AddProfile(b);
    _List_node_base* head = m.ProfileList._M_node;
    _List_node<CProfile*>* first = (_List_node<CProfile*>*)head->_M_next;
    _List_node<CProfile*>* second = (_List_node<CProfile*>*)first->_M_next;
    bool ok = first->_M_data == &b && second->_M_data == &a && second->_M_next == head && head->_M_prev == second
           && second->_M_prev == first && first->_M_prev == head && a.Manager == &m && b.Manager == &m;
    if (!ok) { std::printf("009f8fc0_TEST FAIL\n"); return 1; }
    std::printf("009f8fc0_TEST PASS\n");
    return 0;
}