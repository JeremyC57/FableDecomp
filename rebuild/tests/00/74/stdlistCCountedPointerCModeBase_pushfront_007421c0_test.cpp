// Behaviour test for 0x007421C0 list<CCountedPointer<CTCCreatureModeManager::CModeBase> >::push_front
#include <cstdio>
#include <cstdlib>
#include <new>

struct CCPPointerInfo {
    unsigned int RefCount;
    void (*DeleteFunc)(void*);
    void* Data;
};

template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer(T* d, CCPPointerInfo* i) : Data(d), Info(i) { }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
};

struct CModeBase { int id; };

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
    _List_node_base* _M_node;
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)std::malloc(sizeof(_Node));
        new (&__p->_M_data) _Tp(__x);
        return __p;
    }
    iterator begin() { return iterator(_M_node->_M_next); }
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
    void push_front(const _Tp& __x) { insert(begin(), __x); }
};

typedef CCountedPointer<CModeBase> TModePtr;

int main()
{
    _List_node_base sentinel;
    sentinel._M_next = &sentinel;
    sentinel._M_prev = &sentinel;
    list<TModePtr> modes;
    modes._M_node = &sentinel;
    CModeBase a = { 1 }, b = { 2 };
    CCPPointerInfo ia = { 1, 0, &a };
    TModePtr pa(&a, &ia), pb(&b, 0), pnull(0, 0);
    modes.push_front(pa);
    modes.push_front(pb);
    modes.push_front(pa);
    modes.push_front(pnull);
    int ok = 1;
    if (ia.RefCount != 3) ok = 0;                            // two copies of pa bumped the shared count
    // forward order: pnull, pa, pb, pa (each push lands in front)
    _List_node<TModePtr>* n = (_List_node<TModePtr>*)sentinel._M_next;
    CModeBase* want[4] = { 0, &a, &b, &a };
    for (int i = 0; i < 4; ++i) {
        if (n == (void*)&sentinel || n->_M_data.Data != want[i]) { ok = 0; break; }
        n = (_List_node<TModePtr>*)n->_M_next;
    }
    if (n != (void*)&sentinel) ok = 0;
    // backward links agree
    _List_node<TModePtr>* last = (_List_node<TModePtr>*)sentinel._M_prev;
    if (last->_M_data.Data != &a || last->_M_data.Info != &ia || last->_M_next != &sentinel) ok = 0;
    if (!ok) { std::printf("007421c0_TEST FAIL\n"); return 1; }
    std::printf("007421c0_TEST PASS\n");
    return 0;
}