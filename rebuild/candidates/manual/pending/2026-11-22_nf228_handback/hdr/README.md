# NF228 header overlay (2026-11-22): two add-only, member-specific opt-ins (entry 3, row 0x0041E3F6)

Base copies: `orig/engine/` (the repo tree as of this run). Both opt-ins sit under new macros that no landed row or header defines
(grep over rebuild/), so every existing includer compiles the same tokens as before.

- engine/NUISystem_CUIScreenUnitConverter.h: `FABLE_RETAIL_CUISCREENUNITCONVERTER_SETISUSINGRELATIVECOORDS` declares
  `static void __fastcall SetIsUsingRelativeCoords(bool)` (public; Ego_r.pdb 0x00438D8A / FableWin.pdb 0x01AE527F
  ?SetIsUsingRelativeCoords@CUIScreenUnitConverter@NUISystem@@SIX_N@Z = the self leaf of landed 0x004299A8). Declaration only; inside the
  existing `public:` section after the RESETDIMENSIONS opt-in. pin_asserts: 0 pinned (the header has no static assert).
  Landed includers (36, direct): 00543ee0 0054ef00 0042ec7c 00622a00 004804de 00576c4f 00577216 00576ed1 00577ac0 00603000 0060eeb0 0060c900 00531ec0 005379f5 00537776 005ff190 00595a03 0052eaf0 0052ea30 0052ecc0 0052e470 0052e4d0 0041cf47 0041cc14 0041cc2b 0047d296 0047dd9d 0047e9ae 0047f30f 0047e196 0047ed76 0047e651 0040f0e0 0061b610 0061e6d0 0061a260 
- engine/CGameComponent.h: `FABLE_RETAIL_CGAMECOMPONENT_ISRUNNING` adds the inline `bool IsRunning() { return Running; }` in the public
  section (FableWin.pdb ?IsRunning@CGameComponent@@QAE_NXZ; retail expands it in 0x0041E3F6: `cmp byte ptr [eax+9],bl` at 0x0041E558 on
  the front-end component). No layout change. pin_asserts: 1 pinned (`FABLE_STATIC_ASSERT(sizeof(CGameComponent)==0x10)` moved from
  line 22 to 28 and is now `FABLE_STATIC_ASSERT_AT(22, ...)`, the same typedef name fable_static_assert_22).
  Landed direct includers (3): 00494900 004963a0 004175e5 ; plus the rows that reach it through engine/CNewFrontendGameComponent.h and the
  other game-component headers: none of them defines the macro, so the default path is token-identical apart from the pinned assert.
- User: rows/0041e3f6.cpp (it includes both headers itself, first: an opt-in in a header that another repo header includes needs the
  row's own include, NF213 lesson).
- assert_names_check.py orig/ v hdr/: mismatches 0 (every static-assert typedef name unchanged). IsRunning: no CGameComponent method list in struct_layouts_egor.tsv / fable_types.h (their only IsRunning is CStopWatch::m_bIsRunning), so it is one PDB (FableWin) + the retail body.
