// Behaviour test for 0x00ED9BB0 NScript::CV_TalentlessBardScript::ProcessSongsClothes(): a model; checks the first matching clothing verse is added (nothing, dress, Will robes, armour, apprentice) and nothing when the thread is terminating.
#include <cstdio>
#include <cmath>
#include <vector>
#include <string>
#include <set>
#include <map>

static void Process(std::vector<std::string>& songs, const bool c[5], bool terminating)
{
    static const char* text[5] = { "TEXT_QST_163_WEAR_NOTHING", "TEXT_QST_163_WEAR_DRESS", "TEXT_QST_163_WEAR_WILL", "TEXT_QST_163_WEAR_ARMOUR", "TEXT_QST_163_WEAR_APPRENTICE" };
    for (int i = 0; i < 5; ++i) if (c[i]) { if (terminating) return; songs.push_back(text[i]); return; }
}
int main()
{
    int ok = 1; std::vector<std::string> s;
    bool a[5] = { false, true, true, false, false }; Process(s, a, false); ok &= s.size() == 1 && s[0] == "TEXT_QST_163_WEAR_DRESS";
    bool b[5] = { false, false, false, false, true }; Process(s, b, false); ok &= s.size() == 2 && s[1] == "TEXT_QST_163_WEAR_APPRENTICE";
    Process(s, a, true); bool n[5] = { false, false, false, false, false }; Process(s, n, false); ok &= s.size() == 2;
    std::printf(ok ? "00ED9BB0_TEST PASS\n" : "00ED9BB0_TEST FAIL\n"); return ok ? 0 : 1;
}
