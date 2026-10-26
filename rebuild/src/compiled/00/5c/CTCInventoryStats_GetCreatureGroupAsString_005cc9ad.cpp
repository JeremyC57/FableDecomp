// CTCInventoryStats::GetCreatureGroupAsString(unsigned long groups) -- the localised stats label (TEXT_GUI_STATS_LABEL_CR*) for the first creature group set in a group bit mask, CROTHER when none matches
// Retail: Fable.exe (TLC, VC7.1) @ 0x005CC9AD, 528 bytes (CWideString by value: hidden pointer at [ebp+8], `ret 8`); called once @ 0x005CD6DD.
// Frame-pointer size TU: optimize("y",off) + ("s",on). `this` is unused.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, operator+(const CCharString&, const CCharString&)
// @ 0x0099F570 (`__fastcall`, hidden result in ecx, left operand in edx), NGameText::CDataBank::GetTextBySymbol @ 0x009C95E0 on
// g_pMainGameComponent (0x013B86A0)->TextBank (+0x14).
// Shape: the landed GetHeroSexualityAsString (0x005CCBBD) form: one `return` per bit test in mask order (HOBBES tests 0x20 || 0x40 and UNDEAD 0x400 || 0x800, which VC7.1 folds to one mask test and places at the end;
// ICETROLL 0x2000000 is tested after SUMMONER 0x4000000), each the same sum of two temporaries; VC7.1
// head-merges the `push -1` and tail-merges everything after the per-arm literal push, so only the first arm keeps its own body.
#pragma optimize("y", off)
#pragma optimize("s", on)
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right);   // 0x0099F570
class CWideString { public:
    void* PStringData;
};
struct CDataBank {                                      // NGameText::CDataBank
    CWideString GetTextBySymbol(const CCharString& symbol);   // 0x009C95E0
};
class CMainGameComponent { public:
    unsigned char _pad00[0x14];
    CDataBank*    TextBank;                             // +0x14
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0

namespace NInventory {
struct CTCInventoryStats {
    CWideString GetCreatureGroupAsString(unsigned long groups);
};
CWideString CTCInventoryStats::GetCreatureGroupAsString(unsigned long groups)
{
    CDataBank* bank = g_pMainGameComponent->TextBank;
    if (groups & 0x1)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CRVILLAGERS"));
    if (groups & 0x2)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CRGUARDS"));
    if (groups & 0x4)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CRBANDITS"));
    if (groups & 0x8)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CRBALVERINES"));
    if (groups & 0x10)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CRWHITEBALVERINES"));
    if ((groups & 0x20) || (groups & 0x40))
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CRHOBBES"));
    if (groups & 0x80)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CRSCREAMERS"));
    if (groups & 0x100)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CRWASPS"));
    if (groups & 0x200)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CRSCORPIONS"));
    if ((groups & 0x400) || (groups & 0x800))
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CRUNDEAD"));
    if (groups & 0x1000)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CRRIVALHEROES"));
    if (groups & 0x2000)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CRNYMPHS"));
    if (groups & 0x4000)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CRMINIONS"));
    if (groups & 0x8000)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CREARTHTROLLS"));
    if (groups & 0x10000)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CRROCKTROLLS"));
    if (groups & 0x20000)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CRDRAGONS"));
    if (groups & 0x40000)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CRBEETLES"));
    if (groups & 0x80000)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CRSCORPIONKINGS"));
    if (groups & 0x100000)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CRKRAKEN"));
    if (groups & 0x400000)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CRWASPQUEENS"));
    if (groups & 0x800000)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CRFAMILYMEMBERS"));
    if (groups & 0x1000000)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CRBIRDS"));
    if (groups & 0x4000000)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CRSUMMONER"));
    if (groups & 0x2000000)
        return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CRICETROLL"));
    return bank->GetTextBySymbol(CCharString("TEXT_GUI_STATS_LABEL_") + CCharString("CROTHER"));
}
}