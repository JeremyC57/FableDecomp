# NF165 header overlay

## engine/CEAXListener.h (copy of rebuild/include/engine/CEAXListener.h; base in orig/engine/)
- Added (add-only, default path token-identical): `#ifdef FABLE_RETAIL_CEAXLISTENER_REMOVESOURCE` declaring
  `long RemoveSource(CEAXBuffer*);` (0x00C5FF60, the landed self row's leaf) right after `SetEAX4ErrorCode`, in the public block.
  Used by rows/00c44170.cpp. The name is the landed descriptive one (one address, one name); GameCODA calls it
  `DeleteBuffer` (NAME_FIXES.tsv; renaming needs a header claim).
- pin_asserts.py: pinned 0 (the header has no FABLE_STATIC_ASSERT).
- Landed includers (grep rebuild/src/compiled): 00c5ffb0, 00c5ff50, 00c68f90, 00c66330, 00c66c90. None defines
  FABLE_RETAIL_CEAXLISTENER_REMOVESOURCE, so the opt-in changes none of them.

## Unchanged dependency copies (byte-identical to rebuild/include/engine; bases in orig/engine/)
- engine/EAXNativeTypes.h, engine/EAX40.h, engine/DSound3D.h, engine/CCodaObjectBase.h, engine/CodaMemory.h.
- Reason: CEAXListener.h includes `"EAXNativeTypes.h"` / `"CCodaObjectBase.h"` relative to its own directory, so an overlay
  copy of it only preprocesses when its relative includes sit beside it. No opt-ins, no edits; the coordinator can drop these
  copies when merging (the repo files are identical).

## libpk/CPKVoiceParams.h (copy of rebuild/include/libpk/CPKVoiceParams.h; base in orig/libpk/)
- Added (add-only; default path token-identical): inside the existing `#else` (non-POSITION) layout branch, a nested
  `#ifdef FABLE_RETAIL_CPKVOICEPARAMS_TRACKBITS` that splits `UnrecoveredState[0x50]` into `UnrecoveredState[0x44]`,
  `unsigned char TrackFlags` (+0x44) and `UnrecoveredState45[0x0b]` (same 0x50 bytes) and declares `bool GetTrackBit0/1/2() const`
  (0x00C27B70 / 0x00C27B80 / 0x00C27B90, descriptive). Without the macro the `#else` branch is the original line. Used by
  rows/00c27b70.cpp, 00c27b80.cpp, 00c27b90.cpp and their tests.
- pin_asserts.py: pinned 2 (the two existing POSITION_PASS23 asserts, `FABLE_STATIC_ASSERT_AT(25, ...)` / `(26, ...)`, kept on
  their original line numbers after the inserted block moved them down).
- Landed includers: no row in rebuild/src/compiled includes it directly; it reaches rows through libpk/CPooledResource.h and
  libpk/CSoundEventManager.h. No landed row defines FABLE_RETAIL_CPKVOICEPARAMS_TRACKBITS (nor FABLE_RETAIL_CPKVOICEPARAMS_POSITION_PASS23:
  grep of rebuild/src/compiled finds 0 rows), so the opt-in changes none of them. The POSITION_PASS23 layout lines are unchanged;
  only its two static asserts were rewritten by pin_asserts.py to FABLE_STATIC_ASSERT_AT(25/26, ...) (same conditions, original lines).
- Sibling overlays read first: NF163 / NF164 overlay only engine/ quest-script and creature-action headers; neither touches
  CEAXListener.h or CPKVoiceParams.h.
