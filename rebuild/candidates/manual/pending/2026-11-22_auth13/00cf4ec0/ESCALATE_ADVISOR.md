# ESCALATE_ADVISOR 0x00CF4EC0 NScript::CQ_ArenaHoldingScriptScript::CFanMarker::Main (6164 B), AUTH13 2026-11-22

This file replaces OWNER_PACKET.md, per the coordinator (owner, 2026-11-22: "have advisor decide"). The ruling of 2026-11-22
(ADVISOR_DECISIONS_B, "AUTH12's 0x00CF4EC0 escalation") made (a) admissible only after four checks. Check 4 kills (a). Step 5 was not run:
no candidate list, no readout, no grade. Evidence is in `CHECKS.md`, with its files beside it.

## Best source and oracle (unchanged from AUTH12)
- **Source:** `p6d.cpp` (= pending/2026-11-22_auth12/00cf4ec0/00cf4ec0.cpp), stock VC7.1, no flags, overlay `hdr/` (AUTH12's CTimer opt-in).
  - Reproduced grade: **DIFFER(6180v6164)** (`r_p6d_00cf4ec0.cpp`, object `obj/p6d.obj`).
- **Forced oracle (readout, NOT a grade; reproduced):** fv.sh on p6d, forcing only `??1CScriptGameResourceObjectMovieBase` occurrences
  8..14 to expand (`oracle/fm.json`).
  - Result: 6164v6164. gobj.py on `oracle/p6d.f.obj` gives **RELOCATION_MATCH**.
  - So the text is proven up to those seven inline decisions: the six returns in the three loop-2 fan hit blocks, and the hit arm's scope end.

## The four checks (short)
1. **The fan-block 5-B misfit is not closed.**
   - Six of the seven called movie-dtor sites are inside the three loop-2 fan hit blocks (two returns each). The seventh is the arm's
     scope end.
   - The +5 B is a property of the shared macro, not of this row: sibling 0x00CF2B70 shows +5 at all 7 of its same-macro lines (Ego_d
     390/396/402 v 385/391/397). So the instruction does not depend on jump distance.
   - `continue;` at the end of the wait loop fits this row's whole line table: all six blocks, total 11940 = Ego_d.
   - But `continue;` gives +2 (a short jmp) in the sibling, so it is not adopted.
   - 14 other spellings are negative (table in CHECKS.md).
2. **The movie dtor is compiler-generated** in Ego_d, Ego_r and FableWin. The odall10 calibration matches the PDBs:

   | dtor | PDBs | VC10 (odall10) |
   |---|---|---|
   | implicit movie dtor | Ego_d / FableWin 75 | 75 |
   | GROBase dtor (user-declared) | 28 | 28 |
   | PImp `~CCountedPointer<MovieBase>` | Ego_d / FableWin 19 | 19 with the existing `FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA` spelling `{ ReleaseData(); }`; **112** with the repo's default body |

   - So the original PImp dtor calls a separate ReleaseData member. FableWin lists that member as its own function, not marked inline.
3. **The row's movie-dtor copy does not equal retail.**
   - pk.py: the row's copy is 25 B (it calls 0x006E7AB0 through the declared-only specialization); retail 0x006E7B80 is 66 B.
   - Retail 0x006E7B80 carries the release inline: no call to ReleaseData or to 0x006E7AB0.
   - So the copy the linker kept came from a TU where the PImp dtor body was visible and expanded. In this row's TU, retail calls 0x006E7AB0
     inside each expanded movie dtor.
4. **"cost <= 13" is falsified.**
   - 0x006E7B80 has 1132 call sites. 55 of the landed rows that call it pass a strict screen: stock flags, the shared header, the implicit dtor,
     no Destroy/FORCEINLINE/pragma device.
   - Hand-verified: 0x00EB40C0 (1601 B), where retail both expands the movie dtor (0x00EB418D `call 0x006E7AB0` ...) and calls 0x006E7B80
     (0x00EB429A, 0x00EB42C1) in straight-line code, no EH frame. Also 0x00D44CB0 (944 B), 0x00EEBDF0 (1240 B) and 0x00D52E90.
   - The one-TU premise is confirmed in three PDBs. Ego_d's compiland `FableScripting\Debug\register_scripts.obj` holds CFanMarker,
     BowerstoneFan, HookCoastFan, the PreMelee Guildmaster, CBordelloClient and CGhostFisherman Mains. FableWin's is Mod 0011
     `register_scripts.obj` and Ego_r's is Mod 0012 `.\Release\register_scripts.obj`.

## The constraint any device must meet (proven)
- All quest scripts are one TU, so they share one S.
- This row's call to ~CTimer 0x00CD4470 fixes S at the default 9. ~CTimer (c1 14) is always inlined at S >= 10.
- Retail calls the movie dtor from stock rows in that same TU, so at S9 the movie dtor is scored, not always inlined.
- In this row, at the same loop-2 return statements, retail **expands** the movie dtor (our c2 scores 798 / 99..177 / 74, threshold T74)
  and **calls** ~CTimer (scores 83..588).
- AUTH12's readout says scores at one site are proportional to c1, and the movie dtor costs more than ~CTimer. So no single (S, T) gives
  both decisions with the current headers.
- That leaves two possibilities: retail's score inputs at those sites differ from ours (frequencies, or the callees' c1 as c2 sees them),
  or the text differs where Ego_d still disagrees (check 1).

## Options for a ruling
- **(A) Owner-grade forced oracle.** Record the proven per-callee tie: the forced readout above is RELOCATION_MATCH, and no plain text on
  the current headers gives both decisions. The row stays unlanded with the oracle as evidence. This was AUTH12's (c).
- **(B) One pre-registered readout of the PDB-attested PImp spelling.** Swap the declared-only `~CCountedPointer<MovieBase>`
  specialization for the existing `FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA` (`~CCountedPointer() { ReleaseData(); }`, ReleaseData inline).
  Checks 2 and 3 support it: Ego_d / FableWin 19 B, and the kept copy carries the release.
  - Readout once on this row, then grade once at stock S9.
  - Weakness, stated in advance:
    - Check 4 already rules out always-inline, so this spelling can only win by changing the scores, which retail's mixed per-site
      choices in stock rows constrain.
    - It must also keep `call 0x006E7AB0` inside every expanded movie dtor here, and in all 55 clean stock rows (they become its
      acceptance set if it is made the shared default).
    - AUTH12's score argument says a scored movie dtor cannot win at the 798 sites while ~CTimer loses at 83..588.
- **(C) Keep hunting the 5-B misfit first.** The ruling's "Ego_d first" rule applies, and it is a flow-graph question:
  - The misfit sits in the blocks that own six of the seven sites. If retail's text differs there, c2's frequencies, and so the scores,
    can differ.
  - Next instrument: diff the PROC list of Ego_d's `register_scripts.obj` (raw S_OBJNAME 0x161006 .. 0x7C0806) against the PROCs of our
    VC10 object.
  - An argument-less empty inline call (5 B under -Gr) would show up there as an extra function.
  - Or apply the "different revision" criterion (2026-11-21) to the misfit.
  - Precedent (coordinator FYI, 2026-11-22): 0x008E18D0 reached RELOCATION_MATCH at stock /Gr and default T once its text was
    Ego_d-corrected. Its earlier /d2inlT65 was a wrong premise.
  - AUTH14's readouts give default T = 74 (a site expands iff score < T). That agrees with this row's k14: score 74, called.
- **Also for the ruling:**
  - Does the pick-any criterion "the dtor copy equals 0x006E7B80" apply to this row? Under the current declared-only specialization the
    row's copy (25 v 66) cannot pass whenever it is emitted.

## Land blockers under any ruling
- The 5-B Ego_d misfit is open (check 1).
- Pick-any: `??1CIsThingAlive@NScript@@UAE@XZ` copy 14 B v owner 0x00E241B0 84 B (NEQ). This comes from the inline_depth(0) predicate device.
- Not run, because step 5 was never reached:
  - the local EScriptAIPriority enum under LANDVERIFY_STRICT_HEADERS=1;
  - the CTimer forcing v a row-local body byte-identity check (and CTimer.h's wave-8 comment).

## Debt (either way, per the ruling: Ego_d confirms one TU)
The per-row S values of the 16 landed /d2inlS rows that use this class are fits, not retail's flag:
- S15: 0x00CF18D0, 0x00CF2B70, 0x00CF3D20, 0x00CF68C0 (+T69), 0x00D067A0, 0x00D13580, 0x00D17F20, 0x00D18DF0, 0x00D19BD0, 0x00D1A740,
  0x00D1B200, 0x00D22F30, 0x00D2D930.
- S13: 0x00DE8370 (+T29), 0x00E17840.
- S25: 0x00D30030.
