# NCameraModes::CProjectileWeapon::Update

The input source failed on the supplied includes. Restored the projectile owner, profile and ranged-attack dependencies and relabelled locals using the debug records. Bounds of lock-on, expression expansion in the two visibility tests, and float lifetimes differ. The original private static bool NCameraModes::CProjectileWeapon::InvertY is confirmed at 013BAF98 by Ego_d and FableWin PUB32/GDATA32 at 04AC5B41, plus the native conditional negation.

384 input combinations exercise zoom, axis inversion, sensitivity clamps, lock validity, bounding sphere and visibility branches.

Final native grade: DIFFER, 3409 compiled bytes, 3415 retail bytes. The source and fixture hashes are in RESULT.json and TEST.json.

## Latest verification

The selected body compiles to 3,430 versus 3,415 bytes. The original creature filter and InvertY are now identified. Eight mapped bodies match. The 384-case behavior test passes. Three extra 12-byte vector slots remain the leading frame difference.
