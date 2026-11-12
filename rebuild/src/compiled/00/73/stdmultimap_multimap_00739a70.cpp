// std::multimap<ECombatAbilityDataType, std::pair<CCountedPointer<CCombatAbilityData>, CCombatAbility> >::multimap() -- STLport 4.6 container default constructor: the member _Rb_tree built with _Compare() and allocator_type() (header node allocated, red, self-linked)
// Leaf: ??0?$multimap@W4ECombatAbilityDataType@@U?$pair@V?$CCountedPointer@VCCombatAbilityData@@@@VCCombatAbility@@@std@@U?$less@W4ECombatAbilityDataType@@@3@V?$allocator@U?$pair@$$CBW4ECombatAbilityDataType@@U?$pair@V?$CCountedPointer@VCCombatAbilityData@@@@VCCombatAbility@@@std@@@std@@@3@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00739A70, 54 bytes; node 0x24.
// Identity (proven, agentFAM07): CTCCombat's ability multimap (Ego_r.pdb CTCCombat +124 multimap<ECombatAbilityDataType, pair<CCountedPointer<CCombatAbilityData>,
//   CCombatAbility> >): the landed ~CTCCombat 0x00735340 and SetCombatType 0x007355F0 call this tree's _M_erase 0x00738EB0 (landed with
//   these types); ??0 / ??1 of this multimap are in the PDB name sets; the ctor is the COMDAT right before the multimap's destructor
//   0x00739AB0; node 0x24 = 0x10 + 4 + 8 + 8. The enum's enumerators are not needed here (sizeof only).
//   Value layout: Ego_r.pdb member names and types at the retail sizes (STLport containers: vector 12, map 12), only its size matters here.
//   Was the placeholder CUnprovenContainer_00738eb0::CUnprovenContainer_00738eb0 (name-only change: the same STLport text, the per-instantiation placeholder key/value
//   types replaced by the proven ones).
// Model (agentNF24 port of agentFAM07's draft): the shared stlport/fable_map.h (STLport 4.6 _Rb_tree; FABLE_RETAIL_STLP_MAP) and
//   engine/CCombatAbility.h + engine/CCountedPointer.h replace the draft's copied STLport text and local class model; the explicit instantiation emits the member as its own
//   COMDAT, as retail kept every used inline member (no /OPT:REF).
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCOMBATABILITY
class CCPPointerInfo;
class CCombatAbilityData;
enum ECombatAbilityDataType;
#include "stlport/fable_map.h"
#include "engine/CCountedPointer.h"
#include "engine/CCombatAbility.h"

template std::multimap<ECombatAbilityDataType, std::pair<CCountedPointer<CCombatAbilityData>, CCombatAbility> >::multimap();
