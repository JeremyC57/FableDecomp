// Behaviour test for 0x00A0EA30 _Deque_iterator::operator-(n): the iterator n elements back, the original unchanged.
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <new>
#if !defined(_MSC_VER)
#define __fastcall
#endif

static int g_mallocs = 0, g_frees = 0;
static void* TestMalloc(unsigned int n) { ++g_mallocs; return std::malloc(n); }
static void TestFree(void* p) { ++g_frees; std::free(p); }

class C2DVector {
public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) { }
    C2DVector& Normalise()
    {
        float len = (float)std::sqrt(X * X + Y * Y);
        if (len > 0.0f) { X /= len; Y /= len; }
        return *this;
    }
};

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp*) { }
template <class _Tp> inline void _Destroy(_Tp*, _Tp*) { }

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)TestMalloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) TestFree(__p); }
};

template <class _Tp> struct _Nonconst_traits { typedef _Tp& reference; typedef _Tp* pointer; };
template <class _Tp> struct _Const_traits { typedef const _Tp& reference; typedef const _Tp* pointer; };

template <class _Tp> struct _Deque_iterator_base {
    enum _Constants { _blocksize = 128, __buffer_size = (sizeof(_Tp) < (unsigned int)_blocksize ? ((unsigned int)_blocksize / sizeof(_Tp)) : 1) };
    typedef int difference_type;
    typedef _Tp** _Map_pointer;
    typedef _Deque_iterator_base<_Tp> _Self;

    _Tp*        _M_cur;
    _Tp*        _M_first;
    _Tp*        _M_last;
    _Map_pointer _M_node;

    _Deque_iterator_base() : _M_cur(0), _M_first(0), _M_last(0), _M_node(0) { }
    difference_type _M_subtract(const _Self& __x) const
    {
        return difference_type(__buffer_size) * (_M_node - __x._M_node - 1) + (_M_cur - _M_first) + (__x._M_last - __x._M_cur);
    }
    void _M_increment()
    {
        if (++_M_cur == _M_last) {
            _M_set_node(_M_node + 1);
            _M_cur = _M_first;
        }
    }
    void _M_decrement()
    {
        if (_M_cur == _M_first) {
            _M_set_node(_M_node - 1);
            _M_cur = _M_last;
        }
        --_M_cur;
    }
    void _M_advance(difference_type __n)
    {
        difference_type __offset = __n + (_M_cur - _M_first);
        if (__offset >= 0 && __offset < difference_type(__buffer_size))
            _M_cur += __n;
        else {
            difference_type __node_offset = __offset > 0 ? __offset / __buffer_size : -difference_type((-__offset - 1) / __buffer_size) - 1;
            _M_set_node(_M_node + __node_offset);
            _M_cur = _M_first + (__offset - __node_offset * difference_type(__buffer_size));
        }
    }
    void _M_set_node(_Map_pointer __new_node)
    {
        _M_last = (_M_first = *(_M_node = __new_node)) + difference_type(__buffer_size);
    }
};

template <class _Tp, class _Traits> struct _Deque_iterator : public _Deque_iterator_base<_Tp> {
    typedef typename _Traits::reference reference;
    typedef int difference_type;
    typedef _Deque_iterator<_Tp, _Traits> _Self;
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > _Nonconst_self;

    _Deque_iterator() { }
    _Deque_iterator(const _Nonconst_self& __x) : _Deque_iterator_base<_Tp>(__x) { }

    reference operator*() const { return *this->_M_cur; }
    difference_type operator-(const _Self& __x) const { return this->_M_subtract(__x); }
    _Self& operator++() { this->_M_increment(); return *this; }
    _Self& operator--() { this->_M_decrement(); return *this; }
    _Self& operator+=(difference_type __n) { this->_M_advance(__n); return *this; }
    _Self operator+(difference_type __n) const
    {
        _Self __tmp = *this;
        return __tmp += __n;
    }
    _Self& operator-=(difference_type __n) { return *this += -__n; }
    _Self operator-(difference_type __n) const
    {
        _Self __tmp = *this;
        return __tmp -= __n;
    }
    reference operator[](difference_type __n) const { return *(*this + __n); }
};

template <class _Tp, class _Traits1, class _Traits2>
inline bool __fastcall operator==(const _Deque_iterator<_Tp, _Traits1>& __x, const _Deque_iterator<_Tp, _Traits2>& __y) { return __x._M_cur == __y._M_cur; }
template <class _Tp, class _Traits1, class _Traits2>
inline bool __fastcall operator!=(const _Deque_iterator<_Tp, _Traits1>& __x, const _Deque_iterator<_Tp, _Traits2>& __y) { return __x._M_cur != __y._M_cur; }

template <class _Tp, class _Alloc = allocator<_Tp> > class deque {
public:
    typedef _Tp value_type;
    typedef unsigned int size_type;
    typedef int difference_type;
    typedef _Tp** _Map_pointer;
    typedef _Deque_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _Deque_iterator<_Tp, _Const_traits<_Tp> > const_iterator;

    iterator     _M_start;
    iterator     _M_finish;
    _Map_pointer _M_map;
    size_type    _M_map_size;

    // test fixture helpers (STLport _M_initialize_map / destructor)
    int    m_erases, m_inserts;
    iterator m_eraseFirst, m_eraseLast, m_insertPos;
    size_type m_insertCount;
    const value_type* m_insertValue;
    void Init(size_type __num_elements)
    {
        size_type __num_nodes = __num_elements / buffer_size() + 1;
        _M_map_size = __num_nodes + 2 > 8 ? __num_nodes + 2 : 8;
        _M_map = (_Tp**)TestMalloc(_M_map_size * sizeof(_Tp*));
        for (size_type i = 0; i < _M_map_size; ++i) _M_map[i] = 0;
        _Tp** __nstart = _M_map + (_M_map_size - __num_nodes) / 2;
        _Tp** __nfinish = __nstart + __num_nodes;
        for (_Tp** __cur = __nstart; __cur < __nfinish; ++__cur)
            *__cur = _Alloc().allocate(buffer_size());
        _M_start._M_set_node(__nstart);
        _M_finish._M_set_node(__nfinish - 1);
        _M_start._M_cur = _M_start._M_first;
        _M_finish._M_cur = _M_finish._M_first + __num_elements % buffer_size();
        m_erases = m_inserts = 0;
        for (size_type k = 0; k < __num_elements; ++k)
            *(_M_start + (int)k) = _Tp((float)k, -(float)k);
    }
    void Release()
    {
        for (size_type i = 0; i < _M_map_size; ++i)
            if (_M_map[i]) { TestFree(_M_map[i]); _M_map[i] = 0; }
        TestFree(_M_map);
    }

    static size_type buffer_size() { return (size_type)_Deque_iterator_base<_Tp>::__buffer_size; }
    size_type size() const { return this->_M_finish - this->_M_start; }

    void push_front(const value_type& __t)
    {
        if (this->_M_start._M_cur != this->_M_start._M_first) {
            _Construct(this->_M_start._M_cur - 1, __t);
            --this->_M_start._M_cur;
        }
        else
            _M_push_front_aux_v(__t);
    }
    void pop_back()
    {
        if (this->_M_finish._M_cur != this->_M_finish._M_first) {
            --this->_M_finish._M_cur;
            _Destroy(this->_M_finish._M_cur);
        }
        else
            _M_pop_back_aux();
    }
    iterator erase(iterator __first, iterator __last)                 // test stand-in: records the range
    {
        ++m_erases; m_eraseFirst = __first; m_eraseLast = __last;
        return __first;
    }
    void insert(iterator __pos, size_type __n, const value_type& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const value_type& __x);
    void clear();

    void _M_fill_insert(iterator __pos, size_type __n, const value_type& __x)   // test stand-in: records the request
    {
        ++m_inserts; m_insertPos = __pos; m_insertCount = __n; m_insertValue = &__x;
    }
    void _M_push_front_aux_v(const value_type& __t);
    void _M_pop_back_aux()
    {
        _Alloc().deallocate(this->_M_finish._M_first, this->buffer_size());
        this->_M_finish._M_set_node(this->_M_finish._M_node - 1);
        this->_M_finish._M_cur = this->_M_finish._M_last - 1;
        _Destroy(this->_M_finish._M_cur);
    }
    void _M_reallocate_map(size_type __nodes_to_add, bool __add_at_front)   // STLport body (copy via plain loops)
    {
        size_type __old_num_nodes = this->_M_finish._M_node - this->_M_start._M_node + 1;
        size_type __new_num_nodes = __old_num_nodes + __nodes_to_add;
        _Map_pointer __new_nstart;
        if (this->_M_map_size > 2 * __new_num_nodes) {
            __new_nstart = this->_M_map + (this->_M_map_size - __new_num_nodes) / 2 + (__add_at_front ? __nodes_to_add : 0);
            _Tp* __tmp[64];
            for (size_type i = 0; i < __old_num_nodes; ++i) __tmp[i] = this->_M_start._M_node[i];
            for (size_type j = 0; j < __old_num_nodes; ++j) __new_nstart[j] = __tmp[j];
        }
        else {
            size_type __new_map_size = this->_M_map_size + (this->_M_map_size > __nodes_to_add ? this->_M_map_size : __nodes_to_add) + 2;
            _Map_pointer __new_map = (_Map_pointer)TestMalloc(__new_map_size * sizeof(_Tp*));
            for (size_type i = 0; i < __new_map_size; ++i) __new_map[i] = 0;
            __new_nstart = __new_map + (__new_map_size - __new_num_nodes) / 2 + (__add_at_front ? __nodes_to_add : 0);
            for (size_type j = 0; j < __old_num_nodes; ++j) __new_nstart[j] = this->_M_start._M_node[j];
            TestFree(this->_M_map);
            this->_M_map = __new_map;
            this->_M_map_size = __new_map_size;
        }
        // clear the stale slots outside the moved range so Release() frees each buffer once
        for (size_type k = 0; k < this->_M_map_size; ++k)
            if (this->_M_map + k < __new_nstart || this->_M_map + k >= __new_nstart + __old_num_nodes) this->_M_map[k] = 0;
        this->_M_start._M_set_node(__new_nstart);
        this->_M_finish._M_set_node(__new_nstart + __old_num_nodes - 1);
    }
    void _M_reserve_map_at_back(size_type __nodes_to_add = 1)
    {
        if (__nodes_to_add + 1 > this->_M_map_size - (this->_M_finish._M_node - this->_M_map))
            _M_reallocate_map(__nodes_to_add, false);
    }
    void _M_reserve_map_at_front(size_type __nodes_to_add = 1)
    {
        if (__nodes_to_add > size_type(this->_M_start._M_node - this->_M_map))
            _M_reallocate_map(__nodes_to_add, true);
    }
    iterator _M_reserve_elements_at_front(size_type __n);
    void _M_new_elements_at_front(size_type __new_elements);
    void _M_new_elements_at_back(size_type __new_elements);
};
}
typedef std::deque<C2DVector> Deque;
typedef Deque::iterator It;
typedef Deque::const_iterator CIt;

static bool Is(const C2DVector& v, float k) { return v.X == k && v.Y == -k; }
static bool Same(const C2DVector& v, float x, float y) { return v.X == x && v.Y == y; }
using namespace std;

int main()
{
    Deque d; d.Init(60);
    It it = d._M_start + 45;
    It back = it - 25;
    if (!Is(*back, 20) || !Is(*it, 45)) { std::printf("00a0ea30_TEST FAIL\n"); return 1; }
    It front = it - 45;
    if (front != d._M_start || front._M_node != d._M_start._M_node) { std::printf("00a0ea30_TEST FAIL\n"); return 1; }
    It fwd = it - (-10);
    if (!Is(*fwd, 55)) { std::printf("00a0ea30_TEST FAIL\n"); return 1; }
    if (it - back != 25) { std::printf("00a0ea30_TEST FAIL\n"); return 1; }
    d.Release();
    std::printf("00a0ea30_TEST PASS\n");
    return 0;
}