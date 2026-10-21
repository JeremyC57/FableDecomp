// Behaviour test for 00d3a9b0 CFH_Prisoner::MakeRoundStartComment: line text by round and mood, done-state bail-outs.
#include <cstdio>
#include <cstring>
static bool Make(long round, long started, long mood, bool done, char* said)
{
    char line[64];
    std::sprintf(line, "TEXT_QST_B01_PROPHET%ld_START", round + 1);
    if (started > 0) {
        if (done) return false;
        if (mood == 1) std::strcat(line, "GOOD");
        else if (mood == 2) std::strcat(line, "DEFAULT");
        else if (mood == 3) std::strcat(line, "BAD");
    }
    std::strcpy(said, line);
    return true;
}
int main()
{
    char said[64];
    bool a = Make(0, 0, 1, false, said); bool okA = a && !std::strcmp(said, "TEXT_QST_B01_PROPHET1_START");
    bool b = Make(2, 1, 3, false, said); bool okB = b && !std::strcmp(said, "TEXT_QST_B01_PROPHET3_STARTBAD");
    bool c = Make(2, 1, 3, true, said);
    bool d = Make(4, 1, 7, false, said); bool okD = d && !std::strcmp(said, "TEXT_QST_B01_PROPHET5_START");
    if (!okA || !okB || c || !okD) { std::printf("00d3a9b0_TEST FAIL\n"); return 1; }
    std::printf("00d3a9b0_TEST PASS\n");
    return 0;
}
