# acctB hand-back, longest-row lane 4003 (1 row matched: 24,717 bytes)

Authored by a Fable 5.2 agent (the owner's "longest reserved rows" lane), re-verified by the acctB local coordinator.
Nothing is landed.

| address | result | notes |
|---|---|---|
| 0x00CD52D0 `?RegisterScripts@CScriptInfoManager@@IAEXXZ` | RELOCATION_MATCH 24717v24717, test PASS (g++ and VC7.1) | no flags, no device; matched at the first whole-function compile (g01), g02 / g03 are comment-level regrades |

Coordinator re-verification (cl 13.10.3077 under Wine, `FABLE_HDR_OVERLAY=hdr/`): `coordverify/` (grade, both test runs,
read-only vland: 1 win, no RELOC_IDENTITY / RELOC_CONFLICT / NOT_GENUINE; one `WARN RELOC_NAME`, item 2). Purity and
padded-struct greps are clean: no asm, no `volatile`, no pragma, no forced inline, no flags.

The function is one basic block of 479 statements: 6 `NProgressDisplay::DisplayProgress`, 5 master scripts
(`AddScript(CScriptInfo(name, 1, true, <Script>::Alloc, <Script>::StatGetMasterScriptDataAlloc()), "")`) and 156 scripts of
three statements each (`CheckSectionIsNotAlreadyRegistered(section); AddScript(CScriptInfo(...), section);
SetScriptActiveStatus(name, (EScriptActiveStatus)0);`). The agent paired all 459 relocation symbols one to one with
retail (7 call targets, 166 allocator addresses, 286 string literals) and checked the decoded table against
`refs/script_recovery/native_catalog.json` for all 161 scripts (names, order, section, master flag, id, data allocator).

## Review items for landing

1. **Names.** `RegisterScripts` is protected (`IAEXXZ`, both PDB name sets; sole caller Init 0x00CB5D80); the landed Init
   binds it and `RegisterDefs` 0x00F2A0F0 as public (`QAEXXZ`). `NAME_FIXES.tsv` has 10 landed spellings: those two;
   0x00CB5AC0 (item 2); 0x00CDBD20 `NScript::CQ_SunnyvaleMasterData::Alloc` (landed as the global
   `pSunnyvaleDataAlloc_func`, the FSE name); 0x00E9C300 `NScript::CV_HeroDuelScript::Alloc` (landed as the stand-in
   `Alloc_CVtable_012ed0d4`); five landed script allocators whose Leaf has `CGameScriptInterface&` where both PDBs have
   `CGameScriptInterfaceBase&` (0x00DDACB0, 0x00DFA020, 0x00E0AF60, 0x00E0E760, 0x00E81EB0).
2. **0x00CB5AC0** is `?CheckSectionIsNotAlreadyRegistered@CScriptInfoManager@@IAEXVCCharString@@@Z` (both PDBs, donor
   order directly before GetPScriptInfo, the body only destroys its by-value argument, 156 call sites here). It is landed
   under a propagated `std::_Cons_val` label; that is the vland `WARN RELOC_NAME`.
3. **`LABELS.tsv` (174 lines)**: all 161 `NScript::C<Name>Script::Alloc` addresses (119 are landed rows whose Leaf agrees;
   42 and `CalextestData::Alloc` 0x00CD4A60 are not landed yet) and the 5 data allocators. The allocator names rest on
   the PDB class per script name plus the landed Leafs: script classes have no RTTI in retail.
4. **Not recovered:** the `EScriptActiveStatus` enumerators; the row passes `(EScriptActiveStatus)0` and declares the enum
   and `SetScriptActiveStatus` row-locally, as the landed 0x00CBFAB8 does.
5. **Headers** (`hdr/README.md`): `engine/CScriptInfo.h` gets the opt-in `FABLE_RETAIL_CSCRIPTINFO_CTOR` (the inline
   constructor, FableWin.pdb `??0CScriptInfo@@QAE@ABVCCharString@@J_NP6I..@ZP6I..@Z`, and the real `__fastcall` types of
   the two allocator members; default text unchanged, two landed includers preprocess identically). New
   `engine/CScriptInfoManager.h` (declarations only; Ego_r layout at retail STLport sizes).
6. **Header debt.** The row declares the 161 script classes locally through two macros, each with its static `Alloc`
   only (no layout, no padding): about 100 existing `engine/C*Script.h` headers do not declare the factory and the other
   classes have no header. If a row TU ever includes both this declaration and a class header (for example the new
   `engine/CGameflowScript.h` in `2026-11-12_acctB_f4001`), the class would be defined twice; a shared declaration of the
   factories would remove the macro. The landed CScriptInfoManager rows (0x00CB5AD0, 0x00CB5C70, 0x00CB5C90, 0x00CB5D80)
   still carry local models and can be retyped.

## Behaviour test

`rows/00cd52d0.test.cpp` is generated (agent-side `tools/mktest.py`) from the retail disassembly in address order, not
from the source: the 479-entry call sequence (texts, allocator and data-allocator identities, id, master flag, status),
section / name consistency of each triple, the resulting table (161 unique names and allocators, 118 non-empty sections,
5 masters), every stored pointer called through the table, CCharString temporaries balanced (801 built), and the
constructor copying the name before the other stores. Eleven single-edit mutations each fail it under g++ (agent's
measurement).

## How to land

`rows/00cd52d0.cpp` + `rows/00cd52d0.test.cpp` with the two `hdr/engine/` files, then `payload.json` through your usual
vland.
