// Behaviour test for _STL::__destroy_aux(NFire::CFirePoint*, NFire::CFirePoint*, const __false_type&) (0x007817B0):
// each fire point is destroyed in order -- the singe-effect pointer, then the effect pointer, then the helper name.
#include <cstdio>
#include <new>

static int gLog[32];
static int gCount = 0;

class CCharString {
public:
    int Id;
    explicit CCharString(int id) : Id(id) { }
    ~CCharString() { gLog[gCount++] = Id; }
};

class CBaseIntelligentPointer {
public:
    int Id;
    explicit CBaseIntelligentPointer(int id) : Id(id) { }
    virtual ~CBaseIntelligentPointer() { gLog[gCount++] = Id; }
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    explicit CIntelligentPointer(int id) : CBaseIntelligentPointer(id) { }
};

class CThing;

namespace NFire {
class CFirePoint {
public:
    CCharString                 HelperName;
    long                        HelperIndex;
    long                        EffectIndex;
    long                        Resistance;
    bool                        Burning;
    long                        FireDamageAfterFrame;
    CThing*                     PParentThing;
    CIntelligentPointer<CThing> PEffect;
    CIntelligentPointer<CThing> PSingeEffect;
    explicit CFirePoint(int b) : HelperName(b), PEffect(b + 1), PSingeEffect(b + 2) { }
};
}

namespace _STL {
struct __false_type { __false_type() { } };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
}

int main()
{
    static double storage[64];
    NFire::CFirePoint* f = (NFire::CFirePoint*)storage;
    for (int i = 0; i < 2; ++i)
        new (&f[i]) NFire::CFirePoint(10 * (i + 1));
    _STL::__destroy_aux(f, f + 2, _STL::__false_type());
    static const int expect[6] = { 12, 11, 10, 22, 21, 20 };
    int fails = 0;
    if (gCount != 6) ++fails;
    for (int i = 0; i < 6 && i < gCount; ++i)
        if (gLog[i] != expect[i]) ++fails;
    if (fails) { std::printf("007817b0_TEST FAIL\n"); return 1; }
    std::printf("007817b0_TEST PASS\n");
    return 0;
}