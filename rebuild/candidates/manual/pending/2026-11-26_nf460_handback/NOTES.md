# NF460 (2026-11-26): NAME_FIXES_QUEUE pass over nf452 / nf453 / nf455 / nf457 / nf458 (66 lines)

Scratch: `$S/w22/agentNF460`. Tools: NF457's (`tools/`, paths and lock owner moved to NF460) plus `triage.py` (table/self state per line),
`size.py` (census: name sets, self, retail call/jmp/abs sites -> owner row + lock), `genalloc.py` (TC factory, NF456 b4 recipe), `genpsc.py`
(particle factories), `editclone.py` (DatabaseClone callee respell), `mkb.py` (batch files from a spec), `objproof_map.py` (objproof with an
explicit rename map; objproof.py pairs renames by sorted zip, wrong for 00af7b40), `lk2.py` (direct /FORCE link of HEAD v new: duplicates and the
unresolved-set diff; nf.py link printed no UNRESOLVED lines for batch C, cause not found, so bC uses lk2.py). Census: work/size.txt (every queue target).
Sandbox: `sandbox/` (fresh copies of the 3 repo oracle TSVs taken at the end, descriptive_check.py copy). Repo oracle md5s: work/repo_oracles_md5*.txt;
the repo files changed twice under me (NF459 / coordinator landings at 01:11 and later), never by me; the final sandbox run is on the current repo copy.

## Batches (NF457 layout: rows/, orig/ = exact HEAD copies incl. tests, tests/ where changed, head/payload.json, RESULTS, SELF_SYMBOLS, UNITS,
## CHECKS, TABLE_CHANGES, HEADERS (empty), evidence/ = vland HEAD/new logs, objproof, link)
| batch | rows | queue lines | what |
|---|---:|---|---|
| bA_tc_alloc_units | 4 | 13776-13779 | factories 0x004D4927 / 0x004D3E43 / 0x004D4987 / 0x004D2D98 re-authored on engine/CTCBase.h onto their PDB ?Alloc selves (RegisterTCs binds them), calling the ctors 0x00702AA0 / 0x00770E50 / 0x00787DBC / 0x0072C6E0 as `(CThing&)` (self rows already define that). TABLE 4 replace. |
| bB_psc_alloc_units | 11 | 13762-13764, 13795-13798 | 7 particle factories onto ?Alloc@X@NParticleEngine@@SIPAVCPSCBase@2@XZ (Orbit on its shared header, which declares Alloc; the other six: local opaque model in NParticleEngine + reinterpret_cast to the incomplete CPSCBase, because the class headers declare no Alloc and CPSCBase is abstract) + the 4 DatabaseClone callers off their stand-ins (GetDrawableObject / CreateInternalPrimitive / CreatePrim) onto Alloc. TABLE 7 replace + 4 delete. Orbit re-run as tag bBo (first draft had doubled braces). |
| bC_single_binder | 5 | 13760, 13789, 13790, 13794, 13799 | base-ctor stand-ins -> RTTI bases (0x006F5610 StrafeForward; 0x008FDB55 / 0x008FDB7D CAIStateGroup_Clean), 0x00529CA0 -> `p->CThingPhysicalSwitch::~CThingPhysicalSwitch()`, 0x00AF7B40 -> NParticleEngine::CParticle dtor + class operator delete (self rekeyed PAU -> PAV NParticleEngine, r456a). Tests of 0x00529CA0 / 0x00AF7B40 updated (tag bCt). TABLE 1 delete + 4 replace. |
| held_r460a_creaturemode_alloc | 2 (held) | 13770, 13771, (13758) | both variants of 0x0073EC10 / 0x00743680; see RULINGS_NEEDED r460a. |

Totals: 20 rows landed-ready in 3 batches (bA 4, bB 11, bC 5); 16 queue lines DONE here (one, 0x008BFCE0, pending r460c) + 1 DONE elsewhere (0x0085BBA0, NF459 b7); 49 HELD.

## Gates (every row)
- vland --recheck-landed HEAD payload and new payload at one LANDVERIFY_WORK path (tools/runpair.sh): RELOCATION_MATCH PASS both sides for all 20.
- WARN sets: identical (bA, bB factories, 00529ca0), strictly smaller (bB DatabaseClone: RELOC_NAME gone; 008fdb55 / 008fdb7d / 00af7b40), class-swapped
  on 006f5610 (SHARED_HEADER_AVAILABLE C3DAnimFileHelpersChunk -> CCreatureAction_StrafeForward; RELOC_NAME gone): the r456b pattern, so that row is
  ACCEPT pending r460c.
- objproof on vland's own objects: ALL_EQ for all 20 under only the intended renames; .drectve compared as bytes (unchanged; the CTCCreatureModeManager.h
  variant of r460a is the one that failed here, so it is held).
- link HEAD v new (+ HEAD objects of the callee rows): 0 duplicates in every batch; unresolved strictly fewer (bA -4, bB -11, bC -5), none new.
- sandbox (fresh repo copy, all three batches cumulatively): apply_table --dry 0 misses then applied, multi-target 1 -> 1, SELF_SYMBOLS applied (12),
  clash check 0, descriptive_check 2,026 lines 0 FAIL. Repo oracle files unchanged by me (md5 after the sandbox run = md5 before it).
- Drift: every orig/ file (sources and tests) equals the repo working tree at the end (checked after the coordinator's HEAD-moved note).
- No __asm / volatile; no new /DEFAULTLIB; no new duplicate definitions; no new BEST_GUESS names (every name used is in Ego_r + FableWin, or FableWin + RTTI
  for CAIStateGroup_Clean).

## Duplicate / stand-in greps (r457e)
- No other source in rebuild/src/compiled defines ::Alloc for any of the 11 classes (grep `<Class>::Alloc(` and in-class `Alloc(...) {`): only the batch rows.
- The deleted stand-ins (GetDrawableObject(CPSCRenderSprite*), GetDrawableObject(CPSCEmitterGeneric*), CreateInternalPrimitive(CPSCAttractor*),
  CreatePrim(CPSCLight*), N_008bfce0, N_008fdb27, CThingPhysicalSwitch_dtor, CParticle::inner_dtor / member_free on CParticle) are spelt only by the
  batch rows (other GetDrawableObject / CreateInternalPrimitive / inner_dtor users pass other classes, so they are other symbols).
- bC head/payload.json: the two held rows were removed from it (vland_bC logs still show them; they belong to held_r460a).
- reinterpret_cast in 10 bB rows is a new construct: ruling r460d.

## Method / rules applied
- Self already correct: each wrong binder is an independent caller fix. A line is DONE only when the named wrong spelling has no binder left
  (source grep of the stand-in identifier over rebuild/src/compiled + the binder's HEAD object), e.g. N_008fdb27 has 2 binders, both done.
- Self must change: the unit is the self plus every retail caller row (xref) plus every source that models it; otherwise held.

## Identity checks
- 0x00740160 (nf455): verified CModeStunned's ctor (RTTI vftable 0x01267C74 stored, ret 4); 0x008ED950 binds it as CModeDazed. Fix unit held on r460a (see QUEUE_RESULTS).
- 0x00C20F40 / 0x00C207A0 (nf458): byte twins, both call 0x00C20720; one spelling bound at both -> binding conflict; needs a vendor-naming decision. Held.
- 0x00C69680 (nf453 x2): COORD_HOLD_NF453 lock, not touched. 0x004ADFF0: no retail reference; invented container self; held.
- SVoiceTap (0x00C705B0): not reached; NF458 says the deleted SVoiceTap spelling was live nowhere; its only basis is an older stand-in, so it is not
  evidence by itself (needs the vendor-naming rule for libpk: name by what the struct holds).

## Not reached (resume point, smallest first)
1. r460a / r460b rulings, then 0x0073E410, 0x00742E60, 0x00740160 (+0x007427F0, 0x008ED950).
2. Small self-correct families: 0x0099A2D0 (N_0099a2d0: 3 rows 00404720 00891610 00891d50), WatchFor* 0x00D17090 / 0x00E39140 / 0x00F08590 (one NScript
   binder each, `CScriptThingRuntime` -> `CScriptThing` by value), 0x00B93290 (binders 0x00B320A0 / 0x00B321B0), 0x006E89B0 (0x006EA770),
   0x0076C830 for 0x00686870, 0x00686830 (CBase_00686830: 8 rows), 0x004ABE90 (CScriptThing_Calls 3 rows, ScriptThingCopy 12 rows).
3. Self-change lines: 0x009FC210 (CResourceList_Methods, 6 rows), 0x0064CC65 (35 refs), 0x0097DA40 (28 refs), 0x009D6490 (13 refs).
4. Large: 0x0062F3B0 (CBase_0062f3b0 in 44 rows), 0x004A96F0 (CIntelligentPointerThing in 177 files), 0x00CB7E50 (self is ?Add@CGuiVarTransferStruct..., 554 refs),
   0x00859A20 (289), 0x008AC970 (325), 0x004C7E50 (73; CThing_Retail in 196 files), 0x0097E200 (112), 0x0099A2E0 (1,593), data 0x013B8790 (2 lines, 20 spellings),
   0x0143E8E0, 0x0143E8F8 (x2), 0x01231C4C, 0x00877720 (caller needs STLport list<const CThing*>).
5. MixFadeIn / MixFadeOut (nf452, 4 lines): needs the engine/CSalRenderDisable.h / CSalRenderPan.h private-static opt-ins + header equivalence; the
   retail callers (Mix bodies 0x00C6E8B0 / 0x00C6DB30 area) lie in no catalog row, so each self row is its whole unit.

## Locks (NF460)
Rows changed (20): bA 004d4927 004d3e43 004d4987 004d2d98; bB 00af7e60 00af7f40 00af8090 00af7d10 00af7ed0 00af7fb0 00af8020 00afe580 00b15b00 00b18740 00b1ae70;
bC 006f5610 008fdb55 008fdb7d 00529ca0 00af7b40. Unit members held unchanged (16): 00702aa0 00770e50 00787dbc 0072c6e0 00b0c9c0 00b16eb0 00b1b350 00afb7c0
00b103f0 00b18020 00b1a640 008bfce0 008fdb27 0083d6d0 00af7b60 00af8650. Released: 0073ec10 00743680 0073e410 00742e60.
No git run; nothing written under the repo.

## Last freshness pass (end of run)
HEAD c367be9bd3 (FullDecomp, read from .git/refs). Fresh copy of the repo oracles in `sandbox2/`: every TABLE_CHANGES --dry 0 misses, applied,
multi-target 1 -> 1; SELF_SYMBOLS 20 lines applied (incl. the unchanged caller selves, NF457 style); clash 0; descriptive_check 2,026 lines 0 FAIL;
repo oracle md5 unchanged across the run; orig/ drift 0 (all batches and the held folder).
