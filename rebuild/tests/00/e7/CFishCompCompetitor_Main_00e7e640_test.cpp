// Behaviour test for CFishCompCompetitor::Main (0x00E7E640): line selection (_SCARED/_INTRO/_FISHCOMMENT_n/_TIP), the
// once-per-prize comment bookkeeping and the attack flagging.
#include <cstdio>
#include <cstring>

struct Parent {
    bool CompetitorsAttacked, CompetitorsJustGotAttacked;
    long AttackCounter;
    bool DoneIntro;
    long PrizeNumber;
    long SizeComment[4];
};
static char g_lines[512];
static int g_frame, g_talkEvery, g_hitAt;
static void SelectLine(Parent* p, long myID, const char* myName)
{
    char line[128];
    std::strcpy(line, "TEXT_QST_B15_FISHER");
    std::strcat(line, myName);
    if (p->AttackCounter > 0)
        std::strcat(line, "_SCARED");
    else if (!p->DoneIntro)
        std::strcat(line, "_INTRO");
    else if (p->SizeComment[myID] < p->PrizeNumber) {
        p->SizeComment[myID] = p->PrizeNumber;
        long* comments = p->SizeComment;
        char num[16];
        std::sprintf(num, "%ld", comments[myID]);
        std::strcat(line, "_FISHCOMMENT_");
        std::strcat(line, num);
    } else
        std::strcat(line, "_TIP");
    std::strcat(g_lines, line);
    std::strcat(g_lines, ";");
}
static void Main(Parent* p, const char* myName)
{
    long myID = myName[0] - '0';
    while (!p->CompetitorsAttacked) {
        ++g_frame;
        if (g_frame > 30) return;
        if (g_frame == g_hitAt) {
            p->CompetitorsAttacked = true;
            p->CompetitorsJustGotAttacked = true;
        } else if (g_frame % g_talkEvery == 0) {
            SelectLine(p, myID, myName);
            if (g_frame == 4) p->DoneIntro = true;
            if (g_frame == 8) p->PrizeNumber = 3;
        }
    }
}
int main()
{
    Parent p; std::memset(&p, 0, sizeof(p));
    g_lines[0] = 0; g_frame = 0; g_talkEvery = 4; g_hitAt = 15;
    Main(&p, "2");
    if (std::strcmp(g_lines, "TEXT_QST_B15_FISHER2_INTRO;TEXT_QST_B15_FISHER2_TIP;TEXT_QST_B15_FISHER2_FISHCOMMENT_3;") != 0
        || !p.CompetitorsJustGotAttacked || p.SizeComment[2] != 3) {
        std::printf("00e7e640_TEST FAIL\n");
        return 1;
    }
    std::printf("00e7e640_TEST PASS\n");
    return 0;
}