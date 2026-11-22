# NF285 notes (2026-11-22, round 35b): T1 standin_alias, continuation of NF282 / NF284, hand-back 1

## Summary
- **18 addresses done.** 18 binder rows respelled (rows/). 15 of them were rechecked and 3 are withheld (decision 1). One header opt-in
  (hdr/).
  - Because of the withheld rechecks, 0x00702810 has NO rechecked binder (both are withheld), and entry 1 has one of its three binders
    (0x0048F50C) unrechecked.
  - In APPLIED.tsv / STATUS.tsv:
  - 0x009E1DD0 GFAYToVY (entry 1, D3: both stand-in spellings).
  - The CAIStateGroup_TavernServer family: 0x00966AF0 / 0x00966CF0 / 0x00966DA0 / 0x009670A0 / 0x00967110 / 0x00967180 (entries 14-19).
  - 0x008E97B0, 0x00D17E70, 0x007A9B10, 0x0061A9D0, 0x0061AA80, 0x00702810, 0x006DE8C0, 0x00658BB9, 0x00664E50, 0x007034D0, 0x005D9830.
- **67 addresses skipped-port.** Every entry from 2 to 69 that is not done, plus 0x00B91840, 0x008CFFE0, 0x006A22D0, 0x007FF7D0 and
  0x0071D7C0. Each is an alias of the PDB function, but its binder reaches it through a stand-in CLASS token or a stand-in parameter type:
  - `_Retail` / `Methods` classes;
  - `CBase_` / `CElementNN_` / `CMember_` / `CStatic_` / `CCombatAction_<addr>`;
  - `N_<addr>::` namespaces;
  - ScriptMethods, CThing_Retail, CThingVector;
  - or a static / virtual / private PDB member that no in-place respell can reach.
  None is a respell inside an existing model: renaming the local token to the real class is the nf95b move, so each needs a port (T3).
  The reasons are per entry in STATUS.tsv. No file changed and no lock was taken for them.
  - **How they were read.** 42 skips come from reading the binder source (tri.py lines or the file), plus 2 more marked NOT ATTEMPTED.
    The other 23 (entries 47-69) are **triage from the spellings column only**: no binder was opened.
  - **NOT ATTEMPTED** (3): 0x0097D580 (protected member), 0x009D1260 and 0x00A3AA40 (public members). The flat-view opt-in form used
    for TavernServer may apply to them, so they are not settled T3.
  - **Identity is NOT verified for any skipped entry.** 0x00C25970 (void free stand-in v float C3DVector member, libpk range) and
    0x00A373B0 (CNavigatorBase_ ctor(long) v protected CClassFactory ctor(unsigned char)) look doubtful. So REJECTED.tsv is empty
    because nothing was proven different, not because the aliases were proven.
- **231 addresses still open.** These are entries 70-316 other than the done and skipped ones above.
- **Order (D4).** I worked entries 1-69 in ENTRIES order. Entries 2-13 and 20-69 are all stand-in class tokens. So after line 69 I took
  the remaining in-place-shaped entries out of order: a real class token with a stand-in member name, or a free stand-in for a static the
  shared header already declares. I found them with a scope compare of each stand-in against `pdb_spelling` (tools/cls2.py, scan below).
  Please confirm this is acceptable.

## Per unit (proof files in evidence/)
- **0x009E1DD0 (D3).**
  - 0x0048F50C `float __stdcall GFAYToVY_009e1dd0` (`YG`, the table witness) and 0x00437210 / 0x0043E4D0 `__fastcall GFAYToVY_009e1dd0`
    (`YI`, no table line) now all spell `float __fastcall GFAYToVY(float)`. That is `?GFAYToVY@@YIMM@Z`: Ego_r 00b34b70 + FableWin
    030a6530, as engine/CFontBank.h declares it.
  - The stdcall-to-fastcall move is byte-neutral: objsec ALL_EQ (float on the stack, callee pops, in both conventions).
  - Fresh exe scans: `xsites_009e1dd0.tsv` (119 sites, the same as NF284's scout) and `absrefs_009e1dd0.tsv` (none). The 26 other
    referencing rows already declare `__fastcall GFAYToVY` or take it from CFontBank.h / CRenderManager2D.h (source census).
  - 0x0043E4D0's pick-any `GetVStringExtents` copy equals the new 0x00437210 body: `pickany_e1.txt`, SAME, relocations included.
  - Link: `link_e1.txt`. HEAD leaves the 3 stand-in references unresolved; the new set resolves all of them against the owner. The one
    LNK4006 is pre-existing and appears in both sets.
- **CAIStateGroup_TavernServer family.**
  - Binder 0x00967650 (IsRunnable): a `CAIStateGroupTavernServerMethods` struct derived from the flat header view, which declared the six
    callees itself. The new add-only opt-in `FABLE_RETAIL_CAISTATEGROUP_TAVERNSERVER_FLAT_METHODS` declares them, declarations only, on
    the flat `CAIStateGroup_TavernServer`; the Methods struct drops its six declarations. A member's own mangling carries no class key,
    so these are exactly the PDB `QAE` spellings.
  - Binder 0x00967B40 (FillTankard), in place: `FreeBarPoint_009670a0` -> `FreeServingPoint`. Its comments called 0x009670A0
    FreeBarPoint. The Ego_r donor order (IsCarryingJug / GetServingPoint / FreeServingPoint / FreeBarPoint / GetJug = 0x00966CF0 ..
    0x00967180) and the self row make it FreeServingPoint; FreeBarPoint is 0x00967110.
  - Two sources per name: both PDB name sets, Ego_r donor order, and the self rows' Leaf lines (each self object DEFINES the PDB
    spelling; work/hc/sym_*).
  - `xsites_tavern.tsv`: the other landed binders (0x00967930, 0x00967A20, 0x00967D40) already bind the PDB spellings. `absrefs`: none.
  - objsec ALL_EQ x2. `vtcheck_tav.txt`: the row-local `??_7CIntelligentPointerThing@@6B@` of 0x00967650 is identical HEAD v new
    (pre-existing, not a new table).
  - `link_tav.txt`: every moved reference resolves; 0 duplicates.
  - `equiv_tavern.txt`: the header's 15 includers, 0 differ; the planted-token control flags 11.
- **Batch B (12 binders, in place in their existing local models).**
  - 0x008E9B60 -> LockStrafingCombinationAnimsToMainAnim (private).
  - 0x00D17DA0 -> RemoveBoastQuestInfo. FableWin plus the retail call site; Ego_r has no NScript names.
  - 0x007A9C80 -> ResetCameraOnStick. Made private as both PDBs; the caller is a member.
  - 0x0061B560 / 0x0061B530 -> MoveToNextTestQuest / MoveToPreviousTestQuest.
  - 0x004B1590 / 0x004B4AA0 -> IsActivatable.
  - 0x008455C0 -> OnShowTrophyPerformAction(CThing&). The call is `*GetCreature()`, same bytes.
  - 0x00947840 / 0x00947A30 -> IsEatTime() const.
    - 0x00947840 binds the same stand-in but has no table line; found by source grep.
    - Identity: Ego_r order IsWorkTime / IsCookTime / IsEatTime v retail 0x00658B20 / 0x00658B6B / 0x00658BB9, and the self row's
      EatStartTimes / EatDuration reads.
  - 0x00665190 -> IsThingInViewAngle(const CThing&, float, bool) const.
  - 0x00704150 -> WorldUpdateWeaponTrails. Made protected as both PDBs.
  - Every target: PDB spelling in both name sets (0x00D17E70: FableWin only, plus the call site), and the self row DEFINES it.
  - `xsites_b.tsv`: the only landed callers are these binders. `absrefs_b.tsv`: none.
  - `proof_b.txt`: ALL_EQ x12. `rename_check_b.txt`: one rename each, compared by string. `link_b.txt`: the new set resolves every
    moved reference; the only duplicate is the pre-existing one in both sets.
  - 0x004B4C50 only DECLARES `IsActivatable_00702810`; it never calls 0x00702810, so it is not a binder and was left.
- **0x005D9830.**
  - Binder 0x005BE028 replaces its free `IsAugmentation_005d9830` declaration with `#define FABLE_RETAIL_CTCINVENTORYITEM_KINDS`,
    the landed opt-in its sibling binders 0x0058578B / 0x0058807D / 0x00590235 use. The row already included engine/CTCInventoryItem.h.
  - It also gains a forward declaration `class CWideString;` (one added line; no asserts in the row), because the KINDS block declares
    GetDescription.
  - The call is `CTCInventoryItem::IsExpression(...)`. Ego_r 0055a7a0 + FableWin 01e02b60; the table PDB line has witness 0x006276F0.
  - objsec ALL_EQ, link resolves, recheck PASS.

## Recheck (recheck_final.log)
One vland `--recheck-landed` run, overlay hdr/, `timeout 900` on its own launch line, over 15 changed rows and their unchanged tests:
**15 RELOCATION_MATCH PASS**, 0 DIFFER / FAIL / RELOC_CONFLICT. The remaining warnings are pre-existing:
- SHARED_HEADER_AVAILABLE on local models;
- 4 RELOC_NAME on callees whose catalog labels differ (0x0065E133, 0x00842A80, 0x00842AF0, 0x005D98B0).

The other 3 changed rows are withheld (decision 1).

## Census
- **Names.** No changed object DEFINES a new symbol: objsec is ALL_EQ and the DEF lists are identical. Every newly referenced name is a
  PDB spelling defined by its self row (the link checks).
- **Pre-existing definitions of the same names in other landed rows** (not created here):
  - 0x00967D40 defines `CAIStateGroup_TavernServer::FreeServingPoint` (its same-TU body).
  - 0x009687B0's self row defines `?IsCarryingJug@CAIStateGroup_TavernServer@@QAE_NXZ`, which is 0x00966CF0's name: one name, two
    addresses (NAME_FIXES).
- **Spellings.** Every done address falls to 1 (TABLE_CHANGES). No address grows; no new split.
- **Stand-in tokens left behind.** Only "was ..." comments, and the dead declaration in 0x004B4C50.
- **Tests (D2).** Unchanged, including the standalone host models that name stand-ins: 0x0048F50C, 0x0041A980, 0x00437210, 0x0043E4D0,
  0x004B1590, 0x004B4AA0, 0x00D17DA0, 0x007A9C80, 0x00947840, 0x00947A30, 0x00665190, 0x00704150, 0x005BE028. None links its row.

## Left as graded (not edited, so the handed-back text is what was graded)
- **Old identities in header comments of respelled rows** (follow-ups):
  - 0x00947840 and 0x00947A30: "IsCookTime @ 0x00658BB9".
  - 0x00967B40: "FreeBarPoint @ 0x009670A0" in its Callees line.
  - 0x007A9C80: "(descriptive)" wording in older notes.
  - 0x0043E4D0 / 0x00437210: their Body lines say "GFAYToVY (0x009E1DD0)"; these are right.
- **Leaf lines.** None was added to the binders that have no `// Leaf:` line: 0x00D17DA0, 0x007A9C80, 0x0061B560, 0x0061B530,
  0x004B1590, 0x00947840, 0x00947A30, 0x00665190, 0x005BE028. NF282 / NF284 did add them. Each object's leaf function is SAME HEAD v new
  (proof files), and the graded function did not change.
- **Checks that found nothing:**
  - No changed source is `#include`d by another landed row (funclet / folded includers: grep of each basename over rebuild/src and
    rebuild/tests).
  - None of the 18 changed rows is in the Codex pass 28 RESERVED.tsv or in NF283's ENTRIES / rows.txt.
  - NF283's hdr/ has no copy of the TavernServer header.

## Tool notes
- **tools/pre.sh** has a new opt-in `NOWCHAR=1` (`-Xclang -fno-wchar`). It is VC7.1's default `/Zc:wchar_t-`, so wchar_t is unsigned
  short. 0x0048F50C (`typedef unsigned short wchar` plus `L""`) passes only with it, and its HEAD and new sources were prechecked that
  way (work/pre_e1.log).
- **tools/tri.py and tools/cls2.py**: triage helpers.
- **tools/mkapp2.py**: rebuilds APPLIED.tsv from work/st/.

## Rule slips
- One `rm` of a variable path (`rm $T/engine/CAIStateGroup_TavernServer.h`). It removed a symlink inside my own work/eqtree_new, before
  copying the overlay there. It was harmless, but it breaks the rule.
- equiv_check ran with `S` = the session scratchpad, so its stub directory is `$S/stubinc_equiv`, outside my folder. A `find -newer`
  afterwards showed nothing was written there; next time run it with `S=$A/work`.
- Every grade had `timeout 900` on its own line (hk.sh / nc.sh -> sc.sh; rf.sh) and was launched in the background, then checked with
  kill -0 in a later call. Every compiled source had its own passing clang precheck in an earlier call, after its lock was mine
  (work/pre_e1.log, pre_tav.log, pre_b.log, pre_185.log). Owner rows compiled only for the link checks are back to `released`; the 18
  binder locks read NF285 (rows.txt).

## Decisions needed
1. **Recheck withheld for 3 rows whose unchanged landed test fails the clang precheck** (VC7.1 accepts both):
   - 0x0048F50C: its test includes `<string>`, and clang rejects VC7.1's own `<ostream>` / `<istream>` (`unitbuf`).
   - 0x004B1590 / 0x004B4AA0: their tests hit "explicit specialization of CIntelligentPointer after instantiation".
   The rows themselves pass the precheck, and each is objsec ALL_EQ with the link resolving. May they land on the landing's own recheck?
   Their TABLE_CHANGES deletions (the `YG` GFAYToVY line, the IsActivatable_00702810 line) are conditional on that.
2. **Out-of-order work** after line 69 (see Summary). Continue in ENTRIES order from 70, or keep pulling the in-place-shaped entries first?
3. **The skipped-port entries** are all "stand-in class token" binders. Queue them as T3 ports (by class: CBankFile_Retail x6,
   CCriticalSectionLock_Retail, the CCombatAction_<addr> creature-action ctors, the CElementNN STL-instance rows, the static-init CStatic
   rows ...), or should this tier keep them with a defined port form?
4. **NAME_FIXES**, both pre-existing and outside ENTRIES:
   - 0x009E1E40 GFVYToAY: four CRenderManager2D rows declare it as GFAYToVY, so `?GFAYToVY@@YIMM@Z` is bound at two addresses.
   - 0x009687B0: its self row defines TavernServer::IsCarryingJug, the name of 0x00966CF0.
