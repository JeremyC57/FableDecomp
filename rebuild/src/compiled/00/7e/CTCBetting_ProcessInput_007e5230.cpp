// CTCBetting::ProcessInput(const CInputEvent& event, CProcessedInput& processed, CInputProcessGameBase* input) -- while betting: the left stick's vertical push steps the bet up or down (repeat rate speeds up the longer it is held; clamped to the def's range and the hero's gold) with a tavern sound, the horizontal push snaps the bet to 1 or the previous donation; the accept / cancel game actions close the subtitle and end betting
// Leaf: ProcessInput
// SW2 sweep (advisor/007e5230/probe/t1.cpp).
// Retail 0x007E5230, 832 bytes, `ret 0xc`. Callees: CPlayerManager::GetMainPlayer 0x00449960 (g_pMainGameComponent +0x1c),
// CPlayer::GetPControlledCreature 0x00487DC0, LowerBound 0x0040F020, GetSubDef<CBettingDef> 0x007E5820,
// CInputProcessGameBase::GetEventGameActionVector 0x00687D00 / IsEventButtonPressed 0x00687A90 / IsEventGameAction
// 0x00687CF0, CMainGameComponent::GetFrame 0x00416225, CInputEvent::GetEventType 0x00A03B40, CCharString 0x0099EBF0 /
// dtor 0x0099EAE0, CTCBetting::PlaySound 0x007E51D0, CProcessedInput::SetHandled 0x00A0D390, CSubtitleRenderer::Clear
// 0x00492BAB. Globals: betting flag 0x013BAE1D, bet placed 0x013BAE1E, bet 0x013BAE20, repeat delay/rates 0x01384040..4C.
// Layout: CTCBetting as the landed 0x007E57C0; CBettingDef MaxBet +0x3c, MinBet +0x38, Step +0x40; hero gold TC (4) +0x3c.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class C2DVector { public:
    float x, y;
};
class CInputEvent {
public:
    long GetEventType() const;                          // 0x00A03B40
};
class CProcessedInput {
public:
    void SetHandled();                                  // 0x00A0D390
};
struct CInputProcessGameBase {
    bool IsEventButtonPressed(const CInputEvent& event);                // 0x00687A90
    bool IsEventGameAction(long action, const CInputEvent& event);      // 0x00687CF0
    bool GetEventGameActionVector(long action, const CInputEvent& event, C2DVector* vector);   // 0x00687D00
};
class CSubtitleRenderer { public:
    void Clear(bool now);                               // 0x00492BAB
};
class CPlayerGui { public:
    unsigned char     _pad00[0x18];
    CSubtitleRenderer SubtitleRenderer;                 // +0x18
};
extern CPlayerGui* g_pPlayerGui;                        // 0x013B8790
class CTCHeroStats { public:
    unsigned char _pad00[0x3c];
    long          Gold;                                 // +0x3c
};
struct STCEntry { long type; CTCHeroStats* component; };
struct CTCMap {
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind);    // 0x0040F020
    __forceinline CTCHeroStats* Find(long kind)
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->type) ? m_pLast : it)->component;
    }
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_tcMask[9];                          // +0x20
    CTCMap        m_tcs;                                // +0x44
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCHeroStats* GetTC(long kind) { return m_tcs.Find(kind); }
};
class CThingPlayerCreature : public CThing { };
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                          // 0x00487DC0
};
class CPlayerManager { public:
    const CPlayer& GetMainPlayer() const;                           // 0x00449960
};
class CMainGameComponent { public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* PlayerManager;                      // +0x1c
    long GetFrame();                                    // 0x00416225
};
extern CMainGameComponent* g_pMainGameComponent;       // 0x013B86A0
extern bool g_BettingActive;                            // 0x013BAE1D
extern bool g_BetPlaced;                                // 0x013BAE1E
extern long g_CurrentBet;                               // 0x013BAE20
extern long g_BetRepeatDelay;                           // 0x01384040
extern long g_BetRepeatSlow;                            // 0x01384044
extern long g_BetRepeatFast;                            // 0x01384048
extern long g_BetMultiplierRate;                        // 0x0138404C

class CBettingDef { public:
    virtual void v00();
    virtual void Destroy();                             // slot 1
    long          RefCount;                             // +0x04
    unsigned char _pad08[0x38 - 0x08];
    long          MinBet;                               // +0x38
    long          MaxBet;                               // +0x3c
    long          BetStep;                              // +0x40
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CBettingDef>* out);      // 0x007E5820
};
void* FindDef_CDefClassBase_GetSubDef_0();             // stand-in callee
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CBettingDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
struct CParentThing {
    unsigned char  _pad00[0x70];
    CDefClassBase* m_pDef;                              // +0x70
    CDefClassBase* GetDef() const { return m_pDef; }
};
class CTCBetting { public:
    void*         __vftable;                            // +0x00
    CParentThing* ParentThing;                          // +0x04
    unsigned char _pad08[0x28 - 0x08];
    long          FrameBetWasLastChanged;               // +0x28
    long          FrameBetChangeStarted;                // +0x2c
    long          Mult;                                 // +0x30
    long          FrameToStart;                         // +0x34
    long          PreviousDonation;                     // +0x38
    void PlaySound(const CCharString& name);            // 0x007E51D0
    void ProcessInput(const CInputEvent& event, CProcessedInput& processed, CInputProcessGameBase* input);
};

// same-TU stand-in for CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound (0x0040F020): VC7.1 then knows the returned entry
// is not derived from the key it was handed, so the key slot dies after the call and packs into the def slot.
__declspec(noinline) STCEntry* CTCMap::LowerBound(const unsigned long& kind)
{
    STCEntry* first = m_pFirst;
    long count = m_pLast - m_pFirst;
    while (count > 0) {
        long half = count >> 1;
        STCEntry* mid = first + half;
        if ((unsigned long)mid->type < kind) {
            first = mid + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return first;
}

void CTCBetting::ProcessInput(const CInputEvent& event, CProcessedInput& processed, CInputProcessGameBase* input)
{
    CTCHeroStats* stats = 0;
    CThing* hero = g_pMainGameComponent->PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (hero->HasTC(4))
        stats = hero->GetTC(4);
    CDefPointer<CBettingDef> def;
    if (g_BettingActive && ParentThing->GetDef()->GetSubDef(&def)) {
    C2DVector stick;
    if (input->GetEventGameActionVector(0x72, event, &stick) && FrameToStart <= g_pMainGameComponent->GetFrame()
        && event.GetEventType() == 2) {
        long held = g_pMainGameComponent->GetFrame() - FrameBetChangeStarted;
        bool change = false;
        if (FrameBetWasLastChanged == g_pMainGameComponent->GetFrame() - 1) {
            held -= g_BetRepeatDelay;
            if ((held >= 0 && held % g_BetRepeatSlow == 0) || (held >= g_BetRepeatDelay && held % g_BetRepeatFast == 0))
                change = true;
            if (held != 0 && held % g_BetMultiplierRate == 0)
                Mult *= 2;
        } else {
            FrameBetChangeStarted = g_pMainGameComponent->GetFrame();
            change = true;
            Mult = 1;
        }
        if (stick.y > 0.2f) {
            FrameBetWasLastChanged = g_pMainGameComponent->GetFrame();
            if ((g_CurrentBet < def->MaxBet || def->MaxBet == 0) && change && g_CurrentBet < stats->Gold) {
                PlaySound("TAVERN_GAME_BET_INCREMENT");
                g_CurrentBet += def->BetStep * Mult;
            }
            processed.SetHandled();
        } else if (stick.y < -0.2f) {
            FrameBetWasLastChanged = g_pMainGameComponent->GetFrame();
            if (g_CurrentBet != def->MinBet && change) {
                PlaySound("TAVERN_GAME_BET_DECREMENT");
                g_CurrentBet -= def->BetStep * Mult;
                if (g_CurrentBet < def->MinBet)
                    g_CurrentBet = def->MinBet;
            }
            processed.SetHandled();
        }
        if (stick.x < -0.2f)
            g_CurrentBet = 1;
        else if (stick.x > 0.2f)
            g_CurrentBet = PreviousDonation;
        if (g_CurrentBet > stats->Gold)
            g_CurrentBet = stats->Gold;
    }
    if (input->IsEventButtonPressed(event)) {
        if (input->IsEventGameAction(0x16, event)) {
            g_pPlayerGui->SubtitleRenderer.Clear(false);
            processed.SetHandled();
            g_BettingActive = false;
            g_BetPlaced = true;
            PreviousDonation = g_CurrentBet;
        } else if (input->IsEventGameAction(0x17, event)) {
            g_pPlayerGui->SubtitleRenderer.Clear(false);
            processed.SetHandled();
            g_BettingActive = false;
            g_BetPlaced = false;
        }
    }
    }
}