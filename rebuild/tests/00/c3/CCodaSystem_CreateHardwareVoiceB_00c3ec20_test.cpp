// Behaviour test for CCodaSystem::CreateHardwareVoiceB @ 0x00C3EC20: the system forwards to its audio context's slot 5 (CreateVoiceB) and
// returns that voice unchanged; the other factory is not touched.
#include <cstdio>
class CCodaVoice { public: int is3D; };
class CAudioContext {
public:
    int a, b; CCodaVoice va, vb;
    CAudioContext() : a(0), b(0) { va.is3D = 0; vb.is3D = 1; }
    virtual ~CAudioContext() { }
    virtual CCodaVoice* CreateVoiceA() { ++a; return &va; }
    virtual CCodaVoice* CreateVoiceB() { ++b; return &vb; }
};
class CCodaSystem {
public:
    CAudioContext* m_pContext;
    CCodaVoice* CreateHardwareVoiceA() { return m_pContext->CreateVoiceA(); }
    CCodaVoice* CreateHardwareVoiceB() { return m_pContext->CreateVoiceB(); }
};
int main()
{
    CAudioContext ctx; CCodaSystem sys; sys.m_pContext = &ctx;
    CCodaVoice* v = sys.CreateHardwareVoiceB();
    bool ok = ("B"[0] == 'A') ? (v == &ctx.va && ctx.a == 1 && ctx.b == 0 && v->is3D == 0)
                               : (v == &ctx.vb && ctx.b == 1 && ctx.a == 0 && v->is3D == 1);
    if (!ok) { std::printf("00c3ec20_TEST FAIL\n"); return 1; }
    std::printf("00c3ec20_TEST PASS\n");
    return 0;
}