# 2026-11-12 landing dp -- chunk Q23: 2 parked rows matched

Source: `pending/2026-11-12_linux_handQ23/`. Landed (RELOCATION_MATCH, test PASS under VC7.1, strict headers): 0x006C1250
CTCBuyableHouse::FrameUpdate (the row landing dn held back, now on shared headers), 0x00814260 CTCBoastingPodium::OnInitialActivate.

Headers (9, three-way merged against cfbd396d3, all unchanged upstream; hdrcheck2 default=same for all nine, and no existing opt-in
changes): CThing.h HASTC_FORCEINLINE, CTCBuyableHouse.h, CTCVillage.h, CSubtitleRenderer.h PAUSE, CThingPhysical.h,
CPhysicsMeshObject.h NAVIGATION, CThingSearchTools.h (a `const Proc&` ForEachInRoughArea body), CTCBoastingPosition.h
SETBOASTINGPODIUM, NGraphicAppearance_CTCGraphicAppearance.h DUMMYOBJECTNAMES. Three `#ifdef` lines became `#if defined()` /
`#elif` chains with the other branches' text unchanged.

Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011dp): 0x008147D0 is CTCBoastingPosition::SetBoastingPodium (landed `_unattributed::AssignTarget_008147d0`). Also:
landed caller 0x0063E590 declares ForEachInRoughArea with `Proc&`, while every PDB instance is const with `const Proc&`.
Note: 0x006C1250 defines IsHouseBeingUsed / GetRentsDue / GetPriceForNextDressLevel out of line (VC7.1 inlines them); if
0x006BFB10 / 0x006BF810 / 0x006BF920 land as rows, keep the bodies identical.
