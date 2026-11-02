// CTCAIScratchpad::~CTCAIScratchpad() -- implicit destructor of the AI scratchpad component: the scratchpad def pointer released, then SeatName, TableName, TargetName and Action destroyed, then ~CTCBase
// Leaf: ??1CTCAIScratchpad@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007ADAA0, 73 bytes, thiscall, tail `jmp ~CTCBase` 0x00686830.
// Identity (proven, two sources): its only caller is the scalar deleting dtor 0x007ADA80, slot 0 of vtable 0x0126D07C (rtti-vtables.tsv:
//   RTTI .?AVCTCAIScratchpad@@ : CTCBase : CBaseClassNonCopyable : CBaseClass), the vtable the ctor 0x007ABB80 stores (reached from the
//   Alloc's `push 84h` at 0x004D4E21 = sizeof Ego_r CTCAIScratchpad); the member offsets it destroys (+0x80 def pointer, CCharStrings
//   +0x74 / +0x70 / +0x50 / +0x48) are Ego_r.pdb CTCAIScratchpad's PDef / SeatName / TableName / TargetName / Action. It stores no
//   vtable of its own: the compiler-generated (virtual) destructor ??1CTCAIScratchpad@@UAE@XZ (neither PDB lists it: Ego_r inlines
//   implicit dtors). The catalog label (~CTCTargetingBase) is propagated and wrong.
// Callees: the def's virtual slot 1 Delete on its last release (CDefPointer<T> inline Release), ??1CCharString@@QAE@XZ 0x0099EAE0
//   (x4), ??1CTCBase@@UAE@XZ 0x00686830 (tail).
// Layout: Ego_r.pdb CTCAIScratchpad (0x84, no STL member: retail's, as the ctor's stores confirm). engine/CTCBase.h
//   (FABLE_RETAIL_CTCBASE), engine/CDefPointer.h (FABLE_RETAIL_DEF_CLASS_BASE), fable_string.h (CCharString).
// TU: retail's own ctor ??0CTCAIScratchpad@@QAE@AAVCThing@@@Z (0x007ABB80: CTCBase(thing), the four default CCharStrings, a null
//   PDef) is defined, which emits the vtable, the ??_G and this implicit destructor.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                                 // CTCBase : CBaseClassNonCopyable (vtable 0x0125BE0C)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"                             // CDefPointer<T> (inline Release, null ctor)
#include "fable_string.h"                                   // CCharString (0x0099E4B0 / 0x0099EAE0)

#pragma pack(push, 1)
class C3DVector { public: float X, Y, Z; };                 // retail C3DVector is 1-aligned
#pragma pack(pop)

enum EScriptingStateGroups;                                 // MS forward enum (Ego_r CTCAIScratchpad::ScriptingStateGroup)
class CAIScratchpadDef;

class CTCAIScratchpad : public CTCBase {                    // vtable 0x0126D07C
public:
    CTCAIScratchpad(CThing& thing);                         // ??0CTCAIScratchpad@@QAE@AAVCThing@@@Z (0x007ABB80)
    virtual ETCInterfaceType GetInterfaceType() const;
    virtual CCharString GetClassTypeName() const;
protected:
    virtual void OnSerialise(CPersistContext& context, bool loading);
private:
    virtual void OnCreate();
public:
    unsigned long         ThankingPhrase;                   // +0x0C
    unsigned long         IgnoringPhrase;                   // +0x10
    C3DVector             WanderCentrePoint;                // +0x14
    float                 WanderMinDistance;                // +0x20
    float                 WanderMaxDistance;                // +0x24
    long                  GossipCounter;                    // +0x28
    long                  MaxGossipPhrase;                  // +0x2C
    unsigned long         WarningPhrase;                    // +0x30
    unsigned long         BeerRequestPhrase;                // +0x34
    EScriptingStateGroups ScriptingStateGroup;              // +0x38
    float                 MaxHeroReactionDistance;          // +0x3C
    long                  ActionFrequency;                  // +0x40
    float                 ActionFrequencyVariation;         // +0x44
    CCharString           Action;                           // +0x48
    bool                  FaceHeroForAction;                // +0x4C
    CCharString           TargetName;                       // +0x50
    float                 FollowDistance;                   // +0x54
    bool                  AttackHeroOnSight;                // +0x58
    long                  TimeToSpendHarassingHero;         // +0x5C
    float                 CombatNearbyEnemyFleeingBreakOffRange;   // +0x60
    float                 CombatNearbyBreakOffRange;        // +0x64
    bool                  StealStealableItems;              // +0x68
    bool                  RecoverStealableItems;            // +0x69
    bool                  TakeStealableItemToRandomDestination;    // +0x6A
    bool                  KillSelfAndStealableItemAfterReachingDestination;   // +0x6B
    bool                  AllowedToFollow;                  // +0x6C
    CCharString           TableName;                        // +0x70
    CCharString           SeatName;                         // +0x74
    bool                  DisableHeadLooking;               // +0x78
    bool                  IsPushableByHero;                 // +0x79
    bool                  LookForFiniteTime;                // +0x7A
    bool                  AvoidRegionExits;                 // +0x7B
    float                 TargetingDistanceOffset;          // +0x7C
    CDefPointer<const CAIScratchpadDef> PDef;               // +0x80
};
FABLE_STATIC_ASSERT(sizeof(CTCAIScratchpad) == 0x84);

CTCAIScratchpad::CTCAIScratchpad(CThing& thing)
    : CTCBase(thing)
{
}