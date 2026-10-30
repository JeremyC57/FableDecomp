# Wave 9: remaining class-model debt

The counts come from `scripts/count.py`: rows that define `class|struct <X> {` under rebuild/src/compiled, with
`*.pre.cpp` excluded. "Wave 8 start" is HEAD c8e04967c; "after" is the wave 8 worktree.

| class | wave 8 start | after wave 8 | header | what is left |
|---|---:|---:|---|---|
| CScriptThing | 1,045 | **428** | CScriptThing.h | see the breakdown below |
| CEntityScriptBase | 466 | **293** | CEntityScriptBase.h | 227 follow ST (CScriptThing still local, `esbfollow.sh`); 16 pass the dry run but sit in REVERTED.tsv (CGSI-base / `__vftable` uses); 11 type Game as `void*` / `GameCalls*`; ~15 derived classes whose base-word shape differs; 4 `__vftable` used; 4 inline base ctor bodies |
| CIsThingAlive / CPredicateBase / CIsThingConscious | 235 / 219 / 40 | **154 / 142 / 27** | NScript predicate headers | 138 rows follow ST; 15 dry-run-ok rows are in REVERTED.tsv (`CPredicateBase` redefinition clashes, CPredicate_And templates); 8 `virtual bool IsTrue()`; 9 `ScriptThingCopy Thing`; 5 `m_thing` used outside |
| CActiveEntityScriptBase / CActiveEntityScript | 127 / 120 | **42 / 35** | CActiveEntityScript.h (new) | 35 `EntityScriptBinding_<addr>` / `HenchmenBinding` namespace CreateActiveEntityScript rows: their own CCountedPointer with DeleteFunc and a ShareData body, so the body changes too; 7 other shapes (e.g. DoEntityScriptPersist) |
| CEntityScriptBinding(Base) | 179 / 12 | **94 / 12** | CEntityScriptBinding.h | the same namespace rows, plus the `Register*` binding rows |
| CTimer | 201 | **16** | CTimer.h | 5 `unsigned long` word (comparisons may change sign; test); 3 namespace blocks with bodies; 2 invented `CTimer(long)` ctor; 6 one-offs (`Get()`, `operator>=` in class, `m_first`) |
| CPlayerManager | 487 | **104** | CPlayerManager.h | 19 const-only stand-ins whose non-const call sites would bind QAE 0x00449970 instead of QBE 0x00449960 (REVERTED.tsv); 30 pointer-returning stand-ins (`CPlayer* GetMainPlayer()`, `GetPlayer`, `GetPMainPlayer`; header returns a reference); 11 vptr words; 16 rows include the header as a layout donor (`#define CPlayerManager CPlayerManagerDonor`) and keep a local block (e.g. 0x00418B7A keeps `virtual ~CPlayerManager();`: the unwritten retail branch, vtable 0x01231CD0); 4 with bodies; other one-offs (`IsThingMainPlayer`, `UnfreezePlayerControls` -- not in the header) |
| CScriptGameResourceObjectMovieBase | 85 | 85 | MovieBase.h | 14 rows lost the `inline_depth` scaffolding (MBX); none include the header yet (below) |
| CScriptBase / CLevelScriptBase | 93 / 6 | **88 / 1** | | 5 LSB ctor rows done (LSBX, explicit instantiation); the CScriptBase rest as in wave 7 NEXT |
| CGameScriptInterface / Base | 220 / 36 | 220 / 36 | CGSI headers | not reached (wave 6 NEXT item 6: per-slot overload evidence) |
| CCountedPointer (local) | 3,987 | 4,290 | none | +303: flattened rows need the includer-side minimal model (LESSONS 9) |

## Wave 8 result

- **Rows.** 1,384 rows edited (ROWS.tsv). The per-family split includes multi-family rows:
  - ST2 399, CPM 383, TIMER 185, ESB2 173, AES 120, ST3 89, CAES 85, PRED 85, MBX 14, ST4 9, TR 6, LSBX 5.
- **mcheck.** 1,150 SAME and 234 RELOC_RENAMED:
  - 180 leafsym, mostly the PDB-exact AES and EAE spellings;
  - 47 callee symbol respellings with the same target;
  - 7 with only `$L` labels renumbered.
- **Regrade.** Five batches (`logs/recheck1..5.log`, 1,471 regrades): **1,451 RELOCATION_MATCH PASS + 20 MATCH PASS, 0
  failures**. Every row's latest grade is PASS: 1,364 RELOCATION_MATCH and 20 MATCH.
- **Checks.** `payload_check.py` reports 0 missing or stale. `bindings_multi_check.py` prints 0.
- **Bindings.** 68 respelled (bind_add.tsv), with the same targets.
- **Reverts.** 94 family edits reverted from their pre-images (REVERTED.tsv).

## CScriptThing: the 428 rows left (`mig_st2.py --dry --implicit`)

| reason | rows | plan |
|---|---:|---|
| `class CScriptThing : public CBase` / other non-CBaseClass head | 64 | cbfix covers only the exact CBase shape; 18 name a CBase elsewhere, and the rest derive from CScriptThingMethods / CBaseObject-like stand-ins |
| `InlineCopy` tag | 43 | the mixed-inlining residue from wave 6 (the copy is inline at some sites and called at others); try lesson 7: plain inline, no forcing |
| CCountedPointer defined after the block | 30 | move the row's CCountedPointer above the include (mechanical) |
| moved body / pimp word uses `PImp_Data` / `PImpData` | 47 | the PImp words typed as a pointee: a typed accessor `PImp.Data` respelling in the bodies |
| `void* __vftable` / `m_vtable` / `VTable` | 33 | the flattened layout: rows read the vptr word |
| `void* PImp[2]` / `long Data[2]` / `m_data[2]` | 27 | array words for PImp: respell `PImp[0]` -> `PImp.Data` |
| pad used / pimp pad / size 8 | 29 | the pads the rows read |
| local CBaseClass | 11 | drop the local block (engine/CBaseClass.h) |
| slot mismatches (slot 0 / 1 / 78) | 13 | check the call index by hand |

## Ready next (in order)

1. **Name fixes first** (coordinator).
   - NAME_FIXES.tsv has 57 lines: 55 entity classes need nesting in `NScript::<Script>`. Their `EAE` leaf already
     matches; the PDB spelling is nested.
   - Plus CQuestInfoElementTimer (nested in CDrawQuestInfo, by reference) and the 00efbe20 SetPredicate respelling.
   - Unresolved: 00dd8ba0 CTortureDoorCourtyard (two scripts in the PDB have it); 00dcf4c0 `CVtable_012DB4B8` (an
     invented class name).
2. **A shared engine/CCountedPointer.h.**
   - Real release dtor, plain inline: the MovieBase evidence in HEADERS.md says not to force it.
   - Copy and assign addref, ShareData 0x00CE1040.
   - Then retire the 4,290 local models in families.
   - Check first the rows that force `__forceinline` or declare the dtor out of line.
3. **MovieBase onto the header.** Order of work:
   1. Rows whose local model is the implicit-dtor shape and that VC7.1 reproduces (`mbexp.py` SAME): migrate onto
      engine/CScriptGameResourceObjectMovieBase.h with the shared CCountedPointer.
   2. The 9 `mbexp` DIFF rows and the single-site AB0 rows (32): residue.
   3. Next idea for those 32: the order in which c2 compiles the implicit `??1` COMDAT relative to the leaf. Generate the
      dtor first through an earlier use in the TU, then a later one, and check whether expansion depth changes
      (`mbx/b2.cpp` tried a full-expansion use first: no change).
4. **ST residue** (table above), largest mechanical first: CCountedPointer placement (30), PImp_Data (47), PImp arrays
   (27). After each ST batch, run `esbfollow.sh` and a PRED pass, since ESB and PRED follow ST.
5. **CPM.**
   - The 19 const-only rows: find whether retail's caller held a `const CPlayerManager&` (the QBE call proves it). Model
     that in the row (`const CPlayerManager& pm = ...`) and the header picks QBE.
   - The 30 pointer stand-ins: respell to the reference accessors (`&pm->GetMainPlayer()`).
6. **CAES namespace rows (35).** These have the old body shape: ShareData / DeleteFunc CCountedPointer, and
   `CScriptThing*` arguments where the PDB has `const CScriptThing&`. Rewrite them onto the template binding body of the
   85 CAES rows; byte-check each.
7. **CGSI remainder**: unchanged since wave 6 NEXT item 6.
