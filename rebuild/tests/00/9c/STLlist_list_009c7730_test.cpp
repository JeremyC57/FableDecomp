#include <cstdio>
#include <cstdlib>

static int g_mallocs = 0;
static unsigned int g_lastSize = 0;
static void* test_malloc(unsigned int size) { ++g_mallocs; g_lastSize = size; return std::malloc(size); }

class CFileInstaller {
public:
    class CActiveFile {
    public:
        unsigned int _body[0xB0 / 4];
    };
};

namespace _STL {
template <class _Tp> class allocator {
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)test_malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) std::free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Value __p) : _Alloc(__a), _M_data(__p) {}
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc> class _List_base {
public:
    typedef _Alloc allocator_type;
    _STLP_alloc_proxy<_List_node<_Tp>*, allocator<_List_node<_Tp> > > _M_node;
    _List_base(const allocator_type&) : _M_node(allocator<_List_node<_Tp> >(), (_List_node<_Tp>*)0)
    {
        _List_node<_Tp>* __n = _M_node.allocate(1);
        __n->_M_next = __n;
        __n->_M_prev = __n;
        _M_node._M_data = __n;
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> > class list : public _List_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit list(const allocator_type& __a = allocator_type());
};
template <class _Tp, class _Alloc> list<_Tp, _Alloc>::list(const allocator_type& __a)
    : _List_base<_Tp, _Alloc>(__a)
{
}
}

int main()
{
    _STL::list<CFileInstaller::CActiveFile>* l = new _STL::list<CFileInstaller::CActiveFile>();
    _STL::_List_node<CFileInstaller::CActiveFile>* s = l->_M_node._M_data;
    if (g_mallocs != 1 || s == 0 || s->_M_next != s || s->_M_prev != s ||
        g_lastSize != sizeof(_STL::_List_node<CFileInstaller::CActiveFile>) || sizeof(CFileInstaller::CActiveFile) != 0xB0) {
        std::printf("009c7730_TEST FAIL\n"); return 1;
    }
    _STL::list<CFileInstaller::CActiveFile> l2;
    if (g_mallocs != 2 || l2._M_node._M_data == s || l2._M_node._M_data->_M_next != l2._M_node._M_data) {
        std::printf("009c7730_TEST FAIL\n"); return 1;
    }
    std::free(l2._M_node._M_data);
    std::free(s);
    delete l;
    std::printf("009c7730_TEST PASS\n");
    return 0;
}