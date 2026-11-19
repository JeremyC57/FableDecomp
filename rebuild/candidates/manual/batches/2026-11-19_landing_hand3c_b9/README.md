# Landing hand3c b9 (2026-11-19): CMiniMapDisplay::GetDrawPosition (0x0082A020)

vland --land, strict headers: RELOCATION_MATCH + PASS; 40 grades. FableWin 0x027D3990 shape with Ego_d locals. GetRegionMapExtents stays out of line
(retail calls 0x0066C6A0; CRegionMinimap.h EXTENTS_NOINLINE); GetRelativeMapSize has a visible noinline body from FableWin 0x027D3850 (fixes renderPos.Y's
frame home); five natural brackets (`// Bracket:`, owner rule) move c2's 83-node region cut between the two final filds; 256.0f is a float (retail
0x0125B240). Opt-ins: CMiniMapDisplay.h GETDRAWPOSITION (+0x54 typed CRegionMinimap, inline GetRelativePosOnMap, declarations) with nested
RELATIVEMAPSIZE_BODY; CRegionMinimap.h EXTENTS_NOINLINE. All 29 landed users of the two headers rechecked. Names: 0x013B83B8 / 0x013B83BC are
GDummyInt1 / GDummyInt2 (FableWin publics). Queued: re-port landed 0x00829C70 GetRelativeMapSize (local model) onto RELATIVEMAPSIZE_BODY.
