// CTCFineDialogue::Show(long fine, long money, CTCVillage* village) -- stores the fine, builds "pay for" + the hero's crime report for this region, then shows the hand-over prompt (and registers for updates) or the lack-of-funds prompt
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E27F0, 611 bytes, `ret 0Ch`.
// Callees: CTCFineDialogue::IsAllowed @ 0x007E2790, CCharString(const char*, long) @ 0x0099EBF0 / copy @ 0x0099EC30 /
//          dtor @ 0x0099EAE0, CDataBank::GetTextBySymbol @ 0x009C95E0, CWideString ctor @ 0x0099AED0 / += wchar_t* @ 0x0099B940 /
//          += @ 0x0099B8D0 / dtor @ 0x0099B510, operator+(const CWideString&, const wchar_t*) @ 0x0099BF30,
//          CVectorMap::LowerBound @ 0x0040F020, CTCVillage::UpdateMirroredCrimeData @ 0x0065C5BB, CTCBase::PeekWorldMap @ 0x00686D20,
//          CWorldMap::GetLoadedRegion @ 0x004FB150, CWorldMap::GetRegion (const) @ 0x004FC180, CTCHeroStats::GetCrimeReport @ 0x0057DC22,
//          CTCHeroStats::GetOutstandingFineAndFramesLeft @ 0x0057DAF5, NGameText::ConstructString @ 0x009C7B70,
//          CSubtitleRenderer::SetText @ 0x00492E4B, global update-list registration @ 0x00686ED0 (manifest "list::push_back").
//          Globals: game text @ 0x013B86A0 (data bank +0x14, font bank +0x1614C), player GUI @ 0x013B8790 (subtitles +0x18).
// Layout: Ego_r.pdb CTCFineDialogue (matches retail): Fine +0x0C, Money +0x10, GivePlayerChanceToPayFine +0x14,
//         WaitingForConfirmation +0x15, Village +0x18.
// Shape: the symbols are implicit const char* conversions (temporaries land in the dead fine/village argument slots); the
//        hero-stats TC is fetched without a fallback (retail reads the dead slot when absent); the outstanding fine and
//        frames are fetched and unused, as in retail; the affordable branch comes first.
class CCharString {
public:
    void* Data;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString {
public:
    void* Data;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator+=(const unsigned short* text);    // 0x0099B940
    const CWideString& operator+=(const CWideString& other);  // 0x0099B8D0
};
CWideString __fastcall operator+(const CWideString& left, const unsigned short* right);   // 0x0099BF30
namespace NGameText {
    bool __fastcall ConstructString(CWideString& text, long value);   // 0x009C7B70
}
class CFontBank;
namespace NGameText {
class CDataBank {
public:
    const CWideString GetTextBySymbol(const CCharString& symbol) const;   // 0x009C95E0
};
}
using NGameText::CDataBank;
struct CGameTextManager {
    unsigned char _pad00[0x14];
    CDataBank* PDataBank;                               // +0x14
    unsigned char _pad18[0x1614c - 0x18];
    CFontBank* PFontBank;                               // +0x1614C
};
extern CGameTextManager* g_GameText;                    // 0x013B86A0
class CSubtitleRenderer {
public:
    void SetText(const CFontBank& font, const CWideString& text, unsigned long colour, float time, bool flag, unsigned long mode);   // 0x00492E4B
};
class CPlayerGui { public:
    unsigned char _pad00[0x18];
    CSubtitleRenderer Subtitles;                        // +0x18
};
extern CPlayerGui* g_PlayerGui;                         // 0x013B8790
class CRegion { public:
    unsigned char _pad00[0x18];
    CCharString Name;                                   // +0x18
};
class CWorldMap {
public:
    long GetLoadedRegion() const;                       // 0x004FB150
    const CRegion& GetRegion(long index) const;         // 0x004FC180
};
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long PresentBits[9];                       // +0x20
    CInterfaceMap Interfaces;                           // +0x44
    __forceinline unsigned long HasTC(long type) const { return PresentBits[type >> 5] & (1UL << (type & 31)); }
    __forceinline CTCBase* GetTC(long type)
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        CInterfaceMap* map = &Interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->first ? end : it)->second;
    }
};
class CTCBase {
public:
    virtual void v00();
    CThing* Thing;                                      // +0x04
    unsigned char _pad08[4];
    void AddToGlobalUpdateList();                       // 0x00686ED0
protected:
    const CWorldMap& PeekWorldMap() const;              // 0x00686D20
};
class CTCHeroStats : public CTCBase {
public:
    void GetCrimeReport(const CCharString& region, CWideString& report, bool flag);   // 0x0057DC22
    void GetOutstandingFineAndFramesLeft(const CCharString& region, long& fine, long& frames);   // 0x0057DAF5
};
class CTCVillage : public CTCBase {
public:
    void UpdateMirroredCrimeData();                     // 0x0065C5BB
};
class CTCFineDialogue : public CTCBase {
public:
    long Fine;                                          // +0x0C
    long Money;                                         // +0x10
    bool GivePlayerChanceToPayFine;                     // +0x14
    bool WaitingForConfirmation;                        // +0x15
    CTCVillage* Village;                                // +0x18
    bool IsAllowed();                                   // 0x007E2790
    void Show(long fine, long money, CTCVillage* village);
};

void CTCFineDialogue::Show(long fine, long money, CTCVillage* village)
{
    if (!IsAllowed())
        return;
    if (fine <= 0)
        return;
    if (!village)
        return;
    Fine = fine;
    Money = money;
    Village = village;
    CWideString text = g_GameText->PDataBank->GetTextBySymbol("TEXT_GUI_CRIME_PAY_FOR");
    text += (const unsigned short*)L"\n";
    CWideString report;
    CTCHeroStats* stats;
    if (Thing->HasTC(4))
        stats = (CTCHeroStats*)Thing->GetTC(4);
    village->UpdateMirroredCrimeData();
    CCharString region = PeekWorldMap().GetRegion(PeekWorldMap().GetLoadedRegion()).Name;
    stats->GetCrimeReport(region, report, false);
    text += report + (const unsigned short*)L"\n";
    long outstandingFine;
    long framesLeft;
    stats->GetOutstandingFineAndFramesLeft(region, outstandingFine, framesLeft);
    if (Fine <= Money) {
        CWideString amount = g_GameText->PDataBank->GetTextBySymbol("TEXT_GUI_CRIME_FINE_HAND_OVER");
        NGameText::ConstructString(amount, fine);
        text += amount;
        g_PlayerGui->Subtitles.SetText(*g_GameText->PFontBank, text, 0x24, 0.0f, false, 1);
        WaitingForConfirmation = true;
        AddToGlobalUpdateList();
    } else {
        CWideString amount = g_GameText->PDataBank->GetTextBySymbol("TEXT_GUI_CRIME_FINE_LACK_FUNDS");
        NGameText::ConstructString(amount, fine);
        text += amount;
        g_PlayerGui->Subtitles.SetText(*g_GameText->PFontBank, text, 0x2a, 0.0f, false, 1);
        WaitingForConfirmation = false;
        GivePlayerChanceToPayFine = false;
    }
}