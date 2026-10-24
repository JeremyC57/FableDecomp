// Behaviour test for 0x005355C0 _STL::list<CCountedPointer<CSpawnedFuncBase> >::insert (malloc'd node, counted copy, linked before pos).
#include <cstdio>
#include <cstdlib>
#include <new>

struct CSpawnedFuncBase { int id; };
struct CCPPointerInfo { unsigned long RefCount; void (*DeleteFunc)(void*); void* Data; };
template <class T>
struct CCountedPointer {
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer(T* d, CCPPointerInfo* i) : Data(d), Info(i) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
};
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class T> struct _List_node : _List_node_base { T _M_data; };
template <class T> struct _List_iterator {
    _List_node_base* _M_node;
    _List_iterator(_List_node_base* x) : _M_node(x) { }
    _List_iterator(const _List_iterator& x) : _M_node(x._M_node) { }
};
template <class T> struct list {
    typedef _List_node<T> _Node;
    typedef _List_iterator<T> iterator;
    _List_node_base* _M_node;
    _Node* _M_create_node(const T& x) { _Node* p = (_Node*)std::malloc(sizeof(_Node)); new (&p->_M_data) T(x); return p; }
    iterator insert(iterator pos, const T& x)
    {
        _Node* tmp = _M_create_node(x);
        _List_node_base* n = pos._M_node;
        _List_node_base* p = n->_M_prev;
        tmp->_M_next = n;
        tmp->_M_prev = p;
        p->_M_next = tmp;
        n->_M_prev = tmp;
        return tmp;
    }
};
typedef CCountedPointer<CSpawnedFuncBase> CPtr;

int main()
{
    _List_node_base sentinel;
    sentinel._M_next = &sentinel; sentinel._M_prev = &sentinel;
    list<CPtr> l; l._M_node = &sentinel;
    CSpawnedFuncBase funcs[3];
    CCPPointerInfo info; info.RefCount = 1; info.DeleteFunc = 0; info.Data = 0;
    CPtr p0(&funcs[0], &info), p1(&funcs[1], 0), p2(&funcs[2], &info);
    list<CPtr>::iterator a = l.insert(list<CPtr>::iterator(&sentinel), p0);   // push_back
    list<CPtr>::iterator b = l.insert(list<CPtr>::iterator(&sentinel), p1);   // push_back
    list<CPtr>::iterator c = l.insert(a, p2);                                 // in front of the first
    // order: c, a, b
    if (sentinel._M_next != c._M_node || c._M_node->_M_next != a._M_node || a._M_node->_M_next != b._M_node || b._M_node->_M_next != &sentinel)
        { std::printf("005355c0_TEST FAIL\n"); return 1; }
    if (sentinel._M_prev != b._M_node || b._M_node->_M_prev != a._M_node || a._M_node->_M_prev != c._M_node || c._M_node->_M_prev != &sentinel)
        { std::printf("005355c0_TEST FAIL\n"); return 1; }
    if (static_cast<_List_node<CPtr>*>(a._M_node)->_M_data.Data != &funcs[0] || static_cast<_List_node<CPtr>*>(b._M_node)->_M_data.Info != 0
        || static_cast<_List_node<CPtr>*>(c._M_node)->_M_data.Data != &funcs[2]) { std::printf("005355c0_TEST FAIL\n"); return 1; }
    if (info.RefCount != 3) { std::printf("005355c0_TEST FAIL\n"); return 1; }   // p0 and p2 each copied once
    std::free(a._M_node); std::free(b._M_node); std::free(c._M_node);
    std::printf("005355c0_TEST PASS\n");
    return 0;
}