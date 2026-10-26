// CAIStateGroup_SitRoundCampfire::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name) -- the CAIStateGroupBase initialisation with the same arguments, then FrameToNextSitOnSeat = 0
// Leaf: ?Initialise@CAIStateGroup_SitRoundCampfire@@UAEXAAVCAIBrain@@AAVCThingAICreature@@JW4EScriptAIPriority@@ABVCCharString@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00939270, 46 bytes (+ int3 padding to 48), thiscall, `ret 14h` (virtual, slot 1).
// Identity (proven): CAIStateGroup_SitRoundCampfire::Initialise (Ego_r.pdb holds the leaf); evidence: slot 1 of the CAIStateGroup_SitRoundCampfire vtable 0x0128B714 (RTTI: CAIStateGroupBase <- CBaseClass); the store hits Ego_r.pdb CAIStateGroup_SitRoundCampfire::FrameToNextSitOnSeat (+0x24)
// Label: the manifest's "CAIStateGroup_SitRoundCampfire::Initialise" (doubled class) names this row's own class; family J021: six AI state groups whose
//   Initialise forwards the five arguments and clears one member (retail has no identical-code folding).
// Callees: CAIStateGroupBase::Initialise 0x008FCEC0 (catalog-bound, Ego_r.pdb).
// Layout: CAIStateGroupBase is 0x14 bytes (Ego_r.pdb, no STL member, so retail offsets equal the PDB's); FrameToNextSitOnSeat +0x24
//   (Ego_r.pdb ulong). Local model: no engine header for these classes carries a virtual Initialise.
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
class CBaseClass {
public:
    virtual ~CBaseClass();                                   // 0x0099A2E0
};
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

class CAIStateGroup_SitRoundCampfire : public CAIStateGroupBase {                         // RTTI: CAIStateGroup_SitRoundCampfire <- CAIStateGroupBase <- CBaseClass
public:
    virtual void Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name);
    CIntelligentPointer<CThingObject>        PCampfire;   // +0x14
    long                                     SeatPosIndex;   // +0x1C
    bool                                     GotSeatPos;   // +0x20
    bool                                     Sitting;   // +0x21
    unsigned long                            FrameToNextSitOnSeat;   // +0x24
};

void CAIStateGroup_SitRoundCampfire::Initialise(CAIBrain& brain, CThingAICreature& creature, long priority, EScriptAIPriority scriptPriority, const CCharString& name)
{
    CAIStateGroupBase::Initialise(brain, creature, priority, scriptPriority, name);
    FrameToNextSitOnSeat = 0;
}