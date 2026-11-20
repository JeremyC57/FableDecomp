# 0x00859EF0 CCombatActionBase::OnAnimationLoaded (p25N, 2026-11-20)

**Grade:** RELOCATION_MATCH (1196/1196). No flag, no pragma, no inline device, no Inferred line.
- **Strict vland:** passes (`vland.log`): LANDVERIFY_STRICT_HEADERS=1, FABLE_HDR_OVERLAY=`hdr/`, fresh LANDVERIFY_WORK, behaviour PASS.
  The three warnings are RELOC_ALIAS (`_CIfmod` import thunk) and two RELOC_NAME for labels covered by `NAME_FIXES.tsv`
  (0x0085AB00 SetAttacker, 0x0085B0D0 GetAllThings / filter 0x0085AFD0).
- **Base:** p25G's v5b draft (DIFFER(1195v1196)), with p25E's opt-ins (ONANIMATIONLOADED, CTCFLOURISHTARGET_SETTERS, GETCOMPONENT_BODY,
  NCAMERAMODES setters / spline).

## The residue and its fix
The one-byte / register residue was the evaluation order of the `SetLastActionHandedness` argument.
- Retail reads `phand->EndHandedness` (0x00859FAC) and pushes it **before** the `PCreature->` object expression (GetPItem call 0x00859FB9).
- A cast of the plain member load, `(ECombatAnimationHandedness)phand->EndHandedness`, is evaluated after the object expression. It grades
  DIFFER(1195v1196), with the vtable / lea register names shifted from 0x0085A26B on.
- An inline getter call in the argument is evaluated first. The PDBs name it: Ego_d.pdb
  `?GetEndHandedness@CAnimComponentCombatHandedness@@QBE?AW4ECombatAnimationHandedness@@XZ` (and `?GetStartHandedness@..`). It is inline in
  retail (no out-of-line copy is called). Ego_d line 1530 (30 B at /Od) fits the single call statement.
- Final text: `PCreature->SetLastActionHandedness(phand->GetEndHandedness());`

## Header changes (`hdr/`, `hdr.diff`; strictly add-only, every assert pinned with FABLE_STATIC_ASSERT_AT)
| header | opt-in | source |
|---|---|---|
| CAnimComponentCombatHandedness.h | **new** `FABLE_RETAIL_CANIMCOMPONENTCOMBATHANDEDNESS_GETTERS`: `GetStartHandedness()` / `GetEndHandedness()` const, returning the enum from the +0x08 / +0x0C members | Ego_d.pdb procnames (both getters) + retail 0x00859FAC order; 6 asserts pinned |
| CAnimationEntry.h | GETCOMPONENT_BODY (p25E) | as p25E's batch |
| CCombatActionBase.h | ONANIMATIONLOADED (p25E) | as p25E's batch |
| CTCFlourishTarget.h | SETTERS (p25E) | as p25E's batch; CheckRetailLayout's two asserts pinned by hand (pin_asserts.py warned on the one-line body) |
| NCameraModes_CScriptModuleBase.h | SCRIPTMODULEBASE_SETTERS, SCRIPTMODULESPLINE (p25E) | as p25E's batch |

The only `-` lines in hdr.diff are FABLE_STATIC_ASSERT -> FABLE_STATIC_ASSERT_AT pins. CTCBoss.h is no longer in the overlay: the repo's
NF191 opt-in `FABLE_RETAIL_CTCBOSS_INCREMENTSTRIKESSINCEHITHERO` replaces the draft's CTCBOSS_INCREMENTSTRIKES.

**newly_flagged_rows:** none (`newflags.py hdr/` over rebuild/src/compiled: no landed row's shared-header warnings change).

## Ablation
| change | grade |
|---|---|
| final source | RELOCATION_MATCH |
| `PCreature.Get()->SetLastActionHandedness((ECombatAnimationHandedness)phand->EndHandedness)` (the draft) | DIFFER(1195v1196) |
| `PCreature->SetLastActionHandedness(*(ECombatAnimationHandedness*)&phand->EndHandedness)` | DIFFER(1195v1196) |
| named local `ECombatAnimationHandedness h = ...;` before the call | RELOCATION_MATCH, **rejected**: Ego_d's locals have no such local (this, ptc_hit_not, combat_anim, pcombat, panim, phand + block locals) |

Per-define ablation (`evidence/define_ablation.txt`, one define removed at a time):
- SRC_FAIL / hang without each of the 39 defines not listed below.
- DIFFER without CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE (1190v1196), CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY (1210v1196),
  CANIMATIONENTRY_GETCOMPONENT_BODY (1196v1196), C3DVECTOR_ANGLE_BODIES (1109v1196), CTHINGSEARCHTOOLS_ALLTHINGSBODY (1209v1196).
- Byte-neutral (RELOCATION_MATCH without it): CTHING_GETTC_EXPANDED, DEF_CLASS_BASE, CINTELLIGENTPOINTER_CTOR_OUTOFLINE,
  CCOUNTEDPOINTER_CALLED_CTOR, CENVIRONMENT, CCLASS, CCOMBATABILITY, CTHINGSEARCHTOOLS_ALLTHINGSBODY_EGOD, GFRANDOM_OUTOFLINE.
  Removing all nine together still grades RELOCATION_MATCH. They are kept because the payload test compiles the row text at /Od, where the
  inline versions they select instead pull in out-of-line callees (GFROR13, CBaseIntelligentPointer ctor / SetPItem) the test does not
  stub; each is an existing retail-backed opt-in.

## Text oracle
Ego_d (`egod_lines/`): locals and scopes this, ptc_hit_not, combat_anim, pcombat, panim, phand; { ptc_boss } { tc_combat } { direction }
{ ptc_flourish_target { pkofxdef { things, pcam_man { pspline, pcamera_point } } } }. The source follows them.
`SetUpCombatAnimationNotes` (0x00859EC0) is the retail TU-mate, verbatim under `// Fidelity:` except the one member respelling
(HasPerformAction, NAME_FIXES.tsv).

## Tests
- **`test/00859ef0.cpp`** (payload test, passes in vland): the row source verbatim, followed by the fixtures with Fx* types, and the filter's
  `operator()` defined in the TU as a stand-in. The row object cannot be linked from vland's path: its GetAllThings instance calls the
  anonymous-namespace `CThingFilter_IsValidCombatSpline::operator()`, whose `?A0x<hash>` depends on the compile path (0x006F2DE0 / 0x0055F710
  precedent). Generated by `tools/mkmodel85.py` from the real-object test.
- **`test/00859ef0_realobj.cpp`** links the REAL row object at mut.py's fixed path (`$W/tmp/mut/m.cpp`, hash `?A0xda03d0f7`) through
  `/alternatename` __fastcall stand-ins. 18 scenarios (strike / non-strike, live / dead target, boss / combat TCs, knockdown with and without
  the flourish TC, bullet time, empty / non-empty spline list, environment theme on / off).
- **Mutants:** 25/25 KILLED (`mutants/00859ef0.out`), including the getter swap (Start for End) and a mutant inside the new header opt-in.

## NAME_FIXES.tsv
0x0085AB00 `CTCFlourishTarget::SetAttacker` (landed as AssignTarget_0085ab00), 0x00859EC0 member HasPerformAction (landed local
m_keepActionTime), 0x0085AFD0 `CThingFilter_IsValidCombatSpline::operator()` (landed CUnprovenThingFilter_0085afd0). 0x007AED30 is dropped
from the earlier list: it is already landed as IncrementStrikesSinceHitHero.
