# NF124 notes (round 10, PRIORITY slice 7 remainder: the 7 entries NF120 left open)

Early hand-back (context). Counts from APPLIED.tsv / STATUS.tsv / recheck_final.log:
- Entries: 1 done (q6326), 0 done-already, 0 rejected, 0 blocked, 0 deferred, 6 open (q6453 / q6455 / q4802 / q6454 partial; q6408 / q4747
  not started).
- Rows: 3 ported (0x0089C100, 0x008D6580, 0x008D8EA0); 5 not-a-binder (self rows 0x0076D270, 0x0073A680, 0x0075B8D0, which already
  define their claims; binders 0x0079ACC0 and 0x008EACB0, which already bind them); 1 blocked (0x00B20FD0). The other 25 rows.txt rows
  were not reached (STATUS `none`).
- Every claim re-verified in both PDB name sets (egor_pdb_names.tsv + fablewin_pdb_names.tsv) before acting: all 7 hold.

## q6326 (done)
- 0x0089C100 CGameScriptInterface::ImpSetEntityAsAllyOfThing moved off the CThing_Retail / CTCAllies_Retail / component-map stand-ins onto
  engine/CThing.h (GetTC, GETTC_EXPANDED) and engine/CTCEnemy.h (NF124 opt-ins ADDALLYTHING / REMOVEALLYTHING); kind 0x49 = TCI_ENEMY.
  It now binds ?AddAllyThing@CTCEnemy@@QAEXABVCThing@@@Z (0x0076D270) and ?RemoveAllyThing@CTCEnemy@@QAEXABVCThing@@@Z (0x0076C850).
- Split check (exe scan, refs.py): 0x0076D270 is referenced by 0x0079ACC0 + 0x0089C100, 0x0076C850 by 0x0089C100 + 0x008A7EE0. Both now bind
  one spelling each (the self rows' leaves). Both splits close.
- The row's own CGameScriptInterface model was kept in place, so its own leaf is unchanged (QBE). Both PDBs say private ABE: NAME_FIXES.tsv
  (a 2-row family with its caller 0x008A7EC0, which has no Leaf line).

## q6453 / q6455 (one address, 0x0073A680; open, 2 of 13 wrong binders ported)
- 0x008D6580 (CModeSitBase::OnActivate) and 0x008D8EA0 (CModeBlock::FrameUpdate): the local CModeBase moved to the shared nested
  CTCCreatureModeManager::CModeBase (FABLE_RETAIL_CMODEBASE_VIRTUALS). Collateral callees, each checked with pdiff + reloc_bindings + the
  callee's self Leaf (tools/coll.py). Every new spelling was already in the address's bound set, so no split is created and each one shrinks:
  - 0x0073AAC0 OnActivate UAE, already bound by 0x008EACB0;
  - 0x0073A6F0 GetNoFramesBeforePlayingCombinationIdleAnims IAE EMode, the self row's own leaf;
  - 0x0073AC50 FrameUpdate UAE, already bound by 0x008DF9C0.
- Both rows' own leaves become the PDB virtual overrides (QAE -> UAE; RENAMES section 1, leaf-only). An exe scan finds no landed row that
  references either address.
- 0x008D6580 needed the world seed fetched before the range (the landed statement order): 2 grades. It now uses the shared GFRandom.
- Left (10 not reached, 1 held):
  - 0x008E7B90 deferred: held by NF122.
  - 00862EC0, 008E9850, 008DC900 take 0x0073A680's result as CGlobalComponents* / CDisplayEngine*. Their downstream calls must be
    re-typed onto CWorld.
  - 008D6AC0, 008621A0, 008EA500, 008D7630, 008DCB60, 008DDD70 and 008E7CF0 each model 8-22 classes locally, among them CCharString,
    CThingCreatureBase, CWorld, CAnimateThing and CThing. Two of them also define a local CThing next to engine/_quarantine/CThing.h.
  - Moving CModeBase onto the header pulls in fable_string.h (local CCharString clashes). It also turns ParentThing into CThing&, so the
    local CThingCreatureBase (not derived from CThing) can only be reached by a stand-in cast. Port CCharString and CThingCreatureBase
    first in those rows.

## q6454 (0x0073AAC0; open)
- 0x008D6580 (above) now binds the claim ?OnActivate@CModeBase@CTCCreatureModeManager@@UAEXXZ. 13 of 14 wrong binders are left.
- The self rename QAE -> UAE waits for all of them (family). Self row untouched.

## q4802 (0x0075B8D0; open)
- The 9 CArray rows are binders, not an nf104 fold. Each hand-declares `CArray& operator=(const CArray&)` on a local CArray model, so
  the HEAD binding ??4?$CArray@VC3DVector@@@@QAEAAV0@ABV0@@Z is a row's own declaration, not a header-built function.
- 0x00B20FD0 (CSplineRandomOffsetData::GetPointAtPos) was ported onto engine/CCardinalSpline.h, using the landed 0x00B211B0 recipe. Its
  binding is right: ??4?$vector@VC3DVector@@...@std@@QAEAAV01@ABV01@@Z, the claim.
- 0x00B20FD0 is still blocked after 3 forms: DIFFER(177v177).
  - Best draft: work/00b20fd0_blocked.cpp, `ThisSpline.GetControlPointCount() != PreviousSpline.GetControlPointCount()`. The loads match;
    only `cmp eax,ebx` (retail) v `cmp ebx,eax` at +0x58 differs.
  - The Prev-first form and the named-count form also reorder the loads.
  - The exact 0x00B211B0 recipe form (`long prevCount = PreviousSpline.GetControlPointCount(); if (ThisSpline.GetControlPointCount() !=
    prevCount)`) was not tried (the third form was its mirror image): the first thing to try on resume.
  - HEAD's local `Size()` was `(long)(end-begin)`; the header's GetControlPointCount is `size()` through a long. A next try could be a
    c2 symbol-order lever (GOTCHAS, Account B 2026-11-20).
- Not reached: the other 8 CArray rows, plus 00764E90 / 00BCD850 / 00BCD8F0 / 00BCD950 (CElement12), 00762E10 (CPointVector) and
  00B20B60 (CKeyVector). The CSplineRandomOffsetData rows use ControlPoints directly (private in the header). They need the existing
  CCARDINALSPLINE_CONTROLPOINT / APPENDCONTROLPOINT opt-ins (GetControlPoint, DeleteAllControlPoints) or new accessors.

## q6408 / q4747 (open, not started)
- q6408: NF120's census stands (25 wrong binders).
- q4747: the self row 0x00733480 has no `// Leaf:` line and defines the stand-in CTCCombat_Retail / CDefPointer_MeleeCombatAbilityDef.
  - Its one binder 0x0064BE45 is a 196-line row with local CThing / CDefPointer / CTCCombat / bitset / IS_THING_ALIVE models.
  - It also declares IsShowingBaseAbility (0x00733290) and GetPBaseAbility (0x007334B0), which are in neither PDB, and a non-const
    IsMeleeFlourishValid (both PDBs say QBE).
  - The port needs engine/CTCCombat.h (FABLE_RETAIL_CTCCOMBAT + a GetPMeleeFlourishAbility opt-in), CDefPointer.h and ThingAlive.h.

## Descriptive names
None new.

## Flags lines
None added. All three ported rows grade with their build entries' CompilerFlags (none).

## Funclets
None: a grep of rebuild/src/compiled/01/21/*Unwind* for the three row file names finds no includer. The two leaf-only self renames keep their catalog names, so no
file move is needed for funclets.

## Tests
Unchanged: all three tests are standalone models that link nothing from the rows.

## Tools
- tools/coll.py: after pdiff.sh, checks that each new (target, symbol) pair is already bound in reloc_bindings.tsv or is the target's
  self Leaf.
- tools/dd.py: instruction-level diff of a q.py DIFFER log.
- tools/st.py now reads stdin only for the literal argument `@stdin`, no longer for `-`.

## Final checks
- recheck_final.log: one vland --recheck-landed run over the 3 rows/ rows: 3 RELOCATION_MATCH, every test PASS.
- selfcheck.log: 0 problems. Every FABLE_RETAIL_* macro used in rows/ resolves in rebuild/include or hdr/.
- WARNs reviewed: 7 SHARED_HEADER_AVAILABLE, on local models the landed rows already had and that were kept in place:
  - 0x0089C100: CGameScriptInterface. Its own leaf hangs on it (NAME_FIXES).
  - 0x008D8EA0: CThingCreatureBase, CTCLook, CAttackingCombatantInfo, CCombatantInfo, CCombatManager, CWorld.
- No HARD, RELOC_NAME or RELOC_CONFLICT line.

## Rule slips
- st.py (copied from NF122) read stdin for a `-` argument. One STATUS call with `-` hung and was stopped with TaskStop after ~2 min. No
  compile was running. The two earlier `-` fields were left empty and were corrected.
- 0x008D8EA0 and 0x00B20FD0 (not in rows.txt) were edited and graded before they were claimed: the lock census only printed rows held by
  others. Both locks were free; they were claimed at hand-back (nflocks owner NF124). 0x0089C100's lock was created at the start, but its
  owner file was written one call later.
- The first cpp precheck of 0x0089C100 put rebuild/include before the overlay, so it did not test the overlay. Corrected before the grade.
