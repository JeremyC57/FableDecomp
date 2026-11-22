// std::for_each(CDefendingCombatantInfo::CProng* first, CDefendingCombatantInfo::CProng* last, CGetBestFreeProngForAttacker f) -- applies the
// 32-byte combat-manager functor (out-of-line operator() 0x006F2C00) to every 28-byte prong and returns it
// Leaf: ??$for_each@PAVCProng@CDefendingCombatantInfo@@VCGetBestFreeProngForAttacker@?A0x
// Retail: Fable.exe (TLC, VC7.1) @ 0x006F3530, 54 bytes, `ret 24h`. No direct caller and no absolute reference (callers.py / absrefs.py):
//   the witness CCombatRing::AssignAttackersToRing 0x006EBF70 expands its for_each in place, so this is the TU's out-of-line COMDAT copy.
// STLport 4.6 for_each (stlport/fable_algorithm.h FABLE_RETAIL_STLP_FOR_EACH_BODY), compiled /Gr in retail so it is `__fastcall`: the
//   functor is returned through the hidden pointer (ecx), `first` rides in edx, the rest on the stack; the functor (eight words: six
//   references, BestCompareVal, PBestProng) is copied out with `rep movsd`.
// NF238 (2026-11-22, q12669): moved off its row-local stand-ins (`struct SAttackerSlot { long m_words[7]; }`, `class CRotateThing` with
//   operator()(const SAttackerSlot&), a row-local std::for_each template) onto the shared engine/CGetBestFreeProngForAttacker.h
//   (FABLE_RETAIL_FREE_PRONG_PASS26, the anonymous-namespace functor of combat_manager.obj) and engine/CDefendingCombatantInfo.h (CProng,
//   0x1C asserted there). CRotateThing is a different PDB functor (Ego_r / FableWin / Ego_d: ??RCRotateThing@@QBEXAAV?$CIntelligentPointer@VCThing@@@@@Z,
//   applied to a list<CIntelligentPointer<CThing> >), so the old name was a stand-in, not a folded alias. Names: FableWin.pdb / Ego_d.pdb
//   ??RCGetBestFreeProngForAttacker@?A0x...@@QAEXAAVCProng@CDefendingCombatantInfo@@@Z and the for_each instantiation over
//   vector<CProng> with this functor (both PDBs; their Dinkumware _Vector_iterator is STLport's CProng* in retail); the size and record
//   stride agree (functor 0x20, prong 0x1C).
// Callee: (anonymous namespace)::CGetBestFreeProngForAttacker::operator()(CDefendingCombatantInfo::CProng&) 0x006F2C00.
#define FABLE_RETAIL_STLP_FOR_EACH_BODY
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_PRONG
#define FABLE_RETAIL_CDEFENDINGCOMBATANTINFO_PASS22_INIT
#define FABLE_RETAIL_FREE_PRONG_PASS26
#include "stlport/fable_map.h"
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CDefendingCombatantInfo.h"
#include "engine/CGetBestFreeProngForAttacker.h"
#include "stlport/fable_algorithm.h"

template CGetBestFreeProngForAttacker FABLE_FASTCALL std::for_each(CDefendingCombatantInfo::CProng*, CDefendingCombatantInfo::CProng*,
                                                                    CGetBestFreeProngForAttacker);
