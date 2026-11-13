# 2026-11-13 landing fs -- 0x00839100 CThingGameObject::GetWeaponTrailGraphicIndex (wf20, after repair)

Source: `pending/2026-11-12_linux_handWF20/`. The row matched in workflow wf20 on the real STLport map types, with one member read
through a `long[2]` stand-in. An Opus repair (`lane_tools/workflows/repair_and_verify_row.js`) retyped
CWeaponDef::FlourishSpecialWeaponTrail as the Ego_r struct CWeaponTrailGraphicSet {Attack +0, Knockdown +4} under the existing
FABLE_RETAIL_CWEAPONDEF_WEAPONTRAILS opt-in (default path keeps `long[2]`); the row reads `.Knockdown` for a special strike and
`.Attack` otherwise (retail reads [edi+0x80] / [edi+0x7c]). First grade matched.
Both verifiers confirmed bytes, test and member order (the mechanical one also ran the landing verifier as a dry run: no jump-table
or identity line). Two findings fixed by the coordinator and regraded: a static assert added outside any new `#ifdef` (it changed
the token stream of an existing opt-in path with 25 users) now sits inside the opt-in; the name-fix row for 0x0083AEA0 lists the
second wrong landed spelling.
RELOCATION_MATCH, test PASS, LANDVERIFY_STRICT_HEADERS=1. Headers (additions under opt-ins with no other user; defaults and every
existing path unchanged per hdrcheck2): CWeaponDef.h, stlport/fable_map.h (MAP_REVERSE, TREE_DEC_OUTOFLINE), CThingGameObject.h,
CTCBerserk.h, CTCCombat.h. Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Queued (land1011fs): 0x0078BED0, 0x0078BEE0, 0x00787C18, 0x0083AEA0, 0x0083B170.
