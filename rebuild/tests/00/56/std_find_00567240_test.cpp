// Behaviour test for 0x00567240 std::__find over list<CWideString> iterators.
#include <cstdio>

namespace stl {
struct input_iterator_tag { };
struct __char_traits_ushort {
    static bool eq(const unsigned short& __c1, const unsigned short& __c2) { return __c1 == __c2; }
    static bool lt(const unsigned short& __c1, const unsigned short& __c2) { return __c1 < __c2; }
    static int compare(const unsigned short* __s1, const unsigned short* __s2, unsigned int __n)
    {
        for (unsigned int __i = 0; __i < __n; ++__i)
            if (!eq(__s1[__i], __s2[__i]))
                return lt(__s1[__i], __s2[__i]) ? -1 : 1;
        return 0;
    }
};
class basic_string_ushort {
public:
    unsigned short* _M_start;
    unsigned short* _M_finish;
    unsigned short* _M_end_of_storage;
};
}
class CWideStringData { public:
    stl::basic_string_ushort Data;
    long NoRefs;
};
class CWideString {
public:
    CWideStringData* PStringData;
    bool operator==(const CWideString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData == 0 || other.PStringData == 0)
            return false;
        const unsigned short* otherBegin = other.PStringData->Data._M_start;
        const unsigned short* begin = PStringData->Data._M_start;
        unsigned int length = (unsigned int)(PStringData->Data._M_finish - begin);
        if (length != (unsigned int)(other.PStringData->Data._M_finish - otherBegin))
            return false;
        if (stl::__char_traits_ushort::compare(begin, otherBegin, length) != 0)
            return false;
        return true;
    }
};
namespace stl {
struct _List_node_base { _List_node_base* _M_next; _List_node_base* _M_prev; };
template <class _Tp> struct _List_node : public _List_node_base { _Tp _M_data; };
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* __x) : _M_node(__x) { }
    _List_iterator_base() { }
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator!=(const _List_iterator_base& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> struct _List_iterator : public _List_iterator_base {
    _List_iterator(_List_node<_Tp>* __x) : _List_iterator_base(__x) { }
    _List_iterator() { }
    _List_iterator(const _List_iterator& __x) : _List_iterator_base(__x._M_node) { }
    _Tp& operator*() const { return ((_List_node<_Tp>*)_M_node)->_M_data; }
    _List_iterator& operator++() { this->_M_incr(); return *this; }
};
template <class _InputIter, class _Tp>
_InputIter __find(_InputIter __first, _InputIter __last, const _Tp& __val, const input_iterator_tag&)
{
    while (__first != __last && !(*__first == __val))
        ++__first;
    return __first;
}
}

static unsigned short t_abc[] = { 'a', 'b', 'c', 0 };
static unsigned short t_abc2[] = { 'a', 'b', 'c', 0 };
static unsigned short t_abd[] = { 'a', 'b', 'd', 0 };
static unsigned short t_ab[] = { 'a', 'b', 0 };

static CWideStringData Make(unsigned short* s, int n)
{
    CWideStringData d;
    d.Data._M_start = s; d.Data._M_finish = s + n; d.Data._M_end_of_storage = s + n + 1; d.NoRefs = 1;
    return d;
}

typedef stl::_List_node<CWideString> Node;
typedef stl::_List_iterator<CWideString> It;

int main()
{
    CWideStringData dAbc = Make(t_abc, 3), dAbc2 = Make(t_abc2, 3), dAbd = Make(t_abd, 3), dAb = Make(t_ab, 2);
    Node header, n0, n1, n2, n3;
    header._M_next = &n0; n0._M_next = &n1; n1._M_next = &n2; n2._M_next = &n3; n3._M_next = &header;
    n0._M_data.PStringData = 0;            // null block: never equal to a live one
    n1._M_data.PStringData = &dAb;         // shorter
    n2._M_data.PStringData = &dAbd;        // same length, different text
    n3._M_data.PStringData = &dAbc;
    It first(&n0), last(&header);
    int ok = 1;
    CWideString v;
    v.PStringData = &dAbc2;                // equal text in a different block
    if (stl::__find(first, last, v, stl::input_iterator_tag())._M_node != &n3) ok = 0;
    v.PStringData = &dAbc;                 // the very same block
    if (stl::__find(first, last, v, stl::input_iterator_tag())._M_node != &n3) ok = 0;
    v.PStringData = 0;                     // null matches the null block
    if (stl::__find(first, last, v, stl::input_iterator_tag())._M_node != &n0) ok = 0;
    v.PStringData = &dAbd;
    if (stl::__find(first, last, v, stl::input_iterator_tag())._M_node != &n2) ok = 0;
    CWideStringData dNone = Make(t_abd, 1); // "a": nowhere
    v.PStringData = &dNone;
    if (stl::__find(first, last, v, stl::input_iterator_tag())._M_node != &header) ok = 0;
    if (stl::__find(last, last, v, stl::input_iterator_tag())._M_node != &header) ok = 0;
    if (!ok) { std::printf("00567240_TEST FAIL\n"); return 1; }
    std::printf("00567240_TEST PASS\n");
    return 0;
}