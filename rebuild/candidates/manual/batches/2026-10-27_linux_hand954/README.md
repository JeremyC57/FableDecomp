# Family pass 5 (agent954, 2026-10-27): 560 of 600 rows byte-exact

129 shape families from `famscan.py 1024` over the 2026-10-27 catalog (FAMILIES.tsv: K001-K129, 11 down to 4 instances;
EH unwind funclets and `jmp [import]` thunks excluded). One Opus 5.5 agent at high effort matched each representative,
then generated every instance with its own identity from its own bytes and callers (no labels copied across shape
twins), and graded every instance.

- **Landed:** 560 rows with `vland.py --land` after the land5 conversions (verify.log: 54 MATCH, 506 RELOCATION_MATCH,
  every test PASS, no RELOC_IDENTITY). Families: 121 complete, 8 parked.
- **Identities:** 402 proven, 107 probable, 91 descriptive (NOTES.md lists the probable and descriptive ones); the 383
  proven identities of landed rows are in ghidra_out/labels_familypass5_2026-10-27.tsv.
- **Parked (40 rows, pending/2026-10-27_linux_hand954/differ/):** K005 (6, discarded erase-result temp at +14h vs +10h),
  K006 (6, x87 10^x inline-asm kernel -- a retail_asm candidate under the new evidence rule), K009 (5, one store/load
  pair scheduled differently), K016 (5) and K086 (4) (one SIB byte: retail `[ecx+eax+4]`, ours `[eax+ecx+4]`, 14 forms
  tried), K039 (5, edi/ebx swap), K062 (5, libpk MMX inline-asm kernel -- retail_asm candidate), K072 (4, cmp/fild
  versus def-release scheduling).
- **Bootstrap fixtures:** fixmove.py moved four asm fixture copies (0x0041A0A0, 0x0041A100, 0x0041A180, 0x0041A1E0) out
  of the paths their genuine sources now take, into rebuild/integration/bootstrap_asm/, and updated build_bootstrap.ps1.
- **Process note:** the pass oracle first kept each row's trailing int3 padding (a coordinator error), which made the
  agent's dry run report false DIFFERs; the oracle was trimmed before this landing and agrees with the agent's own.

Review at landing (LESSONS.md has all of them): 11 RELOC_CONFLICT warnings where per-TU copies of the same STLport
template bind different addresses (keep per-address names or unify); about 70 stand-in spellings in reloc_bindings for
`_Rb_global<bool>::_M_increment`/`_M_decrement` (0x004292C0 / 0x004294E2) that K001 now names properly; landed
0x0093CDB0 uses an invented `CThingMask10007` type; 234 SHARED_HEADER_AVAILABLE warnings (header debt).
