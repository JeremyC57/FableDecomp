# Landing nf234 (Account A, 2026-11-22): the nf233 follow-ups (0x00495780 Shutdown, the PClass pair, SetQuit, the nested InfoSet)

Agent NF234. Hand-back: `pending/2026-11-22_nf234_handback/`. Entries q12641-q12652 (entry 2's three vtable families blocked with a census).

- **Recheck** (`logs/recheck.log`): 6 rows, all RELOCATION_MATCH + PASS; HEAD control the same. One RELOC_CONFLICT warning: the file-local
  `_$E2` of the PClass pair TU, the pattern nf219's landed 0x01223EC0 shows. Link (`logs/link_check.txt`): one new LNK4006 (PClass: the
  pair's shared TU is compiled for both rows; accepted pair debt, below), LNK2005 0.
- **0x00495780 CFrontendGameComponent::Shutdown** (form f4): the approved CGRAPHICDATABANK_RELEASEDATA_SIZE_BODY + DELETEDATA_DECL_ONLY
  opt-ins plus two more add-only CCountedPointer.h opt-ins, `CGRAPHICDATABANK_RESET_SIZE_BODY` / `CONSTCFONTBANK_RESET_SIZE_BODY`: all four
  CCountedPointer copies (ReleaseData + Reset for both types) EQ their owners in bytes and relocations, no DeleteData copy. Self leaf QAE ->
  AAE (all three PDBs: private); its one binder 0x00496CF0 respelled in place. 0x00412450 2 -> 1 (all 15 other binders on STLport).
- **0x00418B7A:** SetQuit made protected in place in its local model (only the binding changes): 0x00415930 2 -> 1.
- **The PClass pair** 0x0121A2DC (`dynamic initializer for 'PClass'`) / 0x01228AF1 (`dynamic atexit destructor for 'PClass'`; names from
  all three PDBs): one TU defines PClass on the shared headers; its `_$E1` / `_$E2` match on the first form. 0x013B8760 and 0x00433CC0 2 -> 1.
  Files renamed onto CStartupDefinitionManager.
- **CEngineInterpolationInfoSet nesting:** all three PDBs nest it in CEngineInterpolationInfo; new opt-in
  `FABLE_RETAIL_CENGINEINTERPOLATIONINFO_NESTED_SET` (a typedef keeps the top-level name compiling). Its one user, 0x0042EA8F, now emits the
  nested ctor name the PDBs record; nothing else in its object changes.
- **Header proof:** pin_asserts 0 pinned, assert_names_check 0 mismatches; equivalence over 57,023 landed rows: only this batch's 0x0042EA8F and 0x00495780 differ (both rechecked).
- **Pick-any** (`logs/pickany_vs_retail.tsv`, HEAD and NEW): no new NEQ copy (the one NEQ, 0x00418B7A's `??_GCGameComponent`, is identical at HEAD).
- **New tests:** 0x00495780 (4 cases), 0x0121A2DC, 0x01228AF1.
- **reloc_bindings:** deleted the allocator-less `erase` line (witness 0x00495780) and `?Shutdown@CFrontendGameComponent@@QAEXXZ`; added
  `?Shutdown@CFrontendGameComponent@@AAEXXZ` (witness 0x00496CF0).
- **Coordinator rulings on NF234's questions:**
  1. f4's two extra Reset size-body opt-ins (the fourth form on the row): accepted (nf225's SIZE_BODY pattern; every copy EQ; no form cap).
  2. 0x00418B7A's in-place SetQuit access: accepted under the in-place rule (only its binding changes).
  3. NESTED_SET stays an opt-in; it becomes the default once the nine local top-level models are ported (queued).
  4. Order for entry 2: CFrontendGameComponent family first (0x00495620 / 0x00496070 / 0x00496CF0), then CMeshBankManager (0x00A0A110 /
     0x00A0A150), then CMainGameComponent (0x00418DCA, 469 includers) last; the CEngine ctor 0x00B260B0 (last 0x0122DE50 stand-in) queued.
  5. The PClass pair's double compile (LNK4006 on PClass in a combined link, `.CRT$XCU` registered twice): accepted as pair debt, the nf219
     NullString precedent; waits on a one-TU-for-several-addresses rule (0x004330D2's `??_G` leaf waits on the same rule).
- **Queued** (tag land1122nf234): the entry-2 families in that order, 0x00B260B0, NESTED_SET default, the pair debt, 0x004330D2, tests.
