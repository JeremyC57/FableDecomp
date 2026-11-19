# Landing hand4a b4 (2026-11-19): NCameraModes::CRotateAround::Update (0x0087BD70) -- fully sourced

vland --land, strict headers: RELOCATION_MATCH + PASS; no fitting, no brackets; matched at the first compiled grade. The parked draft was a
local model (its "RotationSpeed +0x90" is CBase::RightStickVector.X, "IgnoreWorldHeightLimits +0xA0" is ViewOccluded); re-written from the
FableWin debug body 0x029C7B70 with Ego_d locals (cam_stick / cam_stick_x, foc_pos, thing_base, d_thet, line_out, new_pos).
New opt-ins (`hdr_README.md`): NCameraModes_CBase.h SETPOSITION_INLINE / VIEWOCCLUDED_INLINE, CGameCameraManager.h WORLDHEIGHT_GETTERS; NEW
engine/NCameraModes_CRotateAround.h (Ego_r members at retail offsets, size 0xC8). Queued: landed CRotateAround rows 0x0087BD40 / 0x0087BD50 /
0x0087BD60 / 0x0087BF40 declare the class locally (re-port onto the new header).
