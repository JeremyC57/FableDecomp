# NF441 hand-back (2026-11-25): libpk / GameCODA best-guess names. Research only, nothing landed

`NOTES.md` has the method, and `RESUME.md` has the resume state. The agent's `work/` folder (retail vftable dumps and SDK extracts) stays in scratch because it holds retail and SDK bytes.

## Why nothing landed
`rebuild/include/engine/` already has about 65 GameCODA headers (CSalRender.h, CSalSw3DVoice.h, CCodaSource.h, CSalOutputKS.h, ...). So renaming a row's local `CVtable_<va>` model to one of those classes makes it SHARED_HEADER_AVAILABLE, and the lander rejects that. No header-free respell exists for this family. The route is a header port, starting with a `_hdr` batch.

## What the 1,000 addresses are (`T3.txt`)
- **216 GameCODA:**
  - 211 by a destructor chain to CCodaObjectBase 0x012B5868 / CodaFree;
  - 5 by an SDK `??_7` bound there, or by a GameCODA chain that stores the vftable.
- **784 Lionhead libpk / audio:** retail has `libpk\PKC*.h` and `.\PKC*.cpp` strings. The SDK does not cover this code, so these classes can be named only from such strings.
- **Version:** retail GameCODA predates SDK 2.10. The slot counts differ (CCodaObject has 1 slot against 11), so a method is tied by its body (an exact instruction count), never by slot position.

## Class ties (`CLASS_TABLE.tsv`, 43 vftables plus 1 function)
- **40 from the SDK:**
  - 8 were already named;
  - 32 are new: 11 render classes, the 4 core classes, and 17 one-source candidates including CCodaObject.
- **1 in SDK style:** `CSalRenderHrtfSSE2` at 0x012BEDE0, a variant that SDK 2.10 does not have.
- **2 held:** 0x012B8CC8 and 0x012BEE40.
- **Two sources:** retail 0x00C55270 is `CSalSw3DVoice::CreateRenderer`. The SDK's order of render constructor relocations matches the order of retail's calls, and that ties the 12 render classes.

## Queued
- **`NAME_FIXES_QUEUE.tsv` (7 nf441 lines):** apply them only together with the port. A caller-only respell would split each address against its current self.
  - Under the tracker, the SIMD render constructors show split_fixable on `??0C3DAnimationUtil@@QAE@XZ`. Caller-respell agents must not move binders onto that name.
- **`T3_PORT_QUEUE.tsv` (nf441 family line), the port route:**
  1. add a member opt-in to CSalRender.h (a `_hdr` batch);
  2. pilot ctor 0x00C6E6A0 and dtor 0x00C6E630, using the landed 0x00C5ECA0 / 0x00C5ED90 tests as the precedent;
  3. add the derived render headers;
  4. hold any row whose code changes under the SDK signatures.
