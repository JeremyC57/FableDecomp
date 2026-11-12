// Behaviour test for 0x00744F70 CTCPerceiveThing::OnKill() (agentFAM05; the catalog test's __fastcall shims do not build under g++
// and it does not link the row). Self-contained on both compilers: the row's body (ResetPerceptionVariables, then the base
// CTCBase::OnKill), checking both run, in that order.
#include <cstdio>
static int Order[2]; static int Calls;
class CTCBase { public: virtual ~CTCBase() {} virtual void OnKill() { Order[Calls++] = 2; } };
class CTCPerceiveThing : public CTCBase {
public:
    virtual void OnKill() { ResetPerceptionVariables(); CTCBase::OnKill(); }
    void ResetPerceptionVariables() { Order[Calls++] = 1; }
};
int main()
{
    CTCPerceiveThing perceive;
    CTCBase& base = perceive;
    base.OnKill();
    bool ok = Calls == 2 && Order[0] == 1 && Order[1] == 2;
    std::printf(ok ? "00744f70_TEST PASS\n" : "00744f70_TEST FAIL\n");
    return ok ? 0 : 1;
}
