// Test for 0x006556A8 std::__uninitialized_copy<CIntelligentPointer<NParticleEngine::CParticleEmitter>*,..>: copy-constructs
// [first, last) at result in order, returns result + (last - first).
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

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&);
}
typedef CIntelligentPointer<NParticleEngine::CParticleEmitter> P;
int main()
{
    P src[3];
    for (int k = 0; k < 3; ++k) src[k].PItem = (CBaseObject*)(0x100 + k);
    static unsigned char raw[4 * sizeof(P)];
    P* d = (P*)raw;
    P* r = std::__uninitialized_copy(src, src + 3, d, std::__false_type());
    if (r != d + 3 || g_copies != 3) return 1;
    for (int k = 0; k < 3; ++k) if (d[k].PItem != (CBaseObject*)(0x100 + k)) return 2;
    if (std::__uninitialized_copy(src, src, d, std::__false_type()) != d || g_copies != 3) return 3;
    puts("006556a8_TEST PASS");
    return 0;
}
