# 2026-11-10 landing as -- Codex pass 18 return (38 of 40 rows)

Source: `pending/2026-11-10_codex_pass18/return_2026-11-10/` (START_HERE_CLAUDE.md, STATUS / LABELS / NAME_FIXES / LESSONS,
HEADER_DEFAULT_AUDIT.md). SHA256SUMS checked. vland --land: 2 MATCH (0x00836DE0 CThingTrackNode::FlipTrackDirection, 0x00CBE583
GetSquaredDistanceBetweenThings) + 36 RELOCATION_MATCH, tests PASS.

- Headers: Codex's 106-header overlay merged 3-way onto HEAD (base = 50d6c07a9, the package's include/): 33 new, 72 merged cleanly,
  no conflicts. engine/GFRandom.h was created the same day by chunk 1014 (no landed user) and by Codex: kept one body, Codex's statement
  order (the spelling that matched 0x008E7F80), with chunk 1014's evidence comment. hdrcheck2: default=same for all 106 (logs/header_merge.log).
- Flags carried from the rows: 0x0099BA70 CWideString::Sprintf `/GS /Zc:wchar_t /d2inlT200` (the PDB leaf is PB_W: native wchar_t);
  0x007E5570 and 0x00A0CDD0 `/Zc:wchar_t`.
- Anonymous-namespace comparators 0x00978790 / 0x0097ABF0: leaf matched by prefix up to `?A0x` (the TU hash is path-dependent).
- 19 manifest names respelled from the row leaves (mangled-name entries, missing namespaces, 0x0069E2E0 catalogued as SetupCameraMode,
  0x00BE1E20 catalogued as CTCCreatureFlyOnSpline -- it is the CEnginePrimitive2DViewportManager ctor).
- 0x009D5DF0 CBankFileAsyncData ctor uses the landed engine/CBankFileAsyncData.h (retail-volatile members, owner 2026-11-10).

Not landed:
- 0x00903F30 CScriptGameResourceObjectScriptedThing::FollowPreCalculatedRoute: parked 254/254, an EBX/EBP role exchange (differ/ + PROBES).
- 0x00BE2120: the assigned 97-byte extent ends inside a call; the body is 106 bytes, ending 0x00BE218A (extent/). Not authored.
Queued `land1011aq`: 87 Codex name fixes + 0x0099BB80 SprintfDynamic PB_W. Many are the CWideString core (ctor/dtor/operator=,
AllocStringData) spelled `PB_W` in the PDBs where landed rows use `PBG`: the string code was built with native wchar_t.
