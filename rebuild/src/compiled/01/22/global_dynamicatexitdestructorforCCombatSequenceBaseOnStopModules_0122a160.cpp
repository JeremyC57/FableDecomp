// dynamic atexit destructor for 'CCombatSequenceBase::OnStopModules' (0x013BAFC0) -- clear the protected static STLport map (recursive _M_erase from the root, header relinked, size zeroed) and free its header node
// Leaf: _$E2
// Retail: Fable.exe (TLC, VC7.1) @ 0x0122A160, 84 bytes.
// agentNF69 (q10824 / q10853): the static map is CCombatSequenceBase::OnStopModules, a protected static
//   map<ECombatSequenceOnStopModuleType, void (__fastcall*)(CThingCreatureBase&, CThingCreatureBase&)> (Ego_d publics + LF_STMEMBER; Ego_r.pdb 0x01013590 / 0x0101D780 and FableWin.pdb 0x03FFA4F0 / 0x04011A40
//   `dynamic initializer for` / `dynamic atexit destructor for 'CCombatSequenceBase::OnStopModules'`; docs/journal/2026-11/EGOD_FINDINGS.md
//   section 1; Ego_d typedef CCombatSequenceBase::TOnStopModuleContainer), defined here on stlport/fable_map.h, as the landed CTCAIScratchpad::Gossip pair
//   (0x0121F160 / 0x01229CA0) does: VC7.1 emits its dynamic initializer $E1 (0x0121F700: the inline map constructor, then atexit($E2)) and
//   the atexit destructor $E2 (0x0122A160: the inline ~map with the out-of-line _M_erase, then free of the header node) from this one definition.
//   CCombatSequenceBase has no shared header (engine/CCombatSequenceBase.h is quarantined; the landed rows 0x008B0B60 / 0x006F1770 model it
//   locally); this row declares only its three static maps, protected as in Ego_d.
//   Was the CRetailTree / SNode stand-in (_unattributed::Cleanup_LTextTreeWalker_4_0122a160, a propagated label).
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

CCombatSequenceBase::TOnStopModuleContainer CCombatSequenceBase::OnStopModules;   // 0x013BAFC0
