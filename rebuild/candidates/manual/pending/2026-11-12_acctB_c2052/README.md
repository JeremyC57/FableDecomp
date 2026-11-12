# acctB hand-back, chunk 2052: 2 of 4 rows matched, 2 not yet authored

Chunk worked one row at a time (project owner's rule). Re-verified by the acctB local coordinator. Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x0049F4C0 `?SaveGameStateInternal@CWorld@@IAE_NABVCWideString@@_N@Z` | RELOCATION_MATCH 2825v2825, test PASS (g++ and VC7.1) | 15 grades. `// Flags: /Gr` |
| 0x004A21F0 `?LoadGameStateInternal@CWorld@@IAE_NABVCWideString@@_N@Z` | RELOCATION_MATCH 2931v2931, test PASS (g++ and VC7.1) | 13 grades. `// Flags: /Gr /GS` |
| 0x0048E78E CSubtitleRenderer::DrawButtonGraphics, 0x007CACB0 CTCHeroReceiveItems::GlobalUpdate | NOT_ATTEMPTED | outlines in `NOTES.md`; both go to the next batch |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (both grades, both
test runs, read-only vland: 2 wins, no RELOC_IDENTITY / RELOC_CONFLICT / NOT_GENUINE; `WARN RELOC_NAME` lines
explained below). Purity and padded-struct greps clean. The overlay is chunk 2051's plus additions, so I regraded
0x004A6E30 CWorld::Init (`2026-11-12_acctB_c2051`) under it: RELOCATION_MATCH. `grades.log` has one line per grade (39).

## What closed each row

- **Save** (first draft 727/746): `StartTime` copied into a named `double start_time` before the GFGetTime call with
  the new total stored to the local before the member; and the file-assembly tail (`header_length` .. `Close()`) in
  its own block, which puts the CRC word on the shared temporary slot. Both FITTED.
- **Load**: from grade 18 the code equalled retail except the frame (0x16C v retail 0x170, 107 displacements). One
  frame-packer trace showed retail gives the file and the parser more weight; the fix is an early
  `if (SerialisingHeroState) return true;` before the clock / menu / autosave tail. The compiler merges the extra
  return into the final one (no code), but it adds destructor references to the locals live there. FITTED.
- PROVEN from retail's own operands: the `GetDefaultVal` construct-copy-destroy shape, the file access types (4 save,
  1 load), the explicit bool defaults and save-type default 2 in load, the out-of-line two-argument `Transfer` calls.

## Review items

- **Flags.** `/Gr` as the TU's CWorld::Init (same evidence: file-level functions mangled fastcall without a keyword);
  `/GS` on Load for the stack cookie, as the TU's landed LoadGameState 0x004A3200.
- **`ESaveType` is nested in `CWorld` in both PDBs** but the repo's `CWorld.h` declares it global, so both rows bind
  0x004AB640 as `??$Transfer@W4ESaveType@@@CPersistContext@@...` where the PDB has `W4ESaveType@CWorld@@`
  (the vland warning on both rows). Nesting it in an opt-in hit a VC7.1 limitation
  (`CPersistTraits() : DefaultVal() {}` cannot value-initialise a nested enum), so the agent reverted it. Needs a
  decision at landing.
- **Two-argument `Transfer` / `GetDefaultVal` placement differs between the two rows**: Save defines both inside the
  row (as the landed TransferBoasts does); Load uses the header's `FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS` body
  and defines only `GetDefaultVal` in the row. Both match; unify as you see fit.
- **`GetDefaultVal<C3DVector>` = `C3DVector(0,0,0)`** specialisation is fitted.
- **Wrong catalog labels:** 0x006B9610 is `?LoadGameState@CFactionManager@@QAEXAAVCPersistContext@@@Z` (catalog
  `CFaction_LoadFromPersist`); 0x004046B0 is the STLport `_List_base<CStringParserCommentBoundaries>` destructor.
- **`NAME_FIXES.tsv`** (six): Start / StopAutoSaveProgress 0x0049E7C0 / 0x0049E8C0 are protected (`IAE`) in both PDBs,
  landed rows bind them public; 0x00410AC0 / 0x004AB710 are template-instance `Transfer` spellings; 0x0049F180 is
  `ActivateWorld(bool)` (a landed row declares `PostLoad(long)`); 0x004AB640 `Transfer<CWorld::ESaveType>`.
- **Made-up names:** only the two file-static flags 0x013B89D0 / 0x013B89D1, spelled as the landed 0x004A05C0 spells them.
- **Headers** (`hdr/README.md`): new `engine/CPersistContextWriteSection.h` (Ego_r layout, inline ctor / dtor);
  `CWorld.h` opt-ins `_SAVEGAMESTATEINTERNAL`, `_LOADGAMESTATEINTERNAL`, `_TYPED_SAVE` (CurrentWorldName as
  CCharString, SaveGameMarkerPos as a C3DVector at the odd offset +0x141 with `FABLE_RETAIL_C3DVECTOR_PACKED`);
  declaration-only opt-ins for the Ego_r callees; two inline-body opt-ins (CTCInGameMenu Get / SetAccumulativePausedTime
  from FableWin, the CCharString overload of `CStringParser::Init`). Default texts unchanged. Land after c2051.

## How to land

`rows/0049f4c0.*` and `rows/004a21f0.*` with `hdr/engine/`, then `payload.json` through your usual vland.
