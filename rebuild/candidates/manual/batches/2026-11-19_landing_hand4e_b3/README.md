# Landing hand4e b3 (2026-11-19): CSoundEmitterLH::GetActiveSampleInfo (0x00AAC5E0), CCamera::MoveSideways (0x00A0C1B0), CParsedItem::InitAsFloat (0x009B9770)

**Result:** strict vland --land gives RELOCATION_MATCH / MATCH / RELOCATION_MATCH + PASS; no flags.

**0x00AAC5E0** is CSoundEmitterLH::GetActiveSampleInfo, taking the shared CASoundEmitter.h slot-24 name; the catalog's GetActiveSoundInfo goes to NAME_FIXES.
- New opt-in FABLE_RETAIL_CACTIVESOUNDINFO_LH_CTOR in engine/CActiveSoundInfo.h: a 3-argument constructor, which also matches retail's own copy at 0x00AAB810.
  - The third word is float StartTime, proven by the `fstp dword` of GFGetTime at the caller of the CActiveSound ctor 0x00AAB910.
  - The header's default assert is pinned at its original line, `FABLE_STATIC_ASSERT_AT(14, ...)`. The opt-in branch's assert uses a distinct name, `_AT(1014, ...)`.

**0x00A0C1B0** uses the existing C3DVECTOR_ORIGINAL_GROUPED_CROSS_PASS22 opt-in (`// Bracket:` line).

**0x009B9770** follows FableWin's order with Ego_d locals and retail literal types; no header change.

**Header-user recheck:** 72 / 72 good, run together with hand3c b18. The NThingMap_CThingMap.h / CStringParser.h overlay copies were identical to the repo.
