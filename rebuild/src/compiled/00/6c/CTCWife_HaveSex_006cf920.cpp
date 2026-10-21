// CTCWife::HaveSex() -- start the wedding-timed black screen fade, pause the game for the current player, flag the fade/sex wait, and for a live hero bump the sex statistic, restore full health and report the activity to the online scoreboard
// Flags: /GS
// Leaf: HaveSex
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006CF920, length 378 bytes (plain `ret`, /GS cookie 0x0139C8A8).
// Callees: CTCBase::AddToGlobalUpdateTCs() 0x00686ED0; CDisplayEngine::StartScreenFadeOut(float, float, CRGBColour, float)
//   0x00434C00 on the main component's display engine (+0x28); CTCBase::GetWorld() const 0x00686A80; CWorld::
//   GetCurrentPlayer() 0x0049D9D0; CMemoryBuffer::Copy 0x009F1760; CMainGameComponent::AddGameEvent 0x0041649C (the
//   landed SendPauseEvent(true) inlined, as the landed InitiateDivorce 0x006CEF20); CTCBase::GetPlayerManager()
//   0x00686D40 (manifest "GetDisplayEngine" propagated); CPlayerManager::GetMainPlayer() 0x00449970; CPlayer::
//   GetHeroThing() 0x00487DC0 (as the landed CTCWallMountEffects::DoEffect 0x007DF6A0); CVectorMap LowerBound 0x0040F020
//   (interfaces 4 and 0x10D); CThingPhysical::ModifyHealth(float, bool) slot 59 (+0xec); CTCHeroOnlineScoreboard::
//   UpdateScoreForActivity(EOnlineScoreboardActivity, float, bool) 0x0055F3B0; __security_check_cookie 0x00BFE9F9.
// Layout: engine/CTCWife.h (PDB = retail; local model as the landed InitiateDivorce): PHeroMarriageDef +0x0c, WaitingForFadeOut +0x27, WaitingForSex +0x2c;
//   CHeroMarriageDef WeddingTimeForScreenToFadeOut +0x28 / WeddingTimeForScreenToFadeIn +0x2c; CThing bitset +0x20
//   (4 = bit 4 of byte +0x20, 0x10D = bit 13 of word +0x40), interface map +0x44, dead bit +0x91, max health +0xb0
//   (descriptive); the interface-4 hero stats' +0x154 sex count (descriptive).
// Shape: the display engine comes through an inline CMainGameComponent getter and the def through CDefPointer's inline
//   operator-> (that pair schedules the fade arguments as retail); the colour is built in its argument slot (user copy
//   constructor); a same-TU LowerBound body lets VC7.1 pack both GetTC keys and the scoreboard pointer into one slot
//   (an external LowerBound costs a 4-byte frame slot); the stats component is the `T* p = 0` preload then
//   `if (HasTC) p = GetTC` and is null-tested; the scoreboard pointer is declared uninitialised (retail reads the shared
//   key slot on a miss).
struct CTCBase;
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);     // 0x009F1760
};
struct CGameEvent : CMemoryBuffer {
    long          Type;                                     // +0x00
    signed char   Player;                                   // +0x04
    unsigned char Data[32];                                 // +0x05
    unsigned char EndPos;                                   // +0x25
    bool          Valid;                                    // +0x26
    bool          Replacement;                              // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddBool(bool value) { value = value ? true : false; Copy(1, &value); }
};
struct CRGBColour {                                         // B,G,R,A bytes over a packed 32-bit colour
    union {
        struct { unsigned char B, G, R, A; };
        unsigned long Colour;
    };
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) : Colour(other.Colour) { }
};
struct CDisplayEngine {
    void StartScreenFadeOut(float fadeOutTime, float fadeInTime, CRGBColour colour, float holdTime);   // 0x00434C00
};
struct CMainGameComponent {
    unsigned char   _pad00[0x28];
    CDisplayEngine* DisplayEngine;                          // +0x28
    CDisplayEngine* GetDisplayEngine() const { return DisplayEngine; }
    void AddGameEvent(const CGameEvent& event);             // 0x0041649C
};
extern CMainGameComponent* g_pMainGameComponent;            // 0x013B86A0

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing {
    virtual void v00();
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags;                                  // +0x91 bit 0: dead
    unsigned char _pad92[0xb0 - 0x92];
    float         MaxHealth;                                // +0xb0 (descriptive)
    bool IsDead() const { return (m_flags & 1) != 0; }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (it == map->last || interfaceType < it->type ? map->last : it)->component;
    }
};
struct CThingPhysical : CThing {
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12();
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18();
    virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30();
    virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36();
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42();
    virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48();
    virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58();
    virtual void ModifyHealth(float amount, bool showEffect);   // slot 59 (+0xec) 0x006A68F0
};
struct CTCHeroStats {                                       // interface 4 (descriptive)
    unsigned char _pad00[0x154];
    long          TimesHadSex;                              // +0x154 (descriptive)
};
enum EOnlineScoreboardActivity { ONLINE_SCOREBOARD_ACTIVITY_SEX = 6 };
struct CTCHeroOnlineScoreboard {                            // interface 0x10D
    void UpdateScoreForActivity(EOnlineScoreboardActivity activity, float value, bool flag);   // 0x0055F3B0
};

struct CPlayer {
    unsigned char _pad_0x00[0x28];
    signed char   m_playerNumber;                           // +0x28
    CThing* GetHeroThing();                                 // 0x00487DC0
};
struct CPlayerManager {
    CPlayer& GetMainPlayer();                               // 0x00449970
};
struct CWorld {
    CPlayer* GetCurrentPlayer() const;                      // 0x0049D9D0
};
struct CHeroMarriageDef {
    unsigned char _pad00[0x28];
    float         WeddingTimeForScreenToFadeOut;            // +0x28
    float         WeddingTimeForScreenToFadeIn;             // +0x2c
};
template <class T> struct CDefPointer {
    T* Object;
    T* operator->() const { return Object; }
};
struct CTCWife {
    virtual void v00();
    void*             ParentThing;                          // +0x04
    unsigned char     _pad_0x08[4];
    CDefPointer<const CHeroMarriageDef> PHeroMarriageDef;   // +0x0c
    unsigned char     _pad_0x10[0x27 - 0x10];
    bool              WaitingForFadeOut;                    // +0x27
    unsigned char     _pad_0x28[0x2c - 0x28];
    bool              WaitingForSex;                        // +0x2c
    void AddToGlobalUpdateTCs();                            // 0x00686ED0 (CTCBase)
    const CWorld* GetWorld() const;                         // 0x00686A80 (CTCBase)
    CPlayerManager* GetPlayerManager();                     // 0x00686D40 (CTCBase)
    void HaveSex();
};

void CTCWife::HaveSex()
{
    AddToGlobalUpdateTCs();
    g_pMainGameComponent->GetDisplayEngine()->StartScreenFadeOut(PHeroMarriageDef->WeddingTimeForScreenToFadeOut,
        PHeroMarriageDef->WeddingTimeForScreenToFadeIn, CRGBColour(0, 0, 0, 255), -1.0f);
    CGameEvent gameEvent(9);                                // SendPauseEvent(true) written out
    gameEvent.Player = GetWorld()->GetCurrentPlayer()->m_playerNumber;
    gameEvent.AddBool(true);
    g_pMainGameComponent->AddGameEvent(gameEvent);
    WaitingForFadeOut = true;
    WaitingForSex = true;
    CThing* hero = GetPlayerManager()->GetMainPlayer().GetHeroThing();
    if (hero && !hero->IsDead()) {
        CTCHeroStats* stats = 0;
        if (hero->HasTC(4))
            stats = (CTCHeroStats*)hero->GetTC(4);
        if (stats)
            ++stats->TimesHadSex;
        ((CThingPhysical*)hero)->ModifyHealth(hero->MaxHealth, true);
        CTCHeroOnlineScoreboard* scoreboard;
        if (hero->HasTC(0x10d))
            scoreboard = (CTCHeroOnlineScoreboard*)hero->GetTC(0x10d);
        scoreboard->UpdateScoreForActivity(ONLINE_SCOREBOARD_ACTIVITY_SEX, (float)stats->TimesHadSex, false);
    }
}