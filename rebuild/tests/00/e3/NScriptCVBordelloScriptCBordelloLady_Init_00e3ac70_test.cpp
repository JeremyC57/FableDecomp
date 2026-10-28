// Behaviour test for 0x00E3AC70 NScript::CV_BordelloScript::CBordelloLady::Init()
#include <stdio.h>
#include <string.h>

static int g_infoCalls = 0, g_killableCalls = 0, g_comboCalls = 0, g_maskCalls = 0;
struct Lady {
    bool WalkingDownstairs, PartiedAlready, HaveTalked; int GoldRequired; bool NoLongerWorking;
    const char* Name; const char* DataString; bool Nunnery;
    static bool Eq(const char* name, const char* text) { return strcmp(name ? name : "", text) == 0; }
    void Init()                                                     // the row's body
    {
        WalkingDownstairs = false; HaveTalked = false; PartiedAlready = false; NoLongerWorking = false;
        Name = DataString;
        if (!Nunnery) ++g_infoCalls;
        ++g_killableCalls; ++g_comboCalls; ++g_maskCalls;
        GoldRequired = 0;
        if (Eq(Name, "POLLY")) GoldRequired = 50;
        if (Eq(Name, "AMELIA")) GoldRequired = 100;
        if (Eq(Name, "LUCREZIA")) GoldRequired = 200;
        if (Eq(Name, "SOPHIA")) GoldRequired = 1000;
        if (Eq(Name, "HEDWIG")) GoldRequired = 2000;
    }
};

int main()
{
    const char* names[6] = { "POLLY", "AMELIA", "LUCREZIA", "SOPHIA", "HEDWIG", "MAUD" };
    int gold[6] = { 50, 100, 200, 1000, 2000, 0 };
    int ok = 1;
    for (int i = 0; i < 6; ++i) {
        Lady l; l.WalkingDownstairs = l.PartiedAlready = l.HaveTalked = l.NoLongerWorking = true; l.GoldRequired = -1;
        l.DataString = names[i]; l.Nunnery = (i == 5);
        l.Init();
        ok &= l.GoldRequired == gold[i] && !l.WalkingDownstairs && !l.PartiedAlready && !l.HaveTalked && !l.NoLongerWorking;
    }
    Lady e; e.DataString = 0; e.Nunnery = false; e.Init();
    ok &= e.GoldRequired == 0 && g_infoCalls == 6 && g_killableCalls == 7 && g_maskCalls == 7;
    printf(ok ? "00e3ac70_TEST PASS\n" : "00e3ac70_TEST FAIL\n");
    return ok ? 0 : 1;
}