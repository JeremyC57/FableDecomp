// Behaviour test for 0x00493C25 std::deque<CSubtitleRenderer::CSetTextInfo>::_M_pop_front_aux: popping the last
// element of the front buffer destroys it, frees that buffer and moves the start iterator to the next map node
#include <stdio.h>
#include <stdlib.h>
static int gFrees = 0;
static void* gFreed = 0;
static int gDtors = 0;
static unsigned long gLastDestroyed = 0;
static void test_free(void* p) { ++gFrees; gFreed = p; free(p); }
struct CWideString { void* PStringData; };
class CSubtitleRenderer {
public:
    class CSetTextInfo {
    public:
        CWideString   Text;
        CWideString   Button1Text;
        CWideString   Button2Text;
        CWideString   Button3Text;
        unsigned long DisplayFlags;
        float         DisplayTime;
        bool          UpdateWhilePaused;
        bool          ChangePlayerMode;
        ~CSetTextInfo();
    };
};
CSubtitleRenderer::CSetTextInfo::~CSetTextInfo() { ++gDtors; gLastDestroyed = DisplayFlags; }   // stand-in
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
}
typedef CSubtitleRenderer::CSetTextInfo CInfo;
typedef std::deque<CInfo> CInfoDeque;
static CInfo* gMap[3];
int main()
{
    unsigned int bs = CInfoDeque::buffer_size();
    CInfo* b0 = (CInfo*)malloc(bs * sizeof(CInfo));
    CInfo* b1 = (CInfo*)malloc(bs * sizeof(CInfo));
    unsigned int i;
    for (i = 0; i < bs; ++i) { b0[i].DisplayFlags = 100 + i; b1[i].DisplayFlags = 200 + i; }
    gMap[0] = b0; gMap[1] = b1; gMap[2] = 0;
    CInfoDeque d;
    d._M_map._M_data = gMap;
    d._M_start._M_set_node(gMap);
    d._M_start._M_cur = b0 + (bs - 1);          // one element left in the front buffer
    d.pop_front();
    bool ok = bs >= 2;
    ok = ok && gDtors == 1 && gLastDestroyed == 100 + (bs - 1);
    ok = ok && gFrees == 1 && gFreed == (void*)b0;
    ok = ok && d._M_start._M_node == gMap + 1 && d._M_start._M_first == b1;
    ok = ok && d._M_start._M_last == b1 + bs && d._M_start._M_cur == b1;
    d.pop_front();                               // the inline fast path: destroy and step, no free
    ok = ok && gDtors == 2 && gLastDestroyed == 200 && gFrees == 1 && d._M_start._M_cur == b1 + 1;
    free(b1);
    if (ok) { printf("00493c25_TEST PASS\n"); return 0; }
    printf("00493c25_TEST FAIL\n");
    return 1;
}