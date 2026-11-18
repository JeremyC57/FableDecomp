# Naming evidence

- Ego_r 0x006BEF20 and FableWin 0x022038E0 give the exact leaf `?IsVolumeInViewingFrustrum@CGameCameraManager@@UBE_NABVC3DVector@@M@Z`.
- Native derives two axes around a camera ray and makes four boundary queries. This corroborates Ego_d cam_pos, perp, one_line_in, other_perp, fov, other_line_in, line and cam_look_at.
- The retail CCamera getter call is 0x00A0BEB0 for GetFOV, and the four CTools calls go to 0x00875120. These identities agree with the PDBs and the sampled camera/sphere calls.

## Behavioral fixture

The fixture calls the selected frustum decision with synthetic visibility results for its sampled points. Camera and line visibility are supplied by fixture objects.
