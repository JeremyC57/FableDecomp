// behaviour test for 0x009D7C00 std::list<CCountedPointer<CBankFileAsyncData> >::erase (model copy of the source; the node/count block are heap)
#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#endif
static int g_freed = 0, g_deleted = 0;
static void my_free(void* p) { ++g_freed; std::free(p); }
#define free my_free
class CCPPointerInfo {
public:
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                ::operator delete(Info);
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CBankFileAsyncData;
namespace std {
template <class _Tp> class allocator { public: allocator() { } void deallocate(_Tp* __p, unsigned int) { free(__p); } };
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc { public: _Value _M_data; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
struct _List_iterator_base { _List_node_base* _M_node; _List_iterator_base(_List_node_base* __x) : _M_node(__x) { } _List_iterator_base() { } };
template <class _Tp> struct _Nonconst_traits { };
template <class _Tp, class _Traits> struct _List_iterator : public _List_iterator_base {
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    typedef _List_node<_Tp> _Node;
    _List_iterator(_Node* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const iterator& __x) : _List_iterator_base(__x._M_node) { }
};
template <class _Tp, class _Alloc> class _List_base { public: typedef _List_node<_Tp> _Node; _STLP_alloc_proxy<_Node*, _Node, allocator<_Node> > _M_node; };
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _List_node<_Tp> _Node;
    typedef _List_iterator<_Tp, _Nonconst_traits<_Tp> > iterator;
    iterator erase(iterator __position)
    {
        _List_node_base* __next_node = __position._M_node->_M_next;
        _List_node_base* __prev_node = __position._M_node->_M_prev;
        _Node* __n = (_Node*)__position._M_node;
        __prev_node->_M_next = __next_node;
        __next_node->_M_prev = __prev_node;
        _Destroy(&__n->_M_data);
        this->_M_node.deallocate(__n, 1);
        return iterator((_Node*)__next_node);
    }
};
}
typedef CCountedPointer<CBankFileAsyncData > TPtr;
typedef std::_List_node<TPtr> TNode;
static void __fastcall Deleter(void* data) { ++g_deleted; }
int main()
{
    std::list<TPtr> l;
    TNode sentinel; sentinel._M_next = &sentinel; sentinel._M_prev = &sentinel; sentinel._M_data.Data = 0; sentinel._M_data.Info = 0;
    l._M_node._M_data = &sentinel;
    TNode* a = (TNode*)std::malloc(sizeof(TNode)); TNode* b = (TNode*)std::malloc(sizeof(TNode));
    CCPPointerInfo* ia = new CCPPointerInfo; ia->RefCount = 1; ia->DeleteFunc = Deleter; ia->Data = 0;
    CCPPointerInfo* ib = new CCPPointerInfo; ib->RefCount = 2; ib->DeleteFunc = Deleter; ib->Data = 0;
    a->_M_data.Data = 0; a->_M_data.Info = ia; b->_M_data.Data = 0; b->_M_data.Info = ib;
    sentinel._M_next = a; a->_M_prev = &sentinel; a->_M_next = b; b->_M_prev = a; b->_M_next = &sentinel; sentinel._M_prev = b;
    std::list<TPtr>::iterator it = l.erase(std::list<TPtr>::iterator(a));
    int ok = it._M_node == b && sentinel._M_next == b && b->_M_prev == &sentinel && g_deleted == 1 && g_freed == 1;
    it = l.erase(std::list<TPtr>::iterator(b));                 // b's block survives (2 -> 1)
    ok = ok && it._M_node == &sentinel && sentinel._M_next == &sentinel && sentinel._M_prev == &sentinel
            && ib->RefCount == 1 && g_deleted == 1 && g_freed == 2;
    if (!ok) { std::printf("009d7c00_TEST FAIL\n"); return 1; }
    std::printf("009d7c00_TEST PASS\n");
    return 0;
}