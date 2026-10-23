// CTCActionUseBed::GlobalUpdate(long) -- the bed's sleep cycle: when a sleep was requested, answer the hero's sleep question (yes: fade the screen out, mark the sleeper's TC, pay for the room or flag an owned bed; no: post the "not sleeping" game event and leave the global list) and drop the question; once the fade-out completes fast-forward the clock to morning/evening, trespass check for a rented bed, tell every village the hero slept, post the woke-up message, refresh the hero (cancel bullet time, reset effects, stamina, sober up), end the fade and post the game event; when waking finishes, leave the global list and clear the sleeper's TC flag
// Grade: DIFFER(1492v1516) -- every return in retail carries its own epilogue with the /GS cookie check before the register pops (ours: one shared tail, check after the pops), retail spills the message manager and keeps 0x10 in ebx (frame 0x44 vs our 0x48); the sleep, trespass, village and hero-refresh blocks are otherwise in order
// Leaf: GlobalUpdate
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x007EC710, 1514 bytes, `ret 4`, /GS cookie.
// Callees: see the declarations (addresses in trailing comments).
#include <stddef.h>
extern "C" void __cdecl free(void*);                        // 0x00BFEA14

struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
struct C3DVector { float X, Y, Z; };
class CThing {
public:
    void* __vftable;
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                             // +0x91 bit 0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
    const C3DVector& GetPosition() const;               // 0x004C73D0
};
class CThingPhysical : public CThing {
public:
    unsigned char _pad92[0xb0 - 0x92];
    long m_b0;                                          // +0xb0
    virtual void v00();
};
struct CBaseIntelligentPointer {
    void* m_p;
    CThing* GetPItem() const;                           // 0x00A01B50
};
bool __fastcall IsThingAlive(const CBaseIntelligentPointer* thing);   // 0x004CC340
bool __fastcall IsTCOwnerAlive(CTCBase* tc);            // 0x007EE540
struct CRGBColour {
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { R = other.R; G = other.G; B = other.B; A = other.A; }
};
struct CDisplayEngine {
    unsigned char _pad00[0xbc];
    bool FadeOutComplete;                               // +0xbc
    unsigned char _padbd[0xc8 - 0xbd];
    bool FadeInRunning;                                 // +0xc8
    bool FadeOutRunning;                                // +0xc9
    void StartScreenFadeOut(float fadeTime, float holdTime, CRGBColour colour, float delay);   // 0x00434C00
    void EndScreenFadeOut();                            // 0x00434C90
};
struct CGameEvent {
    long Type;                                          // +0x00
    bool Flag;                                          // +0x04
    unsigned char _pad05[0x25 - 0x05];
    bool B25, B26, B27;                                 // +0x25..+0x27
    void SetBoolParams(long count, const bool* values); // 0x009F1760
};
struct CPlayer { CThing* GetCharacterThing(); };        // 0x00487DC0
struct CPlayerManager { CPlayer* GetMainPlayer(); };    // 0x00449960
struct CMainGameComponent {
    unsigned char _pad00[0x1c];
    CPlayerManager* PlayerManager;                      // +0x1c
    unsigned char _pad20[0x28 - 0x20];
    CDisplayEngine* DisplayEngine;                      // +0x28
    void AddGameEvent(CGameEvent* event);               // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;        // 0x013B86A0
extern long g_FramesPerSecond;                          // 0x01375550
struct CMessageEvent { unsigned char _pad[0x3c]; long Response; };
struct CMessageEventFilter_IsOfType { long Type; };
struct CMessageEventManager {
    CMessageEvent* FindMostRecentMessage(const CMessageEventFilter_IsOfType& filter);   // 0x004AB8B0
    void RemoveMessage(const CMessageEvent* message);   // 0x00493B5B
    CMessageEvent* AddMessage(long type, unsigned long time, const C3DVector& position, CThing* a, CThing* b, float f1, float f2, long l);   // 0x00687540
};
struct CGameTimeManager {
    bool IsNightTime();                                 // 0x006BB210
    void FastForwardTimeTo(float time, float rate);     // 0x006BB720
};
struct CThingFilter_HasTC { long Type; };
struct CThingVector {
    CThing** First; CThing** Last; CThing** End;
    CThingVector() : First(0), Last(0), End(0) { }
    ~CThingVector() { if (First) free(First); }
    unsigned long size() const { return Last - First; }
};
struct CThingSearchTools {
    void GetAllLeafThings(const CThingFilter_HasTC& filter, CThingVector& out);   // 0x006D4600
};
struct CWorld {
    unsigned char _pad00[0x1c];
    CGameTimeManager* TimeManager;                      // +0x1c
    unsigned char _pad20[0x60 - 0x20];
    CMessageEventManager* MessageManager;               // +0x60
};
struct CHeroHolder { unsigned char _pad[0x34]; CBaseIntelligentPointer Hero; };   // hero pointer at +0x34
struct CHeroHolderOwner { CHeroHolder* GetHolder(); };   // 0x00449970
struct CTCHeroStats {
    void AddMoney(long amount, bool showMessage);       // 0x0057B327
    void ForceStaminaTo(float value);                   // 0x0057B26F
    void SoberUp();                                     // 0x0057A5BE
};
struct CTCSleeper { unsigned char _pad[0xd]; bool Sleeping; };
struct CTCTrespasser { void ForceTrespass(); };         // 0x007BB070
struct CTCVillage { void OnHeroSlept(); };              // 0x0065935E
struct CTCBulletTime { void CancelMove(); };            // 0x0079C640
struct CTCElectrocutedEffect { void OnInitialActivate(); };   // 0x006D8770

#pragma pack(push, 1)
class CTCActionUseBed {
public:
    void* __vftable;
    CThing* m_pThing;                                   // +0x04
    unsigned char _pad08[0x14 - 0x08];
    bool m_bFadingOut;                                  // +0x14
    bool m_bWakingUp;                                   // +0x15
    bool m_bSleeping;                                   // +0x16
    bool m_bSleepRequested;                             // +0x17
    bool m_bEventFlag;                                  // +0x18
    unsigned char _pad19[0x1c - 0x19];
    long m_Price;                                       // +0x1c
    unsigned char _pad20[0x22 - 0x20];
    bool m_bOwnedBedTrespass;                           // +0x22
    CBaseIntelligentPointer m_Bed;                      // +0x23
    unsigned char _pad27[0x2b - 0x27];
    CBaseIntelligentPointer m_Sleeper;                  // +0x2b
    CWorld* GetWorld();                                 // 0x00686A70
    CThingSearchTools* GetThingSearchTools();           // 0x00686D80
    CHeroHolderOwner* GetHeroHolderOwner();             // 0x00686D40
    void RemoveFromGlobalTCs();                         // 0x00686DF0
    void GlobalUpdate(long unused);
};
#pragma pack(pop)

void CTCActionUseBed::GlobalUpdate(long unused)
{
    if (m_bSleepRequested) {
        CMessageEventManager* messages = GetWorld()->MessageManager;
        CMessageEventFilter_IsOfType filter;
        filter.Type = 0x10;
        CMessageEvent* message = messages->FindMostRecentMessage(filter);
        if (message) {
            if (message->Response == 1) {
                g_pMainGameComponent->DisplayEngine->StartScreenFadeOut(2.5f, 2.0f, CRGBColour(0, 0, 0, 0xff), -1.0f);
                m_bFadingOut = true;
                m_bSleeping = true;
                if (m_Sleeper.GetPItem() && !m_Sleeper.GetPItem()->Dead) {
                    CThing* sleeper = m_Sleeper.GetPItem();
                    if (sleeper->HasTC(0xa7))
                        ((CTCSleeper*)sleeper->GetTC(0xa7))->Sleeping = true;
                }
                if (m_Price > 0) {
                    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetCharacterThing();
                    CTCHeroStats* stats;
                    if (hero->HasTC(4))
                        stats = (CTCHeroStats*)hero->GetTC(4);
                    stats->AddMoney(-m_Price, true);
                    messages->RemoveMessage(message);
                    m_bSleepRequested = false;
                    return;
                }
                if (m_Price == -1 && m_Bed.GetPItem())
                    m_bOwnedBedTrespass = true;
            } else {
                CGameEvent event;
                event.Type = 9;
                event.B25 = false;
                event.B26 = true;
                event.B27 = false;
                event.Flag = m_bEventFlag;
                bool none = false;
                event.SetBoolParams(1, &none);
                g_pMainGameComponent->AddGameEvent(&event);
                RemoveFromGlobalTCs();
            }
            messages->RemoveMessage(message);
            m_bSleepRequested = false;
        }
    } else if (m_bFadingOut && g_pMainGameComponent->DisplayEngine->FadeOutComplete &&
               !g_pMainGameComponent->DisplayEngine->FadeOutRunning && !g_pMainGameComponent->DisplayEngine->FadeInRunning) {
        m_bFadingOut = false;
        CGameTimeManager* time = GetWorld()->TimeManager;
        time->FastForwardTimeTo(time->IsNightTime() ? 0.3375f : 0.875f, 0.0f);
        if (m_bOwnedBedTrespass) {
            if (IsThingAlive(&m_Bed) && m_Bed.GetPItem()->HasTC(0x23) && m_Bed.GetPItem()->HasTC(0x23) &&
                IsTCOwnerAlive(m_Bed.GetPItem()->GetTC(0x23))) {
                CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer()->GetCharacterThing();
                if (hero->HasTC(0xbe))
                    ((CTCTrespasser*)hero->GetTC(0xbe))->ForceTrespass();
            }
            m_bOwnedBedTrespass = false;
        }
        CThingVector villages;
        CThingFilter_HasTC filter;
        GetThingSearchTools()->GetAllLeafThings(filter, villages);
        for (unsigned long i = 0; i < villages.size(); ++i) {
            CThing* village = villages.First[i];
            CTCVillage* tc = 0;
            if (village->HasTC(0x22))
                tc = (CTCVillage*)village->GetTC(0x22);
            tc->OnHeroSlept();
        }
        CThing* owner = m_pThing;
        long fps = g_FramesPerSecond;
        GetWorld()->MessageManager->AddMessage(0x42, fps, owner->GetPosition(), 0, 0, 0.0f, 0.0f, 0);
        CThingPhysical* hero = (CThingPhysical*)GetHeroHolderOwner()->GetHolder()->Hero.GetPItem();
        if (hero->HasTC(0x92))
            ((CTCBulletTime*)hero->GetTC(0x92))->CancelMove();
        if (!hero->Dead) {
            if (hero->HasTC(0x68))
                ((CTCElectrocutedEffect*)hero->GetTC(0x68))->OnInitialActivate();
            hero->v00();
            if (hero->HasTC(4)) {
                CTCHeroStats* stats = (CTCHeroStats*)hero->GetTC(4);
                stats->ForceStaminaTo(1.0f);
                stats->SoberUp();
            }
        }
        g_pMainGameComponent->DisplayEngine->EndScreenFadeOut();
        CGameEvent event;
        event.Flag = m_bEventFlag;
        event.Type = 9;
        event.B25 = false;
        event.B26 = true;
        event.B27 = false;
        bool none = false;
        event.SetBoolParams(1, &none);
        g_pMainGameComponent->AddGameEvent(&event);
        m_bWakingUp = true;
    } else if (m_bWakingUp && !g_pMainGameComponent->DisplayEngine->FadeOutComplete) {
        RemoveFromGlobalTCs();
        m_bWakingUp = false;
        m_bSleeping = false;
        if (IsThingAlive(&m_Sleeper) && m_Sleeper.GetPItem()->HasTC(0xa7))
            ((CTCSleeper*)m_Sleeper.GetPItem()->GetTC(0xa7))->Sleeping = false;
    }
}
