// Leaf: ?IsToEvadeAllHits@CCreatureAction_WhisperSommersaultForward@@QAEHXZ
#include "rebuild_abi.h"
struct CCreatureAction_WhisperSommersaultForward { char p[276]; unsigned char f; int IsToEvadeAllHits(); };
int CCreatureAction_WhisperSommersaultForward::IsToEvadeAllHits(){ return this->f==0; }
// Stand-in (owner rule 2026-11-05): the real method on a stand-in class named after the real class (retail ?IsToEvadeAllHits@CCreatureAction_WhisperSommersaultForward@@UBE_NXZ); evidence: RTTI vtable slot + PDB name set.
