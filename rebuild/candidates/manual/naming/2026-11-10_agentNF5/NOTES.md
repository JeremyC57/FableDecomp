# agentNF5: name-fix queue verification and row re-authoring (2026-11-10)

## Result

The queue has 126 lines and 125 distinct addresses (0x009D49F0 is queued twice).

| Verdict | Count | Meaning |
|---|---|---|
| VERIFIED | 107 | The queued symbol is applied. Two sources each: the PDB name sets (Ego_r / FableWin), or for STLport / Fable-template / global spellings, the RELOCATION_MATCH binder row plus the source form; and the address evidence (self row, caller binding, call shape or body). |
| VERIFIED_DESCRIPTIVE | 2 | 0x013BAC18 `g_DayOnlySuffix` and 0x013BAC1C `g_NightOnlySuffix`. No original name exists in either PDB; the names come from the literals. |
| REJECTED_W | 5 | The `_W` spelling is rejected and its `G` twin applied: 0x005149F0, 0x0099B3C0, 0x0099B6B0, 0x0099BA70, 0x0099BB80. |
| REJECTED | 3 | 0x00BFEA30 (`__chkstk` and `__alloca_probe` are one CRT routine). 0x013B8C28 (the real name is `CAnimationEntry::RegisteredComponentsByType`; see NAME_FIXES). 0x00C10940 (not `~CCardPositionsDef`; see below). |
| HELD | 7 | VIRTUAL_SCREEN_WIDTH/HEIGHT (owner gate 2026-11-01). The three CRegisteredBank entries (single source, not reproducible here). 0x008E8A10 (anonymous-namespace hash). 0x00452B4D (the overload is not settled). |
| NOT DONE | 1 | 0x008916A0, a coordinator task: re-authoring the stand-in row onto shared headers was not attempted. |

In STATUS.tsv these verdicts use the brief's applied / partly / rejected wording:
- **applied (27):** verified, and no landed binder is left with a wrong spelling.
- **partly (82):** verified, with the number of binder rows left.
- **rejected (16):** the `_W` five, the three REJECTED entries, the seven HELD entries and the one NOT DONE entry. Each line
  gives its reason.

**Rows changed: 133** (`rows/<addr>.cpp`). 126 grade RELOCATION_MATCH and 7 grade MATCH. There is no HARD finding. No row
gained a BAD binding it did not already have before the change (NEWBAD guard against the pre-change scan).
- Tests: 29 rows carry a new or portable `rows/<addr>_test.cpp`. Every one of the 133 rows passes both g++ -std=c++11
  (64-bit) and VC7.1 /Od, printing `<addr>_TEST PASS`. Rows without a test file use the landed test unchanged.
- 100 rows are batch re-authorings of call sites, by family:
  - worldseed (`GetWorldSeed` 0x0049D810): 22
  - removemode (`RemoveMode` 0x00634190): 18
  - deed (`GetDeedFromHandle` 0x006C58F0): 8
  - addmsg (`AddMessage` 0x00687540): 21
  - lookat (`AIAction_LookAtPos` 0x0097D580): 15
  - movetopos (`AIAction_MoveToPos` 0x0097EC00): 12
  - getstring (`GetString` 0x009D49B0): 11

  Some rows were touched by several families.
- 33 rows are self rows, where the queued address is the row itself:
  0x00445940, 0x00450BA0, 0x004AF7A0, 0x004B73A0, 0x004C9840, 0x005BF2ED, 0x005F4B30, 0x00634190, 0x00669640, 0x00669770,
  0x006A4D00, 0x006C58F0, 0x006D5AB0, 0x007137D0, 0x0084EAF0, 0x00861D00, 0x008FCF40, 0x0096BAC0, 0x0096BC20, 0x0097E200,
  0x00995030, 0x0099B3C0, 0x009D49B0, 0x009D49F0, 0x009E2590, 0x009FBF20, 0x009FBF70, 0x009FCBF0, 0x00A00680, 0x00BACDB0,
  0x00BC6180, 0x00F336F0, 0x00F33730.

**Flags added: none.** `/Zc:wchar_t` was deliberately not added.

**Header overlay:** five repo headers, each with one new `#ifdef` opt-in (`hdr/README.md`):
- `FABLE_RETAIL_CTCENTITYEVENTS_GETEVENT_DECL`
- `FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL`
- `FABLE_RETAIL_CSUBDEFCLASSBASE_CTOR_INLINE`
- `FABLE_RETAIL_CTCSCRIPTEDCONTROL_ACTIONLIST`
- `FABLE_RETAIL_CAIBRAIN_FORCERESET`

Grade with `FABLE_HDR_OVERLAY=hdr`.

## The CWideString native-wchar_t group

The queue's `_W` spellings come from Ego_r.pdb and FableWin.pdb. Both are VS2010-era builds whose default is native
`wchar_t`. The owner decided on 2026-10-29: no `/Zc:wchar_t`, and wide text stays `unsigned short`. Under that decision the
TLC spelling is the `G` twin:

| Address | Applied spelling |
|---|---|
| 0x005149F0 | `?_M_fill_insert@?$vector@GV?$allocator@G@std@@@std@@QAEXPAGIABG@Z` |
| 0x0099B3C0 | `?AllocStringData@CWideString@@IAEPAVCWideStringData@@PBGJ@Z` (protected) |
| 0x0099B6B0 | `??0CWideString@@QAE@PBG@Z` |
| 0x0099BA70 | `?Sprintf@CWideString@@QAAXPBGZZ` |
| 0x0099BB80 | `?SprintfDynamic@CWideString@@QAAXPBGZZ` |

The non-`_W` CWideString entries are verified as queued (0x0099B510 dtor, 0x0099B7D0 `operator=`, 0x0099BE70 `operator+`).
Their wrong-spelling binders are almost all private stand-in models and remain (see "What is left").

**This departs from the brief, and the coordinator should decide.** The brief asked for an opt-in such as
`FABLE_RETAIL_NATIVE_WCHAR` that gives `wchar_t`, plus `/Zc:wchar_t` on each row's `// Flags:` line. I did not do this
because the two written decisions conflict:
- `docs/pipeline/GOTCHAS.md` line 1584 and `docs/journal/FINDINGS_LOG.md` line 815 record an owner decision of 2026-10-29.
  It says wide text is `unsigned short`, with no `/Zc:wchar_t`, and it supersedes the 2026-10-28 native-wchar_t decision.
  The retail evidence cited there: one STLport `uninitialized_fill_n` 0x005123A0 serves both `vector<unsigned short>` and
  the wide string, with no ICF.
- GOTCHAS line 2684 and the 2026-11-10 landing (rows 0x0099BA70, 0x007E5570 and 0x00A0CDD0 now carry `/Zc:wchar_t`) go the
  other way.

Owner rules take precedence over a brief, so I applied the `G` twins. If the owner has since reversed the 2026-10-29
decision again, I found no record of it; the `_W` wave is then straightforward to redo:
- an opt-in in fable_string.h;
- `/Zc:wchar_t` on the five self rows;
- then the binders.

That is a coordinator / owner call, not mine.

## New name fixes found (NAME_FIXES.tsv, beyond the queue)

| Address | Correct spelling | Wrong spellings |
|---|---|---|
| 0x0088BB60 | `?ForceReset@CAIBrain@@QAEXXZ` | `?PrepareCreaturePosition@CAIBrain_Methods@@QAEXXZ` |
| 0x005D97B0 | `?GetInventoryCategory@CTCInventoryItem@@SIJJ@Z` | `callee_5d97b0`, `GetSomething@CWideString`, `GetInventoryCategory@@YIJJ`, `sub_5D97B0` |
| 0x005BF19C | `?IsSpaceInCategoryForItem@CCategory@NInventory@@QBE_NJ@Z` | `tail_5bf19c@Tail` |
| 0x00661F70 | `?IsChild@CThingCreatureBase@@QBE_NXZ` (const) | the QAE form, `@Hero_CheckChild@4` |
| 0x009ADA40 | `?GetPDefFromInstantiationName@CDefinitionManager@@QBE?AV?$CDefPointer@$$CBVCDefClassBase@@@@ABVCCharString@@@Z` | QAE forms returning `UCDefPointer` / `UCDefPointerBase` |
| 0x0042D9E0 | `??0CDefClassBase@@AAE@XZ` (private) | `??0CBase_0042d9e0`, `??0CPSCBase` |
| 0x012E5FF0 | `??_7CConversation@@6B@` | |
| 0x007180C0 | `?DeleteData@?$CCountedPointer@VCActionBase@CTCScriptedControl@@@@SIXPAX@Z` | `?DeleteData@CActionBase@@QAEXXZ` |
| 0x013B8C28 | `CAnimationEntry::RegisteredComponentsByType` | (replaces the rejected queue spelling) |

The descriptive queue items 0x00475E92, 0x00630FE0 and 0x00F33730 are given their mangled STLport spellings.

## Investigations

- **0x00C10940** is not `~CCardPositionsDef`. The real one is 0x004F9CFC: 25 bytes, frees +0x3C, then tail-jumps to the base dtor.
  0x00C10940 is an SEH-framed dtor. It stores vtable 0x012B263C, then calls through two imports: [0x0143FF9C] on member +0x0C and
  [0x01440028] on `this` (the base). So it is a class deriving from a DLL-imported class. Its identity was not resolved here.
- **CRegisteredBank** (0x00992510 / 0x00992630 / 0x00993030) is in neither TLC PDB. The Anniversary 360 publics it is quoted
  from are not on disk here, and the PC `FableAnniversary.exe` in scratch has no such string. The header opt-in
  (`CASoundManager.h`) already uses the name. The coordinator should confirm the 360 publics before landing. The landed self names
  (`CSoundBankRecord` in an invented namespace, `CCamera::UpdateCleanupFlags`) are wrong either way.
- **0x00452B4D** ends `ret 4` (one argument), but both PDBs list only `Transfer(const char*, T&[, const T&])`.
  `Transfer<CCombatAbilityData>(CCombatAbilityData&)` is therefore unproven.

## What is left (not re-authored this pass)

These are the binder rows that still spell a VERIFIED target wrong (STATUS.tsv: "N binder rows left"). They are mostly private
stand-in models where every row is its own rewrite, not one mechanical family:

| Target | Binder rows left |
|---|---|
| CCharString ctor / copy / dtor (0x0099EBF0 / 0x0099EC30 / 0x0099EAE0) | 43 / 73 / 61 |
| CBaseIntelligentPointer (0x00A01B10 / 0x00A01B50 / 0x00A01B90 / 0x00A01C10) and its vtable 0x01238C6C | 17 / 72 / 27 / 21; 34 |
| CVectorMap LowerBound (0x0040F020 / 0x004365B0) | 40 / 30 |
| `PMainGameComponent` 0x013B86A0 | 31 |
| `PeekDefinitionManager` 0x004C7990 | 20 |
| `AddAction` 0x007137D0 callers | 20 |
| `AddTC` 0x004C9D60 | 18 |
| `Terminate` 0x008FCF40 callers | 16 |
| `~CWideString` 0x0099B510 | 16 |
| `AIAction_DoNothing` 0x0097E200 | 14 |
| `RemoveTC` 0x004C9840 | 14 |
| `OnDeactivate` 0x00861D00 | 13 |
| `TransferIn` 0x009E2590 | 11 (the self row is now `CPersistTraitsSLONG::TransferIn` with references; its callers still bind the old global pointer form) |
| `""` literal 0x0122D70E | 11 |
| CEnginePrimitiveManager ctor 0x00BACDB0 | 10 |
| `~vector<CCharString>` 0x00414EA0 | 10 |
| small data globals (0x013BAE1D.., 0x013D28D4/8, 0x013B8384, ...) | 2-7 each |
| single binders (various) | 1-4 each |

STLport COMDAT entries whose binders already bind the queued spelling (0x004518A0, 0x00453406, 0x004533E3, 0x00453465,
0x0045359E, 0x0045433F, 0x00478743, 0x004ED07D, 0x004EDA6C, 0x004EDF44) need no row change; they are VERIFIED with 0 binders left.

## Row debt (graded, but on local models where a header was not usable without wider changes)

- These rows keep a local model, now with retail names:
  - 0x009FCBF0 (`CGraphicFrameData::Sizeof`)
  - 0x00445940 (`CGamePlayerInterface`)
  - 0x004AF7A0 (CBoast nested in a local CQuestManager)
  - 0x005BF2ED (`NInventory::CTCInventoryBase` / `CCategory`, no Categories member in the header)
  - 0x00669640 / 0x00669770 (the generated 46-body GetDef family's CDefPointer model)
  - 0x00450BA0 (`CThingComponentSet` has no engine header)
- 0x0096BAC0 / 0x0096BC20 use the flattened CTCEntityEvents default plus the overlay declaration.
- The tests for 0x00669640, 0x00669770 and 0x009E2590 are the landed ones (still self-contained under the old names). They pass
  against the new objects; respell them at landing for consistency.
- 15 batch rows had landed tests that failed g++ before this pass:
  0x00449D90, 0x004A0220, 0x004AF3F0, 0x004B2B70, 0x0056BE00, 0x0069A8B0, 0x00700120, 0x00726340, 0x00729410, 0x0079DCC0,
  0x007E62D0, 0x00914460, 0x00929B10, 0x0093A080, 0x009425F0.
  - Each gets a portable copy: the calling-convention keywords and `__declspec(x)` compile away off MSVC, with a
    `TempAddr()` helper for `&Temporary(...)`.
  - Under VC7.1 the bodies are unchanged.
  - 0x0093A080's landed test already had `#define __forceinline inline` in its `#ifndef _MSC_VER` shim. It is kept, as a
    portability shim, not use.

## Rows that still bind another (unhandled) queue target wrongly

Some re-authored rows fix one family but still bind a different queue target in a spelling outside the batch families. For example:
- 0x004C9840, 0x008C1600, 0x0091CDC0 and 0x00929B10 still spell LowerBound 0x0040F020 wrong.
- 0x0069A8B0 still binds `g_pGame_013b86a0`.
- 0x007B0950 still binds a `CMessageManager::AddMessage` form.

Each still grades RELOCATION_MATCH, and none of these BADs is new. They are counted in the "left" numbers above.
