# Naming pass wave 2, agent B (large families)

All work is in `naming/w2b/`. Nothing under the repo was written, and git was not run. Every family is a deterministic,
idempotent transform script plus its verified file list. Applying them in any order gives byte-identical files
(composed forward and reversed, `diff -r` clean), and applying them twice changes nothing.

## Applied families (each rechecked baseline vs transformed, dry run --recheck-landed)
| family | decision | script | files | rows | result |
|---|---|---|---|---|---|
| CheckCRC protected | 0x00404500 `?CheckCRC@CPersistContext@@IAEXPBD@Z` | fam_checkcrc.py | 165 | 143 | 143/143 RELOCATION_MATCH+PASS; 142 rows QAE -> IAE |
| CCharString(const char*, long) | 0x0099EBF0 `??0CCharString@@QAE@PBDJ@Z` | fam_ccsctor.py | 368 | 227 | 227/227; 190 rows PBDH -> PBDJ |
| AutoTiny helpers -> ~CScriptThing | 0x004AA840 `??1CScriptThing@@UAE@XZ` | fam_autotiny.py | 158 | 158 | 158/158; 158 rows AutoTiny{First,}MemberDestructor / _004aa840 -> ??1CScriptThing@@UAE@XZ |
| ~CScriptThing virtual | 0x004AA840 | fam_cstdtor.py | 77 | 75 | 75/75 (4 DIFFER rows excluded) |
| ~CBaseIntelligentPointer virtual | 0x00A01C10 `??1CBaseIntelligentPointer@@UAE@XZ` | fam_cbipdtor.py | 531 | 463 | 455/455 shape 1 + 11/11 shape 2 (22 DIFFER + 3 test-compile rows excluded); 328 rows QAE -> UAE |

Composed check: all five scripts applied to fresh repo copies of the 1,299 listed files (`tools/compose.py ov_final ...`),
then the union of 1,064 rows rechecked on that overlay (`runs/all_a.*`, `runs/all_b.*`; the 73 rows whose files carry
the later comment-only ccsctor respelling again in `runs/final_cmt.*`). Per-family baseline/new pairs:
`runs/<fam>_base.*` / `runs/<fam>_new.*` (cbip = cbipdtor shape 1, cbipvptr / cbipvptr2_new = shape 2).

### What each script does
- `fam_checkcrc.py`: in a non-namespaced `class|struct CPersistContext` body, a public `void CheckCRC(const char*);`
  declaration becomes `protected: void CheckCRC(const char* name); public:` (the access in effect before is restored).
- `fam_ccsctor.py`: the `int` length parameter of the non-namespaced CCharString `(const char*, int)` constructor
  (in-class declarations and `CCharString::CCharString` definitions, sources and tests) becomes `long`; comment lines
  in the edited files that spell that constructor `(const char*, int` say `long` too.
- `fam_autotiny.py` (sources only): `extern void __fastcall AutoTiny{First,}MemberDestructor(void*)` /
  `AutoTinySecondMemberDestructor_004aa840` becomes `class CScriptThing { public: virtual ~CScriptThing(); };` and each
  call `F(expr);` becomes `reinterpret_cast<CScriptThing*>(expr)->CScriptThing::~CScriptThing();` (same `lea ecx ; call`).
- `fam_cstdtor.py`: `virtual ` prepended to a plain `~CScriptThing();` in a CScriptThing model that already has another
  virtual member (so no vptr is added).
- `fam_cbipdtor.py`: same for `~CBaseIntelligentPointer();` (shape 1); shape 2 drops an explicit first `void* _vptr;`
  member and makes the dtor virtual. Shape 2 only kept the 9 files (11 rows) that passed.

### Bindings, headers, labels
`BINDINGS.tsv`: every new spelling was already bound to its target, so no respell is needed. Old spellings:
RETIRE-candidates (no source emitter left after apply): `?CheckCRC@CPersistContext@@QAEXPBD@Z`,
`??0CCharString@@QAE@PBDH@Z`, `?AutoTinyFirstMemberDestructor@@YIXPAX@Z`, `?AutoTinyMemberDestructor@@YIXPAX@Z`,
`?AutoTinySecondMemberDestructor_004aa840@@YIXPAX@Z`. KEEP: `??1CBaseIntelligentPointer@@QAE@XZ`,
`??1CScriptThing@@QAE@XZ` (deferred models still emit them), and both CInterfaceMap LowerBound spellings.
No new RELOC_NAME / RELOC_DUPLICATE / RELOC_CONFLICT warning appeared in any family (baseline vs new compared per row).
`HEADERS.md`: engine/CScriptThing.h (vptr modelled as `void* __vftable`, no destructor) cannot emit 0x004AA840, so the
158 autotiny rows use a local class and print `WARN SHARED_HEADER_AVAILABLE CScriptThing` (fatal only under
LANDVERIFY_STRICT_HEADERS=1). `LABELS.tsv`: nothing new (all seven addresses already labelled). `overlay/` is empty:
no hand edits. No file renames (no row's own identity changed).

### Results of the composed check
`runs/all.log`: 1,064 of 1,064 rows MATCH or RELOCATION_MATCH with PASS, and no row gained a warning compared with its
baseline. `runs/final_cmt.log`: the 73 rows with the comment-only ccsctor delta are 73/73 on `ov_final`.
The emitted spellings moved as follows: ~CBaseIntelligentPointer QAE->UAE in 329 rows; CCharString PBDH->PBDJ in 190;
??1CScriptThing@@UAE@XZ is new in 159 rows (157 AutoTiny helpers and 1 QAE spelling replaced); CheckCRC QAE->IAE in 142.
`ov_final/` holds the composed result for the coordinator to copy, or regenerate it with
`python3 tools/compose.py <dir> checkcrc ccsctor autotiny cstdtor cbipdtor`.

### Over the cap
The edited-row total is 1,064, above the ~500 aim. I verified every family in full rather than leaving one half done.
If you want two commits: (1) checkcrc + ccsctor + autotiny + cstdtor = 603 rows, (2) cbipdtor = 463 rows. Those sets
share 2 rows (008bae10, 008baf20: ccsctor source + cbipdtor test). Every family was rechecked on its own and all five
together; a 4-family subset was not rechecked as a unit (its only cross-family rows are also covered by the 5-family
run).

### Reverts (inside verification, never landed)
- cbipdtor shape 2 (explicit `void* _vptr` member dropped, dtor made virtual): 22 rows DIFFER (the destructor-helper
  bodies grow by 4-5 bytes) and 3 rows TCC_FAIL (their tests touch `__vftable`). The 22 rows are out; the 3 rows keep
  only their source edit, and the source + original test then pass.
- cstdtor: 4 rows DIFFER, same length (a later vtable slot moves): 00d81820, 00def3b0, 00f04cc0, 00f051d0. Excluded.

### Measured, not applied (DEFERRED.tsv)
- LowerBound 0x0040F020 / 0x004365B0: `fam_lowerbound.py` fixes all of it in one script. 1,029 src files carry the
  canonical CInterfaceMap model, and the pilot (every 10th of them, `fam_lowerbound.pilot.files.txt`, 103 rows) passed
  103/103, emitting exactly the DECISIONS symbols for both addresses. About 380 more files have other shapes the script
  does not handle. Running it on the full set costs about 30 minutes of recheck in wave 3.
- GetPDefFromGlobalIndex 0x009AD9E0: about 100 rows (68 have a `struct CDefPointer` model). Fixing them needs the
  CDefPointer<const CDefClassBase> template specialization, and those rows' leaf names are wrong too. Not attempted.
  The 156 rows that declare `CDefPointer<CDefClassBase> GetPDefFromGlobalIndex(long)` are correct: they are protected
  (IAE) and bound to 0x009ADA10, which a 3-row probe confirmed.
- Not edited: namespaced CCharString / CBaseIntelligentPointer / CScriptThing models, the no-vptr models, and the
  AutoTiny test stubs.
- Pre-existing: row 0x0080E2F0 binds anonymous-namespace symbols (`?A0x<hash>`). The hash changes with the build path
  (seen as 1bda3e69 / da6e1f32 / 73001ae6 across work dirs, e9830ec2 in reloc_bindings), so those bindings are not
  stable. This is not caused by my edits.
