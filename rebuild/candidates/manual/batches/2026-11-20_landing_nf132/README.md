# 2026-11-20 landing nf132: PRIORITY slice 10 remainder (26 rows)

Records: `naming/2026-11-20_agentNF132/` (selfcheck 0 problems). 10 entries: 2 done (q4019 Uninitialise family, q4055), 5 blocked with rows
left (q4100, q6186, q6233, q6375, q6379), 3 open (q4283, q4285, q4458; NF129's census stands).

## Rows (26): 26 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- Uninitialise family (q4019): 0x0099A360 CInitBaseClass::Uninitialise (protected), 0x00412F60 CGame, 0x004AE990 CNetworkClient,
  0x00487AF0 CPlayer, 0x009A60D0 CSystemManager, 0x00A6C120 CSystemFont, 0x00A0A9D0 CRenderManagerCore::Uninit, 0x00B25D80 CEngine and their
  callers (0x004166A8, 0x004175E5, 0x00A0B590, 0x00B260B0 ...).
- 0x004175E5 CMainGameComponent::Shutdown: its local CMainGameComponent model now derives from engine/CGameComponent.h (keeps the
  CNetworkClient member at retail offsets). Accepted: the defined-symbol diff against the HEAD object is empty.
- CSubTitle list: 0x00A3BC20 CVideoSys::~CVideoSys and 0x00A3BDE0 CMovieSubtitles::~CMovieSubtitles bind 0x009BE060 under the claim.
- Script interface: 0x0088FEE0, 0x00890010, 0x00895A10, 0x00898B30, 0x00898DB0, 0x0089FBB0, 0x0089FC80; 0x00859040, 0x008FD0A0.
- STL owners: 0x009A7990 CBankFileManager::Close, 0x009AA7C0, 0x009AC310.

## Coordinator fix at landing: scaffolding helpers
0x00A3BDE0 and 0x009AC310 forced their implicit destructors out of line through global helpers with invented names
(`DestroyCMovieSubtitles`, `DestroyMapOfCCharStringCCharString`), i.e. strong external symbols retail does not have. Both are now wrapped in
an anonymous namespace and marked `// Fidelity: scaffolding, not retail` (GOTCHAS nf86 / nf88); the symbols are TU-hashed
(`?A0x...`). Regraded: both RELOCATION_MATCH + PASS.

## Headers (add-only opt-ins; the only lines dropped are FABLE_STATIC_ASSERT lines pinned by pin_asserts)
CInitBaseClass.h FABLE_RETAIL_CINITBASECLASS_VALIDATE / _UNINITIALISE; CGame.h FABLE_RETAIL_CGAME_UNINITIALISE; CNetworkClient.h
FABLE_RETAIL_CNETWORKCLIENT_UNINITIALISE / _INITIALISEASLOCAL; CMovieSubtitles.h FABLE_RETAIL_CSUBTITLE_STRUCT (`struct CSubTitle;`);
CEnvironment.h FABLE_RETAIL_CENVIRONMENT_SETTHEMEWEIGHTALLCHANNELS. No landed row defines any of them.

## Checks
- 474 rows rechecked, 474 good; gate 0 created splits; bindings_multi_check 1.
- Table: 0x004115A0 3 -> 4 is witness only (row 0x00859040, p_before / p_land SAME). 0x006B3680 0 -> 1: row 0x00890010 bound the unrecorded
  stand-in `?Apply@Inner@@QAEXHHH@Z` at HEAD and now binds the PDB `?SetThemeWeightAllChannels@CEnvironment@@QAEXJJM@Z`: still one spelling.
  Shrank: 0x0099A360 4 -> 1, 0x0099A310 2 -> 1, 0x006B38D0 2 -> 1, 0x009AD410 12 -> 5, 0x0044C6B0 5 -> 3, 0x009AA7C0 3 -> 2,
  0x00A6C120 3 -> 1, 0x009BE060 3 -> 1, 0x004AE990 2 -> 1.
- One-definition (`logs/symbol_diff.txt`): new symbols are own leaves, shared-header inlines and STLport template COMDATs, plus the two
  helpers above (now internal). Link test over the 26 objects (`LINK=` cleared): no LNK2005.

## Coordinator answers (advisor, owner delegated)
- (a) No blanket adjacency rule for the 6 unreferenced byte-identical STL pairs. Per pair: compare resolved call / data targets first; take
  order as evidence only when both PDBs agree on it and both retail neighbours already carry two-source names. Otherwise they stay blocked.
- (b) CSubTitle stays an opt-in; flipping the default to `struct` is a header claim (needs `U` in both PDBs and a regrade of every landed
  includer that mangles CSubTitle). Queued.
- (c) 0x004175E5 accepted (empty symbol diff).
