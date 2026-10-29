# Wave 3 header changes

Two header changes are delivered and one is measured and deferred. All three come from w2a/HEADERS.md and w2b/HEADERS.md.

## Delivered

| header | change | retail symbol | source |
|---|---|---|---|
| `rebuild/include/engine/CDataInputStream.h` | private `void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);` plus an opt-in friend hook `#ifdef FABLE_CDATAINPUTSTREAM_FRIEND friend FABLE_CDATAINPUTSTREAM_FRIEND; #endif` | 0x00993CA0 `?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z` | `headers/rebuild/include/engine/CDataInputStream.h` (byte-identical to the patched generator's output) |
| `rebuild/include/engine/CWorld.h` | public `static long __fastcall GetFrame();` (one line after `_pad_0x191`) | 0x0049D870 `?GetFrame@CWorld@@SIJXZ` | `headers/rebuild/include/engine/CWorld.h` (the repo header plus that one line) |

- **Generator:** `gen/gen_class_headers.patch` is a unified diff against `tools/decomp_pipeline/gen_class_headers.py`. The patched copy is `gen/new/gen_class_headers.py`.
  - It adds a `HEADER_MEMBERS` table. Per class, the table can hold:
    - `friend_hook`, a macro that becomes a guarded friend declaration;
    - access-labelled member lines;
    - `virtual_dtor`.
  - `emit_header` changes in two ways. `virtual_dtor` replaces the slot-0 `void* __vftable` member with `virtual ~Cls();` and drops its offsetof assert. The hook and member lines are emitted at the end of the class body.
  - Only CDataInputStream and CWorld are opted in. The CScriptThing entry is present but commented out.
- **Regeneration:** a Linux regeneration (`gen/rungen.py`, which maps the manifest's Windows paths) gives `gen/out2/engine/*.h`.
  - For CDataInputStream.h, that output is the delivered file.
  - For CWorld.h, the raw output differs from the repo header only in comments (evidence count 3 → 2, and one whitespace change), on top of the new member. To keep the diff minimal, the delivered CWorld.h is the repo file plus the member line. A Windows regeneration should give the same comments as the repo.
- **How rows use the hook:** a row that needs the private member does `#define FABLE_CDATAINPUTSTREAM_FRIEND <class|struct> <Helper>` before the include (`fam_rwsco_hdr.py`, 35 rows). The helper class keeps its inline body, which calls `ReadWithSrcChunkOverflow` on the stream. Without the define, the header is unchanged for every other includer.

### Includer recheck (byte change in any includer = the item is dropped; none changed)

- **Includer set.** Every row whose source or test mentions `engine/CDataInputStream.h` or `engine/CWorld.h`: `inc_CDataInputStream.*.txt` and `inc_CWorld.*.txt`, 292 rows. This is a superset of the 195 direct `#include` files found by a grep of the current tree (`inc_now.files.txt`).
- **Final run:** `runs/fin_base.*` (repo tree and repo headers) against `runs/fin_new.*` (the composed overlay `final/` plus `inc_w3/`, which is the repo `rebuild/include` with the two headers).
  - The run covers 715 rows: the 461 edited rows plus 254 includer-only rows.
  - Result: 715/715 MATCH or RELOCATION_MATCH with test PASS (`runs/fin_cmp.txt`).
  - Every one of the 254 includer-only rows keeps its relocation pairs and status unchanged.
- **Earlier run with the CScriptThing change (`runs/hdr_cmp.txt`):** all-header measurement `hdr_base` against `hdr_new` over 665 includer rows (`inc_all/`). Every failure there came from CScriptThing. The CDataInputStream and CWorld changes were neutral there too.

## Measured, NOT delivered: `engine/CScriptThing.h` virtual dtor 0x004AA840

Replacing `void* __vftable` with `virtual ~CScriptThing();` (generator `virtual_dtor`, the 158 autotiny rows of `fam_cst_header.py`, prepped as `ov_csth`) breaks **52 of 384 includers**:

| count | what breaks |
|---|---|
| 37 | RELOCATION_MATCH + LINK_FAIL: the self-contained tests now need `??1CScriptThing@@UAE@XZ` and `??_7CScriptThing@@6B@` stubs |
| 9 | SRC_FAIL (8 test PASS, 1 TCC_FAIL): sources that use `->__vftable` or `offsetof(CScriptThing, __vftable)` |
| 1 | RELOCATION_MATCH + TCC_FAIL |
| 5 | RELOCATION_MATCH + PASS with new pairs: local derived classes whose dtor turns QAE → UAE (`ScriptThingCopy`, `CAliveScriptThing`, `CIsThingAlive`, `Handle@Row00e15500`). That is a spelling change in rows outside this family. |

No byte changes were seen in the matching rows. The item still needs 52 row edits: test stubs, `__vftable` users rewritten to `*(void**)this`, and the derived dtors reviewed against the PDB. That is outside this wave's row budget, so the generator entry stays commented out and `fam_cst_header.py` is not delivered as applied.
