// CCombatSequenceGeneric::OnStop() -- dispatch to the def's registered OnStop module, if any
// Leaf: ?OnStop@CCombatSequenceGeneric@@MAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B0870, length 63 bytes (+ int3 padding); protected virtual, vtable 0x01261D74 slot 18.
// Name: Ego_r.pdb + FableWin.pdb carry the Leaf symbol.
// agentNF69 (q10824): the module map 0x013BAFC0 is the protected static CCombatSequenceBase::OnStopModules (Ego_d publics + LF_STMEMBER; Ego_r.pdb /
//   FableWin.pdb `dynamic initializer for 'CCombatSequenceBase::OnStopModules'`; docs/journal/2026-11/EGOD_FINDINGS.md section 1), filled by
//   CCombatSequenceBase::StatRegisterClass 0x008B08B0. Was the CRetailTree_longkey stand-in over local CCombatSequenceBase_Retail /
//   CCombatSequenceGeneric_Retail models; now on engine/CCombatSequenceGeneric.h (FABLE_RETAIL_CCOMBATSEQUENCEGENERIC + the hdr/ overlay
//   opt-in FABLE_RETAIL_CCOMBATSEQUENCEGENERIC_MODULES).
// Body: Def->OnStopType (CCombatSequenceDef +0x50 retail; Ego_r 4 bytes higher over a 16-byte Dinkumware vector) is the key; a zero type means
//   "no module". STLport's map::find calls the out-of-line member-template _Rb_tree::find 0x008B24A0 (hidden iterator result, key by reference);
//   the node is compared with end() (the header) and the module (+0x14) with null before the `__fastcall` call (ecx = the creature,
//   edx = the target, FableWin.pdb's protected inline GetCreature / GetTarget accessors).
// Layout: CCombatSequenceBase and CCombatSequenceDef have no shared header (both are quarantined; the landed rows 0x008B0B60 / 0x006F1770
//   model them locally): CCombatSequenceBase with its Ego_r members at retail offsets, the STLport vector as opaque storage, its static
//   maps and their Ego_d typedefs; CCombatSequenceDef with the three module types only.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE_DECL
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"

class CThingCreatureBase;
enum ECombatSequenceIsValidType;
enum ECombatSequenceOnStartModuleType;
enum ECombatSequenceOnStopModuleType;

class CCombatSequenceDef : public CDefClassBase {           // no shared header (engine/CCombatSequenceDef.h is quarantined)
public:
    unsigned char _pad[0x48 - sizeof(CDefClassBase)];
    ECombatSequenceIsValidType       IsValidType;           // +0x48 (Ego_r +0x4C)
    ECombatSequenceOnStartModuleType OnStartType;           // +0x4C
    ECombatSequenceOnStopModuleType  OnStopType;            // +0x50
};

class CCombatSequenceBase {                                 // vtable 0x01278FEC; no shared header (engine/CCombatSequenceBase.h is quarantined)
public:
    typedef std::map<ECombatSequenceIsValidType, bool (__fastcall*)(CThingCreatureBase const&, CThingCreatureBase const&)> TIsValidPredicateContainer;
    typedef std::map<ECombatSequenceOnStartModuleType, void (__fastcall*)(CThingCreatureBase&, CThingCreatureBase&)> TOnStartModuleContainer;
    typedef std::map<ECombatSequenceOnStopModuleType, void (__fastcall*)(CThingCreatureBase&, CThingCreatureBase&)> TOnStopModuleContainer;
    virtual ~CCombatSequenceBase();                         // slot 0, 0x008B0A60
    virtual bool IsValid() const;                           // slot 12
protected:
    virtual void OnStart();                                 // slot 17
    virtual void OnStop();                                  // slot 18
    CThingCreatureBase& GetCreature() { return *Creature; }               // FableWin.pdb ?GetCreature@CCombatSequenceBase@@IAEAAVCThingCreatureBase@@XZ
    const CThingCreatureBase& GetCreature() const { return *Creature; }   // FableWin.pdb ?GetCreature@CCombatSequenceBase@@IBEABVCThingCreatureBase@@XZ
    CThingCreatureBase& GetTarget() { return *Target; }                   // FableWin.pdb ?GetTarget@CCombatSequenceBase@@IAEAAVCThingCreatureBase@@XZ
    const CThingCreatureBase& GetTarget() const { return *Target; }       // FableWin.pdb ?GetTarget@CCombatSequenceBase@@IBEABVCThingCreatureBase@@XZ
    static TIsValidPredicateContainer IsValidPredicates;    // 0x013BAFA8
    static TOnStartModuleContainer OnStartModules;          // 0x013BAFB4
    static TOnStopModuleContainer OnStopModules;            // 0x013BAFC0
private:
    bool                Running;                            // +0x04
    long                NoAvailableActions;                 // +0x08
    long                CurrentActionIndex;                 // +0x0C
    unsigned char       CombatActions_[0xc];                // std::vector<CCombatAction> CombatActions +0x10
    CThingCreatureBase* Creature;                           // +0x1C
    CThingCreatureBase* Target;                             // +0x20
    bool                CreateCalled;                       // +0x24
};

#define FABLE_RETAIL_CCOMBATSEQUENCEGENERIC
#define FABLE_RETAIL_CCOMBATSEQUENCEGENERIC_MODULES
#include "engine/CCombatSequenceGeneric.h"                  // CCombatSequenceGeneric : CCombatSequenceBase, Def +0x28 (hdr/ overlay opt-in)

void CCombatSequenceGeneric::OnStop()
{
    if (Def->OnStopType != 0) {
        TOnStopModuleContainer::iterator it = OnStopModules.find(Def->OnStopType);
        if (it != OnStopModules.end() && (*it).second != 0)
            (*it).second(GetCreature(), GetTarget());
    }
}
