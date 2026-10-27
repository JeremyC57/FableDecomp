// behaviour test for 0x00D89DA0 NScript::CQ_HerosOldHouseScript::Init() (model: the listed members are cleared, the others untouched)
#include <cstdio>
#include <cstring>
struct S {
    bool Helping;
    bool Helped;
    bool MissionAborted;
    bool WifeAttacked;
    bool WifeAttackedAndLeft;
    bool BootyDugUp;
    void Init()
    {
        Helping = false;
        Helped = false;
        WifeAttacked = false;
        WifeAttackedAndLeft = false;
        BootyDugUp = false;
    }
};
int main()
{
    S s; std::memset(&s, 0x5A, sizeof(s));
    s.Init();
    int ok = 1;
    if (s.Helping != 0) ok = 0;
    if (s.Helped != 0) ok = 0;
    if (s.WifeAttacked != 0) ok = 0;
    if (s.WifeAttackedAndLeft != 0) ok = 0;
    if (s.BootyDugUp != 0) ok = 0;
    { bool v; std::memset(&v, 0x5A, sizeof(v)); if (std::memcmp(&v, &s.MissionAborted, sizeof(v)) != 0) ok = 0; }
    if (!ok) { std::printf("00d89da0_TEST FAIL\n"); return 1; }
    std::printf("00d89da0_TEST PASS\n");
    return 0;
}