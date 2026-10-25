// Test for 0x00655B69 std::_List_base<NPlayerGui::CStatBar::CChangeText>::_List_base(const allocator_type&):
// the constructor (from the source object) must allocate a 0x2C-byte sentinel and self-link it.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
inline void* operator new(size_t, void* p) { return p; }

namespace NPlayerGui {
class CStatBar {
public:
    class CChangeText {                                // 36 bytes (Ego_r.pdb)
    public:
        unsigned char _body[0x24];
    };
};
}

namespace std {

template <class _Tp> class allocator {
public:
    allocator() {}
    allocator(const allocator<_Tp>&) {}
    template <class _Tp1> allocator(const allocator<_Tp1>&) {}
    _Tp* allocate(unsigned int n) { return (_Tp*)malloc(n * sizeof(_Tp)); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) {}
};

struct _List_node_base {
    _List_node_base* _M_next;           // +0x00
    _List_node_base* _M_prev;           // +0x04
};

template <class _Tp> struct _List_node : _List_node_base {
    _Tp _M_data;                        // +0x08
};

template <class _Tp, class _Alloc>
class _List_base {
public:
    typedef _List_node<_Tp> _Node;
    typedef _Alloc allocator_type;
    typedef allocator<_Node> _Node_allocator_type;
    _List_base(const allocator_type& __a);
    _STLP_alloc_proxy<_Node*, _Node, _Node_allocator_type> _M_node;   // +0x00 the sentinel
};



}

typedef std::_List_base<NPlayerGui::CStatBar::CChangeText, std::allocator<NPlayerGui::CStatBar::CChangeText> > Base;

int main()
{
    if (sizeof(std::_List_node<NPlayerGui::CStatBar::CChangeText>) != 0x2C) return 1;
    unsigned long raw[2] = { 0xCDCDCDCDu, 0xCDCDCDCDu };
    std::allocator<NPlayerGui::CStatBar::CChangeText> a;
    Base* b = new (raw) Base(a);
    if ((void*)b != (void*)raw) return 2;
    std::_List_node_base* n = b->_M_node._M_data;
    if (!n || n->_M_next != n || n->_M_prev != n) return 3;
    if (raw[1] != 0xCDCDCDCDu) return 4;                 // writes only the 4-byte proxy
    memset(n, 0x5A, 0x2C);                               // the whole 0x2C node is ours
    free(n);
    puts("00655b69_TEST PASS");
    return 0;
}
