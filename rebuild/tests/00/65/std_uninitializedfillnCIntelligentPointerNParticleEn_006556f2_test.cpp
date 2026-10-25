// Test for 0x006556F2 std::uninitialized_fill_n<CIntelligentPointer<NParticleEngine::CParticleEmitter>*,unsigned int,..>:
// the source object's front end must hand (first, n, x) to __uninitialized_fill_n (stubbed) and return its result.
#include <stdio.h>
class CBaseObject;
namespace NParticleEngine { class CParticleEmitter; }
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer() { }
    CBaseObject* PItem;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() { PItem = 0; }
};
typedef CIntelligentPointer<NParticleEngine::CParticleEmitter> P;
static P* g_first; static unsigned int g_n; static const P* g_x; static int g_calls;
namespace std {
struct __false_type { __false_type() { } };
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&);
template <>
P* __fastcall __uninitialized_fill_n(P* __first, unsigned int __n, const P& __x, const __false_type&)
{
    g_first = __first; g_n = __n; g_x = &__x; ++g_calls;
    return __first + __n;
}
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x);
}
int main()
{
    P buf[4]; P x;
    P* r = std::uninitialized_fill_n(buf, 3u, x);
    if (r != buf + 3 || g_first != buf || g_n != 3 || g_x != &x || g_calls != 1) return 1;
    puts("006556f2_TEST PASS");
    return 0;
}
