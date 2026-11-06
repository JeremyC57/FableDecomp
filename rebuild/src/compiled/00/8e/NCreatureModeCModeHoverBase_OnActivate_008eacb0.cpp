// NCreatureMode::CModeHoverBase::OnActivate() -- activate the hover mode: the base OnActivate (a player also re-reads its movement speeds and finishes its current animation), build the hover-state table (three IDLE sectors, then the four directional SLOW / MEDIUM / FAST states of each speed whose FORWARD animation the creature has), pick the best state for a forward heading, start a random prefix animation unless the creature is acting, and create the hover particle effect attached to the effect helper point
// Leaf: ?OnActivate@CModeHoverBase@NCreatureMode@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008EACB0, 2548 bytes, thiscall `ret`, virtual: slot 5 of the CModeHoverBase vtable 0x01287D14
//   (also of CModeWalkInAllDirections 0x012687AC and CModeHoverNormal 0x012687FC, which inherit it).
// Identity: Ego_r.pdb ?OnActivate@CModeHoverBase@NCreatureMode@@UAEXXZ (donor 0x00911240, right after the ctor 0x009111A0 as retail
//   0x008EACB0 follows the ctor 0x008EAC40); FableWin.pdb 0x02AD16E0.
// Callees: CTCCreatureModeManager::CModeBase::OnActivate 0x0073AAC0 / GetWorld 0x0073A680, CThingPlayerCreature::InitialiseMovementSpeeds
//   0x006A9A00, CVectorMap LowerBound 0x0040F020 (inline GetTC TCI_ANIMATION), CTCAnimationComplex::GetAnimation 0x0070B460,
//   CAnimateThing::SetAsFinished 0x0070BE80 / SetMainAnim 0x0070D580, CAnimInit::Init 0x0070C050, operator+(const CCharString&, const char*)
//   0x0099F600, vector<CHoverState>::push_back 0x008EC1A0, NCreatureMode::GetAnimationFromCreature 0x008618B0, GetBestHoverState
//   0x008EA930, CModeHoverBase::InitialiseMovementSpeeds 0x008EA500, CThingCreatureBase::IsPerformingAnyAction 0x006622A0 /
//   GetRandomAnimFromName 0x00662D80, CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580, CSpecialEffectsDef::GetEffectFromString
//   0x00669D30, CThing::GetPos 0x004C73D0, CTCDParticleEmitter::Create 0x006E0880 / AttachToThing 0x006E0BE0, CBaseIntelligentPointer
//   0x00A01B10 / 0x00A01B50 / 0x00A01B90 / 0x00A01C10, CCharString copy 0x0099EC30 / = 0x0099EFB0 / dtor 0x0099EAE0.
// Virtuals: CModeHoverBase slots 12 GetAnimPrefix (+0x30), 13 GetAnimUpdateType (+0x34), 14 GetEffectName (+0x38), 15
//   GetEffectHelperPointName (+0x3c), all protected const (Ego_r / FableWin MBE).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_ACTIONQUERY
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE_MOVEMENTSPEEDS
#define FABLE_RETAIL_CMODEBASE_VIRTUALS
#define FABLE_RETAIL_STLP_VECTOR_PUSHBACK_OUTOFLINE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCDriverBase.h"
#include "engine/CTCCreatureModeManager.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefClassBase.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDefinitionManager.h"

template<> __declspec(noinline) bool CDefClassBase::GetSubDef<CSpecialEffectsDef>(CDefPointer<const CSpecialEffectsDef>* out) const
{
    const CSubDefInfo* index;
    { CCharString name("CSpecialEffectsDef",-1); index=GetPSubDefInfo(name); }
    if(index) {
        CDefPointer<const CSpecialEffectsDef> result(PDefManager->GetPDefFromGlobalIndex(index->DefIndex));
        *out=result;
        return true;
    }
    return false;
}

CCharString __fastcall operator+(const CCharString& left, const char* right);          // 0x0099F600

enum EAnimUpdateMode;

class CAnimationEntry {
public:
    long BankIndex;                                                                    // +0x00
};

class CBlendPair {
public:
    float First;
    float Second;
    CBlendPair() : First(0.0f), Second(0.0f) { }
};

class CAnimInit {                                                                      // 0x2c bytes, Ego_r.pdb members
public:
    EAnimUpdateMode UpdateMode;                                                        // +0x00
    unsigned char   Type;                                                              // +0x04
    bool            Looping;                                                           // +0x05
    long            Anim;                                                              // +0x08
    float           Speed;                                                             // +0x0c
    float           Alpha;                                                             // +0x10
    float           LastAlpha;                                                         // +0x14
    CBlendPair      Frame;                                                             // +0x18 CAnimFrameInfo
    unsigned long   Flags;                                                             // +0x20
    unsigned long   TypeFlags;                                                         // +0x24
    long            Priority;                                                          // +0x28
    // ?Init@CAnimInit@@QAEXEKJW4EAnimUpdateMode@@_NKJ@Z, out of line; the ctor ??0CAnimInit@@QAE@EKJW4EAnimUpdateMode@@_NKJ@Z is inline
    void Init(unsigned char type, unsigned long typeFlags, long anim, EAnimUpdateMode mode, bool looping, unsigned long flags, long priority);   // 0x0070C050
    CAnimInit(unsigned char type, unsigned long typeFlags, long anim, EAnimUpdateMode mode, bool looping, unsigned long flags, long priority)
    {
        Init(type, typeFlags, anim, mode, looping, flags, priority);
    }
};

class CAnimateThing {
public:
    void SetAsFinished();                                                              // 0x0070BE80
    void SetMainAnim(const CAnimInit& init, long mode, long delay);                    // 0x0070D580
};

class CTCAnimationComplex {
public:
    CAnimateThing& GetAnimation();                                                     // 0x0070B460
};

class CTCDParticleEmitter : public CTCDriverBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_PARTICLE_EMITTER; }
    static CThing* __fastcall Create(long effect, const C3DVector& pos, bool persistent = false);   // 0x006E0880
    void AttachToThing(CThing& thing, long priority, const CCharString& helperName, long helperIndex, float delay);   // 0x006E0BE0
};

namespace NCreatureMode {
const CAnimationEntry* __fastcall GetAnimationFromCreature(const CCharString& name, const CThingCreatureBase& creature);   // 0x008618B0

class CModeHoverBase : public CTCCreatureModeManager::CModeBase {
public:
    class CRadialArea {
    public:
        CRadialArea(float thetaLow, float thetaHigh) : ThetaLow(thetaLow), ThetaHigh(thetaHigh), RadHigh(100.0f) { }
        float ThetaLow;                                                                // +0x00
        float ThetaHigh;                                                               // +0x04
        float RadHigh;                                                                 // +0x08
    };
    class CHoverState {
    public:
        CHoverState(const CCharString& animName, const CRadialArea& area, EMovementType movementType)
            : AnimName(animName), Area(area), MovementType(movementType), MovementSpeed(0.0f) { }
        CCharString   AnimName;                                                        // +0x00
        CRadialArea   Area;                                                            // +0x04
        EMovementType MovementType;                                                    // +0x10
        float         MovementSpeed;                                                   // +0x14
    };
    virtual void OnActivate();
protected:
    virtual CCharString GetAnimPrefix() const = 0;                                     // slot 12 (+0x30)
    virtual EAnimUpdateMode GetAnimUpdateType() const = 0;                             // slot 13 (+0x34)
    virtual CCharString GetEffectName() const;                                         // slot 14 (+0x38)
    virtual CCharString GetEffectHelperPointName() const;                              // slot 15 (+0x3c)
private:
    const CHoverState* GetBestHoverState(const C3DVector& forward, const C3DVector& velocity) const;   // 0x008EA930
    void InitialiseMovementSpeeds();                                                   // 0x008EA500
    std::vector<CHoverState> HoverStates;                                              // +0x1c
    CCharString CurrentName;                                                           // +0x28
    const CHoverState* PCurrentState;                                                  // +0x2c
    long CurrentMovementType;                                                          // +0x30
    std::list<void*> AnimQueue;                                                        // +0x34 list<CCountedPointer<CHoverAnim> >
    CIntelligentPointer<CThing> PParticleEffect;                                       // +0x38
};

void CModeHoverBase::OnActivate()
{
    CTCCreatureModeManager::CModeBase::OnActivate();
    CThing& thing = ParentThing;
    if (thing.GetType() == 2) {
        ((CThingPlayerCreature&)thing).InitialiseMovementSpeeds();
        CTCAnimationComplex* animComplex;
        thing.GetTC(TCI_ANIMATION, &animComplex);
        animComplex->GetAnimation().SetAsFinished();
    }
    CThingCreatureBase& creature = (CThingCreatureBase&)ParentThing;
    CWorld& world = GetWorld();
    CCharString prefix = GetAnimPrefix();
    HoverStates.push_back(CHoverState(prefix + "IDLE", CRadialArea(0.0f / 360.0f, 121.0f / 360.0f), (EMovementType)0));
    HoverStates.push_back(CHoverState(prefix + "IDLE", CRadialArea(120.0f / 360.0f, 241.0f / 360.0f), (EMovementType)0));
    HoverStates.push_back(CHoverState(prefix + "IDLE", CRadialArea(240.0f / 360.0f, 1.0f / 360.0f), (EMovementType)0));
    if (GetAnimationFromCreature(prefix + "FORWARD_SLOW", creature) != 0) {
        HoverStates.push_back(CHoverState(prefix + "FORWARD_SLOW", CRadialArea(-45.0f / 360.0f, 46.0f / 360.0f), (EMovementType)8));
        HoverStates.push_back(CHoverState(prefix + "RIGHT_SLOW", CRadialArea(45.0f / 360.0f, 136.0f / 360.0f), (EMovementType)8));
        HoverStates.push_back(CHoverState(prefix + "BACK_SLOW", CRadialArea(135.0f / 360.0f, 226.0f / 360.0f), (EMovementType)8));
        HoverStates.push_back(CHoverState(prefix + "LEFT_SLOW", CRadialArea(225.0f / 360.0f, 316.0f / 360.0f), (EMovementType)8));
    }
    if (GetAnimationFromCreature(prefix + "FORWARD_MEDIUM", creature) != 0) {
        HoverStates.push_back(CHoverState(prefix + "FORWARD_MEDIUM", CRadialArea(-45.0f / 360.0f, 46.0f / 360.0f), (EMovementType)9));
        HoverStates.push_back(CHoverState(prefix + "RIGHT_MEDIUM", CRadialArea(45.0f / 360.0f, 136.0f / 360.0f), (EMovementType)9));
        HoverStates.push_back(CHoverState(prefix + "BACK_MEDIUM", CRadialArea(135.0f / 360.0f, 226.0f / 360.0f), (EMovementType)9));
        HoverStates.push_back(CHoverState(prefix + "LEFT_MEDIUM", CRadialArea(225.0f / 360.0f, 316.0f / 360.0f), (EMovementType)9));
    }
    if (GetAnimationFromCreature(prefix + "FORWARD_FAST", creature) != 0) {
        HoverStates.push_back(CHoverState(prefix + "FORWARD_FAST", CRadialArea(-45.0f / 360.0f, 46.0f / 360.0f), (EMovementType)10));
        HoverStates.push_back(CHoverState(prefix + "RIGHT_FAST", CRadialArea(45.0f / 360.0f, 136.0f / 360.0f), (EMovementType)10));
        HoverStates.push_back(CHoverState(prefix + "BACK_FAST", CRadialArea(135.0f / 360.0f, 226.0f / 360.0f), (EMovementType)10));
        HoverStates.push_back(CHoverState(prefix + "LEFT_FAST", CRadialArea(225.0f / 360.0f, 316.0f / 360.0f), (EMovementType)10));
    }
    CurrentMovementType = 0;
    PCurrentState = GetBestHoverState(C3DVector(0.0f, 0.0f, 0.0f), C3DVector(1.0f, 0.0f, 0.0f));
    CurrentName = PCurrentState->AnimName;
    InitialiseMovementSpeeds();
    if (!creature.IsPerformingAnyAction()) {
        CTCAnimationComplex* animComplex;
        ParentThing.GetTC(TCI_ANIMATION, &animComplex);
        CAnimateThing& animation = animComplex->GetAnimation();
        const CAnimationEntry& entry = creature.GetRandomAnimFromName(CurrentName, -1);
        animation.SetMainAnim(CAnimInit(5, 0, entry.BankIndex, GetAnimUpdateType(), true, 0, 100), 1, 0);
    }
    CDefPointer<const CSpecialEffectsDef> effects;
    if (creature.PeekBaseDef().GetSubDef(&effects)) {
        long effect = 0;
        if (effects->GetEffectFromString(GetEffectName(), effect)) {
            PParticleEffect = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(effect, creature.GetPos()));
            if (PParticleEffect.Get() && !PParticleEffect->IsDead()) {
                CTCDParticleEmitter* emitter = 0;
                PParticleEffect->PeekTCDriver(&emitter);
                emitter->AttachToThing(creature, 0xc, GetEffectHelperPointName(), 0, 0.0f);
            }
        }
    }
}
}