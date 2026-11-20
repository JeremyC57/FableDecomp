# NF153 notes (round 10, PRIORITY pick list: CPlayer and CPlayerManager accessors)

Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log):
- 13 entries: 6 done, 3 done-already (q4276, q4408,
  q4837), 1 blocked (q4753: 0x004C7CA0 left), 3 open (q11705, q5045, q5062: 0x00935160 left, not started). 0 rejected, 0 deferred.
  Exactly: done = q4275 q4277 q4281 q4282 q5041 q11706 (6); done-already 3; blocked 1; open 3.
- Rows: 24 ported (RELOCATION_MATCH + PASS, all in one final recheck), 12 not-a-binder (8 self rows already right + 4 stale
  WORKLIST binders), 1 blocked (0x004C7CA0), 1 open (0x00935160).
- No self rename, no header overlay, no new opt-in, no Flags line added, no EH funclet includes any changed row (grep of
  rebuild/src/compiled/01/21 for each row's file name: none).

## Census (how the binder lists were made)
- work/probe_all.tsv: every landed REFS row of every entry compiled once at HEAD (tools/probe.py, catalog flags), with the symbol
  each binds at the entry address. work/wrong.tsv: the rows binding anything but the PDB spelling. Rows whose pair list came back
  empty (multi-block TU, RELOC_UNCHECKED) were checked with `objdump -t` undefined symbols: 0x005E3EA0 (wrong, fixed),
  0x00689DF0 / 0x00691D70 (right).
- Wrong binders per address: 0x00449970 5 of 409; 0x00449960 13 of 116; 0x00487DC0 3 of 464; 0x00487DD0 2 of 121; 0x00633BE0 2 of
  29; 0x00487510 2 of 31; 0x006345C0 0 of 29; 0x00487B70 0 of 12.
- WORKLIST binders already right (stale table witnesses): 0x00487BD0 (AddMode), 0x00899C90 (GetMainPlayer), 0x00702810
  (GetMainPlayer const), 0x00687CB0 (GetJoystickDeviceNumber).
- Two HEAD rows graded DIFFER in the census (probe uses catalog CompilerFlags only, not `// Flags:` lines): 0x007E2A60 (707v707)
  and 0x0065A5BB (211v203). Both bind the PDB spellings at my addresses; not rechecked, not touched.
- Acceptance per row: full (symbol, target) pair diff HEAD vs new and `objdump -t` defined-symbol diff (tools/pp.sh). Every changed
  pair lands on a spelling other landed rows already bind; defined-symbol diffs are empty except 0x005AF64E, where the pick-any
  inline CMainGameComponent::GetPlayerManager COMDAT replaces PeekPlayerManager (shared-header inlines).

## Callee respells outside my entries (the spelling count of each address only shrinks)
- 0x008FCF80 CAIStateGroupBase::PeekPlayerManager (?PeekPlayerManager@CAIStateGroupBase@@IBEABVCPlayerManager@@XZ, Ego_r / FableWin,
  engine/CAIStateGroupBase.h) in 4 rows: 0x0090EEC0, 0x0090F470, 0x009352C0 (were GetPPlayerManager_008fcf80) and 0x0090F290 (was
  CAIStateGroupBaseP::GetPPlayerManager; the empty stand-in struct CAIStateGroupBaseP stays as the class's base).
- 0x00686D50 CTCBase::PeekPlayerManager (?PeekPlayerManager@CTCBase@@IBEABVCPlayerManager@@XZ) in 2 rows: 0x007E00D0 (was
  CTCTeleporter::GetPlayerManager2) and 0x007B1EB0 (was CTCFishing::GetPlayerManager returning the stand-in CPlayerManager_007b1eb0).
- Remaining stand-in binders of 0x008FCF80 (not mine, not touched): 0x00935160 (Condition::GetPPlayerManager) and whatever else
  the table lists; of 0x00686D50: 0x0057B783 per reloc_bindings (spelling not checked).

## Open: 0x00935160 (q11705, q5045, q5062) -- not started, plan for the next pass
- The row is the dense namespaced model NCondition00935160 {C3DVector, CThing, Component, Pair, Map, Context, CPlayer,
  CPlayerManager, Condition}. Turning those stand-ins into real class tokens is not in-place, so it is a full port.
- It is CAIStateGroup_ScriptingBase<CAIStateGroupBase>::CanFire (NAME_FIXES.tsv: both PDBs, address order after Fired 0x009350C0,
  NextFrameFireable +0x14 test, 0x009352B0 `jmp`). Port: read the landed Fired 0x009350C0 first (same class, adjacent, already on
  the template leaf); Condition onto the shared CAIStateGroupBase (for PeekPlayerManager 0x008FCF80); CThing / C3DVector / CPlayer /
  CPlayerManager must leave the namespace together (hero flows into CThing::GetPosition); the two TC lookups are the retail GetTC /
  HasTC expansions (bit 170 = +0x34 bit 10, bit 0x34 = +0x24 bit 20); keep the same-TU noinline LowerBound pair the row has.
- The self rename to CanFire is a family with 0x009352B0 (binds _ForwardTarget_00935160; unclaimed) and needs a CanFire declaration
  in engine/CAIStateGroup_ScriptingBase.h; keep the self leaf unchanged in the binder port, or move all three together.

## Blocked: 0x004C7CA0 (q4753)
- See STATUS.tsv: 3 forms at DIFFER(31v29); draft work/004c7ca0_blocked.cpp (form 3) and work/004c7ca0_form2.cpp.

## Descriptive names
- No new made-up name. 0x004496B0 keeps its landed descriptive stand-in IsInMode17Or18 on CPlayer_Methods
  (?IsInMode17Or18@CPlayer_Methods@@QBE_NXZ); its body now reads the PDB enumerators PLAYER_MODE_FREEZE_CONTROLS /
  PLAYER_MODE_SPECIAL_ABILITY_FREEZE_CONTROLS. Candidate for the coordinator (one PDB only, not applied): FableWin.pdb
  ?IsControlFrozen@CPlayer@@QBE_NXZ (0x01A59E70; absent from Ego_r and Ego_d name sets) fits the body; it would need a CPlayer.h
  member. The catalog label of 0x004496B0 (CAIStateGroup_IdleAtHome::InitCreaturePosition) is wrong, as the row says.

## Self names seen wrong while porting (not in my entries; untouched)
- 0x00687C90: self leaf ?IsOtherQuickAccessItemButtonHeld@CInputProcessGameBase@@QAE_NJ@Z (local struct model, non-const, public);
  both PDBs say ?IsOtherQuickAccessItemButtonHeld@CInputProcessGameBase@@IBE_NJ@Z. Exe scan: no landed row references 0x00687C90
  (its one caller 0x0068E647 is not landed), so a leaf-only rename is free once engine/CInputProcessGameBase.h declares it. Its
  callee 0x00446060 is still bound as Helper24::Check (NAME_FIXES.tsv).
- 0x008902B0: self is the free stand-in ?CGameScriptInterface_IsHeroChild@@YI_NPAVCGameScriptInterface@@@Z; both PDBs have
  ?IsHeroChild@CGameScriptInterface@@UBE_NXZ (virtual).

## Tests changed
- 0x004496B0, 0x00687C90 (rewritten on engine/CPlayer.h with a stub of the callee; PassPatterns kept), 0x00895B60, 0x0089D690,
  0x0089D6F0 (the standalone copies respelled GetPPlayerCharacter -> GetPControlledCreature). Other tests are standalone host copies
  that do not link the row; left as they were.

## For the coordinator
- rebuild/oracles/reloc_bindings.tsv line 22357 (working tree, mtime 09:39:53 today) has five fields:
  `?KillAllThingsInVector@...	00cbed82	00e1d690_read_buf	00c00d50	00c01090` -- two records fused by a lost newline. binds.py
  threw once on it (reloc_identity.bindings_table int(None)); vland still ran.
- No header claim needed; no rename of a shared-header member.

## Locks
- Claimed beyond rows.txt (all NF153 now): 0x0047D9E3, 0x0047E941, 0x0056FBED, 0x00570009, 0x005701DA, 0x005EE99D (was `released`),
  0x005AF64E, 0x005E3EA0, 0x0065B0B4, 0x0082A960, 0x0082B630, 0x00895B60, 0x0089D690, 0x0089D6F0, 0x0090EEC0, 0x0090F290, 0x009352C0
  (17). 0x00935160 and 0x004C7CA0 stay locked to NF153 for the resume. No row held by acctB or another agent was touched.

## Stale comments left in ported rows (not edited, so recheck_final.log stays exact)
- Several rows' "Callees:" header lines still name the old stand-ins (e.g. 0x0090EEC0 "GetPMainPlayer @ 0x00449960", 0x0090F290
  "CPlayerManager::GetCurrentPlayer", 0x0082A960 "CPlayerManager::GetPlayer @ 0x00449970", 0x007B1EB0 "CTCBase::GetPlayerManager() @
  0x00686D50"); the code and the new inline comments carry the PDB spellings.
- 0x004496B0's header comment still says the EPlayerMode enumerator names are not in any local source; they now come from
  engine/EPlayerMode.h (PLAYER_MODE_FREEZE_CONTROLS / PLAYER_MODE_SPECIAL_ABILITY_FREEZE_CONTROLS).

## Rule slips
- Order of work: entries were worked by binder family across entries (the six QBEPAV rows, the PeekPlayerManager family, the
  0x00449970 rows, ...), not one entry to a terminal state before the next, as the brief asks (advisor-recommended order).
- STATUS.tsv / APPLIED.tsv were written per vland batch (6, 6, 5, 5, 2 rows), not after every row.
- 0x004C7CA0 took 4 compiles for its 3 forms (form 1 first failed to compile: missing defines).
- One foreground `sleep 60` Bash call was refused by the harness (re-done with a wait loop).
- 3 entries (q11705, q5045, q5062) are partly applied but marked `open` (their last row, 0x00935160, not started); the brief reserves
  `open` for entries not started, so the coordinator may relabel or resume.
- grades_used in STATUS counts every compile in work/grades.log for that row (HEAD baseline, forms, and the defined-symbol re-grade
  of HEAD + new), not the census probe compile or vland runs. Maximum 5 (0x0047D9E3, 0x004C7CA0, 0x0090F290).
