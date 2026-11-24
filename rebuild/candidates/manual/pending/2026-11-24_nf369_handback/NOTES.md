# NF369 hand-back: 0x00749A70 in the option-A form (NF367-Q2 ruling), Account A naming queue, 2026-11-24

Folder `$S/w22/agentNF369/` (S = the 40effb6c scratchpad). Lock name NF369. **Verdict: HOLD, keep HEAD. No run dir.**
Not written: the repo. Git: not run. Header edits: none.

## What the ruling asked
Row 0x00749A70 is graded on its leaf, the implicit `??1CTCNoise@@UAE@XZ`. Under option A it carries 0x00749810's landed text: the CTCNoise
ctor on the shared headers, so the object emits the same ctor, vftable and `~CTCNoise`. The deciding check is an objcmp of the new object
against 0x00749810's HEAD object. The row may land only if every section is SAME (only `.debug$S` may differ).

## What was measured
- Candidate `held/optA_failed/00749a70.cpp` (compiled from `rows/00749a70_optA.cpp`, the path pass_n.log names; moved to held/ after the
  failure, rows/ removed with clean.py): 0x00749810's landed source verbatim. Only the first comment line is replaced (2 comment lines) and
  `// Leaf:` becomes `??1CTCNoise@@UAE@XZ`; `diff` against orig/00749810.cpp shows lines 1-2 only.
- Compiled by `tools/pass.py` (VC7.1 RTM 13.10.3077, catalog flags, repo headers, no overlay). Each address uses one path:
  - HEAD 0x00749810: `work/one/00749810/`.
  - HEAD 0x00749A70 and the candidate: `work/one/00749a70/`.
- Grades by `tools/pass.py` -> `verify_and_land.parity_of` against the catalog oracle (evidence/pass_h.log, pass_n.log, pass_warn.txt);
  `vland.py --recheck-landed` was not run (no run dir):
  | object | grade |
  |---|---|
  | H 0x00749810 | RELOCATION_MATCH (leaf ??0CTCNoise) |
  | H 0x00749A70 | RELOCATION_MATCH (leaf ??1CTCNoise) |
  | **N 0x00749A70 (option A)** | **DIFFER(79v33)**, `WARN leaf '??1CTCNoise@@UAE@XZ' matches no symbol in 00749a70.obj; graded the largest function` |
- **objcmp** (`tools/objcmp369.py`, evidence/objcmp_810head_v_a70optA.txt). It compares every section, keyed by section name plus the
  defining symbol, on bytes, relocations, characteristics, COMDAT selection and associated section. It also compares the `.drectve` tokens
  and the DEF and UND sets.
  - Candidate v H 0x00749810: **ALL_NONDEBUG_SAME**. The 22 function/data COMDATs, the 16 `.debug$F` sections and `.drectve` (libcp /
    LIBC / OLDNAMES) are SAME. DEF and UND sets are equal. Only `.debug$S` differs (203 v 203 B, the source path).
  - **But SAME means the object has no leaf.** `??1CTCNoise@@UAE@XZ` is ABSENT from both objects (evidence/leafcheck_optA.txt) and is UND
    in both. engine/CTCNoise.h (FABLE_RETAIL_HEARING branch) declares `virtual ~CTCNoise();` unconditionally, so a TU on the shared header
    references the destructor but never emits it.
  - The two requirements are therefore mutually exclusive on the shared headers. Sections SAME as 0x00749810 means no leaf. A leaf means
    an extra `??1CTCNoise` section, and per NF367's measurements more besides.
- Candidate v H 0x00749A70 (evidence/objcmp_a70head_v_a70optA.txt): the candidate would DROP `??1CTCNoise@@UAE@XZ` and `??1CTCNoiseDef`.
  - Text scan only, no object scan. `grep -E '(class|struct) CTCNoise\b'` over rebuild/src finds 9 local models besides the shared
    header users (0x008386E0, 0x00749A50, 0x007498E0, 0x007499B0, 0x007497C0, 0x004D3675, 0x004D3658, 0x004EE23F, and HEAD 0x00749A70
    itself). None of them defines a CTCNoise constructor or `~CTCNoise`, so none should emit the vftable or the implicit `??1`.
    reloc_bindings.tsv has lines for `??0CTCNoise` (00749810, witness 004d3658) and `??_7CTCNoise` (01268a74, witness 00749810), and none
    for `??1CTCNoise`.
  - So option A would very likely leave 0x00749810's (and the vftable's) `??1CTCNoise` reference with no landed definer, and would fail the
    drop test.

## Gates
| gate | result |
|---|---|
| deciding objcmp v 0x00749810 HEAD | sections ALL SAME, **but the leaf is not defined**: FAIL (cannot land) |
| grade HEAD v new (pass.py / parity_of; vland --recheck-landed not run) | HEAD RELOCATION_MATCH; new DIFFER(79v33), leaf missing: FAIL |
| test on the shared headers | not written: deciding check failed (HEAD's landed test kept, prechecked clean) |
| both checkers HEAD v new (with pk_addrs.tsv), reach flips | not run: deciding check failed |
| drop test | not run: deciding check failed. By objcmp the candidate drops `??1CTCNoise` (the leaf; text scan finds no other definer) and `??1CTCNoiseDef`: very likely fails |
| link with 0x00749810 / 0x00749A50 / 0x004D3658 / other CTCNoise emitters | not run: deciding check failed. No duplicates to report from this pass. HEAD's known pair LNK2005 on `??0CTCNoise` (different bodies, NF367) stands |
| RENAMES / NAME_FIXES | none (no name changes) |

## Not tried, and why
- No local CTCNoise model (that is NF367's p2e) and no new CTCNoise.h implicit-destructor opt-in. Either is a different form or a new
  device, outside this ruling.
- No existing opt-in in CTCNoise.h / CTCBase.h makes `~CTCNoise` implicit. FABLE_RETAIL_CTCNOISEDEF_IMPLICIT_DTOR only affects
  CTCNoiseDef.
- A user-defined `CTCNoise::~CTCNoise() {}` stores the CTCNoise vptr (NF367 p2b: DIFFER 39 v 33; GOTCHAS agentFAM10).

## Ruling needed (NF369-Q1)
Option A cannot satisfy its own deciding check. Choose one:
- **(a) P2h:** NF367's helper form, `pending/2026-11-24_nf367_handback/held/p2_helper_unit/00749a70.cpp`. Every measured gate passes; it
  is a per-unit `?A0x` scaffolding device.
- **(b) Header opt-in plus the Q1 size device (p2f-like; NOT MEASURED in this form):** a new `FABLE_RETAIL_CTCNOISE_IMPLICIT_DTOR` in
  engine/CTCNoise.h guards the existing `virtual ~CTCNoise();` line (the `#ifndef` pattern of CTCNOISEDEF_IMPLICIT_DTOR). That is an edit
  to an existing shared-header line, not add-only, so it needs equiv_check and a planted control. NF367's p2e / p2f used a local CTCNoise
  model instead. 0x00749A70 then takes 0x00749810's text plus that macro and
  FABLE_RETAIL_CTCNOISEDEF_IMPLICIT_DTOR, plus the NF367-Q1 CTCNoiseDef size block, to avoid the new NEQ CTCNoiseDef copies NF367 measured
  in p2e. The object can then never be SAME as 0x00749810's: it adds `??1CTCNoise` and `??1CTCNoiseDef`, and with the size device
  `??_GCTCNoiseDef` becomes 34 B. Its gate would have to be stated differently: every SHARED section SAME, plus the leaf.
- **(c) Option S:** one source for several addresses (coordinator tooling).
- **(d) Keep HEAD as debt:** T3 nf359 0x00749A70 and nf367 0x01268A74 stay open.

## Rule notes
- Prechecks: clang (`tools/pre.sh`, prints errors only; empty = pass; a planted `int x = ;` control flagged).
  - Landed tests 00749810_test / 00749a70_test were checked before the lock claim (evidence/pre_tests.log).
  - HEAD sources 00749810 / 00749a70 and the candidate were checked after the claim and before the compile (evidence/pre_srcs.log).
- Locks: NF369 claimed 00749a70 and 00749810 (0x00749810 read-only) and released both after the compile (evidence/locks_final.txt: both
  `released`). No other lock taken: 0x00749A50 / 0x004D3658 were not compiled.
- No B claim, Codex reservation or other agent's row touched. No retail bytes in hand-back files (names, sizes, verdicts only).
- Tools (tools/):
  - Run under NF369: `pass.py`, `pre.sh`, `comdatsel.py` (NF367's, repointed) and the new `objcmp369.py`. objcmp369.py is a section
    compare that includes COMDAT selection, the associated section, `.drectve` tokens and DEF / UND sets.
  - Repointed copies never run under NF369: `nf.py` (library for pass.py), `pkpair.sh`, `pk_addrs.tsv`, `links367.sh`, `cmpcomdat.py`,
    `comdatlist.py`, `disfn.py`, `objcmp.py`.
