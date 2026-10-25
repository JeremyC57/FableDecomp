// Behaviour test for 0x00769CB0 std::swap of pair<unsigned long, NParticleEngine::CParticleUpdateParamDesc<unsigned long, unsigned long> >.
#include <cstdio>
#include <cstring>

class CBaseClass {
public:
    static int s_dtors;
    CBaseClass() {}
    virtual ~CBaseClass() { ++s_dtors; }
};
int CBaseClass::s_dtors = 0;
class CParamCallback { public: int Id; };
class CParticleUpdateParamDesc : public CBaseClass {
public:
    CParamCallback* Callback;
    unsigned long   Index;
    CParticleUpdateParamDesc(CParamCallback* c, unsigned long i) : Callback(c), Index(i) {}
};
namespace stlp {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& a, const _T2& b) : first(a), second(b) {}
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& p) : first(p.first), second(p.second) {}
    pair(const pair& p) : first(p.first), second(p.second) {}
};
}
typedef stlp::pair<unsigned long, CParticleUpdateParamDesc> TParamPair;
template <class _Tp> void Swap(_Tp& a, _Tp& b)
{
    _Tp tmp = a;
    a = b;
    b = tmp;
}

int main()
{
    CParamCallback cb1, cb2; cb1.Id = 1; cb2.Id = 2;
    TParamPair a(1, CParticleUpdateParamDesc(&cb1, 10));
    TParamPair b(2, CParticleUpdateParamDesc(&cb2, 20));
    CBaseClass::s_dtors = 0;
    Swap(a, b);
    if (!(a.first == 2 && a.second.Callback == &cb2 && a.second.Index == 20 && b.first == 1 && b.second.Callback == &cb1 && b.second.Index == 10 && CBaseClass::s_dtors == 1)) {
        std::printf("00769cb0_TEST FAIL\n");
        return 1;
    }
    std::printf("00769cb0_TEST PASS\n");
    return 0;
}