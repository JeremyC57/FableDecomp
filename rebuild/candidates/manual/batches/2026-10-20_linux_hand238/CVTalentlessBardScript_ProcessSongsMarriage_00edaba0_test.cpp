// Behaviour test for CV_TalentlessBardScript::ProcessSongsMarriage (0x00EDABA0): bachelor verse alone when never
// married, otherwise one verse per marriage fact, in MARRIED, DIVORCE, MURDER order; inline push_back growth path.
#include <cstdio>
#include <cstring>
struct CCharString {
    const char* Text;
    CCharString(const char* t = 0) : Text(t) {}
    CCharString(const CCharString& o) : Text(o.Text) {}
};
static int g_overflow = 0;
struct CCharStringVector {
    CCharString* _M_start; CCharString* _M_finish; CCharString* _M_end_of_storage;
    CCharString Store[8];
    void _M_insert_overflow(CCharString* pos, const CCharString& x) { ++g_overflow; *pos = x; _M_finish = pos + 1; _M_end_of_storage = Store + 8; }
    void push_back(const CCharString& x) { if (_M_finish != _M_end_of_storage) { *_M_finish = x; ++_M_finish; } else _M_insert_overflow(_M_finish, x); }
    void push_back_inlined(const CCharString& x) { if (_M_finish != _M_end_of_storage) { *_M_finish = x; ++_M_finish; } else _M_insert_overflow(_M_finish, x); }
    CCharStringVector(int cap) : Store() { _M_start = _M_finish = Store; _M_end_of_storage = Store + cap; }
};
struct CGameScriptInterface {
    bool Married, Current, Divorced, Murdered;
    bool GetHeroHasMarried() const { return Married; }
    bool GetHeroHasCurrentMarriage() const { return Current; }
    bool GetHeroHasDivorcedMarriage() const { return Divorced; }
    bool GetHeroHasMurderedWife() const { return Murdered; }
};
struct CV_TalentlessBardScript {
    CGameScriptInterface* Game;
    CCharStringVector* SongItemsP;
    void ProcessSongsMarriage()
    {
        CCharStringVector& SongItems = *SongItemsP;
        if (!Game->GetHeroHasMarried()) {
            SongItems.push_back_inlined("TEXT_QST_163_WED_BACHELOR");
            return;
        }
        if (Game->GetHeroHasCurrentMarriage())
            SongItems.push_back("TEXT_QST_163_WED_MARRIED");
        if (Game->GetHeroHasDivorcedMarriage())
            SongItems.push_back("TEXT_QST_163_WED_DIVORCE");
        if (Game->GetHeroHasMurderedWife())
            SongItems.push_back("TEXT_QST_163_WED_MURDER");
    }
};
int main()
{
    bool ok = true;
    { CGameScriptInterface g = { false, true, true, true }; CCharStringVector v(0); CV_TalentlessBardScript s = { &g, &v };
      s.ProcessSongsMarriage(); ok = ok && v._M_finish - v._M_start == 1 && std::strcmp(v.Store[0].Text, "TEXT_QST_163_WED_BACHELOR") == 0 && g_overflow == 1; }
    { CGameScriptInterface g = { true, true, false, true }; CCharStringVector v(8); CV_TalentlessBardScript s = { &g, &v };
      s.ProcessSongsMarriage(); ok = ok && v._M_finish - v._M_start == 2 && std::strcmp(v.Store[0].Text, "TEXT_QST_163_WED_MARRIED") == 0
          && std::strcmp(v.Store[1].Text, "TEXT_QST_163_WED_MURDER") == 0; }
    { CGameScriptInterface g = { true, false, true, false }; CCharStringVector v(8); CV_TalentlessBardScript s = { &g, &v };
      s.ProcessSongsMarriage(); ok = ok && v._M_finish - v._M_start == 1 && std::strcmp(v.Store[0].Text, "TEXT_QST_163_WED_DIVORCE") == 0; }
    if (!ok) { std::printf("00edaba0_TEST FAIL\n"); return 1; }
    std::printf("00edaba0_TEST PASS\n");
    return 0;
}
