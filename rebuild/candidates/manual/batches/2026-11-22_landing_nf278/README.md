# Landing nf278 (Account A, 2026-11-22): the CMainGameComponent slot self rows onto their PDB spellings (4 rows)

Agent NF278 (entry 1: four rows done, 0x00416E78 open; entry 2 measured, not applied; entry 3 not started). Hand-back:
`pending/2026-11-22_nf278_handback/`. No header or table change.
- **0x00415E83** `?Hook@T@@QAEXXZ` -> `?OnPreDeviceReset@CMainGameComponent@@UAEXXZ` (Ego_r 0x00415023 + FableWin 0x018A169C; retail 0x0122F174 slot 0),
  **0x004197A0** `?SetQuit@T@@QAEXXZ` -> `?SetQuit@CMainGameComponent@@UAEXXZ` (FableWin 0x018A11F0; 0x0122F180 slot 4), **0x004197B0**
  `?IsEditorActive@T@@QAE_NXZ` -> `?IsEditorActive@CMainGameComponent@@UBE_NXZ` (FableWin 0x018A1210; 0x0122F180 slot 9): each drops its `struct T`
  stand-in for `engine/CMainGameComponent.h` (default path or the landed `_INIT_SETQUIT` / `_EDITOR` opt-ins); MATCH on the first form; each object
  defines only its own name.
- **0x004197B3** `??_GCMainGameComponent@@UAEPAXI@Z`: a fold row with no code of its own including the dtor row 0x00418B7A's file (the nf251
  pattern; VC7.1 emits `??_G` only beside the vtables). RELOCATION_MATCH; its dtor call moves off the stand-in `?Dtor@CMainGameComponent@@QAEXXZ`
  onto `??1CMainGameComponent@@UAE@XZ`; every copy equals its owner or has none; one equal-copy LNK4006. (The ctor-TU fold also matches but
  carries one NEQ copy, `~CCountedPointer<CGraphicDataBank>` v 0x004194B5, so the dtor-TU fold is taken.)
- **Census:** these addresses are referenced only from vtable slots, which both emitters 0x00418DCA / 0x00418B7A already bind with these spellings
  (slot 0 `??_E`); 0x00415E83 / 0x004197A0 / 0x004197B0 / 0x004197B3 2 -> 1 spellings, 0x00418B7A 2 -> 1. Link: the three slot names resolve, 0 duplicates.
  funclet_includes_check: 1,360 checked, 0 broken.
- **Recheck** (`logs/recheck.log`, landed tree): the 4 rows + 0x00418B7A / 0x00418DCA: 6 PASS (3 MATCH, 3 RELOCATION_MATCH), 0 RELOC_CONFLICT.

## Coordinator rulings on NF278's questions
1. The four rows land; 0x004197B3 as the dtor-TU fold (no NEQ copy; it compiles 0x00418B7A's local models, which a later port of 0x00418B7A fixes
   for both).
2. 0x00416E78 (GetInputs; non-virtual padded local model) queued as a port, with the NAME_FIXES lines for 0x00415FE0 / 0x00415FF2 (ProcessEvent /
   EventIsSystemEvent, protected IAE) and the 0x004457F0 two-name conflict.
3. Entry 2 (the `_M_erase` split at 0x004507A1): option (b), the four rows 0x00ADC190 / 0x00ADC250 / 0x00ADC4E0 / 0x00ADCB90 queued for a port onto
   `fable_containers.h` / `fable_map.h` / `CVertexGroupBlock.h`; no exception to the in-place rule.
4. 0x00ADCE60 to the next agent.
Slips (the entry-1 link in the foreground; unchanged sources graded inside vland without their own precheck): noted.
