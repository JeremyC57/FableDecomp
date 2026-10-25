// Test for 0x00654E90 std::__uninitialized_fill_n<CIntelligentPointer<NParticleEngine::CParticleEmitter>*,unsigned int,..>:
// n copies of x through the copy constructor, returns first + n (n == 0: nothing, returns first).
#include <stdio.h>
#include <new>
class CBaseObject;
namespace NParticleEngine { class CParticleEmitter; }
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* PItem;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() { PItem = 0; }
    CIntelligentPointer(const CIntelligentPointer<T>& other);
};
static int g_copies;
CBaseIntelligentPointer::~CBaseIntelligentPointer() { }
template <> CIntelligentPointer<NParticleEngine::CParticleEmitter>::CIntelligentPointer(const CIntelligentPointer<NParticleEngine::CParticleEmitter>& o) { PItem = o.PItem; ++g_copies; }
namespace std {
struct __false_type { __false_type() { } };

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&);
}
typedef CIntelligentPointer<NParticleEngine::CParticleEmitter> P;
int main()
{
    static unsigned char raw[5 * sizeof(P)];
    P* d = (P*)raw;
    P x; x.PItem = (CBaseObject*)0x1234;
    P* r = std::__uninitialized_fill_n(d, 4u, x, std::__false_type());
    if (r != d + 4 || g_copies != 4) return 1;
    for (int k = 0; k < 4; ++k) if (d[k].PItem != (CBaseObject*)0x1234) return 2;
    if (std::__uninitialized_fill_n(d, 0u, x, std::__false_type()) != d || g_copies != 4) return 3;
    puts("00654e90_TEST PASS");
    return 0;
}
