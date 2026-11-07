// Behaviour test for NScript::CV_TalentlessBardScript::CTB_Bard::Main (0x00ED8680): when talked to, the bard asks whether to
// sing; yes with more gold than the fee takes the fee and sings BardNumVersesPerVisit random verses, yes without enough gold says
// TEXT_QST_163_NO_CASH<rand()%2+1>, no says the rejection; the first talk plays the intro, later talks TEXT_QST_163_RETURN<rand()%4+1>.
#include <cstdio>
#include <cstring>

struct Bard {
    bool BardIntroComplete; long Gold, Fee, Verses, Sung; char Last[40];
    void Talk(bool yes, int r)
    {
        if (!BardIntroComplete) { std::strcpy(Last, "TEXT_QST_163_INTRO"); BardIntroComplete = true; }
        else std::sprintf(Last, "TEXT_QST_163_RETURN%d", r % 4 + 1);
        if (yes) {
            if (Gold > Fee) { Gold -= Fee; for (long v = 0; v < Verses; ++v) ++Sung; }
            else std::sprintf(Last, "TEXT_QST_163_NO_CASH%d", r % 2 + 1);
        } else std::strcpy(Last, "TEXT_QST_163_REJECTED");
    }
};

int main()
{
    Bard b = { false, 100, 10, 3, 0, "" };
    bool ok = true;
    b.Talk(true, 5);  ok = ok && b.BardIntroComplete && b.Gold == 90 && b.Sung == 3;
    b.Talk(false, 6); ok = ok && !std::strcmp(b.Last, "TEXT_QST_163_REJECTED") && b.Sung == 3;
    b.Gold = 10; b.Talk(true, 3); ok = ok && !std::strcmp(b.Last, "TEXT_QST_163_NO_CASH2") && b.Gold == 10;
    b.Gold = 50; b.Fee = 5; b.Talk(true, 1); ok = ok && !std::strcmp(b.Last, "TEXT_QST_163_RETURN2") && b.Sung == 6;
    if (!ok) { std::printf("00ed8680_TEST FAIL\n"); return 1; }
    std::printf("00ed8680_TEST PASS\n");
    return 0;
}
