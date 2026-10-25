#include <cstdio>
#include <stdlib.h>
#include <new>

struct C3DVector { float X, Y, Z; };

class CSoundManagerLH;

static int g_starts = 0;

class CSoundEmitterLH {
public:
    CSoundManagerLH* Owner;
    C3DVector        Position;
    int              Started;
    CSoundEmitterLH(CSoundManagerLH* owner, const C3DVector& position) : Owner(owner), Position(position), Started(0) { }
    virtual ~CSoundEmitterLH() { }
    virtual void Start() { ++Started; ++g_starts; }
};

namespace std {
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
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator--() { _M_node = _M_node->_M_prev; return *this; }
};
template <class _Tp> class list {
public:
    typedef _List_node<_Tp>     _Node;
    typedef _List_iterator<_Tp> iterator;
    _List_node_base* _M_node;
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)malloc(sizeof(_Node));
        new (&__p->_M_data) _Tp(__x);
        return __p;
    }
    iterator end() { return iterator(_M_node); }
    _Tp& back() { return *(--end()); }
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
    void push_back(const _Tp& __x) { insert(end(), __x); }
};
}

class CSoundManagerLH {
public:
    bool Allow;
    std::list<CSoundEmitterLH*> m_entries;
    virtual ~CSoundManagerLH() { }
    virtual CSoundEmitterLH* vfunc_4(const C3DVector& position);
    virtual bool vfunc_6() { return Allow; }
};

CSoundEmitterLH* CSoundManagerLH::vfunc_4(const C3DVector& position)
{
    if (vfunc_6()) {
        m_entries.push_back(new CSoundEmitterLH(this, position));
        m_entries.back()->Start();
        return m_entries.back();
    }
    return 0;
}

static int Count(std::list<CSoundEmitterLH*>& l)
{
    int n = 0;
    for (std::_List_node_base* p = l._M_node->_M_next; p != l._M_node; p = p->_M_next)
        ++n;
    return n;
}

int main()
{
    CSoundManagerLH m;
    std::_List_node_base sentinel;
    sentinel._M_next = sentinel._M_prev = &sentinel;
    m.m_entries._M_node = &sentinel;
    C3DVector pos = { 1.0f, 2.0f, 3.0f };
    m.Allow = false;
    if (m.vfunc_4(pos) != 0 || Count(m.m_entries) != 0 || g_starts != 0)
        { std::printf("00a3ecb0_TEST FAIL\n"); return 1; }
    m.Allow = true;
    CSoundEmitterLH* a = m.vfunc_4(pos);
    pos.X = 5.0f;
    CSoundEmitterLH* b = m.vfunc_4(pos);
    if (a == 0 || b == 0 || a == b || Count(m.m_entries) != 2 || g_starts != 2 || a->Started != 1 || b->Started != 1
        || a->Owner != &m || b->Position.X < 4.99f || b->Position.X > 5.01f || a->Position.Z < 2.99f || a->Position.Z > 3.01f
        || m.m_entries.back() != b)
        { std::printf("00a3ecb0_TEST FAIL\n"); return 1; }
    std::_List_node_base* p = sentinel._M_next;
    while (p != &sentinel) {
        std::_List_node_base* n = p->_M_next;
        delete ((std::_List_node<CSoundEmitterLH*>*)p)->_M_data;
        free(p);
        p = n;
    }
    std::printf("00a3ecb0_TEST PASS\n");
    return 0;
}