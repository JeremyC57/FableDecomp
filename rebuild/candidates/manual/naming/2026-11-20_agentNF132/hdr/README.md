# NF132 header overlay

Every header here is a copy of the repo working-tree header (base copy in ../orig/, checked byte-equal to the repo at hand-back) with
`#ifdef` opt-ins added. Without the new macros each header is token-identical to its base; the only other difference is
pin_asserts.py's `FABLE_STATIC_ASSERT` -> `FABLE_STATIC_ASSERT_AT(<line>, ...)` pins (same assertion).

| Header | Opt-ins added (all add-only, member-specific) | Used by rows | pin_asserts.py |
|---|---|---|---|
| engine/CInitBaseClass.h | `FABLE_RETAIL_CINITBASECLASS_VALIDATE`: `virtual void Validate() const;` (slot 1, 0x0099A340); `FABLE_RETAIL_CINITBASECLASS_UNINITIALISE`: `protected: void Uninitialise();` (0x0099A360, IAE) then `public:` | VALIDATE: 0x00B260B0; UNINITIALISE: 0x0099A360, 0x00412F60, 0x004AE990, 0x00A6C120 | pinned 0 |
| engine/CGame.h | `FABLE_RETAIL_CGAME_UNINITIALISE`: `void Uninitialise();` (0x00412F60), inside the PASS23 class | 0x00412F60 | pinned 1 (the sizeof assert, line 17) |
| engine/CNetworkClient.h | `FABLE_RETAIL_CNETWORKCLIENT_UNINITIALISE`: `void Uninitialise();` (0x004AE990); `FABLE_RETAIL_CNETWORKCLIENT_INITIALISEASLOCAL`: `bool InitialiseAsLocal(CMainGameComponent*);` (0x004AE940); both in the public part of the UPDATE retail class | UNINITIALISE: 0x004AE990, 0x004166A8, 0x004175E5; INITIALISEASLOCAL: 0x004166A8 | pinned 0 |
| engine/CMovieSubtitles.h | `FABLE_RETAIL_CSUBTITLE_STRUCT`: `struct CSubTitle;` in place of `class CSubTitle;` (both PDBs key it U) | 0x00A3BC20, 0x00A3BDE0 | pinned 0 |
| engine/CEnvironment.h | `FABLE_RETAIL_CENVIRONMENT_SETTHEMEWEIGHTALLCHANNELS`: `void SetThemeWeightAllChannels(long, long, float);` (0x006B3680), next to the LIGHTINGFLASH block | 0x00890010 | pinned 30 |

Landed includers (grep of rebuild/src/compiled): CInitBaseClass.h 5, CGame.h 1, CNetworkClient.h 4, CMovieSubtitles.h 1 direct
(+17 through engine/CVideoSys.h), CEnvironment.h 67. No landed row, test or shared header defines any macro added here (grep of
rebuild/src, rebuild/tests, rebuild/include), so the opt-ins change none of them. Every FABLE_RETAIL_* used in ../rows resolves in
rebuild/include or this overlay (checked). The sibling agents NF131 / NF133 overlay none of these headers.
