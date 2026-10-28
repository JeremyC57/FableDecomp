// CInputProcessCreditsUI::ProcessInput(const CInputEvent& event, CProcessedInput& processed) -- while the hero's credits-UI component (kind 0x101) is active and not already quitting, a press of game action 0x22 starts quitting the credits
// Leaf: ProcessInput
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00691470, length 121 bytes.
// Callees: CPlayer::GetPControlledCreature @ 0x00487DC0 (MyPlayer +0x18), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x0040F020 (the thing's component map at +0x44, key by address), CInputProcessGameBase::IsEventButtonPressed
// @ 0x00687A90 (manifest "IsEventButtonHeld"), IsEventGameAction @ 0x00687CF0, CTCCreditsUI::Quit @ 0x007E49C0
// (`if (Active) Quitting = true`; manifest label "Deactivate").
// Model: the thing / component-map plumbing as the landed CGameScriptInterface rows (presence bitset at +0x20, so kind
// 0x101 is bit 1 of the word at +0x40). CTCCreditsUI from Ego_r.pdb with retail's 12-byte STLport vector: Active +0x0c,
// Quitting +0x1c (PDB +0x20). The input-process base as engine/CInputProcessGameBase.h (MyPlayer +0x18); the
// engine/CInputProcessCreditsUI.h header only flattens that base, so the base members are modelled here.
// Shape: the inlined map find (`it == end || key < it->key` -> end) keeps the component in esi; the event rides in edi.
class CInputEvent;
class CProcessedInput;
struct CTCBase_Retail;
struct STCEntry_Retail {                           // one CVectorMap<ETCInterfaceType, CTCBase*> element
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind);   // 0x0040F020
    __forceinline CTCBase_Retail* Find(long kind)
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
struct CThing_Retail {
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};
class CThingPlayerCreature : public CThing_Retail { };
class CTCCreditsUI {
public:
    unsigned char _base00[0xc];                    // CTCBase (vptr, ParentThing, flags)
    bool          Active;                          // +0x0c
    unsigned char _pad0d[3];
    void*         Components[3];                   // +0x10 std::vector<NUISystem::CComponent*> (12 bytes in retail)
    bool          Quitting;                        // +0x1c
    void Quit();                                   // 0x007E49C0
};
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;              // 0x00487DC0
};
class CWorld; class CGameDefinitionManager; class CMainGameComponent; class CGamePlayerInterface;
class CInputProcessGameBase {                      // engine/CInputProcessGameBase.h layout
public:
    virtual void v00();
    unsigned char           _pad04[0x10];          // +0x04 CAInputProcess
    CWorld*                 World;                 // +0x14
    CPlayer*                MyPlayer;              // +0x18
    CGameDefinitionManager* DefinitionManager;     // +0x1c
    CMainGameComponent*     Component;             // +0x20
    CGamePlayerInterface*   PGamePlayerInterface;  // +0x24
    bool IsEventButtonPressed(const CInputEvent& event);              // 0x00687A90 (kind 0x13)
    bool IsEventGameAction(long action, const CInputEvent& event);    // 0x00687CF0
};
class CInputProcessCreditsUI : public CInputProcessGameBase {
public:
    virtual void ProcessInput(const CInputEvent& event, CProcessedInput& processed);
};

void CInputProcessCreditsUI::ProcessInput(const CInputEvent& event, CProcessedInput& processed)
{
    CThing_Retail* hero = MyPlayer->GetPControlledCreature();
    if (hero->HasTC(0x101)) {
        CTCCreditsUI* credits = (CTCCreditsUI*)hero->m_tcs.Find(0x101);
        if (credits->Active && !credits->Quitting
            && IsEventButtonPressed(event) && IsEventGameAction(0x22, event))
            credits->Quit();
    }
}