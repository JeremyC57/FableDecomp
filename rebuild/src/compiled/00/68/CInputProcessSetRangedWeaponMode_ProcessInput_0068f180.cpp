// CInputProcessSetRangedWeaponMode::ProcessInput(const CInputEvent& event, CProcessedInput& input) -- while the ranged-weapon-mode button (game action 0x2d) is held, queue game event 0x31 for a live hero; when the global gate is set, the player is not first-person targeting and the hero has component kind 0x81, that component's +0x15 flag must also be set
// Leaf: ?ProcessInput@CInputProcessSetRangedWeaponMode@@UAEXABVCInputEvent@@AAVCProcessedInput@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0068F180, 201 bytes, `ret 8`.
// Callees: CInputProcessGameBase::IsEventButtonHeld @ 0x00687BD0 and IsEventGameAction @ 0x00687CF0 (landed model
//   names), CPlayer::IsFirstPersonTargeting @ 0x004874D0, CPlayer::GetHeroThing @ 0x00487DC0 (x2), the const component-map
//   LowerBound @ 0x004365B0, CProcessedInput::AddGameEvent(const CGameEvent&) @ 0x00A0D340.
// Globals: a bool gate @ 0x01375752 (unnamed).
// Layout: engine/CInputProcessGameBase.h (MyPlayer +0x18), local model as in the landed 0x0068EA60 (the class carries
//   the virtual); CGameEvent (Ego_r.pdb, 40 bytes); retail CThing component bitset +0x20 (kind 0x81 = byte +0x30 bit 1),
//   map +0x44, Dead bit +0x91.
// Shape: the gate is `if (g && !first-person) { if (hero->GetInterface(&tc, 0x81) && !tc->flag) return; }` (one hero
//   fetch; the out-parameter lookup leaves the component in eax), then the live-hero test and the event built by a
//   `__forceinline AddGameEvent(long type)` helper on CProcessedInput (the &event push precedes the input reload; the
//   Valid 1 shares cl with the dead-bit mask test).
class CInputEvent;
struct CMemoryBuffer {
};
struct CGameEvent : CMemoryBuffer {
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
    CGameEvent(long type) : Type(type), Player(-1), EndPos(0), Valid(true), Replacement(false) { }
};
class CProcessedInput {
public:
    void AddGameEvent(const CGameEvent& event);    // 0x00A0D340
    __forceinline void AddGameEvent(long type)
    {
        CGameEvent gameEvent(type);
        AddGameEvent(gameEvent);
    }
};
struct CTCBase_Retail;
struct STCEntry_Retail {                           // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind) const;   // 0x004365B0 (the const instantiation)
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
struct CTC81 {                                     // component kind 0x81
    unsigned char _pad00[0x15];
    bool          m_15;                            // +0x15
};
struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x91 - 0x50];
    unsigned char  Dead : 1;                       // +0x91 bit 0
    unsigned char  _bits91 : 7;
    __forceinline bool HasTC(long kind) const
    {
        if (m_tcMask[kind >> 5] & (1UL << (kind & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetInterface(T** out, long kind) const   // the out-parameter lookup
    {
        if (HasTC(kind)) {
            *out = (T*)m_tcs.Find(kind);
            return true;
        }
        return false;
    }
};
class CPlayer {
public:
    bool IsFirstPersonTargeting();                 // 0x004874D0
    CThing* GetHeroThing();                        // 0x00487DC0
};
extern bool g_rangedWeaponModeGate;                // 0x01375752
struct CWorld; struct CGameDefinitionManager; struct CMainGameComponent; struct CGamePlayerInterface;
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00();
    unsigned char           _pad04[0x10];          // +0x04 CAInputProcess
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    bool IsEventButtonHeld(const CInputEvent& event);                 // 0x00687BD0 (kind 0x15)
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
};
class CInputProcessSetRangedWeaponMode : public CInputProcessGameBase {
public:
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& input);
};

void CInputProcessSetRangedWeaponMode::ProcessInput(const CInputEvent& event, CProcessedInput& input)
{
    if (IsEventButtonHeld(event) && IsEventGameAction(0x2d, event)) {
        if (g_rangedWeaponModeGate && !MyPlayer->IsFirstPersonTargeting()) {
            CTC81* tc;
            if (MyPlayer->GetHeroThing()->GetInterface(&tc, 0x81) && !tc->m_15)
                return;
        }
        CThing* hero = MyPlayer->GetHeroThing();
        if (hero && !hero->Dead)
            input.AddGameEvent(0x31);
    }
}