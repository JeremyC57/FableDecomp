// Leaf: ?GetName@CCombatSequence_TrollRockPursue@@UBE?AVCCharString@@XZ
// Class-model move (NF325, 2026-11-24): defined on the real class of engine/CCombatSequence_TrollRockPursue.h under FABLE_RETAIL_CCOMBATSEQUENCE_TROLLROCKPURSUE_VIRTUAL_MODEL (the opt-in polymorphic model;
//   was CCombatSequence_TrollRockPursue_Methods::GetName(CCharString* __ret) calling the stand-in CCharString::Init); the retail RTTI vftable placement names this PDB member here.
//   The by-value PDB return builds the result through CCharString::CCharString(const char*, long) (0x0099EBF0, both PDBs ??0CCharString@@QAE@PBDJ@Z, its landed self row).
// Retail: builds a CCharString from a string literal (len=-1) into the hidden
// return buffer (esi) and returns that buffer.
#define FABLE_RETAIL_CCOMBATSEQUENCE_TROLLROCKPURSUE_VIRTUAL_MODEL
#define FABLE_RETAIL_CCOMBATSEQUENCEBASE
#include "engine/CCombatSequence_TrollRockPursue.h"  // retyped onto the PDB layout; byte parity re-verified
class CCharString {
public:
    CCharString(const char* s, long len);
    void* m_data;
};

// String literal lives at 0x1278aec in retail.
static const char* const s_name = "COMBAT_SEQUENCE_TROLL_ROCK_PURSUE";

CCharString CCombatSequence_TrollRockPursue::GetName() const {
    return CCharString(s_name, -1);
}
