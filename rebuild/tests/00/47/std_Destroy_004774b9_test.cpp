// Behaviour test for 0x004774B9 std::_Destroy<CEngineLocalDetailObjectDef*>: runs the (virtual) destructor of every
// element of [first, last) in order without freeing; an empty range does nothing.
#include <cstdio>
#include <new>
static int g_order[8];
static int g_count = 0;
class CEngineLocalDetailObjectDef {
public:
    int Id;
    explicit CEngineLocalDetailObjectDef(int id) : Id(id) { }
    virtual ~CEngineLocalDetailObjectDef() { g_order[g_count++] = Id; }
    unsigned char _body[0x84 - sizeof(int)];
};
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{ for ( ; __first != __last; ++__first) _Destroy(&*__first); }
template <class _ForwardIterator, class _Tp> inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{ typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor; __destroy_aux(__first, __last, _Trivial_destructor()); }
template <class _ForwardIterator> void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{ __destroy(__first, __last, __value_type(__first)); }
}
int main()
{
    static unsigned char raw[3 * sizeof(CEngineLocalDetailObjectDef)];
    CEngineLocalDetailObjectDef* p = (CEngineLocalDetailObjectDef*)raw;
    for (int i = 0; i < 3; ++i) new (p + i) CEngineLocalDetailObjectDef(10 + i);
    std::_Destroy(p, p);
    if (g_count != 0) { std::printf("004774b9_TEST FAIL\n"); return 1; }
    std::_Destroy(p, p + 3);
    if (sizeof(CEngineLocalDetailObjectDef) != 0x88 || g_count != 3 || g_order[0] != 10 || g_order[2] != 12) { std::printf("004774b9_TEST FAIL\n"); return 1; }
    std::printf("004774b9_TEST PASS\n");
    return 0;
}
