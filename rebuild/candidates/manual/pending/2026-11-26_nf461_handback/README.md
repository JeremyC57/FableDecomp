# NF461 hand-back (2026-11-26): short `??_7` COMDAT tables (r459a) and `/DEFAULTLIB:"libcp"` (r459b)

Measurement only. No repo writes, no git, no locks. NF461 stopped early at the coordinator's request, and the resume point is below. NF461 compiled all 57,160 landed rows fresh with VC7.1 RTM and catalog flags at FullDecomp 085b7e4d7b. It took a drift check during the run: 327 rows were recompiled and none changed.

Files:
- `VFTABLE_SIZES.tsv`: one line per `??_7` definition, with its name, row, size, selection, retail size, retail address, whether retail has an RTTI locator, class and note.
- `evidence/`:
  - `summary.txt`;
  - per-name verdicts in `q1_vft_names.tsv`;
  - the link experiment (`q1_link_experiment.txt`, `q1link_src/`);
  - the libcp `.drectve` variants, the showIncludes chain, the guard overlay diff, the proof sample and the partial route scan.

## Q1: short COMDAT vftables
> **Correction (NF467, 2026-11-26):** `retail_size` overshoots for at least 38 names, because the counter also took a following data dword that looks like a code address. The counts below that depend on retail size are therefore upper bounds; use counted function slots (`pending/2026-11-26_nf467_handback/evidence/vtaudit.tsv`).

There are 26,143 `??_7` definitions in 11,703 row objects, covering 4,948 names. **Every one is selection 2 (pick any).**

**Size problems:**
- 898 names are defined at more than one size. One name has up to 19 sizes.
- 3,723 rows define a copy shorter than the largest copy of that name.
- 4,855 rows define a copy shorter than retail; 3,661 of those are exact, because retail has a locator there.

**Per-name verdicts:**

| Verdict | Names |
|---|---|
| largest copy = retail, one size | 1,821 |
| largest copy = retail, several sizes | 595 |
| largest copy > retail | 177 |
| largest < retail, several sizes | 204 |
| only one size, < retail | 1,875 |
| retail size unknown | 276 |

- 2,079 names have no copy at retail size. For 1,734 of them, the largest copy is a single 4-byte slot.
  - Example: `??_7CCreatureAction_CastBattleCharge@@6B@` is 4 bytes; retail is 324.

**Real link** (VC7.1 `link.exe`, no /FORCE, synthetic 4-byte vs 16-byte copies):
- **The first copy in link order is kept, not the largest.** This holds for plain objects and for `.lib` members, with or without /OPT:REF. There is no warning.
- With the short copy first, the exe page-faults on a call through slot 3.
- Applied to the census:
  - In row-address order, 651 names would get a copy shorter than their largest, and 2,553 names a copy shorter than retail.
  - In `build_candidates.ps1` order, the figures are 439 and 2,367.

**Side findings:**
- 177 names (508 rows) emit more slots than retail. For example, the `*Def` tables have 26 slots where retail has 25.
- 37 names are used for different retail tables. The worst is `??_7T@@6B@`: 185 gapscan destructor rows each declare a local `struct T`.

## Q2: `/DEFAULTLIB:"libcp"`
- **Source:** VC7.1's own `use_ansi.h` emits `#pragma comment(lib,"libcp")` when neither `_MT` nor `_DEBUG` is defined (the lane's /ML).
  - The chain: `<new>` → `<exception>` → `<xstddef>` → `yvals.h` → `use_ansi.h`. Its include guard is `_USE_ANSI_CPP`.
  - 12,440 of 57,160 row objects carry it.
- **Retail** imports MSVCR71 and MSVCP71 (3 `basic_string<char>` functions). So retail was built /MD and linked msvcprt; libcp and LIBC in lane objects are /ML artefacts.
- **Suppression, measured on a sample of 157 rows** (138 with libcp, 19 without), 0 NOT_EQ:
  - **Lane flag `/D_USE_ANSI_CPP`:** 138 of 138 are identical apart from `.drectve`, which is exactly the old text minus libcp. The 19 other rows are fully identical.
  - **Guard before `<new>` in `fable_containers.h`:** 117 of 138. The other 21 include `<new>` or `<map>` themselves, or were not scanned yet.
  - **The route scan** (2,406 of 12,440 rows) says the header guard covers about 94% of rows; the lane flag covers all of them.
  - `/MD` changes code in 44 of 157 rows, so it is not an option.

## Resume point
- Route scan: 10,024 rows (`work/route_todo.txt`, `tools/route461.py`).
- Guard proof: 166 rows (`tools/run_proof.sh`).
- Before reusing the numbers, rerun the drift check and `analyze461.py`.
- Not measured:
  - the 276 names with unknown retail size;
  - exact counts for the 473 short names whose retail table has no locator.

## Owner items
1. **Short vftables:** a real link would silently keep the wrong table for thousands of names. The fix route is each class's shared-header port (r459a).
2. **libcp:** a lane-wide `/D_USE_ANSI_CPP` (or the header guard) would remove it with no code change. This changes `.drectve` in 12,440 objects, so the owner decides.
