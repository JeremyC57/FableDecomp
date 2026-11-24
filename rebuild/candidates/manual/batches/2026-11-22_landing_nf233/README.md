# Landing nf233 (Account A, 2026-11-22): the CNewFrontendGameComponent vtable / ctor / dtor family (13 rows, 4 header opt-ins)

Agent NF233. Hand-back: `pending/2026-11-22_nf233_handback/`. Entries q12636, q12638 (done); q12639 0x00495780 blocked; q12637 / q12640 open.

- **Recheck** (`logs/recheck.log`): 13 rows, 5 MATCH + 8 RELOCATION_MATCH, all PASS, no RELOC_CONFLICT; HEAD control the same. Link
  (`logs/link_check.txt`): LNK4006 2 -> 0 (`??1CNewFrontendGameComponent`, `?Init@T`), LNK2005 0.
- **Vtable names proven first:** all three PDBs carry `??_7CNewFrontendGameComponent@@6BCBaseClassNonCopyable@@@` / `...@@6BCDeviceResetCallback@@@`
  and the CGameComponent pair (FableWin also `??_7CDeviceResetCallback@@6B@`); NF233 decoded retail's RTTI locators (offsets 0 / 4, base
  arrays). The header builds emit exactly these; every vtable slot binds its PDB spelling. Both emitters define SETQUIT.
- **Header opt-ins** (add-only): CNewFrontendGameComponent.h `_CTOR_DTOR` (types the ctor-built members, CurrentMovement as
  `NGameComponentHelpers::ECursorMovement`; declares the ctor, dtor, `Init` (slot 1, RUN path) and the two device-reset overrides);
  CGameComponent.h `FABLE_RETAIL_CGAMECOMPONENT_DTOR_INLINE`; CStopWatch.h `FABLE_RETAIL_CSTOPWATCH_CTOR` (the three timers were never
  constructed); CEngineInterpolationInfo.h `FABLE_RETAIL_CENGINEINTERPOLATIONINFOSET_CTOR` (retail's `fldz` zeroing; Ego_d + FableWin).
  Proof: pin_asserts 2 pinned, assert_names_check 0 mismatches; equivalence over 57,022 landed rows: only six of this batch's own rows differ (0x0042DC5C / 0x0042DC5F / 0x0042DC60 / 0x0042EA8F / 0x0042F50B / 0x0051D8E0; all rechecked above).
- **Inline ~CGameComponent measured at stock flags:** plain `inline` expands in the size TU; 0x0042F50B matched on form 1, no flag sweep,
  no `__forceinline` (the HEAD comment that said forceinline was required is in NF233's REJECTED.tsv).
- **Rows:** full header ports 0x0042F50B (dtor), 0x0042EA8F (ctor; every local model gone, PDB leaf), 0x0051D8E0 (base ctor); vtable-slot
  self rows off stand-ins 0x0042DC5C / 0x0042DC5F / 0x0042DC60 / 0x00415920 / 0x00415930; in-place respells 0x00412F90 (both ctor binders),
  0x00418DCA / 0x00496070 (their PDB ctor signatures + the base ctor binder), 0x004330D2 (`?Dtor` -> `??1`); 0x00433D3A Reset onto
  CCountedPointer.h (FableWin ReleaseData / DeleteData spellings). Catalog / file names follow the PDB spellings.
- **Spellings:** 0x0042EA8F 3 -> 1, 0x0051D8E0 2 -> 1, 0x0042F50B 2 -> 1, 0x00418DCA 2 -> 1, 0x00496070 3 -> 1, 0x0042DC5F 2 -> 1,
  0x00415920 2 -> 1, 0x01230CA0 2 -> 1, 0x00433D3A 2 -> 1; 0x00415930 / 0x0122ECA4 / 0x0122DE50 3 -> 2; 0x01230C94 stand-in -> PDB name.
  No address grew.
- **Pick-any** (`logs/pickany_vs_retail.tsv`, HEAD and NEW objects): no new NEQ copy (every NEW NEQ line has the identical HEAD line);
  fixed: 0x0042EA8F's five `??1CCountedPointer` copies and its local `~CNewFrontendGameComponent`, 0x0051D8E0's `??_GCGameComponent`. The
  ctor / dtor rows end with `#pragma optimize("", on)` (restore-to-TU-default scaffolding), which makes `~vector<CWideString>` EQ 0x00412130.
- **reloc_bindings** (`logs/TABLE_CHANGES.tsv`): 10 stand-in lines deleted, 6 added (PDB vtable / ctor / ReleaseData / DeleteData / dtor
  spellings), 1 re-witnessed (`??_7CGameComponent@@6BCGameComponentInterface@@@` to 0x00495620). The 0x0122DE50 add replaces the deleted
  `CGameListener` line (count unchanged).
- **Coordinator rulings on NF233's questions:**
  1. 0x0042EA8F leaves DTOR_INLINE off (it would emit a size-mode `??_GCGameComponent` NEQ its owner): accepted; opt-ins are per row.
  2. 0x004330D2's `??_G` self leaf: stand-in kept, queued (not a folded alias: the name would come from another row's object).
  3. CEngineInterpolationInfoSet's nesting (Ego_d / FableWin nest it): header claim queued.
  4. 0x00495780: the CGRAPHICDATABANK_RELEASEDATA_SIZE_BODY + DELETEDATA_DECL_ONLY form approved as its next form (queued).
  5. 0x013B8760 / 0x01228AF1 / 0x0121A2DC: queued as an initializer / atexit pair unit.
  6. 0x00418DCA / 0x00496070: accepted under the in-place rule. Their objects define nothing new beyond their leaf (their `??1CGameComponent`
     copy is unchanged from HEAD, an empty body with no retail owner); unlike nf232's 0x00495780, which would have defined new names.
- **Queued** (tag land1122nf233): the five items above and the tests (13 rows here plus seven earlier).
