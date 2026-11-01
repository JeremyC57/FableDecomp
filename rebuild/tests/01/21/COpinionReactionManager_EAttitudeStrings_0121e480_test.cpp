// Behaviour test for the dynamic initializer of COpinionReactionManager::AttitudeStrings (0x0121E480): a static array of
// eighteen CCharStrings built from the attitude names in retail order.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
static long g_liveStrings = 0;
class CCharString { public:
    CCharString(const char* text, long length = -1) { PStringData = (length < 0) ? strdup(text) : strdup(text); ++g_liveStrings; }
    ~CCharString() { free(PStringData); --g_liveStrings; }
    char* PStringData;
};
class COpinionReactionManager { public: static CCharString AttitudeStrings[18]; };
CCharString COpinionReactionManager::AttitudeStrings[18] = {
    "NONE", "RESPECT", "AWE", "DISDAIN", "FEAR", "FRIENDLY", "WORSHIP", "RIDICULOUS", "OFFENSIVE",
    "AGREEABLE", "UGLY", "ATTRACTED", "LOVE", "LOVE", "LIKE", "NEUTRAL", "DISLIKE", "HATE"
};
int main()
{
    static const char* expected[18] = { "NONE", "RESPECT", "AWE", "DISDAIN", "FEAR", "FRIENDLY", "WORSHIP", "RIDICULOUS", "OFFENSIVE",
        "AGREEABLE", "UGLY", "ATTRACTED", "LOVE", "LOVE", "LIKE", "NEUTRAL", "DISLIKE", "HATE" };
    if (g_liveStrings != 18) { printf("0121e480_TEST FAIL live=%ld\n", g_liveStrings); return 1; }
    for (int i = 0; i < 18; ++i)
        if (strcmp(COpinionReactionManager::AttitudeStrings[i].PStringData, expected[i]) != 0) { printf("0121e480_TEST FAIL [%d]\n", i); return 1; }
    printf("0121e480_TEST PASS\n");
    return 0;
}