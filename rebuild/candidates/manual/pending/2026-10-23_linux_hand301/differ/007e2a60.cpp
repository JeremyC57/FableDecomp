// Grade: DIFFER(697v707) -- body and frame layout match; VC shrink-wraps the ebx/ebp/esi pushes into the answer branch where retail pushes all four at entry, which shifts every esp offset
// CTCFineDialogue::GlobalUpdate(bool) -- handles the pay-fine answer: on yes pays the fine, scores it and kicks a trespasser/murderer out
class CCharString {
public:
    void* Data;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString {
public:
    void* Data;
    CWideString();                                      // 0x0099AED0
    ~CWideString();                                     // 0x0099B510
    const CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CFontBank;
class CDataBank {
public:
    CWideString GetTextBySymbol(const CCharString& symbol);   // 0x009C95E0
};
class CThing;
class CMainPlayer {
public:
    CThing* GetCharacterThing();                        // 0x00487DC0
};
class CPlayerManager {
public:
    CMainPlayer* GetMainPlayer();                       // 0x00449960
};
class CDisplayManager {
public:
    void StartScreenFadeOut(float out, float in, CRGBColour colour, float hold);   // 0x00434C00
};
struct CMainGameComponent {
    unsigned char _pad00[0x14];
    CDataBank* PDataBank;                               // +0x14
    unsigned char _pad18[0x1c - 0x18];
    CPlayerManager* PPlayerManager;                     // +0x1C
    unsigned char _pad20[0x28 - 0x20];
    CDisplayManager* PDisplay;                          // +0x28
    unsigned char _pad2c[0x1614c - 0x2c];
    CFontBank* PFontBank;                               // +0x1614C
};
extern CMainGameComponent* g_MainGameComponent;         // 0x013B86A0
inline CDataBank* GetGameTextBank() { return g_MainGameComponent->PDataBank; }
class CSubtitleRenderer {
public:
    void SetText(const CFontBank& font, const CWideString& text, unsigned long colour, float time, bool flag, unsigned long mode);   // 0x00492E4B
};
struct CPlayerGui {
    unsigned char _pad00[0x18];
    CSubtitleRenderer Subtitles;                        // +0x18
};
extern CPlayerGui* g_PlayerGui;                         // 0x013B8790
struct CMessageEvent {
    unsigned char _pad00[0x3c];
    long Answer;                                        // +0x3C
};
struct CMessageEventFilter_IsOfType {
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) {}
};
class CMessageEventManager {
public:
    const CMessageEvent* FindMostRecentMessage(const CMessageEventFilter_IsOfType& filter) const;   // 0x004AB8B0
    void RemoveMessage(const CMessageEvent* message);   // 0x00493B5B
};
class CWorld {
public:
    unsigned char _pad00[0x60];
    CMessageEventManager* PMessages;                    // +0x60
};
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long& type);               // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long PresentBits[9];                       // +0x20
    CInterfaceMap Interfaces;                           // +0x44
    __forceinline bool HasTC(long type) const { return ((PresentBits[type >> 5] >> (type & 31)) & 1) != 0; }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &Interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
    __forceinline CTCBase* GetInterface(long type)
    {
        CTCBase* result;
        const bool& present = HasTC(type);
        if (present)
            result = GetTC(type);
        return result;
    }
};
class CTCBase {
public:
    virtual void v00();
    CThing* Thing;                                      // +0x04
    unsigned char _pad08[4];
    CWorld* GetWorld() const;                           // 0x00686A70
    void RemoveFromGlobalTCs();                         // 0x00686DF0
};
class CTCHeroStats : public CTCBase {
public:
    unsigned char _pad0c[0x100 - 0x0c];
    long FinesPaid;                                     // +0x100
    long TimesKickedOut;                                // +0x104
    void AddMoney(long amount, bool flag);              // 0x0057B327
    void KickHeroToMostRecentSafeRegion();              // 0x0057DF85
};
class CTCTrespasser : public CTCBase {
public:
    bool IsCurrentlyTrespassing();                      // 0x007BAE70
};
class CTCHeroOnlineScoreboard : public CTCBase {
public:
    void UpdateScoreForActivity(long activity, float value, bool flag);   // 0x0055F3B0
};
class CTCVillage : public CTCBase {
public:
    bool IsHeroMurderer();                              // 0x0065A1FD
    void ClearCrimes();                                 // 0x0065C2D0
    void InvalidateAndClearAllSecuritySweepTasks();     // 0x006590BF
};
class CTCFineDialogue : public CTCBase {
public:
    long Fine;                                          // +0x0C
    long Money;                                         // +0x10
    bool GivePlayerChanceToPayFine;                     // +0x14
    bool WaitingForConfirmation;                        // +0x15
    CTCVillage* Village;                                // +0x18
    void GlobalUpdate(bool paused);
};

void CTCFineDialogue::GlobalUpdate(bool paused)
{
    if (WaitingForConfirmation) {
    CMessageEventManager* messages = GetWorld()->PMessages;
    const CMessageEvent* message = messages->FindMostRecentMessage(CMessageEventFilter_IsOfType(0x10));
    if (message) {
    if (message->Answer == 1) {
        bool kickedOut = false;
        CWideString text;
        const bool& isTrespasser = Thing->HasTC(0xbe);
        if (isTrespasser && ((CTCTrespasser*)Thing->GetTC(0xbe))->IsCurrentlyTrespassing()) {
            kickedOut = true;
            text = GetGameTextBank()->GetTextBySymbol("TEXT_GUI_CRIME_KICKED_OUT_TRESPASS");
        }
        if (Village->IsHeroMurderer()) {
            kickedOut = true;
            text = GetGameTextBank()->GetTextBySymbol("TEXT_GUI_CRIME_KICKED_OUT_MURDER");
        }
        CThing* thing = Thing;
        CTCHeroStats* stats = (CTCHeroStats*)thing->GetInterface(4);
        const bool& isHero = thing->HasTC(0xe0);
        if (isHero) {
            stats->AddMoney(-Fine, true);
            Village->ClearCrimes();
        }
        GivePlayerChanceToPayFine = true;
        stats->FinesPaid += Fine;
        CThing* hero = g_MainGameComponent->PPlayerManager->GetMainPlayer()->GetCharacterThing();
        ((CTCHeroOnlineScoreboard*)hero->GetInterface(0x10d))->UpdateScoreForActivity(0x24, (float)stats->FinesPaid, false);
        Village->InvalidateAndClearAllSecuritySweepTasks();
        if (kickedOut) {
            stats->KickHeroToMostRecentSafeRegion();
            ++stats->TimesKickedOut;
            g_MainGameComponent->PDisplay->StartScreenFadeOut(0.5f, 0.5f, CRGBColour(0, 0, 0, 255), -1.0f);
            g_PlayerGui->Subtitles.SetText(*g_MainGameComponent->PFontBank, text, 0xa, 0.0f, false, 1);
        }
    } else {
        GivePlayerChanceToPayFine = false;
    }
    messages->RemoveMessage(message);
    WaitingForConfirmation = false;
    RemoveFromGlobalTCs();
    }
    }
}
