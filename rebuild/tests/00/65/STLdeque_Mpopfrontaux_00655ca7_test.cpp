// Behaviour test for 0x00655CA7 std::deque<std::pair<CCharString,long> >::_M_pop_front_aux: popping the last
// element of the front buffer destroys it, frees that buffer and moves the start iterator to the next map node
#include <stdio.h>
#include <stdlib.h>
static int gFrees = 0;
static void* gFreed = 0;
static int gDtors = 0;
static const char* gLastDestroyed = 0;
static void test_free(void* p) { ++gFrees; gFreed = p; free(p); }
class CCharString {
public:
    const char* PStringData;
    ~CCharString();
};
CCharString::~CCharString() { ++gDtors; gLastDestroyed = PStringData; }   // stand-in
namespace std {
template <class _Tp> class allocator {
public:
    void deallocate(_Tp* p, unsigned int) { if (p != 0) test_free(p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _Tp> struct _Deque_iterator {
    typedef _Tp** _Map_pointer;
    static unsigned int _S_buffer_size() { return sizeof(_Tp) < 128 ? (128 / sizeof(_Tp)) : 1; }
    _Tp*         _M_cur;
    _Tp*         _M_first;
    _Tp*         _M_last;
    _Map_pointer _M_node;
    void _M_set_node(_Map_pointer __new_node) {
        _M_last = (_M_first = *(_M_node = __new_node)) + int(_S_buffer_size());
    }
};
template <class _Tp, class _Alloc> class _Deque_base {
public:
    typedef _Deque_iterator<_Tp> iterator;
    static unsigned int buffer_size() { return iterator::_S_buffer_size(); }
    iterator _M_start;
    iterator _M_finish;
    _STLP_alloc_proxy<_Tp**, _Tp*, allocator<_Tp*> > _M_map;
    _STLP_alloc_proxy<unsigned int, _Tp, _Alloc>     _M_map_size;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class deque : public _Deque_base<_Tp, _Alloc> {
public:
    void pop_front() {
        if (this->_M_start._M_cur != this->_M_start._M_last - 1) {
            _Destroy(this->_M_start._M_cur);
            ++this->_M_start._M_cur;
        } else
            _M_pop_front_aux();
    }
    void _M_pop_front_aux();
};
template <class _Tp, class _Alloc>
void deque<_Tp, _Alloc>::_M_pop_front_aux()
{
    _Destroy(this->_M_start._M_cur);
    this->_M_map_size.deallocate(this->_M_start._M_first, this->buffer_size());
    this->_M_start._M_set_node(this->_M_start._M_node + 1);
    this->_M_start._M_cur = this->_M_start._M_first;
}
template <class _T1, class _T2> struct pair { _T1 first; _T2 second; };
}
typedef std::pair<CCharString, long> CMessage;
typedef std::deque<CMessage> CMessageDeque;
static CMessage* gMap[3];
int main()
{
    unsigned int bs = CMessageDeque::buffer_size();
    CMessage* b0 = (CMessage*)malloc(bs * sizeof(CMessage));
    CMessage* b1 = (CMessage*)malloc(bs * sizeof(CMessage));
    unsigned int i;
    for (i = 0; i < bs; ++i) { b0[i].first.PStringData = "old"; b0[i].second = (long)i; b1[i].first.PStringData = "new"; b1[i].second = 0; }
    b0[bs - 1].first.PStringData = "front";
    gMap[0] = b0; gMap[1] = b1; gMap[2] = 0;
    CMessageDeque d;
    d._M_map._M_data = gMap;
    d._M_start._M_set_node(gMap);
    d._M_start._M_cur = b0 + (bs - 1);
    const char* front = b0[bs - 1].first.PStringData;
    d.pop_front();
    bool ok = bs >= 2;
    ok = ok && gDtors == 1 && gLastDestroyed == front;
    ok = ok && gFrees == 1 && gFreed == (void*)b0;
    ok = ok && d._M_start._M_node == gMap + 1 && d._M_start._M_first == b1;
    ok = ok && d._M_start._M_last == b1 + bs && d._M_start._M_cur == b1;
    d.pop_front();
    ok = ok && gDtors == 2 && gFrees == 1 && d._M_start._M_cur == b1 + 1;
    free(b1);
    if (ok) { printf("00655ca7_TEST PASS\n"); return 0; }
    printf("00655ca7_TEST FAIL\n");
    return 1;
}