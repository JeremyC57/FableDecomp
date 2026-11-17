// dynamic atexit destructor for 'CCombatSequenceBase::IsValidPredicates' (0x013BAFA8) -- clear the protected static STLport map (recursive _M_erase from the root, header relinked, size zeroed) and free its header node
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) @ 0x0122A0A0, 84 bytes.
// agentNF69 (q10822 / q10851): the static map is CCombatSequenceBase::IsValidPredicates, a protected static
//   map<ECombatSequenceIsValidType, bool (__fastcall*)(const CThingCreatureBase&, const CThingCreatureBase&)> (Ego_d publics + LF_STMEMBER; Ego_r.pdb 0x01013490 / 0x0101D720 and FableWin.pdb 0x03FFA490 / 0x04011A00
//   `dynamic initializer for` / `dynamic atexit destructor for 'CCombatSequenceBase::IsValidPredicates'`; docs/journal/2026-11/EGOD_FINDINGS.md
//   section 1; Ego_d typedef CCombatSequenceBase::TIsValidPredicateContainer), defined here on stlport/fable_map.h, as the landed CTCAIScratchpad::Gossip pair
//   (0x0121F160 / 0x01229CA0) does: VC7.1 emits its dynamic initializer $E1 (0x0121F680: the inline map constructor, then atexit($E2)) and
//   the atexit destructor $E2 (0x0122A0A0: the inline ~map with the out-of-line _M_erase, then free of the header node) from this one definition.
//   CCombatSequenceBase has no shared header (engine/CCombatSequenceBase.h is quarantined; the landed rows 0x008B0B60 / 0x006F1770 model it
//   locally); this row declares only its three static maps, protected as in Ego_d.
//   Was the CRetailTree / SNode stand-in (_unattributed::Cleanup_LTextTreeWalker_2_0122a0a0, a propagated label).
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"

class CThingCreatureBase;
enum ECombatSequenceIsValidType;
enum ECombatSequenceOnStartModuleType;
enum ECombatSequenceOnStopModuleType;

class CCombatSequenceBase {                                 // no shared header (engine/CCombatSequenceBase.h is quarantined): static members only
public:
    typedef std::map<ECombatSequenceIsValidType, bool (__fastcall*)(CThingCreatureBase const&, CThingCreatureBase const&)> TIsValidPredicateContainer;
    typedef std::map<ECombatSequenceOnStartModuleType, void (__fastcall*)(CThingCreatureBase&, CThingCreatureBase&)> TOnStartModuleContainer;
    typedef std::map<ECombatSequenceOnStopModuleType, void (__fastcall*)(CThingCreatureBase&, CThingCreatureBase&)> TOnStopModuleContainer;
protected:
    static TIsValidPredicateContainer IsValidPredicates;    // 0x013BAFA8
    static TOnStartModuleContainer OnStartModules;          // 0x013BAFB4
    static TOnStopModuleContainer OnStopModules;            // 0x013BAFC0
};

CCombatSequenceBase::TIsValidPredicateContainer CCombatSequenceBase::IsValidPredicates;   // 0x013BAFA8
