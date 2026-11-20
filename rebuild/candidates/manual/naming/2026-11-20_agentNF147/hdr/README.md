# NF147 header overlay (2026-11-20)

Base copies in ../orig/ (copied from the repo working tree at the start of this pass). The other two agents' overlays were read first:
NF145 overlays CCreatureAction_ThrowShotObject.h / CDisplayManager.h / CMemoryAllocator.h; NF146 has none. No header overlaps.

## engine/NCreatureMode_CModeBriarRoseConcentrate.h
- Copies rebuild/include/engine/NCreatureMode_CModeBriarRoseConcentrate.h.
- Adds one `#ifdef` opt-in right after `public:` in the class: `FABLE_RETAIL_NCREATUREMODE_CMODEBRIARROSECONCENTRATE_CTOR` declares
  `CModeBriarRoseConcentrate(CThing& thing, CThingCreatureBase& target, bool imposter);` = Ego_r.pdb 0x00830DF0 / FableWin.pdb 0x0280A420
  `??0CModeBriarRoseConcentrate@NCreatureMode@@QAE@AAVCThing@@AAVCThingCreatureBase@@_N@Z` (retail 0x008E78C0, out of line). Declaration only;
  no layout change. User: row 0x008C1C30.
- Default path (macro undefined) is token-identical to the repo header apart from the pinned assert line.
- pin_asserts.py: pinned 1 (`FABLE_STATIC_ASSERT(sizeof(NCreatureMode::CModeBriarRoseConcentrate)==164)` -> `FABLE_STATIC_ASSERT_AT(35, ...)`,
  the repo header's line 35).
- Landed includers (grep of rebuild/src/compiled + rebuild/include): 0x008E7F80, 0x008E7E70, 0x00741FC0, 0x00741FB0, 0x0073B130. None
  defines the new macro, so the opt-in changes none of them.

## engine/NCreatureMode_CModeDrained.h
- Copies rebuild/include/engine/NCreatureMode_CModeDrained.h.
- Adds one `#ifdef` opt-in right after `public:` in the class: `FABLE_RETAIL_NCREATUREMODE_CMODEDRAINED_CTOR` declares
  `CModeDrained(CThing& thing, CThing& drainer);` = Ego_r.pdb 0x0083E1B0 / FableWin.pdb 0x0281FB70 `??0CModeDrained@NCreatureMode@@QAE@AAVCThing@@0@Z`
  (retail 0x00868220, out of line). Declaration only; no layout change. User: row 0x008676A0.
- Default path token-identical to the repo header apart from the pinned assert line.
- pin_asserts.py: pinned 1 (`FABLE_STATIC_ASSERT(sizeof(NCreatureMode::CModeDrained)==128)` -> `FABLE_STATIC_ASSERT_AT(27, ...)`, the repo line 27).
- Landed includers: 0x00741460, 0x00741470, 0x008DA7A0, 0x0073B130. None defines the new macro, so the opt-in changes none of them.

## engine/CAnimFilterOfGroup.h
- Copies rebuild/include/engine/CAnimFilterOfGroup.h.
- Adds one add-only opt-in after Clone(): `#if defined(FABLE_RETAIL_CANIMFILTEROFGROUP_CTOR) && !defined(FABLE_RETAIL_CANIMFILTEROFGROUP_INLINE_CTOR)`
  declares `CAnimFilterOfGroup(const CCharString& group);` = FableWin.pdb 0x01DD33C0 `??0CAnimFilterOfGroup@@QAE@ABVCCharString@@@Z` (retail
  0x0073E8F0 = its landed self row; landed 0x0073E9A0 binds this spelling). Declaration only; no layout change. User: row 0x008A1A10.
- Default path token-identical to the repo header (the assert is already FABLE_STATIC_ASSERT_AT(33, ...); pin_asserts.py: pinned 0).
- Landed includers: 86 files under rebuild/src/compiled; files defining FABLE_RETAIL_CANIMFILTEROFGROUP_CTOR before this pass: 0. The opt-in
  changes none of them.
