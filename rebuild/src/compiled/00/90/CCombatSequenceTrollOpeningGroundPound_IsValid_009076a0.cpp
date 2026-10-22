#include "engine/CCombatSequence_TrollOpeningGroundPound.h"  // retyped onto the PDB layout; byte parity re-verified
struct CCombatSequence_TrollOpeningGroundPound; extern int __fastcall iv_helper(CCombatSequence_TrollOpeningGroundPound*);
bool __fastcall IsValid(CCombatSequence_TrollOpeningGroundPound* sequence){ return iv_helper(sequence) > sequence->f28 ? true : false; }