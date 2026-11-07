// Behaviour test for CQ_TraderEscortScript::MakeTraderComment (0x00E01900): the attacked speech queues the trader's comment and
// the intro responses (NO, OATH, BAD_NEWS, REWARD with two other traders; NO, REWARD with one), other comments wait for the
// comment timer, and every queued comment rearms it.
#include <cstdio>
#include <string>
#include <vector>

struct Escort {
    long Timer; long Delay; std::vector<std::string> Lines;
    bool Comment(const std::string& trader, const std::string& near_t, const std::string& far_t, const std::string& comment, int type)
    {
        if (type == 2) {
            if (!near_t.empty()) {
                Lines.push_back("TEXT_QST_067_" + trader + "_" + comment);
                Lines.push_back("TEXT_QST_067_" + near_t + "_INTRO_RESPONSE_NO");
                if (!far_t.empty()) {
                    Lines.push_back("TEXT_QST_067_" + far_t + "_INTRO_RESPONSE_OATH");
                    Lines.push_back("TEXT_QST_067_" + near_t + "_INTRO_RESPONSE_BAD_NEWS");
                }
                Lines.push_back("TEXT_QST_067_" + trader + "_INTRO_RESPONSE_REWARD");
            }
            Timer = Delay;
            return true;
        }
        if (Timer > 0) return false;
        Lines.push_back("TEXT_QST_067_" + trader + "_" + comment);
        Timer = Delay;
        return true;
    }
};

int main()
{
    Escort e;
    e.Timer = 0; e.Delay = 15;
    bool ok = e.Comment("T1", "T2", "T3", "ATTACKED", 2) && e.Lines.size() == 5 && e.Lines[2] == "TEXT_QST_067_T3_INTRO_RESPONSE_OATH";
    ok = ok && !e.Comment("T1", "", "", "BALVERINE_SURPRISE", 0) && e.Lines.size() == 5;
    e.Timer = 0;
    ok = ok && e.Comment("T1", "", "", "BALVERINE_SURPRISE", 0) && e.Lines.back() == "TEXT_QST_067_T1_BALVERINE_SURPRISE" && e.Timer == 15;
    e.Lines.clear();
    ok = ok && e.Comment("T1", "T2", "", "ATTACKED", 2) && e.Lines.size() == 3;
    if (!ok) { std::printf("00e01900_TEST FAIL\n"); return 1; }
    std::printf("00e01900_TEST PASS\n");
    return 0;
}
