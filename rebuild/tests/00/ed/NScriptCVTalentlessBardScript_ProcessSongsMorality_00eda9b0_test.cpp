// Behaviour test for CV_TalentlessBardScript::ProcessSongsMorality (0x00EDA9B0): category -> verse tag, none out of range.
#include <cstdio>
#include <cstring>
struct CCharString {
    const char* Text;
    CCharString(const char* t) : Text(t) {}
};
struct CCharStringVector {
    const char* Items[8]; int Count;
    void push_back(const CCharString& x) { Items[Count++] = x.Text; }
};
struct CGameScriptInterface { long Morality; long GetHeroMoralityCategory() const { return Morality; } };
struct CV_TalentlessBardScript {
    CGameScriptInterface* Game;
    CCharStringVector SongItems;
    void ProcessSongsMorality()
    {
        switch (Game->GetHeroMoralityCategory())
        {
        case 0: SongItems.push_back("TEXT_QST_163_MORAL_EVIL"); return;
        case 1:
        case 2: SongItems.push_back("TEXT_QST_163_MORAL_BAD"); return;
        case 3: SongItems.push_back("TEXT_QST_163_MORAL_NEUT"); return;
        case 4:
        case 5: SongItems.push_back("TEXT_QST_163_MORAL_GOOD"); return;
        case 6: SongItems.push_back("TEXT_QST_163_MORAL_PERFECT"); return;
        }
    }
};
static const char* run(long m)
{
    CGameScriptInterface g; g.Morality = m;
    CV_TalentlessBardScript s; s.Game = &g; s.SongItems.Count = 0;
    s.ProcessSongsMorality();
    return s.SongItems.Count == 1 ? s.SongItems.Items[0] : (s.SongItems.Count == 0 ? "" : "?");
}
int main()
{
    const char* want[9] = { "TEXT_QST_163_MORAL_EVIL", "TEXT_QST_163_MORAL_BAD", "TEXT_QST_163_MORAL_BAD", "TEXT_QST_163_MORAL_NEUT",
        "TEXT_QST_163_MORAL_GOOD", "TEXT_QST_163_MORAL_GOOD", "TEXT_QST_163_MORAL_PERFECT", "", "" };
    bool ok = true;
    for (long m = 0; m < 9; ++m) ok = ok && std::strcmp(run(m), want[m]) == 0;
    ok = ok && std::strcmp(run(-1), "") == 0;
    if (!ok) { std::printf("00eda9b0_TEST FAIL\n"); return 1; }
    std::printf("00eda9b0_TEST PASS\n");
    return 0;
}