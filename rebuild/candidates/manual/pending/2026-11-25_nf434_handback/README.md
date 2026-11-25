# NF434 hand-back (2026-11-25): script-class base slots and 29 new script headers

This continues the NF430 / NF432 placeholder work. NF434's own summary is `AGENT_README.md`; `NOTES.md` has the method and every gate.

## What was wrong before
On HEAD, the 102 NF430 debt rows emitted 463 wrong vftable slots.
- 144 of them were `CScriptBase::OnPersist`: 0x00CBD4E0 was still landed as the `?Hook@T@@QAEXH@Z` stand-in.
- FableWin.pdb gives three override shapes:
  - RegisterMain + Main (36 classes);
  - + Init (27);
  - + Init + OnPersist (35).

## Landed: 36 rows

### `batches/2026-11-24_landing_nf434_g1o` (7 rows)
- **Renames** from the `?Hook@T` stand-ins:
  - 0x00CBD4E0 to `CScriptBase::OnPersist`;
  - 0x00CDEBB0 / C0 / D0 / E0 to `CEntityScriptBase::Init` / `OnPersist` / `OnPredicateFail` / `OnInterrupted`.

  Two sources each: retail vftable slots (0x012C1648, 0x012C3224) and FableWin `??_7`.
- **Two ports.** 0x00DC3EA0 and 0x00DCC000 were the only local emitters of those names. They move onto `engine/CQ_OpeningGraveyardSecretPassageScript.h`:
  - an add-only `_CTORS` opt-in;
  - its two size asserts pinned with `FABLE_STATIC_ASSERT_AT`.
- **Link:** /FORCE duplicates go from 6 to 1.

### `batches/2026-11-24_landing_nf434_s48a` (29 rows)
- **What:** the 0x48 Alloc rows that had no header now sit on 29 new class headers made by `gen/gen434.py`.
- **Header contents:**
  - layout from Ego_r.pdb;
  - base `CLevelScriptBase<T>`, with T read from FableWin's base class;
  - inline ctor;
  - FableWin overrides in `??_7` order;
  - compiler-generated destructor;
  - size assert.
- **Wrong T fixed.** Ten HEAD models had the wrong T: CalextestScript, CTestQuestCard2/3Script, CTestNewNav1-5Script, Cjoss_test_scriptsScript and CDanyalTestScriptsScript. No bound table or self name carries the wrong T.
- **Copies:** every `??_G` copy equals retail at its vftable slot 0 (58 / 58).
- **Undefined references:** 33 → 2.

### Coordinator checks at landing
- **Equivalence.** equiv_check copy against copy, with all 30 headers, on the tree at landing time: 0 of 57,160 rows differ.
- **g1o's last /FORCE duplicate** is the `CSecretPassageSkeleton` ctor against 0x00DC3DF0, which is the R-NF434-1 debt.
- **Slot check against the binding table.** vt432 ran against `reloc_bindings.tsv` plus self symbols, after g1o landed, on all 29 s48a objects: 170 slots agree, 0 wrong.
  - 120 slots are unbound. Slot 0 is the row's own pick-any `??_G`; the others are the class's own members, still landed under stand-ins (the nf434 debt line).
  - NF434's FableWin check matches every slot name of all 58 vftables exactly (58 / 58), so the unbound slots carry proven spellings.
- **Tracker, per address:** 0 newly open, 0 status changes, 5 closed (g1o's renames).

## Ruling R-NF434-1 (advisor decision, owner delegated, 2026-11-25): granted
0x00DC3EA0 still emits an unreferenced pick-any `??0CSecretPassageSkeleton` copy that is not byte-equal to the landed strong row 0x00DC3DF0.
- HEAD already emitted that name, as a strong definition and also unequal. The port weakens it to an unreferenced pick-any copy (nf399 form), so nothing gets worse.
- This is not the nf391 R1 case of a new extra copy.
- The close is one unit with 0x00DC3DF0 on the header's `_CTORS` ctor; it is queued.

## Debt (T3 family lines nf434 / nf432)
- **73 debt rows not ported:**
  - 5 Alloc rows of the 0x48 shape on existing headers;
  - 36 Alloc rows of other sizes (9 with no header);
  - 25 ctor rows.
- **Rows that model the 29 new classes locally:** 265 landed rows and 110 tests (`new_header_local_model_rows.txt`).
  - 59 of the rows model one of the ten wrong-T classes (`WRONG_T_LOCAL_MODEL_ROWS.txt`). They carry a wrong type, so they have their own line.
  - None is reserved for Codex pass 29.
- **Not done:** CEntityScriptBase rows 0x00EB2000 / 0x00DEF6A0 (nf328) and 0x0043B570 (CDrawNPCName swapped slots).
