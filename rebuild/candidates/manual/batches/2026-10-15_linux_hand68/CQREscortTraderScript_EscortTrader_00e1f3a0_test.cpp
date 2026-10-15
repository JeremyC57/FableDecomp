// Behaviour test for CQR_EscortTraderScript::EscortTrader (0x00E1F3A0): the loop pumps until the
// trader departs or the script thread terminates, then makes one final terminating check.
#include <cstdio>

struct CScript;
struct CInterface {
    CScript* owner;
    int pumps;
    int departAt;
    int terminateAt;
    void Pump();
};

struct CScript {
    CInterface* m_pInterface;
    bool TraderDeparted;
    bool terminating;
    int checks;
    unsigned char IsActiveThreadTerminating() { ++checks; return terminating ? 1 : 0; }
    void EscortTrader()
    {
        while (!TraderDeparted) {
            m_pInterface->Pump();
            if (IsActiveThreadTerminating())
                return;
        }
        IsActiveThreadTerminating();
    }
};

void CInterface::Pump()
{
    ++pumps;
    if (pumps == departAt) owner->TraderDeparted = true;
    if (pumps == terminateAt) owner->terminating = true;
}

static int Run(int departAt, int terminateAt, bool startDeparted, int* checks)
{
    CScript s; CInterface i;
    i.owner = &s; i.pumps = 0; i.departAt = departAt; i.terminateAt = terminateAt;
    s.m_pInterface = &i; s.TraderDeparted = startDeparted; s.terminating = false; s.checks = 0;
    s.EscortTrader();
    *checks = s.checks;
    return i.pumps;
}

int main()
{
    int checks = 0;
    if (Run(0, 0, true, &checks) != 0 || checks != 1) { std::printf("00e1f3a0_TEST FAIL\n"); return 1; }
    if (Run(3, 0, false, &checks) != 3 || checks != 4) { std::printf("00e1f3a0_TEST FAIL\n"); return 1; }
    if (Run(9, 2, false, &checks) != 2 || checks != 2) { std::printf("00e1f3a0_TEST FAIL\n"); return 1; }
    std::printf("00e1f3a0_TEST PASS\n");
    return 0;
}
