# The scoreboard crypto TU: 0x00560BB0 GetDecryptedDocument + its four TU-mates (p25N, 2026-11-21)

One batch, five rows, all on shared headers. They share one define block, one `// Flags: /Gr` line and no header change.

| row | kind | grade | same-TU copies in the file | test |
|---|---|---|---|---|
| 0x0055E040 `anonymous namespace'::EncipherMorsel | re-port of a landed row | MATCH (312) | none | landed model test (XTEA vectors), unchanged |
| 0x0055E180 `anonymous namespace'::DecipherMorsel | re-port of a landed row | MATCH (312) | none | landed model test, unchanged |
| 0x0055E2C0 `anonymous namespace'::CryptoMatic | **NEW** row (in functions.tsv, not in the catalogue) | RELOCATION_MATCH (549) | 0x0055E040, 0x0055E180 | new model test (`test/0055e2c0.cpp`) |
| 0x00560920 `anonymous namespace'::Explode | re-port of a landed row | RELOCATION_MATCH (653) | none | landed model test, unchanged |
| 0x00560BB0 CTCHeroOnlineScoreboard::GetDecryptedDocument | **NEW** row (my reserved row) | RELOCATION_MATCH (321) | all four | new real-object test (`test/00560bb0.cpp`) |

**Landing order.** Land the three re-ports and the new 0x0055E2C0 first, then 0x00560BB0. Every copy is byte-identical to its row file
(checked by script).

**Strict vland:** passes on all five (`vland.log`). The run used `--recheck-landed` (three rows are landed), LANDVERIFY_STRICT_HEADERS=1,
an empty FABLE_HDR_OVERLAY and a fresh LANDVERIFY_WORK. There is no shadow root: 0x0055E2C0's oracle is in `oracle.tsv` (bytes read from
Fable.exe). The only warnings are two RELOC_ALIAS on 0x00560920 (puts / abort import thunks).

**Catalogue (GOTCHAS 2026-11-19):** the three re-ported rows move to `/Gr`, so their `CompilerFlags` in build_candidates.ps1 must be set
to `/Gr` when they land. The two new rows need catalogue entries with `/Gr`.

## Where the text comes from
- **Source text.** The landed **0x00563480** DownloadScoreboardForSelectedClan file already carries GetDecryptedDocument, Explode, CryptoMatic
  and the two morsels as same-TU text. It uses `/Gr` and the shared headers, but these copies are not marked Fidelity.
- **Graded with /Gr, it matches.** Graded against each address with /Gr, that text matches all five rows once the parts these rows do not
  use are removed (the score-pair push_back specialisation, its auto_inline pragmas and `reverse`).
- **Edits.** This batch only:
  - removes those parts and 12 neutral defines;
  - splits the morsels' first declaration over two lines, as Ego_d's line table has them.
- **Ego_d.pdb (file id 2880) agrees with the text:**
  - every parameter and local name (`evidence/` and the Identity lines);
  - the statement order and record counts of GetDecryptedDocument and Explode (`evidence/od_lines.txt`). GetDecryptedDocument has
    decrypted_size before doc_str, and Explode is a do/while with no `do` record.
  - Ego_d is a Dinkumware build, so its STL lines are other sizes.
- **0x00563480 is left as is.** Its copies are not marked Fidelity, and its morsel declarations sit on one line. That is not my row, so I
  did not touch it.

## /Gr
- **The retail TU is __fastcall-default.**
  - Ego_r mangles every helper YI.
  - The STLport instances it calls out of line (search 0x00567120, `__uninitialized_copy` 0x00568E70) take ecx/edx without a
    `__fastcall` spelling in STLport.
- **What the flag changes:**
  - Without /Gr, Explode grades DIFFER(472v653): _M_insert_overflow stays a call. /d2inlT180 inlines `search` instead (672 bytes).
  - 0x00560BB0 matches with or without /Gr.
- **Precedent:** the landed 0x00563480 already uses /Gr (1,050 landed rows do).
- **Rule question:** this is a calling-convention flag, not an inline-budget control, so it falls outside my brief's flag rule. Your
  call. I see no /Gr-free text: the alternative would be to spell `__fastcall` onto STLport templates retail never spelled.

## Same-TU bodies (`evidence/same_tu_ablation.txt`)
- **0x00560BB0 needs Explode's body.** Declaration-only gives DIFFER(336v321).
  - Retail keeps both buffer starts and the separator in registers across the later calls.
  - VC7.1 does that only when it can see the callee keeps no local's address.
- **0x0055E2C0 needs the two morsel bodies.** Declaration-only gives DIFFER(584v549).
- **CryptoMatic and the morsels are byte-neutral in 0x00560BB0.** They are still the retail TU-mates, and they are in the file so the
  behaviour test links the real object.
  - An anonymous-namespace function cannot be stood in from another TU, because its `?A0x` hash follows the compile path (measured: the same
    text at three paths gave three hashes).
  - With every anonymous helper defined in the row, the object's only external callees are LHHttp2's.
- **operator new[] / delete[] declarations** (as in the 0x00563480 text) are needed.
  - Without them, `new char[]` binds `??2` / `??3`, and strict vland rejects 0x0055E2C0 with RELOC_IDENTITY.
  - Retail calls the operator_new[] / operator_delete[] thunks 0x00BFEB22 / 0x00BFEB1C.

## Define block
- 14 defines, the same in all five files.
- 12 defines of the 0x00563480 block are byte-neutral for all five rows. They were dropped together, and the five grades are unchanged.
- Every kept define fails without it: SRC_FAIL, or a DIFFER on one of the rows (listed in `evidence/same_tu_ablation.txt`).

## Tests
- **0x00560BB0** (`test/00560bb0.cpp`, passes in vland). It links the real row object, so DecipherMorsel, CryptoMatic (mode 1), Explode
  and the STLport templates run for real.
  - **Stand-ins:** GetDocumentSize / GetDocument are `/alternatename` stand-ins that log their `this` (the PHttp pointer at +0x0C).
    CCharString::NullChar is a data stand-in.
  - **Inputs:** each document is the test's own mode-0 encryption of a known text with the given key (a reference encoder written in the
    test). So the row must decrypt with its key argument and split at CRLF.
  - **Scenarios:**
    - no document: returns -1 and reads nothing;
    - four lines with the tail dropped;
    - a second key appending three lines, with earlier entries kept;
    - no CRLF;
    - one short line.
  - **Also checked:** GetDocument's buffer is zero-filled, in its own malloc block of size + 1 (`_msize`).
  - **Mutants:** 14/15 KILLED (`mutants/00560bb0.out`). They cover the row body and the Decipher / CryptoMatic / Explode copies.
  - **The one survivor** is CryptoMatic `dlen-=4` -> `dlen-=0`. It decrypts one extra partial block of slack bytes after the zero padding,
    past the last CRLF, inside the heap block's rounding, so nothing observable changes.
- **0x0055E2C0** (`test/0055e2c0.cpp`): a self-contained copy of the row text (the anonymous function cannot be bound).
  - Mode 0: size, length word, nothing past the last block, first block = XTEA under the key-derived schedule.
  - Mode 1: restores the zero-padded text and returns the stored length.
  - A wrong key does not decrypt, and an oversized stored length is clamped.
- **Re-ports:** keep their landed model tests unchanged. Their row texts now also run for real inside 0x00560BB0's test, except
  EncipherMorsel (mode 0 only).

## NAME_FIXES.tsv
- The landed morsels' parameters (in/out/key, no `delta`) and Explode's (text/separator/out, pos/found/piece, a while loop) become Ego_d's.
- The re-ports drop the landed files' function-pointer "keep" globals, local STLport models and noinline stand-ins.
