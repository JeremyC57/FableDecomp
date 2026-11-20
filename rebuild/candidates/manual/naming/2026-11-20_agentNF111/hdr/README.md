# NF111 header overlay

Two headers, one `#ifdef` opt-in each (declarations only, no layout change). Base copies in `orig/engine/` are byte-equal to the current repo
headers (checked at hand-back, repo at the nf108 landing + Account B merge). No other agent's overlay (NF108, NF110) touches these two headers.

| header | opt-in | added | used by |
|---|---|---|---|
| engine/CThingManager.h | `FABLE_RETAIL_CTHINGMANAGER_REMOVEFROMPHYSICALLIST` | after `void InitThingTypes();` (public): `void RemoveFromPhysicalList(class CThingPhysical& thing); // 0x0051DFE0` (Ego_r 0x00527F00 / FableWin 0x01D40AC0) | rows/0051dfe0.cpp |
| engine/CThingPhysical.h | `FABLE_RETAIL_CTHINGPHYSICAL_HEALTHOVERTIMEHELPER` | after the bit-field block's `public:`: the Ego_r.pdb nested `class CHealthOverTimeHelper { public: long FramesLeftForAddingHealth; float HealthLeftToAdd; };` (sizeof 8) | rows/005283c0.cpp |

- Default path token-identical: with the two blocks removed, each overlay equals its orig/ copy (checked by script).
- pin_asserts.py: `engine/CThingManager.h pinned 0`, `engine/CThingPhysical.h pinned 0`: every assert below the insertions is already a
  pinned `FABLE_STATIC_ASSERT_AT(<old line>, ...)` (CThingManager.h line 268 `_AT(216, ...)`; CThingPhysical.h all `_AT`), so no assert
  name moved.
- Header users: 66 landed rows include engine/CThingManager.h and 75 include engine/CThingPhysical.h; no landed row or test defines either new
  macro (grep), so the opt-ins change none of them.
- Not applied (needs header claim): engine/NUISystem_CText.h declares `virtual const CWideString& GetValue() const;` (slot 142) while
  FableWin.pdb names the non-const `?GetValue@CText@NUISystem@@UAEABVCWideString@@XZ`; the CText rows here use engine/CText.h, which has the
  PDB spelling. See NOTES.md.
