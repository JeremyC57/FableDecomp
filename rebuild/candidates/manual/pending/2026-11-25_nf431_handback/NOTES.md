# NF431 hand-back (2026-11-25): CGameScriptInterface script API, round 1

The family is 152 open tracker addresses: 121 `rtti_identified`, 3 `rtti_conflict`, and 28 split, stand-in or T1 rows. This round ported **18 rtti_identified virtual slots** onto the real class. They land as one ACCEPT batch, and one more row is authored and HELD. The other 133 are residue, each with a reason code (`RESIDUE.tsv`, `T3.txt`).

| batch | rows | form | grades (HEAD -> new) |
|---|---:|---|---|
| `batches/nf431_cgsi_a` | 18 | free / local-model stand-in -> `CGameScriptInterface::<Method>` on engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT) | 7 MATCH -> MATCH, 11 RELOCATION_MATCH -> RELOCATION_MATCH |
| `batches/nf431_hold` | 1 | 0x008901E0, same form; every gate passes but name_gate (callee label, see NAME_FIXES) | RELOCATION_MATCH -> RELOCATION_MATCH |

## The port
- The header already declares every one of the 918 slots, in the retail order with the exact PDB signatures. A row needs only:
  - `#define FABLE_RETAIL_NSCRIPT`;
  - `#include "engine/CGameScriptInterface.h"`;
  - the callee classes from their own shared headers;
  - the out-of-line `R CGameScriptInterface::M(...) [const]`.
- The self symbol is then the RTTI spelling by construction.
- No row emits `??_7` / `??_R`, so vtx427 has nothing to check (objcmp: 0 new vftable/RTTI symbols).
- Names, from two sources each:
  - RTTI vftable placement (`labels_rtti_port.tsv`, vtable 0x01260F0C), plus the Ego_r and FableWin name sets for the self;
  - FableWin's /Od body (`tools/fw.py`) for every callee and its argument order, cross-checked against the retail call or jmp target.
- Debug-only ASSERTs in FableWin (CDebugManager::DoErrorMessage blocks) are not in retail and are left out (0x008A7EC0).

## Shapes (histogram of the 18)
| shape | n | rows |
|---|---:|---|
| member read / write through World or a singleton (FableWin calls an inline accessor; retail expands it) | 6 | 0088f450 0088f950 0088e380 0088f460 00891c40 0088fd60 |
| tail jump / forward to a named member (incl. static) | 9 | 0088e070 0088e4f0 0088f960 0088fe80 0088f970 008901a0 00890670 0088ff10 0088fff0 |
| call with a constant argument | 2 | 008a7ec0 0088e090 |
| empty body (`ret 8`) | 1 | 0088f560 |

## Header opt-ins (add-only; byte-identical in both batches' hdr/; originals in orig/hdr_*.h)
| header | opt-in | why |
|---|---|---|
| CGameScriptInterface.h | FABLE_RETAIL_CGAMESCRIPTINTERFACE_IMPSETENTITYASALLY | private const ?ImpSetEntityAsAllyOfThing@...@@ABEXABVCScriptThing@@0_N@Z (both PDBs, retail 0x0089C100) |
| CEnvironment.h | FABLE_RETAIL_CENVIRONMENT_DEFAULTTHEME_CHANNEL | ?TransitionToDefaultTheme@CEnvironment@@QAEXJJM@Z (both PDBs, retail 0x006B3880) |
| CEnvironmentSpellEffectInterpolator.h | FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_SCRIPTFORWARDS | RadialBlurSetCenterWorldPos (unless the BULLETTIME_PASS23 copy is on) and DisplacementMonochromeEffectColourFadeOut (both PDBs) |
| CPlayerGui.h | FABLE_RETAIL_CPLAYERGUI_SETSHOWMONEYBAG | the inline FableWin.pdb ?SetShowMoneyBag@CPlayerGui@@QAEX_N@Z (ShowMoneyBag is private) |
| CThing.h | FABLE_RETAIL_CTHING_ISINLIMBO_DIRECT | `return InLimbo;` (byte `shr al,5 ; and al,1`, = FableWin's /Od ?IsInLimbo@CThing@@QBE_NXZ and retail 0x0088FD77); the default `InLimbo != 0` widens. Same form as FABLE_RETAIL_CTHING_ISDEAD_DIRECT |

- **hdrcheck:** every header gives the same preprocessed text under the default macros and under every macro already in the repo copy.
- **equiv_check** (copy against copy, `eq/old` = repo include and `eq/new` = it plus hdr/): `evidence/equiv.tsv` (empty) and `evidence/equiv.log`: **0 of 57,160 landed rows differ** (row and test preprocessed text).

## Tests (no class body; link the row object)
- Each test copies the row's prelude verbatim (its `#define`s and `#include`s).
- It calls the row by its own symbol with a qualified call, `p->CGameScriptInterface::M(...)`: no vftable, no alias.
- Callees are defined in the test as out-of-line members of the header classes. Each records `this`, its arguments and a call count.
- Static data (CPlayerGui PClass, CMainGameComponent::PMainGameComponent) comes from test arrays through `/alternatename`.
- Objects are raw arrays at the retail offsets, and the offsets are asserted against the header with `offsetof` where the member is public.
- Every test prints `PASS_<addr>` and the HEAD pass pattern (catalog PassPattern unchanged).
- The tests cannot link without the row object: the self symbol is only in it. vland links `sobj + tobj`.
- Mutation control: each row has a mutant in `mut/`, a changed argument / constant / dropped call.
  - All 18 give DIFFER and test FAIL, and every mutant exe was built, so each is a real behaviour fail, not a link fail.
  - The empty 0x0088F560 mutant writes CurrentPlayer, and its test checks that nothing is written.

## Gates (batch nf431_cgsi_a; all from the final run `fin`, overlay = hdr/ with all five opt-ins)
- **vland HEAD/new**, one work path (`work/wd_fin{h,n,m}`):
  - grade kept for 18/18, tests PASS for 18/18;
  - 0 LITERAL_DIFF;
  - leaf = the full mangled self (`// Leaf:`), strict;
  - new warn kinds: none (new has 0 WARN lines; HEAD had 16 SHARED_HEADER_AVAILABLE and 5 RELOC_NAME).
- **name_gate --test** (row + test): 18/18 NAME_GATE PASS.
  - There are 2 DEBT lines, on existing split data addresses whose spelling the row reuses:
    - 0x013B8790 CPlayerGui PClass;
    - 0x013B86A0 CMainGameComponent::PMainGameComponent.
- **Object section compare** (`tools/objcmp.py`, HEAD v new object):
  - 18/18 SELF_EQ: the self section's bytes are equal, and relocation offsets and types are equal.
  - Symbol pairs that differ are listed in CHECKS.tsv: only callee respells to the PDB names, at the same retail targets (RELOCATION_MATCH).
  - Extra pick-any COMDATs are only header inline out-of-line copies: CWorld::GetGameTimeManager, CThing::IsDead / IsInLimbo / IS_THING_ALIVE<CThing*>, CEnvironment::GetSpellEffectInterpolator, CPlayerGui::SetShowMoneyBag.
- **pk_all**, HEAD and new: 0 NEQ, 0 reached copies, 0 reach flips.
  - New copies are NOADDR and unreferenced (leaf=n, reach=n).
  - IS_THING_ALIVE<CThing*> is EQ at 0x0040DCE0.
  - Debt note (nf399 form): with ISINLIMBO_DIRECT, the unreferenced pick-any ?IsInLimbo@CThing@@QBE_NXZ from 0x0088FD60's TU has byte-form bytes, while copies from other TUs have the `!= 0` form. No retail out-of-line copy is placed, and none is referenced.
- **/FORCE link** (`final/linkcheck.txt`, response files):
  - duplicates (LNK2005 + LNK4006): HEAD 0, new 0. The control is a renamed copy of one object, and it fires 1.
  - Unresolved symbols, HEAD -> new: 6 stand-in spellings are gone and 6 PDB spellings come in at the same targets, plus `__fltused` (CRT, from the float arguments).
  - Census of other landed definers of the 18 new self symbols: none (self_symbols, `// Leaf:` lines and out-of-line bodies). Only unrelated self-contained tests define same-named stubs.
- **apply_table --dry:** 3 deletes and 2 adds, 0 misses, multi-target symbols 1 before and 1 after, nothing new.
  - Deleted lines are stand-in spellings that no other landed source names next to the target.
  - One line is kept, `evidence/TABLE_KEPT.tsv`: ?Method@CInner at 0x006B3950, which another landed source still names.
- **reserved_check** (RESERVED_REFS=""): 0.
- **T3 lines resolved:** the 10 nf324 `port` lines of 0088f560 0088f450 0088f950 0088e380 0088f460 0088fd60 00891c40 0088e090 0088ff10 0088fff0 (T3_RESOLVED.tsv), all ported by this batch; release them with `--release-t3 nf324` for those addresses only.

## Residue (133 + 1 held; RESIDUE.tsv / T3.txt, reason per row; locks released)
| shape | n | what it needs |
|---|---:|---|
| body_port | 80 | the same hand port. Each row's callees need their classes' headers, and often an add-only opt-in. Dossiers (HEAD row, retail disassembly, FableWin /Od body with named callees) are in `dos/` |
| t3_research / t1_caller_respell | 31 | the non-rtti split and stand-in rows. The 8 T1 rows fit gen423 caller respells |
| scratchpad_static | 7 | one data-symbol unit: the CTCAIScratchpad statics (NAME_FIXES) plus the g_* getter / setter rows that bind them |
| tc_query | 5 | IsThingABed / Chest / Door / Smashable / Searchable: IS_THING_ALIVE + CThing::GetTC<CTC*>. Retail keeps only the UsingComponents bit test, so these need a GetTC expansion opt-in |
| byvalue_return | 5 | CScriptThing returned by value: CreateEffect x2, CreateExperienceOrb, TryToRespawnDefNamed, GetRandomThingWithScriptName |
| container_signature | 4 | STLport list / map / set / CArray parameters |
| thing_virtual (0x00890250) | 1 | held by flatten_pilot's standing HELD set |
| tail_jump (0x008901E0) | 1 | nf431_hold; lands once the 0x006B8390 label is fixed |

## Tools (tools/)
- dos.py / brief.sh: per-row dossiers.
- fw.py / fwva.py: FableWin /Od body with named callees, and data VA to public.
- mkrow.py / mktest.py: the generators, from a spec, with the shared prelude.
- run431.sh / mkpay.py: vland h/n/m at one path.
- qall.sh, ng.sh, objcmp.py, pk.sh, linkcheck.sh, assemble.py.

## Extra checks (after the advisor review)
- In-class census (NF324 inclass_census.py, `evidence/inclass_census.txt`): 73 hits, all in other rows' self-contained TESTS (local
  CGameScriptInterface stub models); 0 in landed sources, so no pick-any copy of any new self in a landed object.
- Identity guards: no reloc_bindings line binds any of the 18 new self symbols to another target; no other address carries the same
  labels_rtti_port label. Newest COORDINATION_CLAIMS lines for 0088e090 / 008a7ec0 are Account A's (naming queue); none is Account B's.
- RESULTS `old` = the HEAD object's graded self; it equals self_symbols.tsv for 16 rows; 0088e070 / 00890670 have an empty symbol there
  (their HEAD selves are `_CGameScriptInterface_GetWorldFrame` / `@CGameScriptInterface_IsHeroInProjectileWeaponMode@0`).
- bytescan.py over the agent folder's batches / root files: 0 hits. `dos/` and `work/` hold retail disassembly, oracles and objects:
  scratch only, never copy them into the repo.
- Headers rechecked against the repo just before hand-back: all five orig/hdr_* still equal rebuild/include (not stale).

## Judgment calls for the coordinator
- Four rows (0088f450, 0088f950, 0088e380, 0088f460) write / read the public CWorld member directly where FableWin's /Od body calls an
  inline accessor (IsHeroSleepingEnabled is under a _PASS28 switch; the other three accessors are not in the header). Same bytes; adding the
  accessors as opt-ins is an option. 00891c40 uses the accessor (new opt-in) because the member is private.
- FABLE_RETAIL_CTHING_ISINLIMBO_DIRECT gives 0x0088FD60's TU an unreferenced pick-any ?IsInLimbo@CThing@@QBE_NXZ whose bytes differ from
  other TUs' `!= 0` copies (nf399 debt form; no retail out-of-line copy is placed or referenced).
- nf431_hold's vland run predates the CGameScriptInterface / CEnvironment opt-ins (both inert #ifdef blocks it does not define); re-run
  it when it lands.

## Resume note (next agent)
- Recipe per row: `tools/brief.sh <addr>` (HEAD row + retail + FableWin callees), write a block in a spec (`g1/spec.txt` form: D: macros,
  I: headers, E: evidence, M: mutation, body), `tools/mkrow.py`, `tools/qall.sh` (QV=1 shows the diff); test block in `tspec.txt`
  form, `tools/mktest.py`; then `tools/run431.sh <tag> <dir> <addrs>`, `tools/ng.sh`, `tools/objcmp.py`, `tools/pk.sh`,
  `tools/linkcheck.sh`, `tools/assemble.py`.
- CPlayerGui needs the macro set FABLE_RETAIL_CCHARSTRING CCLASS DEF_CLASS_BASE CINTELLIGENTPOINTER SCOPED_PTR
  NHEROINFORMATIONSCREENS_CMANAGER CENGINEPERSISTENTPRIMITIVEBASE CIVCOUNTEDPOINTER CENGINEPRIMITIVEHANDLE CPLAYERGUI CPLAYERGUI_MEMBERS;
  a wrong macro set HANGS Wine cl.exe (kill only your own cl PID) instead of erroring.
- Next shapes in order: the small body_port rows (EntitySet*Enabled 0088f7d0/f810/f850, GFDebugPDynamicCast<CThingPhysical> rows
  00890e80/ec0/f00/400/430, UpdateMiniGameInfoBar / DisplayMiniGameInfo via CPlayerGui::GetQuestInfo), then the scratchpad_static unit,
  then tc_query (needs a GetTC expansion opt-in), then the container / by-value rows.
