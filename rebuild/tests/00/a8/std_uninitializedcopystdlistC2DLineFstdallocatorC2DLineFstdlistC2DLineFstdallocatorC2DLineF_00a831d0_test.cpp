// Behaviour test for 0x00A831D0 UninitCopy_List16_00a831d0: copy-construct STLport lists into raw storage
// (fresh self-linked header, then insert the source range before begin()).
#include <cstdio>
#include <cstdlib>
#include <new>

struct __false_type { __false_type() { } };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
struct SValue16 { long m_0, m_4, m_8, m_c; };
struct _List_node : _List_node_base { SValue16 _M_data; };
struct _List_iterator_base { _List_node_base* _M_node; _List_iterator_base(_List_node_base* x) : _M_node(x) { } };
struct iterator : _List_iterator_base {
    iterator(_List_node_base* x) : _List_iterator_base(x) { }
    iterator(const iterator& x) : _List_iterator_base(x._M_node) { }
};
struct const_iterator : _List_iterator_base { const_iterator(_List_node_base* x) : _List_iterator_base(x) { } };
template <class T> struct allocator { allocator() { } };
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
    _STLP_alloc_proxy(const allocator<T>& a, T* p) : allocator<T>(a), _M_data(p) { }
};
struct _List_base {
    _STLP_alloc_proxy<_List_node> _M_node;
    _List_base(const allocator<_List_node>& a) : _M_node(a, 0)
    {
        _List_node* n = (_List_node*)std::malloc(sizeof(_List_node));
        n->_M_next = n;
        n->_M_prev = n;
        _M_node._M_data = n;
    }
};
struct SList : _List_base {
    SList() : _List_base(allocator<_List_node>()) { }
    allocator<_List_node> get_allocator() const { return allocator<_List_node>(); }
    iterator begin() { return iterator(_M_node._M_data->_M_next); }
    const_iterator begin() const { return const_iterator(_M_node._M_data->_M_next); }
    const_iterator end() const { return const_iterator(_M_node._M_data); }
    void _M_insert_dispatch(iterator pos, const_iterator first, const_iterator last, const __false_type&)
    {
        for (_List_node_base* s = first._M_node; s != last._M_node; s = s->_M_next) {
            _List_node* n = (_List_node*)std::malloc(sizeof(_List_node));
            n->_M_data = ((_List_node*)s)->_M_data;
            n->_M_next = pos._M_node;
            n->_M_prev = pos._M_node->_M_prev;
            pos._M_node->_M_prev->_M_next = n;
            pos._M_node->_M_prev = n;
        }
    }
    void insert(iterator pos, const_iterator first, const_iterator last) { _M_insert_dispatch(pos, first, last, __false_type()); }
    void push_back(long v)
    {
        _List_node_base* h = _M_node._M_data;
        _List_node* n = (_List_node*)std::malloc(sizeof(_List_node));
        n->_M_data.m_0 = v; n->_M_data.m_4 = v + 1; n->_M_data.m_8 = v + 2; n->_M_data.m_c = v + 3;
        n->_M_next = h;
        n->_M_prev = h->_M_prev;
        h->_M_prev->_M_next = n;
        h->_M_prev = n;
    }
    SList(const SList& x) : _List_base(x.get_allocator()) { insert(begin(), x.begin(), x.end()); }
};

static SList* UninitCopy(const SList* first, const SList* last, SList* result, const __false_type&)
{
    SList* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) SList(*first);
    return cur;
}

static int Count(const SList& l, long* sum)
{
    int n = 0;
    *sum = 0;
    for (_List_node_base* p = l._M_node._M_data->_M_next; p != l._M_node._M_data; p = p->_M_next) {
        ++n;
        *sum += ((_List_node*)p)->_M_data.m_0 + ((_List_node*)p)->_M_data.m_c;
    }
    return n;
}

int main()
{
    SList src[3];
    src[0].push_back(1);
    src[0].push_back(5);
    src[2].push_back(9);
    static SList* storage[3];
    SList* dst = (SList*)storage;
    __false_type tag;
    SList* r = UninitCopy(src, src + 3, dst, tag);
    bool ok = r == dst + 3;
    long s0 = 0, s1 = 0, s2 = 0;
    if (Count(dst[0], &s0) != 2 || s0 != (1 + 4) + (5 + 8)) ok = false;
    if (Count(dst[1], &s1) != 0 || s1 != 0) ok = false;
    if (Count(dst[2], &s2) != 1 || s2 != 9 + 12) ok = false;
    if (dst[0]._M_node._M_data == src[0]._M_node._M_data || dst[0]._M_node._M_data->_M_next == src[0]._M_node._M_data->_M_next) ok = false;
    if (UninitCopy(src, src, dst, tag) != dst) ok = false;
    if (!ok) { std::printf("00a831d0_TEST FAIL\n"); return 1; }
    std::printf("00a831d0_TEST PASS\n");
    return 0;
}