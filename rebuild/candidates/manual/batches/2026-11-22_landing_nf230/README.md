# Landing nf230 (Account A, 2026-11-22): the nf229 follow-ups (0x0042A8E0 `??_G`, the 0x013B871C stand-ins, 11 header tests)

Agent NF230. Hand-back: `pending/2026-11-22_nf230_handback/`. Entries q12620-q12624.

- **Recheck** (`logs/recheck.log`): 18 rows (17 from NF230 + the in-place 0x0042EBB6), 4 MATCH + 14 RELOCATION_MATCH, all PASS, no
  RELOC_CONFLICT; HEAD control the same. Link (`logs/link_check.txt`): the one LNK4006 the HEAD control has, nothing new; LNK2005 0.
- **0x0042A8E0** leaf `??_G?$pair@$$CBVCCharString@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@QAEPAXI@Z` (Ego_r + FableWin
  S_GPROC32 in ui_manager.obj; the body calls 0x00429E16): an explicit `std::_Destroy` instantiation under 0x0042B8FC's defines, which makes
  VC7.1 emit the pair's `??_G` COMDAT. 0x00429E16 2 -> 1 spelling (`@CAssoc_dtor@4` deleted). New header test.
- **0x013B871C, 4 spellings -> 1.** Onto CNewFrontendGameComponent.h's static: 0x00541F20, 0x00542140 (`?System_013b871c` -> `Get()->GetTextBank()`),
  0x00597BF2 (public-static `@@2` -> `Get()->ResetAttractModeTimer()`), 0x0059A238, 0x0041D0A8 (`Get()->GetPSoundBank()`), and 0x0042EBB6 as
  NF230's in-place respell (its local model's static renamed to the PDB private static; no new defined symbols; full header port queued).
  No compiled object emits `?g_pNewFrontendGameComponent` any more, so its table line is deleted. Two tests (0x0054F410 / 0x0054F370) and
  0x0042EBB6's test still model `?g_` in the host (tests only; queued).
- **Header opt-ins** (engine/CNewFrontendGameComponent.h, add-only, no layout change): `_RESETATTRACTMODETIMER` (Ego_r + FableWin, public in
  Ego_d), `_SETQUIT` (the public virtual override: FableWin, Ego_d, retail RTTI vtable slot 4 = 0x00430340, not the base's 0x00415930),
  `_GETPSOUNDBANK` (Ego_d field list + the retail read at +0x50). Proof: pin_asserts 0, assert_names_check 0 mismatches; equivalence over 57,022 landed rows: only the three users 0x0041D0A8 / 0x00597BF2 / 0x0059A238 differ (rechecked above).
- **Tests:** 11 new self-contained header tests (CObservable 0x0055CA40 / 0x0055C9C0 / 0x0055CAE0 / 0x0055CA90 / 0x0055CA00 / 0x0055C950;
  CManager 0x0042D4F0 / 0x0042D5AA / 0x0042D592 / 0x0041E3B2 / 0x0041DED2) plus 0x0042A8E0's.
- **Pick-any** (`logs/pickany_vs_retail.tsv`): owned copies EQ; 0x00597BF2's three NEQ copies and 0x0059A238's one are byte-identical NEQ at
  HEAD (existing debt, nf203 ruling).
- **Coordinator rulings on NF230's questions:**
  1. 0x0042A8E0's leaf as the `??_G` COMDAT beside an owner-less `_Destroy` instantiation: accepted (0x0042B8FC defines the same `_Destroy` copy).
  2. SetQuit opt-in (the class's public override of a protected base virtual): accepted on three sources.
  3. GetPSoundBank on Ego_d + the retail body: accepted (the one-PDB rule, nf228).
  4. 0x0042EBB6: the in-place draft lands (precedent: nf227's in-place change on an existing local model that defines only its leaf); the full
     header port is queued. Graded through vland with its HEAD test above.
  5. 0x00542140 / 0x0059A238 / 0x0041D0A8 land with their HEAD host-model tests; rewrite queued (nf229 ruling 2).
- **reloc_bindings:** 2 lines deleted (`@CAssoc_dtor@4` at 0x00429E16, `?g_` at 0x013B871C); NF230's proposed re-witness to 0x0042EBB6 not
  needed. No line added.
- **Queued** (tag land1122nf230): 0x00430340 SetQuit `QAE` -> `UAE` self row; 0x0042EBB6's full header port; 9 rows still on HEAD host-model tests.
