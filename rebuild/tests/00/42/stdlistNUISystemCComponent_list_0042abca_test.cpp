#include <cstdio>
#include <cstdlib>
static int g_frees = 0; static void* g_freed[8]; static int g_clears = 0;
extern "C" void __cdecl myfree(void* p) { if (g_frees < 8) g_freed[g_frees] = p; ++g_frees; }
#define free myfree
namespace NUISystem { class CComponent; }
namespace std {
template <class T> struct _List_node { _List_node* _M_next; _List_node* _M_prev; T _M_data; };
template <class T> struct allocator { void deallocate(_List_node<T>* p, unsigned long) { if (p) ::free(p); } };
template <class T, class A> class list {
public:
    _List_node<T>* _M_node;
    void clear();
    ~list();
};
template <class T, class A> list<T, A>::~list() { clear(); A().deallocate(_M_node, 1); }
template <class T, class A> void list<T, A>::clear()
{
    ++g_clears;
    _List_node<T>* cur = _M_node->_M_next;
    while (cur != _M_node) { _List_node<T>* n = cur; cur = cur->_M_next; ::free(n); }
    _M_node->_M_next = _M_node; _M_node->_M_prev = _M_node;
}
template class list<NUISystem::CComponent*, allocator<NUISystem::CComponent*> >;
}
#undef free
int main()
{
    typedef std::_List_node<NUISystem::CComponent*> Node;
    static Node sentinel, a, b;
    sentinel._M_next = &a; a._M_prev = &sentinel; a._M_next = &b; b._M_prev = &a; b._M_next = &sentinel; sentinel._M_prev = &b;
    static unsigned char raw[sizeof(std::list<NUISystem::CComponent*, std::allocator<NUISystem::CComponent*> >)];
    std::list<NUISystem::CComponent*, std::allocator<NUISystem::CComponent*> >* l = (std::list<NUISystem::CComponent*, std::allocator<NUISystem::CComponent*> >*)raw;
    l->_M_node = &sentinel;
    l->~list();
    bool ok = g_clears == 1 && g_frees == 3 && g_freed[0] == &a && g_freed[1] == &b && g_freed[2] == &sentinel && sentinel._M_next == &sentinel;
    l->_M_node = 0; l->_M_node = &sentinel; sentinel._M_next = sentinel._M_prev = &sentinel;
    l->_M_node = 0;                         // a moved-from list: clear must not be reached with a null sentinel, so test the null guard separately
    std::printf(ok ? "0042abca_TEST PASS\n" : "0042abca_TEST FAIL\n");
    return ok ? 0 : 1;
}
