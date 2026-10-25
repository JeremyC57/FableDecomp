// Behaviour test for 0x0043F850 std::__destroy_aux<CIntelligentPointer<NParticleEngine::CParticleEmitter>*>.
#include <cstdio>
#include <cstring>
#define CHECK(c) do { if (!(c)) { std::printf("0043f850_TEST FAIL\n"); return 1; } } while (0)

static int g_dtorCount = 0;
class CBaseObject;
class CBaseIntelligentPointer {
public:
    CBaseObject* PItem;
    CBaseIntelligentPointer() : PItem(0) { }
    virtual ~CBaseIntelligentPointer() { ++g_dtorCount; }
};
namespace NParticleEngine { class CParticleEmitter; }
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
typedef CIntelligentPointer<NParticleEngine::CParticleEmitter> TEmitterPtr;

#include <new>
// the function under test: STLport __destroy_aux(first, last, __false_type)
template <class _ForwardIterator>
void destroy_aux_under_test(_ForwardIterator __first, _ForwardIterator __last)
{
    for (; __first != __last; ++__first)
        (&*__first)->~TEmitterPtr();
}

int main()
{
    int i;
    void* raw = ::operator new(sizeof(TEmitterPtr) * 5);
    TEmitterPtr* p = (TEmitterPtr*)raw;
    for (i = 0; i < 5; ++i) new (p + i) TEmitterPtr();
    g_dtorCount = 0;
    destroy_aux_under_test(p + 1, p + 4);
    CHECK(g_dtorCount == 3);
    destroy_aux_under_test(p + 4, p + 4);
    CHECK(g_dtorCount == 3);
    destroy_aux_under_test(p, p + 1);
    destroy_aux_under_test(p + 4, p + 5);
    CHECK(g_dtorCount == 5);
    ::operator delete(raw);
    std::printf("0043f850_TEST PASS\n");
    return 0;
}