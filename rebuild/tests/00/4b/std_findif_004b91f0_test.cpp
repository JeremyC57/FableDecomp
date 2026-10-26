// behaviour test for 0x004B91F0 (STLport __find_if over a list of strings with a string-pointer predicate)
#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
class CCharStringData { public:                        // the shared string block
    const char*   m_pData;                      // +0x00
    unsigned long m_length;                     // +0x04
};

long __fastcall Compare_00411570(const char* a, const char* b)   // 0x00411570 (the landed body)
{
    for (;;) {
        char ca = *a;
        if (ca == 0 && *b == 0)
            return 0;
        char cb = *b;
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        ++a;
        ++b;
    }
}

class CCharString { public:
    CCharStringData* PStringData;               // +0x00
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->m_length == other.PStringData->m_length)
            return Compare_00411570(PStringData->m_pData, other.PStringData->m_pData) ? false : true;
        return false;
    }
};

// "is this element's string the wanted one?" (one pointer, passed by value)
struct CStringIs {
    const CCharString* m_pWanted;               // +0x00
    bool operator()(const CCharString& s) const { return s == *m_pWanted; }
};

namespace std {
struct input_iterator_tag {};
template <class T> struct _Nonconst_traits {};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> struct _List_node : _List_node_base {
    T _M_data;
};
struct _List_iterator_base {
    _List_node_base* _M_node;
    _List_iterator_base(_List_node_base* x) : _M_node(x) {}
    void _M_incr() { _M_node = _M_node->_M_next; }
    bool operator==(const _List_iterator_base& y) const { return _M_node == y._M_node; }
    bool operator!=(const _List_iterator_base& y) const { return _M_node != y._M_node; }
};
template <class T, class Traits> struct _List_iterator : _List_iterator_base {
    typedef _List_iterator<T, _Nonconst_traits<T> > iterator;
    _List_iterator(_List_node_base* x) : _List_iterator_base(x) {}
    _List_iterator(const iterator& x) : _List_iterator_base(x._M_node) {}
    T& operator*() const { return ((_List_node<T>*)_M_node)->_M_data; }
    _List_iterator& operator++() { _M_incr(); return *this; }
};
template <class InputIter, class Predicate>
InputIter __fastcall __find_if(InputIter first, InputIter last, Predicate pred, const input_iterator_tag&)
{
    while (first != last && !pred(*first))
        ++first;
    return first;
}
typedef _List_iterator<CCharString, _Nonconst_traits<CCharString> > _StringListIter;
}

typedef std::_StringListIter It;
static CCharString Make(CCharStringData* d) { CCharString s; s.PStringData = d; return s; }
int main()
{
    CCharStringData dA = { "alpha", 5 }, dB = { "beta", 4 }, dB2 = { "beta", 4 }, dC = { "gamma", 5 }, dD = { "betb", 4 };
    std::_List_node_base head; std::_List_node<CCharString> n1, n2, n3;
    head._M_next = &n1; n1._M_next = &n2; n2._M_next = &n3; n3._M_next = &head;
    n1._M_data = Make(&dA); n2._M_data = Make(0); n3._M_data = Make(&dB);
    CCharString sB2 = Make(&dB2), sA = Make(&dA), sC = Make(&dC), sD = Make(&dD), sNull = Make(0);
    CStringIs pB2, pA, pC, pD, pNull; pB2.m_pWanted = &sB2; pA.m_pWanted = &sA; pC.m_pWanted = &sC; pD.m_pWanted = &sD; pNull.m_pWanted = &sNull;
    std::input_iterator_tag tag;
    int ok = std::__find_if(It(&n1), It(&head), pB2, tag)._M_node == &n3      // equal text, different block
          && std::__find_if(It(&n1), It(&head), pA, tag)._M_node == &n1       // same block
          && std::__find_if(It(&n1), It(&head), pC, tag)._M_node == &head     // absent
          && std::__find_if(It(&n1), It(&head), pD, tag)._M_node == &head     // same length, other text
          && std::__find_if(It(&n1), It(&head), pNull, tag)._M_node == &n2    // null block equals null block
          && std::__find_if(It(&head), It(&head), pA, tag)._M_node == &head;  // empty range
    if (!ok) { printf("004b91f0_TEST FAIL\n"); return 1; }
    printf("004b91f0_TEST PASS\n");
    return 0;
}