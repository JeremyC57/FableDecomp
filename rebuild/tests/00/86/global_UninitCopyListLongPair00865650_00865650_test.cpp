// Behaviour test for 0x00865650 UninitCopy_ListLongPair_00865650: copy-construct {list<T>, long} records into raw storage.
#include <cstdio>
#include <cstdlib>
#include <new>

struct __false_type { __false_type() { } };
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
struct SValue1C { long m_words[7]; };
struct _List_node : _List_node_base { SValue1C _M_data; };
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
        for (int w = 0; w < 7; ++w) n->_M_data.m_words[w] = v + w;
        n->_M_next = h;
        n->_M_prev = h->_M_prev;
        h->_M_prev->_M_next = n;
        h->_M_prev = n;
    }
    SList(const SList& x) : _List_base(x.get_allocator()) { insert(begin(), x.begin(), x.end()); }
};
struct SListLongPair { SList m_list; long m_value; };

static SListLongPair* UninitCopy(const SListLongPair* first, const SListLongPair* last, SListLongPair* result, const __false_type&)
{
    SListLongPair* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) SListLongPair(*first);
    return cur;
}

static long Sum(const SList& l, int* count)
{
    long s = 0;
    *count = 0;
    for (_List_node_base* p = l._M_node._M_data->_M_next; p != l._M_node._M_data; p = p->_M_next) {
        ++*count;
        s += ((_List_node*)p)->_M_data.m_words[0] + ((_List_node*)p)->_M_data.m_words[6];
    }
    return s;
}

int main()
{
    SListLongPair src[2];
    src[0].m_list.push_back(3);
    src[0].m_list.push_back(4);
    src[0].m_value = 77;
    src[1].m_value = -1;
    static double storage[2 * sizeof(SListLongPair) / sizeof(double) + 1];
    SListLongPair* dst = (SListLongPair*)storage;
    __false_type tag;
    SListLongPair* r = UninitCopy(src, src + 2, dst, tag);
    bool ok = r == dst + 2 && dst[0].m_value == 77 && dst[1].m_value == -1;
    int c0 = 0, c1 = 0;
    if (Sum(dst[0].m_list, &c0) != (3 + 9) + (4 + 10) || c0 != 2) ok = false;
    if (Sum(dst[1].m_list, &c1) != 0 || c1 != 0) ok = false;
    if (dst[0].m_list._M_node._M_data == src[0].m_list._M_node._M_data) ok = false;
    if (UninitCopy(src, src, dst, tag) != dst) ok = false;
    if (!ok) { std::printf("00865650_TEST FAIL\n"); return 1; }
    std::printf("00865650_TEST PASS\n");
    return 0;
}