// CTCCreatureOpinionOfHero::OnHit(bool heavy) -- count one more hit by the hero (1.5 for a heavy hit, else 1), push back the decay frame, and report whether the creature's tolerance to being hit is used up (always true for a heavy hit)
// Leaf: ?OnHit@CTCCreatureOpinionOfHero@@QAE_N_N@Z
// Naming wave 25: Ego_r.pdb / FableWin.pdb order GetToleranceToBeingHit / GetToleranceToBeingHitRemaining / OnHit(bool) as retail
//   0x00681940 / 0x00681980 / this row: OnHit (was UpdateHitTolerance); the tail is GetToleranceToBeingHitRemaining 0x00681980 inlined,
//   over GetToleranceToBeingHit 0x00681940 (was spelled GetToleranceToBeingHitRemaining).
// Naming wave 18: the global stand-in g_hitCountDecaySeconds (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006819D0, length 149 bytes (padded to 160), `ret 4`.
// Label correction: the catalog's "CViewBase::" class is wrong: `this` is a CTCCreatureOpinionOfHero (ParentThing +4,
// NumberOfTimesHit +0x10, ToleranceToBeingHitOverride +0x14, FrameToDecayNumberOfTimesHit +0x18, PPersonalityDef +0x20); the
// tail is the landed CTCCreatureOpinionOfHero::GetToleranceToBeingHitRemaining 0x00681940 inlined.
// Callees: CWorld::GetWorldFrame (static) @ 0x0049D870 (returns the frame global 0x013B89BC; catalog "GFGetPreMainMemoryUsed"),
// CTCBase::PeekWorld @ 0x00686A80, COpinionReactionManager::GetToleranceToBeingHit(const CThing*) @ 0x00701940.
// Data: the hit-decay time 0x01375550 (a long scaled by 0x78 frames), 1.5f @ 0x0125BA98, 1.0f @ 0x0122DED8, 0.0f @ 0x0122DEDC.
// Shape: the decay global is a named local read before the frame call (edi); `remaining - hits <= 0.0f` returned as an int-widened compare
// (`mov eax,1` / `xor eax,eax`) while the heavy-hit exit is a byte `mov al,1`.
class CThing;
class COpinionReactionManager { public:
    float GetToleranceToBeingHit(const CThing* thing) const;        // 0x00701940
};
class CWorld { public:
    unsigned char _pad00[0x74];
    COpinionReactionManager* POpinionReactionManager;   // +0x74
    static long __fastcall GetFrame();                         // 0x0049D870
};
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};
class COpinionPersonalityDef { public:
    unsigned char _pad00[0x50];
    float ToleranceToBeingHit;                     // +0x50
};
class CTCBase
{
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80
};

class CTCCreatureOpinionOfHero : public CTCBase { public:
    void*   __vftable;                             // +0x00 (CTCBase)
    CThing* ParentThing;                           // +0x04
    bool    ToBeRemoved;                           // +0x08
    bool    IsInGlobalUpdateTCs;                   // +0x09
    long    LastOpinionReactionFrame;              // +0x0c
    float   NumberOfTimesHit;                      // +0x10
    float   ToleranceToBeingHitOverride;           // +0x14
    long    FrameToDecayNumberOfTimesHit;          // +0x18
    void*   PDef;                                  // +0x1c CDefPointer<COpinionOfHeroDef const>
    COpinionPersonalityDef* PPersonalityDef;       // +0x20 CDefPointer<COpinionPersonalityDef const>
    float GetToleranceToBeingHit() const           // 0x00681940 (landed; inline here)
    {
        if (ToleranceToBeingHitOverride >= 0.0f)
            return ToleranceToBeingHitOverride;
        CThing* thing = ParentThing;
        COpinionReactionManager* manager = PeekWorld().POpinionReactionManager;
        float tolerance = manager->GetToleranceToBeingHit(thing);
        if (PPersonalityDef)
            tolerance += PPersonalityDef->ToleranceToBeingHit;
        return tolerance;
    }
    float GetToleranceToBeingHitRemaining() const  // 0x00681980 (landed; inline here)
    {
        return GetToleranceToBeingHit() - NumberOfTimesHit;
    }
    bool OnHit(bool heavy);
};
bool CTCCreatureOpinionOfHero::OnHit(bool heavy)
{
    NumberOfTimesHit += heavy ? 1.5f : 1.0f;
    long decaySeconds = CMainGameComponent::GetConstantFPS();
    FrameToDecayNumberOfTimesHit = CWorld::GetFrame() + decaySeconds * 0x78;
    if (heavy)
        return true;
    return GetToleranceToBeingHitRemaining() <= 0.0f;
}