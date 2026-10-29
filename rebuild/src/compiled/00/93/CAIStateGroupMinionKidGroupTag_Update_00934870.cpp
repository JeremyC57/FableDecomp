// CAIStateGroup_MinionKidGroupTag::Update() -- the kids' game of tag: each pass a 5% chance ends the game; otherwise the tagger chases (TagTarget) and everyone else runs (EvadeTarget), repeating while the do-nothing action reports 4; then Terminate
// Leaf: ?Update@CAIStateGroup_MinionKidGroupTag@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00934870, 169 bytes, both exits tail-jump to Terminate.
// Identity: Ego_r.pdb `?Update@CAIStateGroup_MinionKidGroupTag@@UAEXXZ`; donor order InitStateGroup / OnReplace / IsStillValid /
//   TagTarget / OnActivate / EvadeTarget / Cleanup / IsRunnable / Update against retail 0x00934090 / 0x009340F0 / 0x00934100 /
//   0x00934120 / 0x009341C0 / 0x00934410 / 0x009347B0 / 0x00934870 / 0x00934920. The catalog label is right.
// Callees: CAIStateGroupBase::PeekWorld() const 0x008FCFA0 (settled `?PeekWorld@CAIStateGroupBase@@IBEABVCWorld@@XZ`),
//   CWorld::GetFrame() 0x0049D870 (static `?GetFrame@CWorld@@SIJXZ`; called through the PeekWorld reference, value unused),
//   CAIStateGroupBase::GetPWorldSeed() const 0x008FCFD0 (Ego_r.pdb `IBEPAKXZ`), GFROR13 0x00497890 (`?GFROR13@@YIJK@Z`), _CIfmod
//   0x00BFEB28, CTCCreatureGroupTag::GetTagger() const 0x008138E0 (Ego_r.pdb `QBEPBVCThingAICreature@@XZ`), the private
//   TagTarget 0x009341C0 / EvadeTarget 0x00934410 (Ego_r.pdb AAEXXZ), CAIStateGroupBase::AIAction_DoNothing 0x0097E200 (Ego_r.pdb
//   protected `IAE?AW4EAIActionReturn@@XZ`), CAIStateGroupBase::Terminate 0x008FCF40 (Ego_r.pdb QAEXXZ).
// Inline: GFFloatRandom(float, unsigned long*) (Ego_r.pdb `?GFFloatRandom@@YIMMPAK@Z`): seed = ROR13(seed * 0x24A1 + 0x24DF), then
//   fmod(seed, range * 65536) / 65536 (constants 6553600.0 0x012922C8, 1/65536 0x012387C0, 2^32 0x012387C8, 5.0f 0x01292280).
// Layout (Ego_r.pdb, no containers): CAIStateGroupBase PBrain +0x04 .. Enabled +0x12; PTCGroupTag +0x18; CAIBrain Creature +0x18.
// Shape: the creature comes through the inline reference accessor CAIStateGroupBase::GetCreature(), read before the
//   GetTagger call and kept in edi (a raw `PBrain->Creature` operand is re-read after the call); `do { if (chance < 5) { Terminate(); return; } ... } while (AIAction_DoNothing() == 4); Terminate();` (two tails).
extern "C" double __cdecl fmod(double x, double y);
#pragma intrinsic(fmod)
long __fastcall GFROR13(unsigned long value);               // 0x00497890
inline float __fastcall GFFloatRandom(float range, unsigned long* seed)
{
    *seed = *seed * 0x24A1 + 0x24DF;
    *seed = GFROR13(*seed);
    return (float)(fmod((double)*seed, range * 65536.0) / 65536.0);
}

class CWorld { public:
    static long __fastcall GetFrame();                      // 0x0049D870
};
class CThingAICreature;
class CTCCreatureGroupTag { public:
    const CThingAICreature* GetTagger() const;              // 0x008138E0
};
class CAIBrain { public:
    unsigned char       _pad_0x00[0x18];
    CThingAICreature*   Creature;                           // +0x18
};
class CCharString { public:
    void* PStringData;
};
enum EScriptAIPriority { };
enum EAIActionReturn { };

class CBaseClass { public:
    virtual ~CBaseClass();                                  // 0x0099A2E0
};
class CAIStateGroupBase : public CBaseClass { public:
    CAIBrain*           PBrain;                             // +0x04
    EScriptAIPriority   ScriptAIPriority;                   // +0x08
    CCharString         Name;                               // +0x0c
    bool                Terminated;                         // +0x10
    unsigned char       Priority;                           // +0x11
    bool                Enabled;                            // +0x12
    void Terminate();                                       // 0x008FCF40
protected:
    CThingAICreature& GetCreature() const { return *PBrain->Creature; }   // Ego_r.pdb ?GetCreature@CAIStateGroupBase@@IBEAAVCThingAICreature@@XZ (inline)
    const CWorld& PeekWorld() const;                        // 0x008FCFA0
    unsigned long* GetPWorldSeed() const;                   // 0x008FCFD0
    EAIActionReturn AIAction_DoNothing();                   // 0x0097E200
};

class C3DVector { public:
    float X, Y, Z;
};
class CAIStateGroup_MinionKidGroupTag : public CAIStateGroupBase { public:
    bool                    InitiatedTag;                   // +0x14
    CTCCreatureGroupTag*    PTCGroupTag;                    // +0x18
    C3DVector               InitialTagPos;                  // +0x1c
    bool                    StandModeAddedToCreature;       // +0x28
    long                    FrameLastPlayedIn;              // +0x2c
    virtual void Update();
private:
    void TagTarget();                                       // 0x009341C0
    void EvadeTarget();                                     // 0x00934410
};

void CAIStateGroup_MinionKidGroupTag::Update()
{
    do {
        PeekWorld().GetFrame();
        if (GFFloatRandom(100.0f, GetPWorldSeed()) < 5.0f) {
            Terminate();
            return;
        }
        if (PTCGroupTag->GetTagger() == &GetCreature())
            TagTarget();
        else
            EvadeTarget();
    } while (AIAction_DoNothing() == 4);
    Terminate();
}