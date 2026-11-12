# 2026-11-12 landing fg -- name-fix pass NF31: 38 queue entries, 46 landed rows changed, 29 renamed

Source: `naming/2026-11-12_agentNF31/` (STATUS, APPLIED, REJECTED, RENAMES, NOTES, LESSONS, QUEUE, hdr README). Run as the workflow
`lane_tools/workflows/name_fix_queue_pass.js`, all Opus: apply, adversarial verify, one repair, verify again (ok: notes only).

**Queue (tags land1011eu .. fd):** 25 done, 12 partial, 1 rejected (0x01436E9C GPLightingManager: the data name appears only in
the Anniversary Test PDB, which may only confirm a name another PDB has). One queued name was wrong and is corrected: 0x013B8760
is `CStartupDefinitionManager::PClass` (FableWin.pdb names its dynamic initializer and atexit destructor), not a descriptive global.
**Rows:** 46 landed rows changed: 29 renames of the row's own name (RENAMES_applied.tsv section 1) and 28 caller respellings
(section 2). Examples: 0x007324A0 was landed as a CEnginePrimitiveLine constructor and is the CEnginePrimitiveShadowedSpotLight
one; 0x0099ED10 ToLower -> GetAsLowerCase with ten binders; 0x004C4160 -> NGraphicAppearance::CPrimitive::~CPrimitive; 0x0094FDF0
IsStillRunnable is private. Five STLport instances (0x00883860, 0x00493BC2, 0x00493BDD, 0x005F8150, 0x007C2980) take their STLport
spelling: retail is STLport while both PDBs are Dinkumware builds, so these are descriptive-by-library, backed by body shape and
the retail call graph. All 46 rechecked in this landing: MATCH or RELOCATION_MATCH, every test PASS (`logs/recheck.log`). The
0x004C7580 test was rewritten without inline asm and now links the row.
**Verifier findings repaired before landing:** a missed landed binder of `CManagedTexture::operator=` (0x009FE550 still bound the
non-const-return spelling); three RENAMES section-1 lines had `Class::Method` in the name column and shortened template arguments.
**Headers:** CManagedTextureData.h (RELEASETEXTURE), CStartupDefinitionManager.h (PCLASS; the stale "no PDB data name" comment gets
a correction line), CThingManager.h (GETWORLD). Defaults unchanged per hdrcheck2.
**Bindings:** rebuilt by rebind.py for the 46 rows (45 removed, 38 added); bindings_multi_check at baseline;
funclet_includes_check 1317 checked, 0 broken.
**Carried over (queue tag land1011fg):** the 12 partial entries, marked AUTHORING / PORT / CARRY (callers built on stand-in classes
that must first move onto shared headers; three need a new byte match). They do not trigger a naming pass by themselves. Four new
follow-ups found by the verifier are queued under the same tag as ordinary entries: 0x00493911, 0x007044B0, 0x00875FB0, 0x006924B0.
