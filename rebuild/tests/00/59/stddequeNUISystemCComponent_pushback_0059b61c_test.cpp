// test for 0059b61c deque::push_back
#include <stdio.h>
#include <string.h>
#include <new>
namespace NUISystem { class CComponent; }
namespace std {
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class T> struct _Deque_iterator {
    T*  _M_cur;
    T*  _M_first;
    T*  _M_last;
    T** _M_node;
};
template <class T> class deque {
public:
    _Deque_iterator<T> _M_start;                         // +0x00
    _Deque_iterator<T> _M_finish;                        // +0x10
    T**                _M_map;                           // +0x20
    size_t             _M_map_size;                      // +0x24
    void push_back(const T& t)
    {
        if (_M_finish._M_cur != _M_finish._M_last - 1) {
            _Construct(_M_finish._M_cur, t);
            ++_M_finish._M_cur;
        }
        else
            _M_push_back_aux_v(t);
    }
    void _M_push_back_aux_v(const T& t);                 // 0x0059B576
};
template void deque<NUISystem::CComponent*>::push_back(NUISystem::CComponent* const&);
}

typedef NUISystem::CComponent* P;
static int g_aux; static P g_auxv;
template <> void std::deque<P>::_M_push_back_aux_v(const P& t) { ++g_aux; g_auxv = t; }
int main()
{
    static P block[4];
    std::deque<P> d; memset(&d, 0, sizeof(d));
    d._M_finish._M_cur = block; d._M_finish._M_first = block; d._M_finish._M_last = block + 4;
    P a = (P)0x11, b = (P)0x22, c = (P)0x33, e = (P)0x44;
    d.push_back(a); d.push_back(b); d.push_back(c);
    int ok = block[0] == a && block[1] == b && block[2] == c && d._M_finish._M_cur == block + 3 && g_aux == 0;
    d.push_back(e);
    ok &= g_aux == 1 && g_auxv == e && d._M_finish._M_cur == block + 3;
    printf(ok ? "0059b61c_TEST PASS\n" : "0059b61c_TEST FAIL\n");
    return ok ? 0 : 1;
}