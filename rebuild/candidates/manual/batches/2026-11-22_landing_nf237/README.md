# Landing nf237 (Account A, 2026-11-22): CODEXP27 + AUTH24 name fixes, first part (8 rows)

Agent NF237 (second naming agent beside NF236). Hand-back: `pending/2026-11-22_nf237_handback/` (APPLIED.tsv tallies the 137 queue lines:
8 done, 60 done-already, 22 deferred, 6 blocked, 41 open; NOTES section 3 lists the open ones by family).
- **Recheck** (`logs/recheck.log`, landed tree): 8 rows, 1 MATCH + 7 RELOCATION_MATCH, every test PASS. No header changed.
- **CMaterialDef:** self rows 0x008EE790 / 0x008EE720 on the const PDB forms (2 -> 1 each; AUTH24 lines).
- **CUserProfileManager getters:** 0x0048DB35 renamed CUserProfileManager::IsSubtitlingEnabled (was a free-function stand-in under
  IsCameraResetting; files, catalog and manifest renamed here; its HEAD test's inline `__asm` replaced by a clean test). 0x00548790's eight
  GetOption<addr> stand-ins onto the FableWin spellings 0x00548B50 binds; 0x00548630 is IsUsingHeadPhones (+0x30), ending one name at two
  addresses; GetShowTargetingStatus replaces IsShowTargetingStatus (in no PDB). Nine binder addresses 2 -> 1.
- **Binders:** 0x004D3F50 (ctor takes CThing&, 0x006C5A80 2 -> 1); 0x006EB390 (private ABE, 0x006EAE20 3 -> 2); 0x009AE150 calls
  CheckUnknownSymbols explicitly after `Flags |= 5` (retail order; 0x009BBBF0 2 -> 1).
- **reloc_bindings:** 13 stand-in lines deleted, 4 added (TABLE_CHANGES.tsv); every touched address shrinks or stays.
- **Symbols / pick-any / link (agent):** only the respelled symbols change; two changed row-local stand-in inlines have no owner; 0 duplicates.

## Coordinator rulings on NF237's questions
1. The 22 deferred lines are released (NF236 landed); they go to the next agent with the 41 open ones.
2. L130-L133 are debt lines, not spelling claims: they stand as queued debt.
3. L105 (`~CDataInputStream` v the implicit dtor in header-only 0x00A4FE60): debt; a header item to declare the dtor is queued.
4. 0x0122DEE0: `?g_fMinusOne@@3MA` is in no PDB; the address is the compiler's `-1.0f` literal, so the spelling is `__real@bf800000`
   (respell the g_fMinusOne binders onto a literal; queued).
5. 0x0129BA3C label: `?DELTA_NOTIONAL_ZERO@@3MB` (engine/C3DVector.h + the table) replaces the label `?g_NormaliseEpsilon@@3MB` (labels file below).
