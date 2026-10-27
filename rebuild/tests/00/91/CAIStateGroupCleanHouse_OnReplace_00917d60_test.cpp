// behaviour test for 0x00917D60 CAIStateGroup_CleanHouse::OnReplace() (model; base and scheduler are recording stubs)
#include <cstdio>
static int g_base = 0, g_idle = 0;
class CTCIdleScheduler { public: void ChooseNewIdle() { ++g_idle; } };
class CThing { public: CTCIdleScheduler* s; bool GetInterface(CTCIdleScheduler** out, long type) { if (type != 0xD3 || !s) return false; *out = s; return true; } };
class CAIBrain { public: CThing* PCreature; };
struct CBase { CAIBrain* PBrain; virtual ~CBase() { } virtual void OnReplace() { ++g_base; } };
struct CGroup : CBase {
    virtual void OnReplace()
    {
        CBase::OnReplace();
        CTCIdleScheduler* pScheduler;
        if (PBrain->PCreature->GetInterface(&pScheduler, 0xD3))
            pScheduler->ChooseNewIdle();
    }
};
int main()
{
    CTCIdleScheduler s; CThing t; t.s = &s; CAIBrain b; b.PCreature = &t; CGroup g; g.PBrain = &b;
    g.OnReplace();
    int ok = g_base == 1 && g_idle == 1;
    t.s = 0; g.OnReplace();
    ok = ok && g_base == 2 && g_idle == 1;
    if (!ok) { std::printf("00917d60_TEST FAIL\n"); return 1; }
    std::printf("00917d60_TEST PASS\n");
    return 0;
}