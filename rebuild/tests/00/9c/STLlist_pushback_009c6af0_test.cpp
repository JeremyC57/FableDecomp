#include <cstdio>
#include <cstdlib>
#include <new>

class CBufferChunk { public: int Id; };

class CFileInstaller {
public:
    class CPendingWrite {
    public:
        CBufferChunk* PBufferChunk;
        unsigned long FilePos;
        unsigned long WriteLength;
    };
};

namespace std {
template <class _Tp> class allocator { };
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
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    typedef _List_node<_Tp>     _Node;
    typedef _List_iterator<_Tp> iterator;
    _List_node_base* _M_node;
    iterator end() { return iterator(_M_node); }
    _Node* _M_create_node(const _Tp& __x)
    {
        _Node* __p = (_Node*)std::malloc(sizeof(_Node));
        new (&__p->_M_data) _Tp(__x);
        return __p;
    }
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
    void push_back(const _Tp& __x);
};
template <class _Tp, class _Alloc> void list<_Tp, _Alloc>::push_back(const _Tp& __x)
{
    insert(end(), __x);
}
}

typedef std::_List_node<CFileInstaller::CPendingWrite> Node;

int main()
{
    std::list<CFileInstaller::CPendingWrite> writes;
    std::_List_node_base sentinel;
    sentinel._M_next = &sentinel;
    sentinel._M_prev = &sentinel;
    writes._M_node = &sentinel;
    CBufferChunk chunks[3];
    for (int i = 0; i < 3; ++i) {
        CFileInstaller::CPendingWrite w;
        w.PBufferChunk = &chunks[i];
        w.FilePos = 0x1000u * (i + 1);
        w.WriteLength = 0x200u + i;
        writes.push_back(w);
    }
    std::_List_node_base* n = sentinel._M_next;
    for (int i = 0; i < 3; ++i) {
        Node* node = (Node*)n;
        if (n == &sentinel || node->_M_data.PBufferChunk != &chunks[i] || node->_M_data.FilePos != 0x1000u * (i + 1) ||
            node->_M_data.WriteLength != 0x200u + i || n->_M_next->_M_prev != n) {
            std::printf("009c6af0_TEST FAIL\n"); return 1;
        }
        n = n->_M_next;
    }
    if (n != &sentinel || ((Node*)sentinel._M_prev)->_M_data.PBufferChunk != &chunks[2]) {
        std::printf("009c6af0_TEST FAIL\n"); return 1;
    }
    while (sentinel._M_next != &sentinel) { n = sentinel._M_next; sentinel._M_next = n->_M_next; std::free(n); }
    std::printf("009c6af0_TEST PASS\n");
    return 0;
}