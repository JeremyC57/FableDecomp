# Landing nf183 (Account A, 2026-11-20): global data and free-function names (21 rows)

Agent NF183 (round 10) took the globals and free-function entries. It handed back early (context) with 52 entries: 9 done,
8 done-already, 13 rejected, 5 blocked, 1 deferred and 16 open (not reached). Its files are in `naming/2026-11-20_agentNF183/`:
`TRIAGE.md`, the probe census in `work/probe/`, the held 0x0054ED90 draft in `held/`, and the DELTA_NOTIONAL_ZERO literal list in
`work/`. The open entries go back to the queue unclaimed.

- 21 rows, all RELOCATION_MATCH, every test PASS. Every new spelling is a PDB name (Ego_d publics, Ego_r, FableWin); none is descriptive.
  - Globals: GVersion 0x013B85E4 (0x00416268 / 0x0041627F), _GetReadingString 0x0143E684 (0x00CB28F0), GD3DFormats 0x0129BA40
    (0x00B2C4E0; the old 0x0129BA4C binding was its AlphaBits field at +0xC), gApplicationLanguage 0x0138E060 (0x0099D470 /
    0x0099D520 / 0x0099E260), pConfig 0x013BD6E8 (0x004017B0), BUILDING_BOUNDRY 0x013BAC98 (0x012297F0, with ~CCharString), the
    CEngineLightingManager LOCAL_HEAP_* heaps (0x00B49AA0 / 0x00B49A40 / 0x00B49FE0 / 0x00B47980), GFastOneOverSqrtTable 0x013CE558
    (0x0092B800).
  - Functions: GFUHUGEToCharString 0x0099F930 (six callers that bound a CCharString(_K) ctor stand-in), GFGetSystemManager 0x009A4EC0
    (0x00AB4700).
  - Tests 0x004017B0, 0x012297F0 and 0x00AB4700 are rewritten onto the shared headers.
- Header: engine/CDynamicObjectHeap.h (`orig` equals HEAD) gets FABLE_RETAIL_CDYNAMICOBJECTHEAP_FREE_INLINE, an inline Free body.
  The default path is token-identical; pin_asserts pinned 0. Its one landed includer, 0x00B47FD0, is unchanged.
- New external data definitions (coordinator check): 0x00CB28F0 now defines `_GetReadingString` (PDB `3` global spelling; before, a
  file static), 0x0099E260 defines gApplicationLanguage, 0x004017B0 defines pConfig. A tree grep finds no second definer of any of them;
  every other user declares them extern. pickany_check lists the three as NO-OWNER-OBJ (data addresses, self definitions).
- 0x00416268 / 0x0041627F: their callees 0x009F4CF0 / 0x009F5E10 move from `?Begin@CLoader@@QAEXPBDH@Z` / `?Begin_009f5e10@...` to the same
  stand-ins with the PDB `const CVersion&` first parameter. The self rows define the PDB `CInputManager::StreamInputsToFile` /
  `StreamInputsFromFile`, not the replaced spelling. So these two callers keep a split that existed at HEAD, with the same count, and
  porting them onto the CInputManager spelling is queued.
- Held: 0x0054ED90 (q4288, CFontManager::GetFont). The port grades RELOCATION_MATCH. But it would newly define
  `??4?$CCountedPointer@VCFontBank@@@@QAEAAV0@ABV0@@Z` at 72 bytes, against the owner 0x00419463's 23. Retail inlines the operator=
  here, so a declaration-only form cannot match. Landed twin 0x00541FD0 already defines the same 72-byte copy, but HEAD copies are debt,
  not licence (advisor rule), so it stays held; the draft is in `naming/.../held/` and both items are queued.
- Rejected with replacements, queued (no landed row binds them yet, each family moves together): q4422 -> Ego_d GPRenderManager, q4573 /
  q4578 -> NEngineInternal::GPWeatherRenderer / GPSurfaceCompositionManager. Header items queued: q6513 NullString access (one source),
  candidates/object_family_compiled.h's GFROR13 return type (q4996), CSystemManager +0x7C (blocks 0x004022B0). Also queued: the
  labels_landing_2026-11-11cl.tsv g_NormaliseEpsilon label that overrides DELTA_NOTIONAL_ZERO for 0x0129BA3C.
- Recheck (`logs/recheck.log`): 21 RELOCATION_MATCH. rebind multi-check 1 (baseline); split gate 0 created. Referencing rows: 274 good,
  1 bad, 0x009DF060 DIFFER(1736v1736) TCC_FAIL. It includes only fable_compiler.h and fable_state_block_sold.h, which nf183 does not
  touch, and it was bad the same way in nf142. Owner recheck good.
- tablecheck (`logs/tablecheck.txt`): eight addresses shrink (0x01436ED0 3 -> 1, 0x0138E060 3 -> 1, 0x01436FB0 3 -> 1, 0x0129BA4C 1 -> 0,
  ...), none grows. No out-of-image oracle line was added (the older 0x012165B8 one is queued).
- Link (`logs/link_landing.log`): 3 LNK2005, the same symbol set as HEAD (`logs/link_control_head.log`).
