# NF341 notes (Account A naming queue, 2026-11-24)

## Item 1: GetDef<CMaterialDef> 0x0066C340 pick-any copy
- Owner row 0x0066C340 is landed (local-model row, RELOCATION_MATCH at HEAD on the lane, 108 B). Retail 0x0066C340 and 0x0063F3A0
  (GetDef<CEngineThemeDef>) are instruction-identical 108-byte bodies.
- HEAD 0x008F7950 (RELOCATION_MATCH) emits a 116-byte GetDef<CMaterialDef> copy (bytes differ). Cause: the GETDEF_GLOBAL_BODY template
  calls out->SetDynamicCast(def), whose DEFAULT CDefPointer.h form is Assign(other.InternalGetObject()) (pointer cached in a register,
  `mov esi,eax`); retail re-reads the source handle after the release (the FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD form,
  "proven on 0x00410890"), and the cached form also leaves a second null test before the temporary's release.
- Probes (work/probe, toy.py): 0x008F7950's preamble + one caller: 116 B NEQ; same + SETDYNAMICCAST_REREAD: 108 B bytes_equal True.
  B's 0x00839940 preamble (overlay hdr/00839940, RELEASE_SELFDELETING_POINTEE + IVCOUNTED ORIGINAL_SCOPES): 116 B NEQ; + REREAD: EQ.
  So the release variant does not matter; only the SetDynamicCast form does.
- Landed rows that emit GetDef<CMaterialDef> (source mentions + GLOBAL_BODY): 0x0066B370 (has REREAD), 0x00703150, 0x007F0300,
  0x0082D5C0, 0x008F7950. 161 landed src rows name GLOBAL_BODY (159 define it; see Counts).
- Fix (hdr/engine/CDefinitionManager.h, 37 lines inserted after line 234, before the GLOBAL_BODY block's #endif; final lines 235-271):
  a comment (Fidelity: scaffolding, not retail), then inside `#ifndef FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD`: `class CMaterialDef;` +
  an `inline` __declspec(noinline) explicit specialization GetDef<CMaterialDef> = the template body with the REREAD SetDynamicCast body
  written out on `out`. No existing line edited; pin_asserts: 0 to pin; hdrcheck2: all macros =same (vacuous for this nested block:
  hdrcheck2 defines one macro at a time and the block needs FABLE_RETAIL_CDEFINITIONMANAGER too).
- Counts: 161 landed rows name FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY (text grep); 159 define it (0x00607330 / 0x0078B070 only
  mention it in a comment). Stay-off screen: 13 holes (not compiled) + 148 clear = the compiled set work/nset.tsv (incl. the 2
  comment-only rows, 0x00859EC0 and the 5 CMaterialDef emitters), plus B's 0x00839940 read-only = 149 objects HEAD and new.
- Exact-text probes (work/probe a2 / b2 / c2): 108 B bytes_equal True in all three configs (0x008F7950's; B's 0x00839940 with its overlay;
  0x00703150's SELFDELETING_POINTEE + RELEASE_DELETES). Negative controls a0 / b0 / c0 (HEAD text): 116 B NEQ.
- D1 (decision, see hand-back): this is a default-path correction of the GETDEF_GLOBAL_BODY block (no new macro), not an add-only opt-in:
  B's staged payload must compile unchanged and pick it up, which a new macro cannot do (prefix allows "a correction to the
  GETDEF_GLOBAL_BODY block if every includer stays byte-identical"). The standing "add-only opt-ins" rule is therefore not met literally.
- HEAD objects (one-path compile, tree work/trees/cur = HEAD include): emitters of the GetDef<CMaterialDef> COMDAT: 0x0066B370 108 B
  bytes_equal True (it defines SETDYNAMICCAST_REREAD), 0x00703150 / 0x007F0300 / 0x0082D5C0 / 0x008F7950 116 B NEQ; all five rows
  RELOCATION_MATCH. B's 0x00839940 HEAD (staged payload + hdr/00839940): RELOCATION_MATCH; leafcopies.py: 13 copies, 12 EQ + 1 NEQ
  (GetDef<CMaterialDef>) = B's own count.

### Item 1 results (guarded header = hdr/engine/CDefinitionManager.h = batches/b1/hdr)
- Guard: the first (unguarded) text dropped the unreferenced SetDynamicCast COMDAT from 0x0066B370, a SETDYNAMICCAST_REREAD TU whose copy
  was already EQ (evidence/proof1_n.txt, NOT_EQ there). The specialization is now inside `#ifndef FABLE_RETAIL_CDEFPOINTER_SETDYNAMICCAST_REREAD`,
  so REREAD TUs see exactly HEAD's tokens. Recompiled with the guard: 0x0066B370 + the 16 REREAD rows of the compiled set (work/reread.tsv)
  are ALL_EQ against HEAD (evidence/proof1_g.txt, proof1_rr.txt); all RELOCATION_MATCH.
- Non-REREAD rows: the guard adds only `#ifndef` / `#endif` around the specialization, so their preprocessed tokens are the unguarded
  run's: evidence/equiv_unguarded_v_guarded.tsv (159-row changed set) differs on exactly the 18 REREAD definers (16 recompiled +
  0x0066B370 + hole 0x0040C790) and 0x00627BE0's test; their N objects from the unguarded pass are kept.
- Compiled set (one path, HEAD and new): 148 landed rows (work/nset.tsv = the 148 clear rows that name GLOBAL_BODY, incl. 0x00859EC0 and the 5 CMaterialDef emitters) and
  B's staged 0x00839940 (read-only, its payload + hdr/00839940 overlay). Grades identical HEAD v new for all 149 (RELOCATION_MATCH / MATCH).
- objsec (proof1, keyed by COMDAT symbol): 143 ALL_EQ; 0x00703150, 0x007F0300, 0x0082D5C0, 0x008F7950 and B's 0x00839940
  FIXED_COPY_PLUS_DROPPED_HELPERS: the GetDef<CMaterialDef> COMDAT becomes 108 B equal to retail 0x0066C340 and to the owner row's object
  (same relocation symbols); the two inline helper COMDATs SetDynamicCast<CDefClassBase const> / Assign of CDefPointer<CMaterialDef const>
  disappear (nothing referenced them at HEAD: tools/refs.py); every other section is equal.
- B's 0x00839940 (guarded, read-only): RELOCATION_MATCH; leafcopies 13 copies, 0 NEQ (evidence/leafcopies_N_00839940.txt; HEAD 12 EQ + 1 NEQ).
- Coordinator gate (pk_all.py --tsv and pickany_vs_retail.py, HEAD and new, the 148 compiled clear rows + B's; the 13 holes NOT run, D4): evidence/pk/{H,N}_<addr>.{pkall.tsv,pvr.txt},
  comparison evidence/pk_gate.txt (tools/pkcmp.py): 0 turned NEQ, 0 newly NEQ, 0 dropped copies with an address; 5 FIXED
  (0x0066C340 in 0x00703150, 0x007F0300, 0x0082D5C0, 0x008F7950, 0x00839940); the 0x0066C340 copy is EQ in all 6 emitters (incl. 0x0066B370);
  286 row/checker pairs SAME. GATE PASS in both checkers. Planted control (sides swapped, evidence/pk_gate_control_reversed.txt): 5 TURNED_NEQ per
  checker, GATE FAIL.
- Link (/FORCE, evidence/link_i1.txt): owner 0x0066C340 object + the 6 emitter objects, HEAD set and new set: 0 duplicates in both; the
  same 5 unresolved externals (outside this set) in both.
- vland --recheck-landed (batches/b1/logs, HEAD no overlay / new FABLE_HDR_OVERLAY=work/hdr_i1): the 5 emitters + the 4 compiled rows
  whose tests change tokens (0x00627BE0, 0x0064E978, 0x006C14D0, 0x00774B40): all RELOCATION_MATCH PASS in both; 0x0064E978's
  RELOC_CONFLICT warning (C3DAnimationBlendState ctor) is on HEAD too and has nothing to do with this.

## Item 2: 0x006EA430 = CCombatManager::SetAttackerAsBeenStruck
- Sources: (1) Ego_r.pdb release\combat_manager.obj record 80 PerformSequenceReset (458 B), record 81 SetAttackerAsBeenStruck (32 B);
  DeregisterCombatant is record 112 (after ~CCombatManager). Retail 0x006EA2C0 PerformSequenceReset (355 B) is followed by 0x006EA430
  (33 B). (2) FableWin.pdb debug\combat_manager.obj 212 PerformSequenceReset, 213 PerformCurrentActionReset, 214 SetAttackerAsBeenStruck;
  donor FableWin.exe body of SetAttackerAsBeenStruck calls IsInMeleeAsAttacker then PerformSequenceReset (= retail 0x006EA430, which calls
  0x00662410 then 0x006EA2C0); donor DeregisterCombatant calls IsInMeleeAsAttacker, IsAttacker, then DeregisterAttackingCombatant
  (= retail 0x006ED9C0 -> 0x006ED510). Both PDB name sets spell ?SetAttackerAsBeenStruck@CCombatManager@@QAEXAAVCThingCreatureBase@@@Z.
- Name census: SetAttackerAsBeenStruck appears in no landed source, test, header, table line or catalog entry (no conflict).
- Binders (exe scan, tools/xscan.py): 0x006EA430 is called only by landed 0x00734050 CTCCombat::SetBlocked (0x007340A7) and B's staged
  0x00839940 (0x0083A37B). No reloc_bindings line exists for 0x006EA430; the DeregisterCombatant line is 0x006ED9C0 / witness 0x007355F0
  (HEAD object binds it there: xr).
- Changes: rows/006ea430.cpp (self + Leaf), rows/00734050.cpp (local model declaration + call + comments), hdr/engine/CCombatManager.h
  add-only opt-in FABLE_RETAIL_CCOMBATMANAGER_SETATTACKERASBEENSTRUCK (declaration only; pin_asserts 0). Tests unchanged (0x006EA430's is
  a standalone stub test, NF284 D2; 0x00734050's uses a stub stand-in).
- Proof: grades RELOCATION_MATCH before and after; objsec ALL_EQ under the one rename (evidence/proof_i2.txt); link (evidence/link_i2.txt):
  HEAD set has the duplicate strong DeregisterCombatant (0x006EA430 v 0x006ED9C0), new set none and nothing unresolved.

## Process notes
- Stay-off screen of the 161 landed GLOBAL_BODY definers (tools/screen.py, work/screen161.txt): 13 not clear. Compiled read-only anyway: none.
  Not compiled (holes, for the landing recheck): T3 column 2 0x00584B86, 0x005D9AA0, 0x006628E0, 0x006AC200, 0x007EAC10, 0x007FC210,
  0x00801920, 0x0082D2D0; T3 column 3 0x0040C790; B's open author claims (2026-11-20 p25C) 0x007BD2D1, 0x00859EF0; B claims marked
  done 0x00591B7F, 0x00774D20 (not compiled either). 13 holes in all. None names CMaterialDef in its source, and 0x0040C790 defines
  SETDYNAMICCAST_REREAD (so the guarded specialization is skipped there). They still need the landing recheck (D4).
- "161 GLOBAL_BODY rows" is a text grep: 0x00607330 and 0x0078B070 only name the macro in a comment (no #define), which is why
  equiv_check finds them token-identical. The real definers are 159 src rows (+ the tests listed in the equiv files).
- Prechecks: work/pre.log (clang; 0x00859EC0 fails clang only on a shared-header FABLE_STATIC_ASSERT typedef redefinition in
  CTCFlourishTarget.h, so its D1 `cl /Zs` pass is in work/pre_zs.log).
- One-path compiles: tools/pass.py keeps each row at work/one/<addr>/<addr>.cpp in both passes; the tree is FABLE_HDR_OVERLAY=work/trees/cur
  (a full copy of rebuild/include) whose engine/CDefinitionManager.h is swapped from orig/ to hdr/ between the HEAD and new passes.
- equiv_check runs are preprocessing only (no compile), launched with `timeout 7000` on their launch line (a full 57k-row run takes ~12 min,
  more than 900 s); every compile / grade / link has its own `timeout 900`.

## Header checks (final hdr/, both headers)
- pin_asserts: 0 to pin for engine/CDefinitionManager.h and engine/CCombatManager.h.
- hdrcheck2 (evidence/hdrcheck2_final.txt): CDefinitionManager.h all 36 macros + default =same (vacuous for the nested GLOBAL_BODY block,
  see LESSONS); CCombatManager.h every macro =same except the new FABLE_RETAIL_CCOMBATMANAGER_SETATTACKERASBEENSTRUCK (=DIFF, the
  intended declaration; no landed row defines it). Planted control (work/hc_plant: a typedef after CDefinitionManager's size assert and a
  member outside the opt-in in CCombatManager; evidence/hdrcheck2_plant.txt): FABLE_RETAIL_CDEFINITIONMANAGER=DIFF and default + every
  CCOMBATMANAGER macro =DIFF, so the check sees both headers.
- equiv_check: evidence/equiv_summary.txt (tools/eqsum.py). There is one complete all-rows run (the unguarded text, 57,150 rows: 159 src + 6
  tests). Final texts run on the rows that can change: HEAD v final = 141 src + 5 tests (the unguarded set minus the 18 REREAD definers;
  every row compiled or a hole, every test rechecked or a hole test). The CCombatManager opt-in shows 0 differences on the 210 rows that name
  CCombatManager; no file defines the macro. The planted controls flag 141 + 5 and 38. The final-text all-rows runs were stopped on the
  loaded box: see the summary for why the restricted runs cover every row.

## Decisions needed
- D1: Item 1 is a default-path correction inside the GETDEF_GLOBAL_BODY block (no new macro), because B's staged payload must pick it up
  unchanged. It drops two unreferenced inline-helper COMDATs (SetDynamicCast<CDefClassBase const> / Assign of CDefPointer<CMaterialDef const>)
  from the 5 emitter objects; NF318 Q5 says a dropped COMDAT is normally a hold. Accept (nothing referenced them; grades, links, pk gate
  unchanged) or rule otherwise.
- D2: B's 0x00839940 still calls 0x006EA430 as DeregisterCombatant (0x0083A37B), which after Item 2 is a RELOC_CONFLICT in B's row. B (or
  the coordinator at landing) needs `#define FABLE_RETAIL_CCOMBATMANAGER_SETATTACKERASBEENSTRUCK` plus one token at its line 333
  (`SetAttackerAsBeenStruck(creature)`): work copy work/bcopy/00839940_respell.cpp compiles RELOCATION_MATCH, objsec ALL_EQ against the
  staged row under the one rename, xr binds 0x0083A37B to the new spelling, 13/13 copies EQ (evidence/bwork_*).
- D3: the HEAD census finds 121 NEQ GetDef<T> pick-any copies in 110 objects (26 owner addresses, e.g. GetDef<CEngineThemeDef> 0x0063F3A0
  in 0x008F7950), all from the same cached SetDynamicCast form. A generic re-reading GetDef template body (instead of per-T
  specializations) would fix them all; that is a bigger change than this item and needs its own ruling and recheck.
- D4: 13 GLOBAL_BODY definers were not compiled (stay-off: T3 column 2/3 and B claims); none names CMaterialDef; they need the landing recheck.
- D5: NAME_FIXES_QUEUE.tsv line 8829 (land1011cr) gives 0x006EA430 as DeregisterCombatant; it is wrong (close it); the catalog entry for
  0x006EA430 should be renamed to CCombatManager::SetAttackerAsBeenStruck (clears vland's RELOC_NAME / STANDIN_LEAF warnings).
- Locks: all 154 NF341 locks released after the last compile (lock.sh ... release).
- Rule slips (recorded): one `sleep 0` early on; one attempted foreground wait loop on the equiv runs (denied by the harness, not run);
  one write attempt to /tmp outside my folder (denied). eqC (unguarded v guarded, 159 rows) ran with S = the scratchpad root, so
  equiv_check used the shared scratchpad/stubinc_equiv as its stub dir: `find -newer work/eqC.pid` there lists nothing, so no file was
  created; every other equiv run used S=$A (own stubinc_equiv).
- evidence/equiv_plant.tsv (old v plant with the unguarded text) was from a run I killed when adding the guard: moved to
  work/equiv_plant_PARTIAL_killed.tsv; not evidence.
- D1 support: in the HEAD pk_all output of the 5 emitters, both dropped helper symbols are NOADDR (no retail address anywhere): no
  verifiable copy is lost.
- eqB (HEAD tree v final + plant inside the guarded block, all rows) was killed part-way (my PID) to free CPU: by construction it flags the
  same rows as HEAD v final, so it proves nothing; the item-1 planted control is evidence/equiv_plant2_v_new2.tsv (final v final+plant on
  the 159-row changed set): exactly the 141 non-REREAD definers + 5 tests, none of the 18 REREAD rows. Partial file: work/equiv_plant2_PARTIAL_killed.tsv.
- Item 2 header equiv: evidence/equiv_cm_mentioners.tsv (final tree v + CCombatManager.h opt-in, the 210 landed rows whose source names
  CCombatManager): 0 differing; planted control on the same rows (evidence/equiv_cm_plant_mentioners.tsv): 38 differing. No file defines
  the macro, so no other row can change (evidence/equiv_summary.txt; the all-rows run was stopped part-way, see there).
