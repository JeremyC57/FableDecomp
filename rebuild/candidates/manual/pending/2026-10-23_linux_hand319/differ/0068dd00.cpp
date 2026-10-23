// Grade: DIFFER(1469v1471) -- frame 0x44 v 0x40: retail keeps the TC keys / IS_THING_ALIVE temp in the dead event-parameter and UID slots, spills hero and CSEs the 0xd key into ebp in the usable-item and target blocks
// CInputProcessUseEnvironment::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- the hero's "use" action
//   (game action 6): pressed / released / held build a button mask; a carried usable item, a targeted creature (talk or
//   pick-up events) or a targeted usable object raise the matching game event; a locked door plays DOOR_OPEN_FAIL
// Leaf: ProcessInput
// Flags: /GS
// Layout: engine/CInputProcessGameBase.h layout (World +0x14, MyPlayer +0x18), modelled locally as the landed siblings
//   (CInputProcessOracleMinigame::ProcessInput 0x00691680); CThing component bitset +0x20, map +0x44 with the const
//   (0x004365B0) and non-const (0x0040F020) LowerBound as the landed CThingDeadCreature::WorldUpdate 0x00836260.
struct CWorld;
class CPlayer;
class CInputEvent;
class CInputProcessGameBase {
public:
    virtual void v00();
    unsigned char _pad04[0x10];                    // +0x04
    CWorld*       World;                           // +0x14
    CPlayer*      MyPlayer;                        // +0x18
    unsigned char _pad1c[0x0c];                    // +0x1C
};
struct CCharStringData {
    const char*   m_pData;
    unsigned long m_length;
};
long __fastcall Compare(const char* a, const char* b);  // 0x00411570
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->m_length == other.PStringData->m_length)
            return Compare(PStringData->m_pData, other.PStringData->m_pData) ? false : true;
        return false;
    }
};
struct CMemoryBuffer {
    void Copy(unsigned long length, const void* data);   // 0x009F1760
};
struct CThingUID { unsigned long Low, High; };
struct CGameEvent : CMemoryBuffer {
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent() : Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
    void AddUID(CThingUID value) { Copy(sizeof(value), &value); }
    void AddBool(bool value) { Copy(1, &value); }
};
class CProcessedInput {
public:
    void AddGameEvent(const CGameEvent& event);    // 0x00A0D340
    void SetHandled();                             // 0x00A0D390
};
struct CThing;
struct CTCBase { virtual void v00(); };
struct CTCMapEntry { long Kind; CTCBase* TC; };
struct CTCMap {
    CTCMapEntry* m_pFirst;
    CTCMapEntry* m_pLast;
    CTCMapEntry* m_pEnd;
    CTCMapEntry* LowerBound(const long* kind);               // 0x0040F020
    const CTCMapEntry* LowerBound(const long* kind) const;   // 0x004365B0
    __forceinline CTCBase* FindC(long kind) const
    {
        long key = kind;
        const CTCMapEntry* it = LowerBound(&key);
        return ((it == m_pLast || kind < it->Kind) ? m_pLast : it)->TC;
    }
    __forceinline CTCBase* Find(long kind)
    {
        long key = kind;
        CTCMapEntry* it = LowerBound(&key);
        return ((it == m_pLast || kind < it->Kind) ? m_pLast : it)->TC;
    }
};
struct CThing {
    unsigned char _pad00[0x18];
    CThingUID     UID;                             // +0x18
    unsigned long m_tcMask[9];                     // +0x20
    CTCMap        m_tcs;                           // +0x44
    unsigned char _pad50[0x41];
    unsigned char m_flags91;                       // +0x91 (bit 0: being destroyed)
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCBase* GetTCC(long kind) const { return m_tcs.FindC(kind); }
    __forceinline CTCBase* GetTC(long kind) { return m_tcs.Find(kind); }
};
struct CThingCreatureBase : CThing { };
struct CBaseIntelligentPointer {
    void* __vftable;
    void* m_pItem;
    CThing* GetPItem() const;                      // 0x00A01B50
};
class CPlayer {
public:
    unsigned char           _pad00[0x34];
    CBaseIntelligentPointer HeroPointer;           // +0x34
    unsigned char           _pad3c[0x218 - 0x3c];
    bool                    InputLocked;           // +0x218 (descriptive)
    CThingCreatureBase* GetHeroCreature();         // 0x00487DC0
};
struct CWorld {
    unsigned char _pad00[0xda];
    bool          AllowLockedUse;                  // +0xDA (descriptive)
    CPlayer* GetMainPlayer();                      // 0x0049D9D0
};
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0043DBD0 (CThing*)
bool __fastcall IsThingVulnerableToMeleeStrikesFromThing(CThing* target, CThing* attacker);   // 0x007F59F0
struct CTCCarrying : CTCBase {
    CThing* GetThingInPrimarySlot();               // 0x006A1440
};
struct CTCTargeting : CTCBase {
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32();
    virtual CThing* GetTargetedThing();            // slot 33 (+0x84)
    virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37();
    virtual bool IsTargetedThingInRange(long type);   // slot 38 (+0x98)
    bool IsTargetedThingOfType(long type);         // 0x007F5A70
};
struct CTCUseDef { unsigned char _pad[0x30]; long TutorialCategory; };
struct CTCOwner { unsigned char _pad[0x25]; bool Read; };
struct CTCActionUse : CTCBase {
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual CCharString GetClassName() const;      // slot 22 (+0x58)
    virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28();
    virtual unsigned long GetButtonMask() const;   // slot 29 (+0x74)
    unsigned char _pad04[0x08];
    CTCUseDef*    Def;                             // +0x0C
    bool          Enabled;                         // +0x10
    unsigned char _pad11[3];
    CTCOwner*     Owner;                           // +0x14
    long GetTutorialCategory() const;              // 0x007EAC00
};
struct CTCFlag : CTCBase { unsigned char _pad[0x08]; bool Set; };   // +0x0C
struct CTCHero : CTCBase {
    bool GiveTutorialForCategory(long category, bool force);   // 0x006C9FA0
};
struct CSoundInfo : CTCBase {
    void PlaySound(const CCharString& name, long a, long b, long c, long d);   // 0x0066BD00
};
class CInputProcessUseEnvironment : public CInputProcessGameBase {
public:
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90
    bool IsEventButtonReleased(const CInputEvent& event);             // 0x00687B30
    bool IsEventButtonHeld(const CInputEvent& event);                 // 0x00687BD0
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessUseEnvironment::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    if (MyPlayer->InputLocked)
        return;
    CThing* hero = MyPlayer->HeroPointer.GetPItem();
    if (!IsEventGameAction(6, event))
        return;
    unsigned long buttons = IsEventButtonPressed(event) ? 1 : 0;
    buttons |= IsEventButtonReleased(event) ? 2 : 0;
    buttons |= IsEventButtonHeld(event) ? 4 : 0;
    if (!buttons)
        return;
    CTCCarrying* carrying = 0;
    if (hero->HasTC(0x46))
        carrying = (CTCCarrying*)hero->GetTCC(0x46);
    CThing* item = carrying->GetThingInPrimarySlot();
    if (item && !(item->m_flags91 & 1) && item->HasTC(0xe)) {
        if (!item->HasTC(0xd))
            return;
        CTCActionUse* use = (CTCActionUse*)item->GetTCC(0xd);
        if (!(buttons & use->GetButtonMask()))
            return;
        if (!use->Enabled)
            return;
        CGameEvent ev(0x11);
        ev.AddUID(hero->UID);
        ev.AddUID(item->UID);
        processed.AddGameEvent(ev);
        return;
    }
    CTCTargeting* targeting = hero->HasTC(8) ? (CTCTargeting*)hero->GetTCC(8) : 0;
    CThing* enemy = targeting->GetTargetedThing();
    if (enemy && !(enemy->m_flags91 & 1) && IsThingVulnerableToMeleeStrikesFromThing(enemy, hero))
        return;
    CThing* person;
    if (targeting->IsTargetedThingOfType(2) && targeting->IsTargetedThingInRange(2) && (person = targeting->GetTargetedThing()) != 0) {
        if (!(buttons & 1))
            return;
        CGameEvent ev;
        if (person->HasTC(0x8f)) {
            ev.Type = 0x13;
            processed.AddGameEvent(ev);
            return;
        }
        ev.Type = 0x23;
        ev.AddBool(false);
        processed.AddGameEvent(ev);
        return;
    }
    if (!targeting->IsTargetedThingOfType(1))
        return;
    if (!targeting->IsTargetedThingInRange(1))
        return;
    CThing* thing = targeting->GetTargetedThing();
    if (!thing || (thing->m_flags91 & 1))
        return;
    bool usable = false;
    if (thing->HasTC(0xd)) {
        CTCActionUse* use = (CTCActionUse*)thing->GetTCC(0xd);
        if (use->Enabled) {
            if (use->GetTutorialCategory()) {
                CThingCreatureBase* player = World->GetMainPlayer()->GetHeroCreature();
                if (IS_THING_ALIVE<CThing*>(player) && player->HasTC(0x29)) {
                    CTCHero* heroTC = (CTCHero*)player->GetTC(0x29);
                    if (heroTC && heroTC->GiveTutorialForCategory(use->GetTutorialCategory(), false)) {
                        processed.SetHandled();
                        return;
                    }
                }
            }
            if (buttons & use->GetButtonMask())
                usable = true;
        }
    }
    if (((buttons & 1) && thing->HasTC(0xc2) && ((CTCFlag*)thing->GetTCC(0xc2))->Set) || usable) {
        if (thing->HasTC(0xd)) {
            CTCActionUse* use = (CTCActionUse*)thing->GetTC(0xd);
            if (use->GetClassName() == CCharString("CTCActionUseReadable")) {
                if (use->Owner->Read)
                    goto locked;
            }
        }
        if (!(thing->HasTC(0x6b) && !World->AllowLockedUse)) {
            CGameEvent ev(0x11);
            ev.AddUID(hero->UID);
            ev.AddUID(thing->UID);
            processed.AddGameEvent(ev);
            return;
        }
    }
locked:
    if (thing->HasTC(0x26) && (buttons & 1) && thing->HasTC(0x10)) {
        ((CSoundInfo*)thing->GetTC(0x10))->PlaySound(CCharString("DOOR_OPEN_FAIL"), 0, 0, 0, -1);
        processed.SetHandled();
    }
}
