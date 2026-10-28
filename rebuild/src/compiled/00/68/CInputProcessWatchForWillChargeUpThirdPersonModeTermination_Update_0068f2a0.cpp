// CInputProcessWatchForWillChargeUpThirdPersonModeTermination::Update(CProcessedInput& processed) -- while the live hero's will-charge component (kind 0xa7) still runs, hold the quit timer at 5; once it has ended, count the timer down and leave player mode 0x23 when it runs out
// Leaf: Update
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0068F2A0, length 120 bytes.
// Callees: CPlayer::GetPControlledCreature @ 0x00487DC0 (MyPlayer +0x18), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound (const)
// @ 0x004365B0, the component's running test @ 0x0072A860 (`Running != 0` at +0xc; manifest CASuspendableProcess::
// CheckTermination), CPlayer::RemoveMode @ 0x00634190.
// Model: engine/CInputProcessWatchForWillChargeUpThirdPersonModeTermination.h (QuitModeTimer +0x28) over the
// CInputProcessGameBase members; the thing's presence bitset (+0x20: kind 0xa7 is bit 7 of the byte at +0x34, tested as
// `test dl,dl ; jns`), component map +0x44 and dead bit +0x91 as the landed CGameScriptInterface rows.
// Shape: the component is a null-initialised local assigned only when present (the zero preloaded in ecx; a ternary
// materialises it in eax) and handed to the test unchecked; the
// timer decrement is `--QuitModeTimer < 0` (`dec [edi+28h] ; jns`).
class CProcessedInput;
struct CTCBase_Retail;
struct STCEntry_Retail {
    long            m_kind;
    CTCBase_Retail* m_pTC;
};
struct CTCMap_Retail {
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const unsigned long& kind) const;   // 0x004365B0
    __forceinline CTCBase_Retail* Find(long kind) const
    {
        STCEntry_Retail* it = LowerBound((unsigned long)kind);
        if (it == m_pLast || kind < it->m_kind)
            it = m_pLast;
        return it->m_pTC;
    }
};
struct CThing_Retail {
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                    // +0x20
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    unsigned char  _pad50[0x41];
    unsigned char  m_dead : 1;                     // +0x91 bit 0
    unsigned char  m_91_1 : 7;
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline CTCBase_Retail* GetTC(long kind) const { return m_tcs.Find(kind); }
};
class CThingPlayerCreature : public CThing_Retail { };
class CASuspendableProcess { public:
    bool CheckTermination();                       // 0x0072A860
};
class CPlayer {
public:
    CThingPlayerCreature* GetPControlledCreature() const;              // 0x00487DC0
    void RemoveMode(long mode);                    // 0x00634190
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
};
class CInputProcessWatchForWillChargeUpThirdPersonModeTermination : public CInputProcessGameBase {
public:
    long QuitModeTimer;                            // +0x28
    virtual void Update(CProcessedInput& processed);
};

void CInputProcessWatchForWillChargeUpThirdPersonModeTermination::Update(CProcessedInput& processed)
{
    CThing_Retail* hero = MyPlayer->GetPControlledCreature();
    if (hero && !hero->m_dead) {
        CASuspendableProcess* charge = 0;
        if (hero->HasTC(0xa7))
            charge = (CASuspendableProcess*)hero->GetTC(0xa7);
        if (charge->CheckTermination()) {
            QuitModeTimer = 5;
            return;
        }
        if (--QuitModeTimer < 0)
            MyPlayer->RemoveMode(0x23);
    }
}