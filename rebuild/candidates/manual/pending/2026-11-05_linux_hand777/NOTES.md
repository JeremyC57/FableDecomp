# Chunk 777 notes (5 ledger rows, 1,045-1,272 bytes)

## Results
| address | name | status | grades | result |
|---|---|---|---|---|
| 0x0055F710 | CTCHeroOnlineScoreboard::ResetAndOpenHTTPAndPrepareRequest | parked | 19 | DIFFER(1121v1122) dl=46, register choice only |
| 0x005E8220 | CTCQuestCompletionUI::OnSerialise | matched | 6 | RELOCATION_MATCH, vland PASS |
| 0x006303C0 | NUISystem::CUIDef::CUIDef(const CUIDef&) | matched | 3 | RELOCATION_MATCH, vland PASS |
| 0x00803540 | CTCTargetingPlayer::UpdateTargetedPositionRandomWalk | parked | 30 | DIFFER(1108v1108) dl=15, scheduling only |
| 0x00AC92C0 | QuadricOptimiserInternals::COptimisedPrimitive::ComputeContraction | matched | 36 | RELOCATION_MATCH, vland PASS |

vland (read-only, no --recheck-landed): WINS 3. Warnings: STANDIN_LEAF on 005e8220 / 00ac92c0 (the catalog names were guesses,
see NAME_FIXES.tsv); 00ac92c0 RELOC_NAME for 0x00AD4A70 (labelled CTwoLongSets::~CTwoLongSets; it is ~CUniqueVertex) and
0x00AD3A10 (labelled _Rb_tree::_Rb_tree; it is the set<unsigned long> copy ctor); RELOC_CONFLICT: the same set copy spelling is bound
to 0x00513700 by the landed 0x0051C470 (two retail instances of one template spelling; one needs its own name).

Tests: test/*.test.cpp and differ/*.test.cpp pass under g++ -std=c++98 and compile under VC7.1 /Od /EHsc (vtest/).

## Header overlay (hdr/, every change behind a macro; hdrcheck2.txt: default=same everywhere)
- engine/NUISystem_CUIStateDef.h (NEW): retail CUIStateDef : CParentDefClassBase (0x7C bytes; ctors 0x00630350 / 0x006250B0, dtor
  0x00624DC0).
- engine/LHHttp2.h: FABLE_RETAIL_LHHTTP2_SCOREBOARD (LHHttpHeaders::Reset 0x0083FBC0 out of line + dtor; LHHttp2 ctor/dtor,
  SetProxySettings, SetConnectionDetails). hdrcheck2 FABLE_RETAIL_LHHTTP2=DIFF is an artifact (the header's relative
  `#include "LHNetworkTypes.h"` resolves to an empty stub from the overlay dir); cpp against base_include v mirror differ only in
  static-assert __LINE__ numbers.
- boost/scoped_ptr.h: FABLE_RETAIL_SCOPED_PTR_RESET (checked_delete, inline dtor, reset via swap).
- engine/CUserProfileManager.h: FABLE_RETAIL_CUSERPROFILEMANAGER_SCOREBOARD (scoreboard accessors, members +0x110..+0x120).
- stlport/fable_containers.h: FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE (noinline vector::resize).
- stlport/fable_map.h: FABLE_RETAIL_STLP_SET_COPY (out-of-line set copy ctor declaration, 0x00AD3A10).
- engine/CTCTargetingPlayer.h: FABLE_RETAIL_CTCTARGETINGPLAYER (CTCTargetingPlayer : CTCTargetingBase, typed PDB members +0x38..+0xF4,
  UpdateTargetedPositionRandomWalk declared).
- engine/CTCHeroRangedAttacks.h: FABLE_RETAIL_CTCHERORANGEDATTACKS (: CTCBase, TCI_HERO_RANGED_ATTACKS, GetPullBackAmount 0x00789A70,
  GetShotAccuracyFraction 0x00789A80).
- engine/CHeroCombatDef.h: FABLE_RETAIL_CHEROCOMBATDEF_RANGED (RangedAccuracyShakeyCursor +0x60, ...Weighting +0x68,
  RangedAccuracyShakeFractionOfRadius +0x74); NEW engine/CFloatRange.h and engine/CHeroAbilityStatWeight.h (GetParameter 0x00851BD0).
- engine/CThingCreatureBase.h: FABLE_RETAIL_CTHINGCREATUREBASE_HEROCOMBATDEF (GetPHeroCombatDef 0x00663340).
- Siblings: no landed row defines any of the new macros; one landed includer per changed header recompiled on mirror v base_include
  gives identical objdump (SIBLINGS.tsv).

## Header debt
- CTCQuestCompletionUI (0x005E8220): no shared header; modelled locally on CTCBase with the three maps at +0x84/+0x90/+0x9C.
- CTCHeroOnlineScoreboard (0x0055F710): no shared header (PHttp scoped_ptr +0x0C, TaskState +0x54).
- QuadricOptimiserInternals (0x00AC92C0): CQuadric, CIndexList, CUniqueVertex, CSubContraction, CVertexPairContraction,
  COptimisedVertex are local (as every landed row of the TU); engine/COptimisedPrimitive.h is the flat generated header, used under the
  `#define COptimisedPrimitive COptimisedPrimitiveFields` alias like the landed UpdateCost 0x00AC8A60. A retail QuadricOptimiserInternals
  header family would remove ~10 local models across the TU.
- CTCHeroExperience retail branch lays members from Ego_r (TrainableStatLevels +0x24 etc. disagree, see the landed 0x006D6EA0 note);
  only used as a type here.

## Callee names used (exact PDB spellings)
CQuadric::Clear 0x00AC6CE0, operator+= 0x00AC6E80, EvaluateError 0x00AC6D50; COptimisedPrimitive::ComputeSelfContraction 0x00AC7EB0,
ComputeOptimalVertexPlacement 0x00AC7750, ComputeSubContractionCost 0x00AC82A0, GetRemainingVertices 0x00AC8840;
CVertexPairContraction::ContainsAllVertices 0x00AC7A90; COptimisedVertex::GetGroupDifference 0x00AC7FF0; CUniqueVertex::~CUniqueVertex
0x00AD4A70; set<unsigned long>(const set&) 0x00AD3A10; CTCBase::GetWorld 0x00686A70; CWorld::GetWorldSeed 0x0049D810; GFFloatRandom
0x004978A0; C3DVector::AccurateNormalise 0x00A14440; CAIGameCameraBase::GetCameraManager(const CGameCameraManager**) const slot 10;
CTCHeroRangedAttacks::GetPullBackAmount 0x00789A70 / GetShotAccuracyFraction 0x00789A80; CThingCreatureBase::GetPHeroCombatDef
0x00663340; CHeroAbilityStatWeight::GetParameter 0x00851BD0; TWO_PI 0x0124F2B8; DELTA_NOTIONAL_ZERO 0x0129BA3C.

## Parked residues and next ideas
- 0x0055F710 (dl=46, 1121v1122): register choice only -- operand roles of `add eax,esi` / `lea edx,[esi+eax]` at the proxy / server /
  page dynamic string temporaries, literal-ctor lea registers, AddHeader argument scheduling, resize tag ecx v edx, key edx v eax, and
  the post-CryptoMatic `mov ebp,eax ; lea eax,[tag] ; add esi,edi` order. Tried: explicit page / user-name strings, direct CryptoMatic,
  request.data(), buffer.begin(), int / unsigned long lengths, tag spellings, fable_string_bodies.h. Next: same-TU noinline bodies for
  CryptoMatic and the string range constructor as StdEncryptString 0x0055F090 does, or the LHString-typed request the landed callers use.
- 0x00803540 (dl=15, same length): scheduling only -- the sine member store (`mov [esi+0CCh],edx`) lands before `fcos` (retail after)
  and `lea eax,[esi+9Ch] ; mov ecx,eax ; pop edi` three x87 instructions later than retail (a `C3DVector&` to TargetedPosOffset3D moves
  it too early, dl=14). Next: other splits of the two TargetedPosOffset3D statements / a C2DVector-returning helper for (sin, cos), and
  an instruction-by-instruction comparison of the x87 dependency chains in that block.

## GOTCHAS one-liners
- VC7.1 constructs implicit-conversion argument temporaries (const char* -> std::string) before the other arguments; an explicit
  `std::string(x)` is evaluated in the normal right-to-left pass (0x0055F710: port first).
- The first map walk's loop form decides VC7.1's per-site `_M_insert_overflow` inline expansion count for the whole function: `while`
  with the iterator initialised at its declaration gave retail's single expansion (0x005E8220).
- Same-TU noinline callee bodies decide register allocation and loop shape: visible CQuadric::Clear / operator+= keep edx across the
  calls and hoist the quadric loop count; a write-free ComputeSubContractionCost lets VC7.1 strength-reduce the cost loop and keep both
  flags in memory (0x00AC92C0).
- Local declaration order picks callee-saved registers: `vertex1, unique1, vertex2, unique2` matched where `vertex1, vertex2, unique1,
  unique2` swapped edi/ebp/ebx (0x00AC92C0).
- x87 compares: `!(a < b)` emits `test ah,5 ; jnp` (retail 0x00803540), `a >= b` `test ah,1 ; jne`; `!(a > b)` emits `test ah,41h ; je`
  where `a <= b` gives `jp` (0x00AC92C0).
- `C3DVector v(0.0f, 0.0f, 0.0f)` then setting X/Y (dead zero stores drop) gave retail's operand order in an inlined operator^; a
  default-constructed vector with only Z set loads the other operand first (0x00803540).
- A preceding `member = C2DVector(sin, cos)` temporary flips the X add of the next same-object `C2DVector +=` (Offset.X loaded first);
  named float sine / cosine locals give retail's right-operand-first order (0x00803540).
- hdrcheck2 reports FABLE_RETAIL_LHHTTP2=DIFF for any engine/LHHttp2.h overlay: its relative `#include "LHNetworkTypes.h"` resolves to an
  empty stub from the overlay dir.
