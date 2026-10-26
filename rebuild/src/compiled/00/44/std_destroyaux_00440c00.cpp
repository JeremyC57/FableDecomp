// std::__destroy_aux(_Deque_iterator<pair<CCharString, long> > first, _Deque_iterator<pair<CCharString, long> > last, const __false_type&) -- STLport range destroy over a deque: run each 8-byte element's destructor (its CCharString), stepping to the next 16-element block at each block end
// Leaf: __destroy_aux
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00440C00, length 61 bytes, callee-cleaned `ret 20h` (two 16-byte iterators by value).
// Callees: CCharString::~CCharString @ 0x0099EAE0 (pair<CCharString, long>'s implicit destructor).
// Label: the catalog "Vector_CopyPairRange" is behavioural. Callers 0x004415B4, 0x004425B4, 0x00443310 (deque teardown /
// pop paths; 0x00443310 sits in the deque destructor 0x004432B0 that NPlayerGui::CDrawGuildSeal::Messages, a
// queue<pair<CCharString, long> >, runs) plus a data reference.
// Model: STLport 4.x `__destroy_aux(first, last, const __false_type&)` (the loop `_Destroy(&*first)` per element) emitted
// out of line as `__fastcall` (/Gr): the tag reference rides in ecx and is never read, the two by-value iterators are the
// 0x20 callee-popped bytes. Writing the loop in `_Destroy(first, last)` instead copies `last` into a local (inlined
// by-value parameter) and aligns the loop. The deque iterator is {_M_cur, _M_first, _M_last, _M_node}; `++` steps `_M_cur`
// and at `_M_last` calls `_M_set_node(_M_node + 1)` (buffer = 128 / sizeof(T) = 16 elements = 0x80 bytes) then
// `_M_cur = _M_first`; `!=` compares `_M_cur` only. Speed TU.
class CCharString { public:
    void* m_pStringData;
    ~CCharString();                                // 0x0099EAE0
};

namespace std {
struct __false_type { };
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

template <class T> struct _Deque_iterator {
    T*  _M_cur;
    T*  _M_first;
    T*  _M_last;
    T** _M_node;
    static unsigned int _S_buffer_size() { return sizeof(T) < 128 ? 128 / sizeof(T) : 1; }
    void _M_set_node(T** new_node)
    {
        _M_last = (_M_first = *(_M_node = new_node)) + (int)_S_buffer_size();
    }
    T& operator*() const { return *_M_cur; }
    _Deque_iterator& operator++()
    {
        if (++_M_cur == _M_last) {
            _M_set_node(_M_node + 1);
            _M_cur = _M_first;
        }
        return *this;
    }
    bool operator!=(const _Deque_iterator& other) const { return _M_cur != other._M_cur; }
};

template <class T> inline void _Destroy(T* p) { p->~T(); }

template <class ForwardIter>
void __fastcall __destroy_aux(ForwardIter first, ForwardIter last, const __false_type&)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}

template void __fastcall __destroy_aux<_Deque_iterator<pair<CCharString, long> > >(_Deque_iterator<pair<CCharString, long> > first,
                                                                                 _Deque_iterator<pair<CCharString, long> > last,
                                                                                 const __false_type&);
}