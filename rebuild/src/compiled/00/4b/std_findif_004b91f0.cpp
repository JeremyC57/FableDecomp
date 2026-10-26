// std::__find_if(first, last, CStringIs pred, const input_iterator_tag&) -- STLport linear search of a list whose elements start with a CCharString for the first one equal to the predicate's string
// Leaf: __find_if
// Retail: Fable.exe @ 0x004B91F0, 96 bytes, `ret 0Ch`: __fastcall with the hidden iterator result in ecx, the empty
// category tag BY REFERENCE in edx (never read), and first / last / a 4-byte predicate on the stack. Call sites:
// 0x004B1698 in CManager::AddQuestCardEntry (0x004B1670: pushes the name's address, `lea edx` for the tag) and
// 0x004B9E90 in the out-of-line STLport find_if(first, last, pred) at 0x004B9E70 (its own predicate word arrives on
// the stack while its edx is unused, so the third argument is a class, not the `const T&` of __find).
// Callees: Compare(const char*, const char*) @ 0x00411570 (__fastcall, the string compare of CCharString ==).
// Shape: the predicate holds a pointer to the wanted string; the inlined CCharString operator== (same block -> equal;
// both blocks non-null with equal length -> Compare == 0, written `Compare(..) ? false : true` for the 8-bit
// `neg ; sbb al,al ; inc al`). The wanted string's block is loaded ONCE before the loop (`mov edi,[eax]`) across the
// Compare call: Compare's real body (landed 0x00411570) sits in the TU under `#pragma auto_inline(off)` so VC7.1 knows
// it writes no memory (the landed 0x0075D2B0 HasMessage model). List node: _M_next +0, _M_prev +4, data +8.
// The empty-range exit keeps its own copy of the epilogue (`mov [ebx],esi ... ret 0Ch` twice).
class CCharStringData { public:                        // the shared string block
    const char*   m_pData;                      // +0x00
    unsigned long m_length;                     // +0x04
};

#pragma auto_inline(off)
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
#pragma auto_inline(on)

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
template _StringListIter __fastcall __find_if(_StringListIter, _StringListIter, CStringIs, const input_iterator_tag&);
}