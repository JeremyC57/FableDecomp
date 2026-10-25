// Behaviour test for 0x0051D240: STLport vector<set<unsigned long> >::resize(n) -- resize(n, set()): grow with copies of a
// fresh empty set or truncate, then destroy the temporary set (its header node is freed again).
// The test's set keeps STLport's shape (alloc-proxied header node, node count, clear() via _M_erase) over a simple
// node chain; vector::resize(n, x) is a test copy of the out-of-line 0x0051D030.
#include <cstdio>
#include <cstdlib>
#include <new>
#include <cstring>

static int gHeaders = 0;
static int gNodes = 0;
namespace std {
template <class T> class allocator {
public:
    allocator() { }
    template <class U> allocator(const allocator<U>&) { }
    T* allocate(unsigned int n) { return (T*)std::malloc(n * sizeof(T)); }
    void deallocate(T* p, unsigned int) { if (p != 0) std::free(p); }
};
template <class T> class less {
public:
    bool operator()(const T& a, const T& b) const { return a < b; }
};
struct _Rb_tree_node_base {
    bool                _M_color;
    _Rb_tree_node_base* _M_parent;                      // the test's node chain hangs off the header's parent link
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    V _M_value_field;
};
template <class V, class Tp, class A> class _STLP_alloc_proxy : public A {
public:
    V _M_data;
    _STLP_alloc_proxy(const A& a, V p) : A(a), _M_data(p) { }
};
template <class V, class A> class _Rb_tree_base {
public:
    typedef _Rb_tree_node<V> _Node;
    _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_header;
    _Rb_tree_base(const A& a) : _M_header(allocator<_Node>(a), (_Node*)0)
    {
        _M_header._M_data = _M_header.allocate(1);
        ++gHeaders;
    }
    ~_Rb_tree_base()
    {
        --gHeaders;
        _M_header.deallocate(_M_header._M_data, 1);
    }
};
template <class K, class Compare, class A> class _Rb_tree : public _Rb_tree_base<K, A> {
public:
    typedef _Rb_tree_node<K> _Node;
    unsigned int _M_node_count;
    Compare      _M_key_compare;
    _Rb_tree(const Compare& comp, const A& a) : _Rb_tree_base<K, A>(a), _M_node_count(0), _M_key_compare(comp)
    {
        _M_empty_initialize();
    }
    _Rb_tree(const _Rb_tree& x) : _Rb_tree_base<K, A>(A()), _M_node_count(0), _M_key_compare(x._M_key_compare)
    {
        _M_empty_initialize();
        for (_Rb_tree_node_base* n = x._M_header._M_data->_M_parent; n != 0; n = n->_M_parent)
            insert(((_Node*)n)->_M_value_field);
    }
    ~_Rb_tree() { clear(); }
    void _M_empty_initialize()
    {
        this->_M_header._M_data->_M_color = false;
        this->_M_header._M_data->_M_parent = 0;
        this->_M_header._M_data->_M_left = this->_M_header._M_data;
        this->_M_header._M_data->_M_right = this->_M_header._M_data;
    }
    void _M_erase(_Node* x)                             // test copy of 0x004A9CB0 over the chain
    {
        while (x != 0) {
            _Node* next = (_Node*)x->_M_parent;
            std::free(x);
            --gNodes;
            x = next;
        }
    }
    void clear()
    {
        if (_M_node_count != 0) {
            _M_erase((_Node*)this->_M_header._M_data->_M_parent);
            this->_M_header._M_data->_M_left = this->_M_header._M_data;
            this->_M_header._M_data->_M_parent = 0;
            this->_M_header._M_data->_M_right = this->_M_header._M_data;
            _M_node_count = 0;
        }
    }
    void insert(const K& v)
    {
        _Node* n = (_Node*)std::malloc(sizeof(_Node));
        ++gNodes;
        n->_M_value_field = v;
        n->_M_parent = this->_M_header._M_data->_M_parent;
        this->_M_header._M_data->_M_parent = n;
        ++_M_node_count;
    }
    unsigned int size() const { return _M_node_count; }
};
template <class K, class C = less<K>, class A = allocator<K> > class set {
public:
    _Rb_tree<K, C, A> _M_t;
    set() : _M_t(C(), A()) { }
    unsigned int size() const { return _M_t.size(); }
    void insert(const K& v) { _M_t.insert(v); }
};

template <class _Tp> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp> {
public:
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    void resize(unsigned int __new_size, const _Tp& __x)   // test copy of 0x0051D030
    {
        while (size() > __new_size) {
            --this->_M_finish;
            this->_M_finish->~_Tp();
        }
        if (size() < __new_size) {
            unsigned int __cap = (unsigned int)(this->_M_end_of_storage - this->_M_start);
            if (__cap < __new_size) {
                _Tp* __s = (_Tp*)std::malloc(__new_size * sizeof(_Tp));
                unsigned int __bytes = (unsigned int)((char*)this->_M_finish - (char*)this->_M_start);
                if (__bytes)
                    std::memcpy(__s, this->_M_start, __bytes);  // the test set is relocatable (its header is a pointer)
                std::free(this->_M_start);
                this->_M_finish = (_Tp*)((char*)__s + __bytes);
                this->_M_start = __s;
                this->_M_end_of_storage = __s + __new_size;
            }
            while (size() < __new_size) {
                new (this->_M_finish) _Tp(__x);
                ++this->_M_finish;
            }
        }
    }
    void resize(unsigned int __new_size) { resize(__new_size, _Tp()); }
};
}
typedef std::set<unsigned long> TSet;
typedef std::vector<TSet> TVec;

int main()
{
    TVec v;
    v._M_start = v._M_finish = v._M_end_of_storage = 0;
    v.resize(4);
    bool ok = v.size() == 4 && gHeaders == 4;           // the temporary set's header is gone again
    v._M_start[1].insert(7);
    v._M_start[1].insert(9);
    v.resize(6);
    ok = ok && v.size() == 6 && gHeaders == 6 && v._M_start[1].size() == 2 && v._M_start[5].size() == 0 && gNodes == 2;
    v.resize(2);
    ok = ok && v.size() == 2 && gHeaders == 2 && v._M_start[1].size() == 2;
    v.resize(0);
    ok = ok && v.size() == 0 && gHeaders == 0 && gNodes == 0;
    std::free(v._M_start);
    if (!ok) {
        std::printf("0051d240_TEST FAIL\n");
        return 1;
    }
    std::printf("0051d240_TEST PASS\n");
    return 0;
}