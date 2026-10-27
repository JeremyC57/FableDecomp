// behaviour test for 0x00CF06D0 NScript::CQ_ArenaHoldingScriptScript::Init() (model: the listed members are cleared, the others untouched)
#include <cstdio>
#include <cstring>
struct S {
    bool PlayerAttackedFans;
    bool FansCanLeaveNow;
    bool OakvaleInsult;
    bool ArenaOver;
    bool InHitCutsceneAlready;
    void Init()
    {
        PlayerAttackedFans = false;
        FansCanLeaveNow = false;
        OakvaleInsult = false;
        ArenaOver = false;
        InHitCutsceneAlready = false;
    }
};
int main()
{
    S s; std::memset(&s, 0x5A, sizeof(s));
    s.Init();
    int ok = 1;
    if (s.PlayerAttackedFans != 0) ok = 0;
    if (s.FansCanLeaveNow != 0) ok = 0;
    if (s.OakvaleInsult != 0) ok = 0;
    if (s.ArenaOver != 0) ok = 0;
    if (s.InHitCutsceneAlready != 0) ok = 0;
    if (!ok) { std::printf("00cf06d0_TEST FAIL\n"); return 1; }
    std::printf("00cf06d0_TEST PASS\n");
    return 0;
}