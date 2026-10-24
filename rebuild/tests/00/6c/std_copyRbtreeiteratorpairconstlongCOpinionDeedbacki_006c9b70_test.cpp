// Behaviour test for 0x006C9B70 std::__copy(map iterator range, back_insert_iterator<vector<pair<long, COpinionDeed*> > >):
// walk the red-black tree in order and push_back every (key, deed) pair, growing the vector through _M_insert_overflow.
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#endif
#include <stdlib.h>
#include <new>

class COpinionDeed;

namespace std {

struct input_iterator_tag { };
struct __false_type { __false_type() { } };

template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
    template <class U1, class U2> pair(const pair<U1, U2>& p) : first(p.first), second(p.second) { }
    pair(const pair<T1, T2>& o) : first(o.first), second(o.second) { }
};

template <class T> struct __type_traits { typedef __false_type is_POD_type; };

template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }

template <class T> class allocator { public: allocator() { } };

template <class V, class A> struct _STLP_alloc_proxy : public A {
    V _M_data;
};

template <class T> class vector {
public:
    typedef T value_type;
    typedef typename __type_traits<T>::is_POD_type _IsPODType;
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    _STLP_alloc_proxy<T*, allocator<T> > _M_end_of_storage; // +0x08
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);   // 0x006C80E0
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage._M_data) {
            _Construct(_M_finish, x);
            ++_M_finish;
        }
        else
            _M_insert_overflow(_M_finish, x, _IsPODType(), 1UL, true);
    }
};

template <class T>
__declspec(noinline) void vector<T>::_M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend)
{
    unsigned int old_size = this->_M_finish - this->_M_start;
    unsigned int len = old_size + (old_size > fill_len ? old_size : fill_len);
    T* new_start = (T*)malloc(len * sizeof(T));
    T* new_finish = new_start;
    for (T* p = this->_M_start; p != position; ++p, ++new_finish)
        _Construct(new_finish, *p);
    for (unsigned int i = 0; i < fill_len; ++i, ++new_finish)
        _Construct(new_finish, x);
    if (!atend)
        for (T* p = position; p != this->_M_finish; ++p, ++new_finish)
            _Construct(new_finish, *p);
    if (this->_M_start)
        free(this->_M_start);
    this->_M_start = new_start;
    this->_M_finish = new_finish;
    this->_M_end_of_storage._M_data = new_start + len;
}

template <class Container> class back_insert_iterator {
protected:
    Container* container;
public:
    explicit back_insert_iterator(Container& x) : container(&x) { }
    back_insert_iterator& operator=(const typename Container::value_type& value) { container->push_back(value); return *this; }
    back_insert_iterator& operator*() { return *this; }
    back_insert_iterator& operator++() { return *this; }
};

struct _Rb_tree_node_base {
    unsigned char       _M_color;                           // +0x00
    _Rb_tree_node_base* _M_parent;                          // +0x04
    _Rb_tree_node_base* _M_left;                            // +0x08
    _Rb_tree_node_base* _M_right;                           // +0x0C
};

template <class V> struct _Rb_tree_node : public _Rb_tree_node_base {
    V _M_value_field;                                       // +0x10
};

template <class Dummy> struct _Rb_global {
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* x)
    {
        if (x->_M_right != 0) {
            x = x->_M_right;
            while (x->_M_left != 0)
                x = x->_M_left;
        }
        else {
            _Rb_tree_node_base* y = x->_M_parent;
            while (x == y->_M_right) {
                x = y;
                y = y->_M_parent;
            }
            if (x->_M_right != y)
                x = y;
        }
        return x;
    }
};

struct _Rb_tree_base_iterator {
    _Rb_tree_node_base* _M_node;
    void _M_increment() { _M_node = _Rb_global<bool>::_M_increment(_M_node); }
};

inline bool operator!=(const _Rb_tree_base_iterator& x, const _Rb_tree_base_iterator& y) { return x._M_node != y._M_node; }

template <class V> struct _Rb_tree_iterator : public _Rb_tree_base_iterator {
    _Rb_tree_iterator(_Rb_tree_node_base* x) { _M_node = x; }
    _Rb_tree_iterator(const _Rb_tree_iterator& it) { _M_node = it._M_node; }
    V& operator*() const { return ((_Rb_tree_node<V>*)_M_node)->_M_value_field; }
    _Rb_tree_iterator& operator++() { _M_increment(); return *this; }
};

template <class InputIter, class OutputIter, class Distance>
OutputIter __fastcall __copy(InputIter first, InputIter last, OutputIter result, const input_iterator_tag&, Distance*)
{
    for ( ; first != last; ++result, ++first)
        *result = *first;
    return result;
}

}

typedef std::pair<const long, COpinionDeed*> DeedMapValue;
typedef std::vector<std::pair<long, COpinionDeed*> > DeedVector;


int main()
{
    typedef std::_Rb_tree_node<DeedMapValue> Node;
    std::_Rb_tree_node_base header;
    Node* n[5];
    for (int k = 0; k < 5; ++k) {
        n[k] = (Node*)malloc(sizeof(Node));
        *(long*)&n[k]->_M_value_field.first = 10 * (k + 1);
        n[k]->_M_value_field.second = (COpinionDeed*)(size_t)(0x100 + k);
        n[k]->_M_left = n[k]->_M_right = 0;
    }
    // tree: n2 root; n1 (left) with n0 left child; n3 (right) with n4 right child
    header._M_parent = n[2]; header._M_left = n[0]; header._M_right = n[4];
    n[2]->_M_parent = &header; n[2]->_M_left = n[1]; n[2]->_M_right = n[3];
    n[1]->_M_parent = n[2]; n[1]->_M_left = n[0];
    n[0]->_M_parent = n[1];
    n[3]->_M_parent = n[2]; n[3]->_M_right = n[4];
    n[4]->_M_parent = n[3];
    DeedVector v; v._M_start = v._M_finish = v._M_end_of_storage._M_data = 0;
    std::back_insert_iterator<DeedVector> out(v);
    std::input_iterator_tag tag;
    std::__copy(std::_Rb_tree_iterator<DeedMapValue>(n[0]), std::_Rb_tree_iterator<DeedMapValue>(&header), out, tag, (int*)0);
    int ok = 1;
    if (v._M_finish - v._M_start != 5) ok = 0;
    for (int k = 0; k < 5 && ok; ++k)
        if (v._M_start[k].first != 10 * (k + 1) || v._M_start[k].second != (COpinionDeed*)(size_t)(0x100 + k)) ok = 0;
    printf(ok ? "006c9b70_TEST PASS\n" : "006c9b70_TEST FAIL\n");
    return ok ? 0 : 1;
}