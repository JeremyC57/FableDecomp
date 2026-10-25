// Behaviour test for 00c1d8c0: CScriptModuleTimer::UpdateState -- -1 without a dispatch target; else the target's Dispatch(remaining, total, frame, value, argument, mode) mapped to 0 (handled) / -1
#include <cstdio>

struct CScriptContext { long m_base; };
struct CScriptArgument {
    long m_value;
    long GetValue(CScriptContext* context) { return m_value + context->m_base; }
};

struct CDispatchTarget {
    bool m_result; int m_calls;
    unsigned int a, b, c; long d, f; CScriptArgument* e;
    virtual void v00() { }
    virtual bool Dispatch(unsigned int remaining, unsigned int total, unsigned int frame, long value, CScriptArgument* argument, long mode)
    { ++m_calls; a = remaining; b = total; c = frame; d = value; e = argument; f = mode; return m_result; }
};
struct CModeSource { long m_mode; };
struct CModeOwner { CModeSource* m_pSource; };
struct CScriptModuleOwner { void* _pad00; CDispatchTarget* m_pTarget; CModeOwner* m_pModeOwner; };

struct CScriptModuleTimer {
    CScriptModuleOwner* POwner;
    unsigned int GetCurrentFrame() { return 12; }
    unsigned int GetRemainingTimeSeconds() { return 3; }
    unsigned int GetTotalTimeSeconds() { return 10; }
    long UpdateState(CScriptContext* context, CScriptArgument* argument)
    {
        if (!POwner->m_pTarget)
            return -1;
        bool handled = POwner->m_pTarget->Dispatch(GetRemainingTimeSeconds(), GetTotalTimeSeconds(), GetCurrentFrame(),
                                                   argument->GetValue(context), argument,
                                                   POwner->m_pModeOwner->m_pSource->m_mode);
        return handled ? 0 : -1;
    }
};

int main()
{
    CModeSource src = { 7 }; CModeOwner mo = { &src };
    CDispatchTarget target; target.m_calls = 0; target.m_result = true;
    CScriptModuleOwner owner = { 0, 0, &mo };
    CScriptModuleTimer t; t.POwner = &owner;
    CScriptContext ctx = { 100 }; CScriptArgument arg = { 5 };
    if (t.UpdateState(&ctx, &arg) != -1 || target.m_calls != 0) { std::printf("00c1d8c0_TEST FAIL\n"); return 1; }
    owner.m_pTarget = &target;
    if (t.UpdateState(&ctx, &arg) != 0 || target.m_calls != 1) { std::printf("00c1d8c0_TEST FAIL\n"); return 1; }
    if (target.a != 3 || target.b != 10 || target.c != 12 || target.d != 105 || target.e != &arg || target.f != 7) {
        std::printf("00c1d8c0_TEST FAIL\n"); return 1;
    }
    target.m_result = false;
    if (t.UpdateState(&ctx, &arg) != -1 || target.m_calls != 2) { std::printf("00c1d8c0_TEST FAIL\n"); return 1; }
    std::printf("00c1d8c0_TEST PASS\n");
    return 0;
}