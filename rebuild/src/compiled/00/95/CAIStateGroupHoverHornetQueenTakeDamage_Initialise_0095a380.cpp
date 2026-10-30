// CAIStateGroup_HoverHornetQueenTakeDamage::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name) -- the CAIStateGroup_HoverMovementBase initialisation with the same arguments, then FrameOfLastHit = 0
// Leaf: ?Initialise@CAIStateGroup_HoverHornetQueenTakeDamage@@UAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095A380, 46 bytes (+ int3 padding to 48), thiscall, `ret 14h` (virtual, slot 1).
// Identity (proven): CAIStateGroup_HoverHornetQueenTakeDamage::Initialise (Ego_r.pdb holds the leaf); evidence: slot 1 of the CAIStateGroup_HoverHornetQueenTakeDamage vtable 0x0128C7B4 (RTTI: CAIStateGroup_HoverMovementBase <- CAIStateGroup_EnemyCreatureBase <- CAIStateGroupBase <- CBaseClass); the store hits Ego_r.pdb CAIStateGroup_HoverHornetQueenTakeDamage::FrameOfLastHit (+0x24)
// Label: the manifest's "CAIStateGroup_HoverHornetQueenTakeDamage::Initialise" (doubled class) names this row's own class; family J021: six AI state groups whose
//   Initialise forwards the five arguments and clears one member (retail has no identical-code folding).
// Callees: CAIStateGroup_EnemyCreatureBase::Initialise 0x0091D820 (the landed CAIStateGroup_EnemyCreatureBase::Initialise; Ego_r.pdb UAE spelling (the catalog's QAE ...JJ... is a stand-in)).
// Layout: CAIStateGroupBase is 0x14 bytes (Ego_r.pdb, no STL member, so retail offsets equal the PDB's); FrameOfLastHit +0x24
//   (Ego_r.pdb long). Local model: no engine header for these classes carries a virtual Initialise.
class CAIBrain;
class CThingAICreature;
class CThing;
class CThingObject;
class CThingCreatureBase;
class CCharStringData;
class CCharString { public: CCharStringData* PStringData; };
enum EScriptAIPriority { EScriptAIPriority_Force32 = 0x7fffffff };
class CBaseObjectPointer;
class CBaseIntelligentPointer {                              // 8 bytes (Ego_r.pdb)
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObjectPointer* PData;                               // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CAIStateGroupBase : public CBaseClass {               // 0x14 bytes
public:
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name);   // 0x008FCEC0
    CAIBrain*         PBrain;                                // +0x04
    EScriptAIPriority ScriptAIPriority;                      // +0x08
    CCharString       Name;                                  // +0x0C
    bool              Terminated;                            // +0x10
    unsigned char     Priority;                              // +0x11
    bool              Enabled;                               // +0x12
};
class CAIStateGroup_EnemyCreatureBase : public CAIStateGroupBase {   // 0x1C bytes (Ego_r.pdb)
public:
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name);   // 0x0091D820
    CIntelligentPointer<CThingCreatureBase>  PTargetThing;   // +0x14
};
class CAIStateGroup_HoverMovementBase : public CAIStateGroup_EnemyCreatureBase { };   // no own members, no Initialise override
class CAIStateGroup_HoverHornetQueenTakeDamage : public CAIStateGroup_HoverMovementBase {                         // RTTI: CAIStateGroup_HoverHornetQueenTakeDamage <- CAIStateGroup_HoverMovementBase <- CAIStateGroup_EnemyCreatureBase <- CAIStateGroupBase <- CBaseClass
public:
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name);
    bool                                     LashOut;   // +0x1C
    bool                                     bIsRangeAttack;   // +0x1D
    long                                     RangeAttackCount;   // +0x20
    long                                     FrameOfLastHit;   // +0x24
};

void CAIStateGroup_HoverHornetQueenTakeDamage::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name)
{
    CAIStateGroup_HoverMovementBase::Initialise(brain, creature, priority, scriptPriority, name);
    FrameOfLastHit = 0;
}