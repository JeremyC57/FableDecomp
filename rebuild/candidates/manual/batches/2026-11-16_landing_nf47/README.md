# 2026-11-16 landing nf47: name-fix round 7, NF47 (54 rows: game-component singletons and CBankFile::FindIndexBySymbolCRC)

Owner: "Continue the naming fix queue with two agents until complete".

NF47 took three families from NF41's half, 57 rows in all, using NF41's start-of-round probe:
- **e18:** `?PMainGameComponent@CMainGameComponent@@1PAV1@A` (0x013B86A0);
- **e19:** `?m_pNewFrontendGameComponent@CNewFrontendGameComponent@@0PAV1@A` (0x013B871C);
- **e74:** `?FindIndexBySymbolCRC@CBankFile@@QBEKK@Z` (0x009CC410).

A container restart killed NF47's first run with 56 of 57 rows already done. The restarted agent finished the last row and ran
the final recheck. Brief, worklist and records: `naming/2026-11-16_agentNF47/`.

## Rows (57, all terminal; STATUS.tsv)

| Status | Rows |
|---|---|
| ported | 54 |
| not-a-binder | 1 (0x005EB4B0, already fixed in naming wave 18) |
| blocked | 2 |

**Blocked rows (1 grade each):**
- **0x008AF850:** its local CCharString / CASoundBank / CDefStringTable models collide with the header's `GetSampleBank()`.
- **0x0065C781:** engine/NGameText_CDataBank.h has no CTextEntry SpeechBank (+0x1C) / SoundName (+0x24) members.

## Entries (APPLIED.tsv)
- **e19 done:** 9 binders, the 6 listed plus 3 more found.
- **e18 blocked:** 32 binders fixed; 0x008AF850 is left.
- **e74 blocked:** 22 binders fixed; 0x0065C781 is left.
  - The claim holds: FindIndexBySymbolCRC is in both PDBs, and none of the stand-in callee names (`NGameText::CDataBank::LookupValue`
    and others) is in either PDB.
  - So no caller kept a different true callee through ICF.

## Rows and checks
- **Self renames (section 1, 3 rows; no file moves):**
  - 0x0042F75E: CNewFrontendGameComponent::Init is now virtual (`UAE`);
  - 0x00497B30: CProgressDisplay::GetPTextBank;
  - 0x0082A180: CMiniMapDisplay::InitialiseOnRegionLoad.

  Their callers already use the new names.
- **Section 2:** 80 binder respellings.
- **Tests:** 0x0047DB73 and 0x00497B30 had tests that linked the made-up globals. Their tests are rewritten on the new spellings,
  with the same PassPattern.
- **Recheck (`logs/recheck_part0..2.log`):** all 54 graded from the repo after landing. All 54 are RELOCATION_MATCH, every test
  PASS.
- **Checks:**
  - funclet_includes_check reports 1,355 checked, 0 broken;
  - `naming/rebind.py` removed 157 binding lines and added 101;
  - bindings_multi_check is at its baseline (`NullCoord`).

## Accepted with row debt
- **The 16 e74 "fallback" rows** keep a local NGameText::CDataBank model that predates NF47, although
  engine/NGameText_CDataBank.h exists. The coordinator read 0x004023F0, the largest of these edits, and accepted them, because they
  move the rows toward the shared headers:
  - the local CWideString / CCharString / CCRC models are deleted in favour of `fable_string.h`;
  - the local CDataBank now derives from the shared engine/CBankFile.h;
  - the made-up `LookupValue` is gone.

  Moving them fully onto NGameText_CDataBank.h is row debt.
- **0x004175E5** adds the PDB static `PMainGameComponent` to its existing local CMainGameComponent model. This is the same kind of
  debt.

## Headers (opt-ins only; default path unchanged)
- engine/CMiniMapDisplay.h: `FABLE_RETAIL_CMINIMAPDISPLAY_INITIALISEONREGIONLOAD`.
- engine/CNewFrontendGameComponent.h: `FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_INIT`.

## Name fixes
NF47's `NAME_FIXES.tsv` lists 5 proven fixes. Four were already queued (0x006E3AB0, 0x007BEDF0, 0x00890670 and 0x00A39CC0).
0x006E3890, the index-plus-name PlaySpeech overload, is queued now as `land1116fu`.

## Rule slips
- One temporary file was written to /tmp outside the agent directory, then deleted.
- One read-only repo grep ran without `timeout`.
