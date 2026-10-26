// Behaviour test for 0x00EDAAD0 CV_TalentlessBardScript::ProcessSongsRenown: renown level -> verse tag (or none).
#include <cstdio>
#include <cstring>
class CCharString { public: char text[40]; CCharString(const char* t, long length = -1) { std::strcpy(text, t); (void)length; } };
struct CCharStringVector {
    int count; char last[40];
    void push_back(const CCharString& x) { ++count; std::strcpy(last, x.text); }
};
class CGameScriptInterface { public: long renown; virtual long GetHeroRenownLevel() { return renown; } };
struct CV_TalentlessBardScript {
    CGameScriptInterface* Game; CCharStringVector SongItems;
    void ProcessSongsRenown()
    {
        switch (Game->GetHeroRenownLevel())
        {
        case 0: case 1:
        case 2: SongItems.push_back("TEXT_QST_163_RENOWN_NONE"); return;
        case 3: case 4:
        case 5: SongItems.push_back("TEXT_QST_163_RENOWN_SOME"); return;
        case 6:
        case 7: SongItems.push_back("TEXT_QST_163_RENOWN_LOTS"); return;
        }
    }
};
int main()
{
    static const char* expect[10] = { "NONE", "NONE", "NONE", "SOME", "SOME", "SOME", "LOTS", "LOTS", 0, 0 };
    CGameScriptInterface g; CV_TalentlessBardScript s; s.Game = &g;
    for (long r = -1; r < 10; ++r) {
        s.SongItems.count = 0; s.SongItems.last[0] = 0; g.renown = r;
        s.ProcessSongsRenown();
        const char* e = (r >= 0) ? expect[r] : 0;
        if (!e) { if (s.SongItems.count != 0) { std::printf("00edaad0_TEST FAIL\n"); return 1; } continue; }
        char want[40]; std::sprintf(want, "TEXT_QST_163_RENOWN_%s", e);
        if (s.SongItems.count != 1 || std::strcmp(s.SongItems.last, want)) { std::printf("00edaad0_TEST FAIL\n"); return 1; }
    }
    std::printf("00edaad0_TEST PASS\n");
    return 0;
}