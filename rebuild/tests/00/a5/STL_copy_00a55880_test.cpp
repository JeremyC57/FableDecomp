// Behaviour test for 0x00A55880 std::__copy(map<CCharString,int> iterators, back_insert_iterator<vector<pair<CCharString,int> > >, ...)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

static int g_live = 0;

class CCharString {
public:
    char* PStringData;
    CCharString(const char* text)
    {
        unsigned int n = (unsigned int)std::strlen(text);
        PStringData = (char*)std::malloc(n + 1);
        std::memcpy(PStringData, text, n + 1);
        ++g_live;
    }
    CCharString(const CCharString& other)                   // 0x0099EC30 stand-in
    {
        unsigned int n = (unsigned int)std::strlen(other.PStringData);
        PStringData = (char*)std::malloc(n + 1);
        std::memcpy(PStringData, other.PStringData, n + 1);
        ++g_live;
    }
    ~CCharString() { std::free(PStringData); --g_live; }    // 0x0099EAE0 stand-in
};

namespace std {
struct input_iterator_tag { input_iterator_tag() { } };
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _T1, class _T2> class pair {
public:
    _T1 first;
    _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) { }
};
struct _Rb_tree_node_base {
    bool _M_color;
    _Rb_tree_node_base* _M_parent;
    _Rb_tree_node_base* _M_left;
    _Rb_tree_node_base* _M_right;
};
template <class _Value> struct _Rb_tree_node : _Rb_tree_node_base {
    _Value _M_value_field;
};
struct _Rb_global {
    static _Rb_tree_node_base* _M_increment(_Rb_tree_node_base* _M_node)
    {
        if (_M_node->_M_right != 0) {
            _M_node = _M_node->_M_right;
            while (_M_node->_M_left != 0)
                _M_node = _M_node->_M_left;
        }
        else {
            _Rb_tree_node_base* __y = _M_node->_M_parent;
            while (_M_node == __y->_M_right) {
                _M_node = __y;
                __y = __y->_M_parent;
            }
            if (_M_node->_M_right != __y)
                _M_node = __y;
        }
        return _M_node;
    }
};
template <class _Value> struct _Rb_tree_iterator {
    typedef _Rb_tree_iterator<_Value> _Self;
    _Rb_tree_node_base* _M_node;
    _Rb_tree_iterator(_Rb_tree_node_base* __x) : _M_node(__x) { }
    _Rb_tree_iterator(const _Self& __it) : _M_node(__it._M_node) { }
    _Value& operator*() const { return ((_Rb_tree_node<_Value>*)_M_node)->_M_value_field; }
    _Self& operator++() { _M_node = _Rb_global::_M_increment(_M_node); return *this; }
    bool operator!=(const _Self& __y) const { return _M_node != __y._M_node; }
};
template <class _Tp> class vector {
public:
    typedef _Tp value_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector()
    {
        for (_Tp* p = _M_start; p != _M_finish; ++p) p->~_Tp();
        std::free(_M_start);
    }
    // stand-in for 0x006FC130: grow and append fill_len copies of x
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool)
    {
        unsigned int size = (unsigned int)(_M_finish - _M_start);
        unsigned int cap = size ? size * 2 : 1;
        if (cap < size + __fill_len) cap = size + __fill_len;
        _Tp* mem = (_Tp*)std::malloc(cap * sizeof(_Tp));
        _Tp* out = mem;
        for (_Tp* p = _M_start; p != __position; ++p, ++out) _Construct(out, *p);
        for (unsigned int i = 0; i < __fill_len; ++i, ++out) _Construct(out, __x);
        for (_Tp* p = _M_start; p != _M_finish; ++p) p->~_Tp();
        std::free(_M_start);
        _M_start = mem;
        _M_finish = out;
        _M_end_of_storage = mem + cap;
    }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};
template <class _Container> class back_insert_iterator {
protected:
    _Container* container;
public:
    explicit back_insert_iterator(_Container& __x) : container(&__x) { }
    back_insert_iterator<_Container>& operator=(const typename _Container::value_type& __value)
    {
        container->push_back(__value);
        return *this;
    }
    back_insert_iterator<_Container>& operator*() { return *this; }
    back_insert_iterator<_Container>& operator++() { return *this; }
};
template <class _InputIter, class _OutputIter, class _Distance>
_OutputIter __copy(_InputIter __first, _InputIter __last, _OutputIter __result, const input_iterator_tag&, _Distance*)
{
    for (; __first != __last; ++__result, ++__first)
        *__result = *__first;
    return __result;
}
}   // namespace std

typedef std::pair<const CCharString, int> CMapValue;
typedef std::_Rb_tree_node<CMapValue> CNode;
typedef std::_Rb_tree_iterator<CMapValue> CIter;
typedef std::vector<std::pair<CCharString, int> > CVec;

int main()
{
    bool ok = true;
    {
        // header + a four-node tree: bat (root) / ant, cow (emu): in order ant, bat, cow, emu
        CNode* n[4];
        const char* keys[4] = { "ant", "bat", "cow", "emu" };
        for (int i = 0; i < 4; ++i) {
            n[i] = (CNode*)std::malloc(sizeof(CNode));
            new (&n[i]->_M_value_field) CMapValue(CCharString(keys[i]), (i + 1) * 100);
            n[i]->_M_color = true;
            n[i]->_M_left = n[i]->_M_right = 0;
        }
        std::_Rb_tree_node_base header;
        header._M_color = false;
        header._M_parent = n[1]; header._M_left = n[0]; header._M_right = n[3];
        n[1]->_M_parent = &header; n[1]->_M_left = n[0]; n[1]->_M_right = n[2];
        n[0]->_M_parent = n[1];
        n[2]->_M_parent = n[1]; n[2]->_M_right = n[3];
        n[3]->_M_parent = n[2];
        CVec v;
        std::back_insert_iterator<CVec> r = std::__copy(CIter(header._M_left), CIter(&header),
                                                          std::back_insert_iterator<CVec>(v),
                                                          std::input_iterator_tag(), (int*)0);
        r = std::pair<CCharString, int>(CCharString("fly"), 7);    // the returned inserter still targets v
        ok = v._M_finish - v._M_start == 5;
        for (int i = 0; ok && i < 4; ++i)
            ok = std::strcmp(v._M_start[i].first.PStringData, keys[i]) == 0 && v._M_start[i].second == (i + 1) * 100;
        ok = ok && std::strcmp(v._M_start[4].first.PStringData, "fly") == 0;
        CVec empty;                                                 // first == last copies nothing
        std::__copy(CIter(&header), CIter(&header), std::back_insert_iterator<CVec>(empty), std::input_iterator_tag(),
                     (int*)0);
        ok = ok && empty._M_start == empty._M_finish;
        for (int i = 0; i < 4; ++i) { n[i]->_M_value_field.~CMapValue(); std::free(n[i]); }
    }
    ok = ok && g_live == 0;                                         // every temporary pair was destroyed
    if (!ok) { std::printf("00a55880_TEST FAIL\n"); return 1; }
    std::printf("00a55880_TEST PASS\n");
    return 0;
}