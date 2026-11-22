# NF253 notes (2026-11-22, round 19b): ??_GCIEngine, UnsheatheMeleeWeapon + 0x00890C80, the GetLocalSeed binder, the four ctor rows

Base: the repo tree with nf251 / nf250 landed. NF252 runs beside me (no overlap: its rows are the 0x0096DB60 unit). None of my rows is in
codex_pass28 RESERVED.tsv. Locks: work/locks.log. Tools: NF251's set re-pathed (tools/), plus gw.sh (grade any source), vtov.py (per slot: the
class's retail target, the base's retail target, the landed Leaf, the candidate override spelling and whether the PDB name sets carry it),
eq3.sh (whole-tree equiv_check on a hard-link tree copy). Read-first list: the brief chain NF253 -> NF215, NF214 and its read-first list
(NF92 BRIEF / LESSONS / NOTES, NF44 BRIEF, NF88-90 LESSONS), GOTCHAS.md in full, EXTRA_RULES.md, ASM_RULES.md, COORDINATION.md (head), the
nf251 / nf250 landing READMEs and hand-backs.
Result: 4 rows changed (0x00B23770, 0x00890C80, 0x006649C0, 0x008EE720), all RELOCATION_MATCH + PASS in one vland --recheck-landed run
(recheck_final.log), 0 RELOC_CONFLICT. One header changed: CThingCreatureBase.h, one add-only opt-in (hdr/README.md; whole-tree
equiv_check 0 of 57,125 rows differ). No new split; every touched address ends on the same or fewer spellings. Descriptive names added:
none (0x00B23770 reuses 0x00B23750's scaffolding class name, hashed per TU). Flags lines added: none (0x00890C80 needs /GS, which its
catalog CompilerFlags already has). Funclets: no EH funclet or fold row includes any changed file (grep of `#include "../src/compiled`).
Tests: all four keep their HEAD tests (standalone; PASS).

## Entry 1 (q12710): 0x00B23770 ??_GCIEngine -- done
- Row: rows/00b23770.cpp = the 0x00B23750 source (CIEngine.h CTOR / INLINE_DTOR / RENDER, the anonymous-namespace derived class with an
  out-of-line ctor) with `// Leaf: ??_GCIEngine@@UAEPAXI@Z`; the HEAD row had no Leaf line. RELOCATION_MATCH first form.
- Census: no E8 / E9 caller and no table line; the only reference is slot 0 of CIEngine's vtable 0x012A0D24 (absrefs.py), whose emitters
  (the CEngine pair rows, 0x00B23750 and now this row) bind ??_ECIEngine / ??_GCIEngine. Self spelling unchanged (the HEAD object defined
  the same ??_GCIEngine symbol from its local models).
- Symbol diff (evidence/sym_00b23770*.txt): gone ??0CIEngine@@QAE@XZ (the last QAE definer), ??0CInitBaseClass@@QAE@XZ (now UND, bound to
  0x0099A310), ??_7 / ??_GCInitBaseClass (local-model copies), ?MakeOne@@YAPAXXZ; new ??0CIEngine@@IAE@XZ (pick-any, pk EQ owner 0x00B23750),
  ??1CIEngine@@UAE@XZ (11 B, no owner; 0x00B23750's object has it too), the header's 85-slot ??_7CIEngine@@6B@ (relocation list equal to
  0x00B23750's, evidence/vt_ie_00b23770.txt; vtmap: 7 SAME, 78 NOLEAF, 0 DIFF) and the hashed scaffolding symbols. HEAD's own ??_7CIEngine@@6B@
  was a 2-slot stand-in with the slots swapped against retail (slot 0 ?v00@CInitBaseClass@@UAEXXZ, slot 1 ??_ECIEngine; retail slot 0 is
  0x00B23770, slot 1 0x0099A340 Validate).
- Link (evidence/lk1_ciengine_set.txt; set 0x00B23770 0x00B23750 0x0099A310 0x00B260B0 0x00B261D0 0x00B2AB10 0x00834CD0, one of the 255
  landed rows whose source defines a global `void* MakeOne()`): HEAD 3 (??0CInitBaseClass v 0x0099A310, ?MakeOne v 0x00834CD0, the nf251 fold's
  ??0CEngine); new 1 (the fold only; one object per TU: 0).
- vland prints a non-blocking STANDIN_LEAF for the catalog text `ScalarDeletingDestructor` (as nf250 ruling 6): RENAMES s1 offers the
  catalog name `scalar deleting destructor'` as the sibling 0x00B2AB10 spells it (decision 2).

## Entry 2 (q12711): the UnsheatheMeleeWeapon opt-in, 0x00890C80, the three UnsheatheWeapon rows -- done
- Evidence: ?UnsheatheMeleeWeapon@CThingCreatureBase@@QAEXXZ in Ego_r.pdb (1), FableWin.pdb (5), Ego_d.pdb (5) (raw grep; name sets Ego_r
  0x00540810 / FableWin 0x01D95690); Ego_r donor order puts UnsheatheWeapon / UnsheatheMeleeWeapon at 0x00540710 / 0x00540810 = retail
  0x006648D0 / 0x006649C0; the retail body (79 B: slot 77, 0x006A15A0, SheatheAllWeapons, UnsheatheWeapon twice with def +0xD8 / +0xDC).
- Header: hdr/engine/CThingCreatureBase.h, FABLE_RETAIL_CTHINGCREATUREBASE_UNSHEATHEMELEEWEAPON (hdr/README.md: pinned 0; equiv_check 0 of
  57,125 differ; 999 transitive includers, none defines the macro).
- 0x00890C80: the 0x00890D80 recipe (NF249) with the melee members (CTCCarrying.h ISTHINGCARRYINGMELEEWEAPON, UNSHEATHE_MELEE_WEAPON = 4,
  GAME_EVENT_CONTROLLED_CREATURE_UNSHEATHE_MELEE_WEAPON = 33); the header first, so the overlay copy wins the include. RELOCATION_MATCH
  first form (after one grade without /GS: tool slip, below). Self leaf ?EntityUnsheatheMeleeWeapon@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
  (both PDBs; vtable slot 508 = 0x012616FC in CGameScriptInterface's vtable 0x01260F0C, its only reference). The table's emitter of
  ??_7CGameScriptInterface@@6B@ (0x006E7740, the ctor) models the class locally with one virtual, so no landed row binds slot 508 (HEAD or new). Binders now PDB: 0x006A15A0, 0x006649C0, 0x006EBDD0, 0x009F1760
  (RENAMES s2, evidence/symdiff_00890c80.txt). Pick-any copies: ??0CGameEvent@@QAE@J@Z EQ owner 0x004159B0; the rest header inlines with no
  owner (evidence/pk_00890c80.tsv).
- 0x006649C0: the 0x00664A10 recipe (NF247) with the new opt-in, CThingCreatureDef PrimaryMeleeWeaponDef / SecondaryMeleeWeaponDef (+0xD8 /
  +0xDC, PDB +224 / +228) and slot 77 IsPlayerCharacter. RELOCATION_MATCH first form; 0x006A15A0 moved off the free stand-in.
- 0x006648D0 / 0x00664A50: not-a-binder. Each HEAD object's UND symbols all map onto their table spellings (UnsheatheWeapon IAE, the
  CTCCarrying / CThingObject / CDefinitionManager / CTCCombat members, UnsheatheMeleeWeapon QAE); their ports off the local models are
  local-model debt with no naming gain and were not attempted (context). Queue them as port debt.
- Census of 0x006649C0 (callers.py): 0x00664A50, 0x00666830, 0x008B7BA0 bind QAE CThingCreatureBase (in place, right); 0x00890C80 moved;
  left: 0x0096B600 (?UnsheatheMeleeWeapon@CThing@@QAEXXZ on a local CThing model: needs a port; NAME_FIXES) and the forwarder 0x00666820
  (`_ForwardTarget_006649c0`, no table line; NAME_FIXES). Table spellings 3 -> 2.
- 0x006A15A0: 6 -> 4 table spellings (two stand-in lines go; TABLE_CHANGES); the other stand-in binders are in NAME_FIXES.
- Link (evidence/lk2_e2e3_set.txt, with the owners 0x004159B0 / 0x004CDAE0 / 0x0040F220 and the siblings 0x00890D80 / 0x00664A10): HEAD 1,
  new 3, all LNK4006 between an EQ pick-any copy and an owner that defines the same function strongly: 0x00890C80's ??0CGameEvent (the same
  copy the landed sibling 0x00890D80 has at HEAD) and 0x008EE720's GFRandom v 0x004CDAE0 (decision 3; 84 landed sources include GFRandom.h without OUTOFLINE: a grep, emission not
  compiled).

## Entry 3 (q12712): 0x008EE720 onto the shared headers -- done
- CMaterialDef.h is already retail-laid-out (12-byte STLport vectors, BerserkedSwordHitParticleEffect +0x80, sizeof 0x98 asserted), so the
  row moved whole: CMaterialDef.h SWORDHITEFFECTS (declares the self member), CWorld.h GETLOCALSEED (NF250), CMainGameComponent.h
  FABLE_RETAIL_CMAINGAMECOMPONENT + _GETWORLD (FableWin inline GetWorld), GFRandom.h's inline GFRandom. No header edit.
  RELOCATION_MATCH first form. The stand-ins CEngineRandomOwner / CRandomGenerator and the local CMainGameComponent / vector / CMaterialDef
  models are gone (CEngineRandomOwner was not renamed to CWorld in place: nf95b).
- 0x0049D800: 5 callers (callers.py), 4 bound ?GetLocalSeed@CWorld@@QBEPAKXZ; this row now does too: 2 -> 1 spellings (its stand-in table
  line goes). Pick-any copies: GFRandom EQ owner 0x004CDAE0, ??A vector<long> const EQ owner 0x0040F220.
- The four sibling rows 0x008EE5A0 / 0x008EE600 / 0x008EE660 / 0x008EE6C0 still model CWorld / CMainGameComponent / CMaterialDef / vector
  locally (in place, right spellings); their comment "CMaterialDef.h carries the Ego_r offsets" is stale. Port debt.

## Entry 4 (q12713): L72 / L74 / L92 / L100 -- blocked, census done (handed back before it: context)
Per slot of each draft's 70-slot vtable (evidence/e4/vtov_*.txt, vtmap_*.txt; retail class vtables 0x0127ADDC / 0x01282354 / 0x01293E54 /
0x012948AC, base CCombatActionBase 0x01259E44, CCreatureActionBase 0x01259D24). Slot counts DIFFER: the drafts emit 70 slots, the four
  retail class vtables and CCombatActionBase's each run 81 contiguous .text pointers before the next non-.text word (measured in Fable.exe),
  so the drafts' header branch also lacks the last 11 CCombatActionBase-level virtuals. The landed emitter 0x00955620's define block gives
  full tables: its CCombatIdle vtable is 82 slots (NF250 work/vt_head_00955620_006f8010.txt, 82 lines: one slot more than the four classes, not checked why).
- The class's own overrides (retail target differs from CCombatActionBase's; spellings from the Ego_r + FableWin name sets):
  - CCreatureAction_SummonCreatures (0x008B83E0): Clone UBE (0x008B89A0), Initialise UAE (0x008B7A20, landed, PDB Leaf).
  - CCreatureAction_NymphSummonOutOf (0x008CA600): Clone UBE (0x008CAA70), FinishAction UAE (0x008C9E40).
  - CCreatureAction_DragonFireBeamInto (0x009535B0): Clone UBE (declared already under JACKDRAGON_HOVER_PASS27, 0x00953CF0), Initialise UAE
    (0x008F41D0, landed, PDB Leaf).
  - CCombatAction_WillHealInto (0x00956870): Clone EBE (private, 0x00956A20), FinishAction UAE (0x008C2AD0, landed, PDB Leaf), Initialise UAE
    (0x008C0560, landed, no Leaf line).
- Slot self rows that would split if the vtables landed today: the four Clone rows are a generated stand-in family (`CBase_0062f3b0* Clone()`
  non-virtual QAE on a local `CBase_0062f3b0` model: "one of 35 bodies"), and 0x008C9E40 is a free `__fastcall`
  ?CCreatureAction_NymphSummonOutOf_FinishAction@@YIX... stand-in. Each must move to its PDB spelling in the same hand-back as its ctor (the
  Clone rows onto the CCombatActionBase copy-ctor opt-in, the 0x00956960 ThrowShotObject recipe).
- CCombatActionBase-level slots (retail target = CCombatActionBase's own override) that the drafts' header branch (CCREATUREACTIONBASE +
  SOUNDCTOR + TARGETCTOR) binds to CCreatureActionBase spellings: 4 FinishAction (MAE), 51 OnBlowBlocked, 53 UpdateActionRunningTime,
  56 FrameUpdate (UAE), 62 GetFrameUpdateTime (UBE), 68 OnAnimationLoaded (MAE). These are not the classes' overrides; the fix is the
  CCombatActionBase branch the landed vtable emitters use (the 0x00955620 / 0x00951FB0 define blocks NF250 used for L50 / L95 / L96), not new
  class declarations.
- Slots 50 / 65 / 67 (0x00692670 / 0x00692880 / 0x006928B0, CCreatureActionBase) have free-function stand-in self rows and no table line;
  the landed emitter 0x00955620 already binds the member spellings there at HEAD (NF250 work/vt_head_00955620_006f8010.txt slots 50 / 65 / 67:
  ?IsPreventingLookTurning / ?OverrideFaceWithDefault / ?CanAnimationTransitionOut@CCreatureActionBase): HEAD debt, not created by these rows.
- Plan per class (one at a time): add-only opt-in in the class header declaring its overrides (above); the ctor on the landed emitter's
  define block; port its Clone row (and 0x008C9E40 for L74) onto the headers; vtmap every new ??_7 to 0 new DIFF against landed leaves;
  pk.py the ??_G / Clone copies; link with the emitter rows. L92 is the smallest (one declaration + the Clone row).

## Entry 5 (q12659): 0x00418DCA -- open (not started).

## Rule slips
- 0x00890C80 was compiled three times (g2 DIFFER without /GS, g3 RELOCATION_MATCH, the gh2 HEAD compile) before I claimed its lock (the directory did not exist; claimed
  right after; no other agent touched it).
- The HEAD compile of 0x00B23770 (gh1) ran without a clang precheck, and the lk1 link compiles were launched seconds before their precheck
  finished (all seven sources prechecked clean).
- The first 0x00890C80 grade ran without /GS: my g3.sh took flags only from a `// Flags:` line (fixed: catalog fallback).
- Five Bash calls exited non-zero (the first lock check, the 0x00890C80 precheck's trailing `kill -0`, two `cat` / `wc` calls on test paths
  looked up under src/, the first vtov loop).
- Control rows compiled for the links (0x00B23750, 0x0099A310, 0x00B260B0, 0x00B261D0, 0x00B2AB10, 0x00834CD0, 0x004159B0, 0x004CDAE0,
  0x0040F220, 0x00890D80, 0x00664A10) were locked NF253 first and are released at hand-back; 0x00A5C570 / 0x00B26340 / 0x00B25D80 were
  locked but never compiled (released). No git, no foreground sleep, every grade under `timeout`, no rm -rf (tools/clean.py), nothing written
  in the repo (work/root_before.txt v root_after.txt: identical except the .git directory's mtime, which I did not touch: no git command was run).

## Decisions for the coordinator
1. q12711: accept 0x006648D0 / 0x00664A50 as not-a-binder (all bindings already PDB) and queue their local-model ports as debt?
2. 0x00B23770's catalog name: rename `ScalarDeletingDestructor` -> `scalar deleting destructor'` (the sibling 0x00B2AB10's text; it silences
   vland's STANDIN_LEAF) or keep?
3. The two new LNK4006 between EQ pick-any copies and strongly defining owners (0x00890C80 ??0CGameEvent v 0x004159B0, as its landed sibling
   0x00890D80; 0x008EE720 GFRandom v 0x004CDAE0; 84 landed sources include GFRandom.h without OUTOFLINE: a grep, emission not compiled): the equal-copy ruling (nf250 ruling 3), with the
   owners' nf86 scaffolding queued?
4. Entry 4: run it as its own unit (one class per pass, L92 first) on the plan above, including the Clone stand-in family rows.
