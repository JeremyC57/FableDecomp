// Behaviour test for 0x0051A040 CSetAndArray_0051a040 default constructor: the set gets a fresh red, self-linked
// header with a null root and zero count, and the array pointers and count after it are all zero.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
struct _Rb_tree_node_base {
    char _M_color; _Rb_tree_node_base* _M_parent; _Rb_tree_node_base* _M_left; _Rb_tree_node_base* _M_right;
};
struct _Rb_tree_node_ulong : _Rb_tree_node_base { unsigned long _M_value_field; };
class CULongSet {
public:
    _Rb_tree_node_ulong* _M_header;
    unsigned long _M_node_count;
    struct less { } _M_key_compare;
    CULongSet() : _M_header(0)
    {
        _M_header = (_Rb_tree_node_ulong*)std::malloc(sizeof(_Rb_tree_node_ulong));
        _M_node_count = 0;
        _M_header->_M_color = 0;
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
};
template <class T> class CArray {
public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
class CSetAndArray_0051a040 {
public:
    CULongSet Keys;
    CArray<unsigned long> Items;
    unsigned long Count;
    CSetAndArray_0051a040();
};
CSetAndArray_0051a040::CSetAndArray_0051a040() : Count(0) {}

int main()
{
    static unsigned char buf[sizeof(CSetAndArray_0051a040)];
    std::memset(buf, 0xAB, sizeof(buf));
    CSetAndArray_0051a040* p = new (buf) CSetAndArray_0051a040;
    _Rb_tree_node_ulong* h = p->Keys._M_header;
    int ok = h != 0 && h->_M_color == 0 && h->_M_parent == 0 && h->_M_left == h && h->_M_right == h
          && p->Keys._M_node_count == 0 && p->Items._M_start == 0 && p->Items._M_finish == 0
          && p->Items._M_end_of_storage == 0 && p->Count == 0;
    CSetAndArray_0051a040 q;
    ok &= q.Keys._M_header != 0 && q.Keys._M_header != h && q.Keys._M_header->_M_left == q.Keys._M_header;
    std::free(h); std::free(q.Keys._M_header);
    if (!ok) { std::printf("0051a040_TEST FAIL\n"); return 1; }
    std::printf("0051a040_TEST PASS\n");
    return 0;
}