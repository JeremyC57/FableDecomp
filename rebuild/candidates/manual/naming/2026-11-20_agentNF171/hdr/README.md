# NF171 header overlay (2026-11-20)

Base copies of every overlaid header are in `../orig/engine/` (byte-equal to the repo working tree when copied, checked).
Each overlay adds ONE `#ifdef` opt-in block with a member-specific macro; nothing else changes. Default path and every existing
opt-in path are token-identical to the repo header (cpp of orig vs hdr with the retail macro and with no macro: identical md5).
NF169 / NF170 overlays do not touch these three headers (their hdr/ holds other files).

| header | opt-in | declares | used by |
|---|---|---|---|
| engine/CMiniMapDisplay.h | FABLE_RETAIL_CMINIMAPDISPLAY_INITIALISE | `void Initialise(float);` 0x0082A310 (Ego_r 0x00732DE0 + FableWin 0x027CF940 ?Initialise@CMiniMapDisplay@@QAEXM@Z) | rows/0082a310.cpp |
| engine/CGamePlayerInterface.h | FABLE_RETAIL_CGAMEPLAYERINTERFACE_ISOTHERQUICKACCESSITEMBUTTONHELD | `bool IsOtherQuickAccessItemButtonHeld(long, long) const;` 0x00446060 (retail path; Ego_r 0x00442FD0 + FableWin 0x01A9B210) | rows/00687c90.cpp, rows/00687c90_test.cpp |
| engine/CScriptGameResourceObjectScriptedThing.h | FABLE_RETAIL_SCRIPTEDTHING_CLEARALLACTIONS | `virtual void ClearAllActions();` / `virtual void ClearAllActionsIncludingLoopingAnimations();` 0x00903920 / 0x00903970 (Ego_r 0x0092A230 / 0x0092A280 + FableWin 0x02B4C080 / 0x02B4C100; vtable 0x0128D86C slots 21 / 22) | rows/00903920.cpp, rows/00903970.cpp |

Declarations only; no layout change; no FABLE_FORCEINLINE.

pin_asserts.py: CMiniMapDisplay.h pinned 0 (already `_AT`); CGamePlayerInterface.h pinned 0; CScriptGameResourceObjectScriptedThing.h
pinned 8 (its plain `FABLE_STATIC_ASSERT` lines at repo lines 44, 72..78 now `FABLE_STATIC_ASSERT_AT(<repo line>, ...)`).

Landed includers (grep of rebuild/src/compiled / rebuild/tests; the full lists are in hdr/INCLUDERS.txt): CMiniMapDisplay.h 19 / 1, CGamePlayerInterface.h 57 / 5,
CScriptGameResourceObjectScriptedThing.h 31 / 4. None defines the new macros, so the opt-ins change none of them (and the pinned
assert names equal the repo's line numbers).
