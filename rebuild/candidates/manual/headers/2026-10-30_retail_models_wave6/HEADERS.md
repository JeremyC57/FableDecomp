# Wave 6 headers: what changed and the evidence

No new class headers in wave 6. Two existing headers gained non-virtual member declarations (layout, vtables and every
existing slot untouched). The NScript bulk rows moved onto the wave 1 headers `engine/CScriptBase.h` /
`engine/CLevelScriptBase.h` unchanged.

| header | change | evidence | includers checked |
|---|---|---|---|
| engine/CGameScriptInterface.h (FABLE_RETAIL_NSCRIPT) | `long GetScriptSpriteBySymbol(const CCharString&) const;` 0x006E77F0, public | 3 sources, below | 80 random unchanged includers mcheck `--all` SAME (logs/mc_cgsi_hdr_sample.txt) |
| engine/CPlayerManager.h (generated, flattened) | `// Retail override` mark + 5 PDB methods: GetPlayer(long) / const 0x004498C0 / 0x00449910, GetMainPlayer() / const 0x00449970 / 0x00449960, GetMainPlayerNumber() const 0x00449700; `class CPlayer;` forward | retail bodies + both PDBs + address order, below | all 20 unchanged includers mcheck `--all`: 18 SAME, 2 `$L` label renumbering only (logs/mc_pm_incl.txt) |

## CGameScriptInterface::GetScriptSpriteBySymbol (0x006E77F0)

Landed rows called it `GetTextIDFromSymbol` (an invented name: Ghidra labels the address `LTextSystem::GetIDFromSymbol`).
Neither PDB has that name.

- **Body.** `push esi; call 0x00438570; lea esi,[eax+0x934]; push symbol; mov ecx,esi; call 0x0042B467 (map find);
  cmp eax,[esi]; je -> xor eax,eax; else mov eax,[eax+0x14]; ret 4`. `this` is never read. 0x00438570 lazily caches
  (0x013B878C) the definition named "PLAYER_GUI_PC" (string 0x0123173C) from CGameDefinitionManager.
- **The member at def+0x934.** Retail's CPlayerGuiDef binder registers `lea eax,[esi+0x934]` under the member-name
  string "ScriptSprites" (0x012362CC; code 0x00474C8F..0x00474C9D), then +0x940 "GameActionValues", +0x94C
  "MiniMapGraphics" (`tools`: scripts/strxref.py). Ego_r.pdb: `CPlayerGuiDef::ScriptSprites` is
  `map<CCharString,long>`; the node's value at +0x14 is the long (STLport node 0x10 + CCharString key 4).
- **The name.** Both PDBs list exactly one CGameScriptInterface method with this shape (const, `long`, one
  `const CCharString&`): `?GetScriptSpriteBySymbol@CGameScriptInterface@@QBEJABVCCharString@@@Z` (Ego_r 0x007109B0,
  FableWin 0x024678F0). NPlayerGui::CDrawQuestInfo has a same-named sprite lookup (bound 0x0064A37B). The callers are
  sprite users (MiniMapAddMarker / MiniMapSetMarkerGraphic pass the result to CMiniMapDisplay as a marker graphic).
- **Not proven.** Whether retail declared it `inline` somewhere (FableWin expansions were not searched for).

## CPlayerManager accessor methods

- **Bodies.** 0x00449700 `mov eax,[ecx+0x1c]; ret` (MainPlayer +0x1c). 0x004498C0 and 0x00449910 are byte-identical:
  scan Players (+0x0c..+0x10) for the CPlayer whose +0x28 number equals the argument, else
  `Players[PlayerNeutral (+0x18)]`; `ret 4`. 0x00449970 `mov eax,[ecx+0x1c]; push eax; call 0x004498C0; ret`;
  0x00449960 the same calling 0x00449910.
- **Names and const pairing.** Ego_r.pdb lists the five in retail address order: GetMainPlayerNumber 0x00444EE0,
  GetPlayer QAE 0x00444FE0, GetPlayer QBE 0x00445030, GetMainPlayer QBE 0x004450A0, GetMainPlayer QAE 0x004450F0 —
  matching retail 0x00449700, 0x004498C0, 0x00449910, 0x00449960, 0x00449970. FableWin.pdb has all five. The landed
  bindings agree (QAEAAV -> 0x00449970, QBEABV -> 0x00449960, GetMainPlayerNumber -> 0x00449700).
- **Generator.** The `// Retail override` line makes `gen_class_headers.py` keep the hand edit (its RETAIL_MARK).
- **Not added.** The other PDB methods (CreatePlayer, IsThingMainPlayer, ...) and the vtable: the header stays the
  flattened layout (`void* __vftable`). A retail-branch CPlayerManager (vtable 0x01231CD0, `??1` UAE) is wave 7 scope
  (487 rows still define CPlayerManager locally).

## CScriptBase / CLevelScriptBase<T> (wave 1 headers, first bulk use)

1,248 rows now include them (mig_sb.py). Nothing in the headers changed. What the bulk showed:
- Rows whose derived level script declared `void Main();` / `void Init();` non-virtually now override the header's
  protected virtual slots; the leaf symbol becomes the PDB `UAEXXZ` spelling (65 rows, `leafsym` in the mcheck logs).
- The invented `SpawnFunction(CSpawnedFuncBase*, const char* = "")` helper is gone from 208 rows: its calls became
  `AddSpawnedFunction(f, CCharString(r))` (0x00CB7E50), the expansion it always had.
