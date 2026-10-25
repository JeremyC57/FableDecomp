// Behaviour test for 0x00661287 std::copy(set iterator, set iterator, back_insert_iterator<vector<unsigned long> >):
// copy forwards (first, last, result) to __copy_aux with a __false_type tag and returns the inserter __copy_aux returns.
// __copy_aux is a working stand-in that walks a node list and appends to the vector.
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif

namespace std {

struct __false_type { __false_type() { } };

struct _Rb_tree_node_base { _Rb_tree_node_base* Next; unsigned long Value; };
template <class T> struct _Rb_tree_iterator {
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator() { }
    _Rb_tree_iterator(_Rb_tree_node_base* x) : _M_node(x) { }
};

template <class T> struct vector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
};

template <class C> struct back_insert_iterator {
    C* container;
    explicit back_insert_iterator(C& x) : container(&x) { }
};

typedef _Rb_tree_iterator<unsigned long> TSetIterator;
typedef back_insert_iterator<vector<unsigned long> > TInserter;

static int g_auxCalls = 0;
TInserter __fastcall __copy_aux(TSetIterator first, TSetIterator last, TInserter result, const __false_type& tag)
{
    ++g_auxCalls;
    for (; first._M_node != last._M_node; first._M_node = first._M_node->Next)
        *result.container->_M_finish++ = first._M_node->Value;
    return result;
}

TInserter __fastcall copy(TSetIterator first, TSetIterator last, TInserter result)
{
    return __copy_aux(first, last, result, __false_type());
}

}

int main()
{
    using namespace std;
    _Rb_tree_node_base header, n3, n2, n1;
    n1.Next = &n2; n1.Value = 4; n2.Next = &n3; n2.Value = 8; n3.Next = &header; n3.Value = 15;
    unsigned long storage[8];
    vector<unsigned long> v; v._M_start = storage; v._M_finish = storage; v._M_end_of_storage = storage + 8;
    TInserter out = copy(TSetIterator(&n1), TSetIterator(&header), TInserter(v));
    if (g_auxCalls != 1 || out.container != &v || v._M_finish != storage + 3 || storage[0] != 4 || storage[1] != 8 || storage[2] != 15) {
        std::printf("00661287_TEST FAIL\n"); return 1;
    }
    std::printf("00661287_TEST PASS\n");
    return 0;
}