// CTCGuard::FrameUpdate() -- a pending region kick sends the hero to his last safe region once the game allows it; otherwise handle the hero's apologies seen by the guard (accept/reject them against the village's crimes), track criminal sightings, fine a dead-locked criminal hero in a fining region (paying the village, kicking him out with a fade and subtitle), process pending crime reports, and show the bribe pool above the guard
// Grade: DIFFER(1568v1592) -- first drafts: control flow, callees and the uninitialised-lookup else-loads as retail; residue is register assignment (this edi->ebx, village kept in a register v [esp+14h]), retail's load/add/store and stack-copied fild on the hero-stats fine counters (ours `add [m],r` / direct fild), the region def addref order, frame 0x20 v 0x1c
// Leaf: ?FrameUpdate@CTCGuard
//
// Retail 0x007B3120, 1600 bytes (FPO `sub esp,1Ch`, `ret`).
// Callees: CTCBase::GetPlayerManager 0x00686D40, CPlayerManager::GetMainPlayer 0x00449970 / 0x00449960, CPlayer::GetCharacterThing
//   0x00487DC0, CVectorMap::LowerBound 0x0040F020, CTCHeroStats::KickHeroToMostRecentSafeRegion 0x0057DF85,
//   CTCVillageMember::GetPVillage 0x007EE540, the entity events' Remove 0x008112D0 / AddEvent 0x008113C0, CDefString::operator==
//   0x005B3440, CTCGuard::GetTimeAgoThatCrimesCanBeApologisedFor 0x007B29B0, CTCVillage::IterateCrimesAndRemoveApologisable
//   0x0065CE56 / IsHeroCriminal 0x0065BFCE / GetFineAmountForCurrentCrimes 0x0065A41C / PayFines 0x0065C760 / the no-stats crime
//   clear 0x0065C2D0, the world frame getter 0x0049D870 (static), operator new 0x00BFEA1A, the look-at TC's target 0x007446A0 /
//   IsFading 0x00661600, CTCBase::GetWorld 0x00686A70, __ftol2 0x00BFEA70, CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   GetLifeState 0x00662010, CTCBase::GetWorldMap 0x00686D30, CWorldMap::GetRegionNumberMapIsIn 0x004FC190 / the region getter
//   0x004FB490, CTCHeroStats::AddMoney 0x0057B327 / PrepareToKickHeroToMostRecentSafeRegion 0x0057DE57,
//   CTCHeroOnlineScoreboard::UpdateScoreForActivity 0x0055F3B0, CCharString 0x0099EBF0 / ~ 0x0099EAE0, CDataBank::GetTextBySymbol
//   0x009C95E0, the number substitution 0x009C7B70 (__fastcall), CDisplayEngine::StartScreenFadeOut 0x00434C00,
//   CSubtitleRenderer::SetText 0x00492E4B, ~CWideString 0x0099B510, the deed-report processor 0x007B2E20, IsSpeaking 0x008644E0,
//   GFIntToCharString 0x0099F830 (__fastcall), CCharString::ToWideString 0x0099E480, CTCEmoteIcon::SetEmoteText 0x0071AC70.
// Layout: Ego_r.pdb CTCGuard (retail offsets): PDef +0x0C, BribePool +0x10, PendingCrimes deque +0x14 (start cur +0x14, finish
//   cur +0x24), FramePendingCrimesAdded +0x3C, FrameLastReceivedApology +0x4C, WaitingForRegionKickConfirmation +0x50.
#include <stddef.h>
void* __cdecl operator new(size_t size);               // 0x00BFEA1A

struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    struct CWideString ToWideString() const;            // 0x0099E480
};
struct CWideString {
    void* PStringData;
    ~CWideString();                                     // 0x0099B510
};
CCharString __fastcall GFIntToCharString(long value);  // 0x0099F830
void __fastcall SubstituteNumber(CWideString& text, long value);   // 0x009C7B70
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;
    unsigned long m_uid[2];
    void* GetPItem() const;                             // 0x00A01B50
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};
struct CThing;
struct CThingCreature { long GetLifeState(); };         // 0x00662010
#define VF(n) virtual void v##n();
struct CThing {
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) VF(10) VF(11) VF(12) VF(13) VF(14)
    VF(15) VF(16) VF(17) VF(18) VF(19) VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29)
    VF(30) VF(31) VF(32) VF(33) VF(34) VF(35) VF(36) VF(37) VF(38) VF(39) VF(40) VF(41) VF(42) VF(43) VF(44)
    VF(45) VF(46) VF(47) VF(48) VF(49) VF(50) VF(51) VF(52) VF(53) VF(54) VF(55) VF(56) VF(57) VF(58)
    virtual void SetHealth(float health, bool notify);  // slot 59 (+0xEC)
    unsigned char  _pad04[0x20 - 0x04];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x91 - 0x4c];
    unsigned char  Flags91;                             // +0x91
    unsigned char  _pad92[0xb0 - 0x92];
    float          Health;                              // +0xB0
    unsigned long  LifeFactor;                          // +0xB4 (float bits)
    unsigned char  _padb8[0xf4 - 0xb8];
    CIntelligentPointer<CThingCreature> Mount;          // +0xF4
    const C3DVector& GetPosition() const;               // 0x004C73D0
    bool IsSpeaking();                                  // 0x008644E0
    template<class T> __forceinline T* LookupTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        T* tc;
        bool present = IsTCPresent(interfaceType);
        if (present)
            tc = LookupTC<T>(interfaceType);
        return tc;
    }
    __forceinline bool IsTCPresent(long interfaceType) const { return (m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1; }
    __forceinline unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};
struct CPlayer { CThing* GetCharacterThing(); };       // 0x00487DC0
struct CPlayerManager {
    CPlayer* GetMainPlayer();                           // 0x00449970
    CPlayer* GetMainPlayerConst();                      // 0x00449960
};
struct CTCHeroStats {
    unsigned char _pad00[0x3c];
    long Money;                                         // +0x3C
    unsigned char _pad40[0x100 - 0x40];
    long FinesPaid;                                     // +0x100
    long TimesKickedOut;                                // +0x104
    void KickHeroToMostRecentSafeRegion();              // 0x0057DF85
    void PrepareToKickHeroToMostRecentSafeRegion();     // 0x0057DE57
    void AddMoney(long amount, bool notify);            // 0x0057B327
};
struct CTCVillage {
    unsigned char _pad00[0x80];
    long LastSeenCriminalFrame;                         // +0x80
    unsigned char _pad84[0xac - 0x84];
    long CriminalUntilFrame;                            // +0xAC
    void IterateCrimesAndRemoveApologisable(long timeAgo);   // 0x0065CE56
    bool IsHeroCriminal();                              // 0x0065BFCE
    long GetFineAmountForCurrentCrimes();               // 0x0065A41C
    void PayFines(long amount);                         // 0x0065C760
    void ClearCrimesNoStats();                          // 0x0065C2D0
};
struct CTCVillageMember { CTCVillage* GetPVillage(); };   // 0x007EE540
struct CDefString {
    long Index;
    bool operator==(const char* name) const;            // 0x005B3440
};
struct CRefCounted {
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long RefCount;                                      // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CExpressionDef : CRefCounted {
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09();
    virtual CDefString GetName() const;                 // slot 10 (+0x28)
};
template<class T> struct CCountedPtr {
    T* Object;
    CCountedPtr(const CCountedPtr& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CCountedPtr() { if (Object) Object->Release(); }
    T* operator->() const { return Object; }
};
struct CEntityEvent {
    virtual void v00();
    virtual long GetType() const;                       // slot 1
    unsigned char _pad04[0x10 - 0x04];
    CCountedPtr<CExpressionDef> Expression;             // +0x10
};
struct CEntityEventBase {
    virtual void v00();
    long Unused;                                        // +0x04
    CEntityEventBase() : Unused(0) {}
};
struct CEventApologyResponse : CEntityEventBase {
    virtual void v01();
    bool Accepted;                                      // +0x08
    CEventApologyResponse(bool accepted) : Accepted(accepted) {}
};
struct SEventNode { SEventNode* Next; SEventNode* Prev; CEntityEvent* Event; };
struct CTCEntityEvents {
    unsigned char _pad00[0x0c];
    SEventNode*   Head;                                 // +0x0C (std::list sentinel)
    void Remove(CEntityEvent* event);                   // 0x008112D0
    void AddEvent(CEventApologyResponse* event);        // 0x008113C0
};
struct CTCLookAt {
    CThing* GetTarget();                                // 0x007446A0
    bool IsFading();                                    // 0x00661600
};
struct CRegionDef : CRefCounted {
    unsigned char _pad08[0x9d - 0x08];
    bool FinesCriminals;                                // +0x9D
};
struct CRegion { unsigned char _pad00[0x24]; CCountedPtr<CRegionDef> Def; };   // +0x24
struct CWorldMap {
    virtual void m00(); virtual void m01(); virtual void m02(); virtual void m03(); virtual void m04();
    virtual void m05(); virtual void m06(); virtual void m07(); virtual void m08(); virtual void m09();
    virtual void m10(); virtual void m11(); virtual void m12(); virtual void m13(); virtual void m14();
    virtual void m15();
    virtual long GetRegionMapAt(const C3DVector& pos);  // slot 16 (+0x40)
    long GetRegionNumberMapIsIn(long map);              // 0x004FC190
    CRegion& GetRegion(long number);                    // 0x004FB490
};
struct CWorld { static long GetFrame(); };              // 0x0049D870
struct CTCHeroOnlineScoreboard { void UpdateScoreForActivity(long activity, float score, long a3); };   // 0x0055F3B0
struct CRGBColour {
    union {
        struct { unsigned char B, G, R, A; };
        unsigned long Colour;
    };
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) : Colour(other.Colour) { }
};
struct CDisplayEngine { void StartScreenFadeOut(float fadeOutTime, float fadeInTime, CRGBColour colour, float holdTime); };   // 0x00434C00
struct CDataBank { CWideString GetTextBySymbol(const CCharString& symbol); };   // 0x009C95E0
struct CMainGameComponent {
    unsigned char   _pad00[0x14];
    CDataBank*      TextBank;                           // +0x14
    unsigned char   _pad18[0x1c - 0x18];
    CPlayerManager* PlayerManager;                      // +0x1C
    unsigned char   _pad20[0x28 - 0x20];
    CDisplayEngine* DisplayEngine;                      // +0x28
    unsigned char   _pad2c[0x1614c - 0x2c];
    long            SubtitleFont;                       // +0x1614C
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
struct CSubtitleRenderer {
    void SetText(long font, const CWideString& text, long priority, long a4, long a5, bool a6);   // 0x00492E4B
};
struct CGame {
    unsigned char     _pad00[0x18];
    CSubtitleRenderer Subtitles;                        // +0x18
    unsigned char     _pad19[0xfd - 0x19];
    bool              InCutscene;                       // +0xFD
};
extern CGame* g_pGame;                                  // 0x013B8790
extern long g_FramesPerSecond;                          // 0x01375550
struct CTCEmoteIcon {
    unsigned char _pad00[0x25];
    bool          ShowText;                             // +0x25
    void SetEmoteText(long type, const CWideString& text, float scale, float time);   // 0x0071AC70
};
template<class T> inline const T& Max(const T& a, const T& b) { return a < b ? b : a; }
struct SDequeIterator { void* Cur; void* First; void* Last; void* Node; };

class CTCGuard {
public:
    virtual void v00();
    CThing*         ParentThing;                        // +0x04
    unsigned char   _pad08[0x10 - 0x08];
    long            BribePool;                          // +0x10
    SDequeIterator  PendingCrimesStart;                 // +0x14
    SDequeIterator  PendingCrimesFinish;                // +0x24
    unsigned char   _pad34[0x3c - 0x34];
    long            FramePendingCrimesAdded;            // +0x3C
    unsigned char   _pad40[0x4c - 0x40];
    long            FrameLastReceivedApology;           // +0x4C
    bool            WaitingForRegionKickConfirmation;   // +0x50
    CPlayerManager* GetPlayerManager();                 // 0x00686D40
    CWorld* GetWorld();                                 // 0x00686A70
    CWorldMap* GetWorldMap();                           // 0x00686D30
    long GetTimeAgoThatCrimesCanBeApologisedFor() const;   // 0x007B29B0
    void ProcessDeedReports(CTCVillage* village);       // 0x007B2E20
    void FrameUpdate();
};

void CTCGuard::FrameUpdate()
{
    if (WaitingForRegionKickConfirmation) {
        if (!g_pGame->InCutscene) {
            WaitingForRegionKickConfirmation = false;
            CThing* hero = GetPlayerManager()->GetMainPlayer()->GetCharacterThing();
            if (hero && !(hero->Flags91 & 1) && hero->HasTC(4))
                hero->LookupTC<CTCHeroStats>(4)->KickHeroToMostRecentSafeRegion();
        }
        return;
    }
    if (!ParentThing || (ParentThing->Flags91 & 1))
        return;
    CTCVillage* village = ParentThing->GetTC<CTCVillageMember>(0x23)->GetPVillage();
    CThing* parent = ParentThing;
    bool hasEvents = parent->IsTCPresent(0x42);
    if (hasEvents) {
        CTCEntityEvents* events = parent->LookupTC<CTCEntityEvents>(0x42);
        SEventNode* node = events->Head->Next;
        while (node != events->Head) {
            SEventNode* current = node;
            node = node->Next;
            CEntityEvent* event = current->Event;
            if (event->GetType() == 3) {
                CCountedPtr<CExpressionDef> expression = event->Expression;
                if (expression->GetName() == "EXPRESSION_APOLOGY") {
                    events->Remove(event);
                    if (village) {
                        long timeAgo = GetTimeAgoThatCrimesCanBeApologisedFor();
                        if (timeAgo > 0)
                            village->IterateCrimesAndRemoveApologisable(timeAgo);
                        bool accepted = !village->IsHeroCriminal();
                        if (accepted)
                            FrameLastReceivedApology = CWorld::GetFrame();
                        events->AddEvent(new CEventApologyResponse(accepted));
                    }
                }
            }
        }
    }
    if (village) {
        CThing* hero = GetPlayerManager()->GetMainPlayer()->GetCharacterThing();
        if (village->IsHeroCriminal()) {
            CThing* guard = ParentThing;
            bool hasLook = guard->IsTCPresent(0x34);
            if (hasLook) {
                CTCLookAt* look = guard->LookupTC<CTCLookAt>(0x34);
                if (look->GetTarget() && look->IsFading() && look->GetTarget() == hero)
                    village->LastSeenCriminalFrame = CWorld::GetFrame();
            }
            long until = GetWorld()->GetFrame() + 300;
            long current = (long)(float)village->CriminalUntilFrame;
            village->CriminalUntilFrame = (long)(float)Max(current, until);
        }
        if (hero && hero->LifeFactor == 0x3f800000 && hero->Mount.GetPItem() && hero->Mount.GetPItem()->GetLifeState() == 3) {
            CWorldMap* worldMap = GetWorldMap();
            CCountedPtr<CRegionDef> region = GetWorldMap()->GetRegion(GetWorldMap()->GetRegionNumberMapIsIn(worldMap->GetRegionMapAt(ParentThing->GetPosition()))).Def;
            if (region.Object) {
                if (region->FinesCriminals) {
                    long fine = village->GetFineAmountForCurrentCrimes();
                    if (fine > 0) {
                        bool hasStats = hero->IsTCPresent(4);
                        if (hasStats) {
                            CTCHeroStats* stats = hero->LookupTC<CTCHeroStats>(4);
                            if (stats->Money < fine)
                                fine = stats->Money;
                            stats->AddMoney(-fine, true);
                            village->PayFines(fine);
                            stats->FinesPaid += fine;
                            CThing* player = g_pMainGameComponent->PlayerManager->GetMainPlayerConst()->GetCharacterThing();
                            player->GetTC<CTCHeroOnlineScoreboard>(0x10d)->UpdateScoreForActivity(0x24, (float)stats->FinesPaid, 0);
                            stats->PrepareToKickHeroToMostRecentSafeRegion();
                            WaitingForRegionKickConfirmation = true;
                            ++stats->TimesKickedOut;
                        } else
                            village->ClearCrimesNoStats();
                        CWideString text = g_pMainGameComponent->TextBank->GetTextBySymbol("TEXT_GUI_CRIME_FINE_KICKED_OUT");
                        SubstituteNumber(text, fine);
                        hero->SetHealth(hero->Health * 0.25f, true);
                        g_pMainGameComponent->DisplayEngine->StartScreenFadeOut(0.5f, 0.5f, CRGBColour(0, 0, 0, 0xff), -1.0f);
                        g_pGame->Subtitles.SetText(g_pMainGameComponent->SubtitleFont, text, 10, 0, 0, true);
                    }
                }
            }
        }
    }
    if (PendingCrimesFinish.Cur != PendingCrimesStart.Cur && g_FramesPerSecond * 15 < CWorld::GetFrame() - FramePendingCrimesAdded)
        ProcessDeedReports(village);
    if (BribePool) {
        if (!parent->IsSpeaking()) {
            CTCEmoteIcon* icon;
            if (parent->HasTC(0x45))
                icon = parent->LookupTC<CTCEmoteIcon>(0x45);
            CCharString pool = GFIntToCharString(BribePool);
            icon->SetEmoteText(6, pool.ToWideString(), 1.0f, 0.3f);
            icon->ShowText = true;
        }
    }
}
