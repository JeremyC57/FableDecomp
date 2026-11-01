# Naming pass wave 24 (2026-11-01): 60 landed rows respelled, all byte-identical (60/60 RELOCATION_MATCH + PASS)

## Inputs and index
- Inputs:
  - the brief naming/briefs/BRIEF_W24.md;
  - QUEUE_W24_input.tsv, which includes the c691 rows and the DECISIONS_W23_RESOLVED per-row items;
  - DEFERRED_W23_INPUT.tsv.
- Index:
  - idx/full.jsonl is HEAD: the wave 23 after.jsonl plus the 45 delta rows changed since then (the 7 chunk 691 rows are new), stored as idx/delta_head.jsonl.
  - Before hand-back I re-checked against HEAD 759adf5a6. Nothing under rebuild/src, rebuild/oracles, ghidra_out or the naming tree is newer than idx/full.jsonl, and there is no labels_chunk692.
  - idx/after.jsonl is HEAD plus ov_final.

## Commands
Run from $S/naming/w24 with the lane env sourced:
1. `python3 scripts/compose24.py ov_final` runs the stages on fresh repo copies and writes runs/<fam>.rows.txt and ov_<fam>/.
2. `bash scripts/recheck.sh final ov_final $PWD/runs/final.rows.txt 3`.
3. `scripts/mkindex.py $PWD/idx/ovfinal.jsonl $PWD/ov_final $PWD/runs/final.rows.txt`, merged into idx/after.jsonl. A single-row fix uses `scripts/mergefix.py <tag>`; the last one was `peek`, row 0x007066A0.
4. The generator chain, in order:
   - `qscan24 idx/after.jsonl runs/qscan_final.tsv`
   - `mkq24` (with mkq24_notes)
   - `mkdef24 idx/after.jsonl`
   - `mkbind24 runs/rk_final.pairs.jsonl idx/after.jsonl runs/final.rows.txt`
   - `mkren24 $PWD/idx/after.jsonl`, then `fixren24`
   - `mklabels24`, then `fixlab24`
5. `bindings_multi_check.py runs/reloc_bindings.after.tsv` reports 0.

## Families
Listed in commit order. Each family has runs/<fam>.rows.txt and a cumulative ov_<fam>/; the editing script is hand24/h24_<fam>.py.

### defmgr (10 rows)
Rows: 00416005 004336bc 006f9be0 009a76a0 009a76c0 009acb10 009b08c0 009d4ce0 00cbf647 01224030.
- 0x009D4CE0 is SaveTable. It was SaveBinary.
- 0x009B08C0 is ?Compile@CDefinitionManager@@QAEXXZ. It now uses the engine-header rename pattern, with the protected PDB callees on std::vector<CWideString>.
- 0x009ACB10 is re-authored as ClearSymbols, which calls `PSymbolMap_Data->Clear()`. Its test was rewritten. CGameDefinitionManager 0x00416005 now derives from CDefinitionManager.
- 0x00CBF647 is ?SetGlobalScriptDef@@YIXXZ. It works on the ESTABLISHED pGlobalScriptDef through `GetDef<CScriptDef>`.
- NBankFileManager:
  - 0x009A76A0 is GetHeaderDir. It was GetAutoSaveFileName, and its test was rewritten.
  - 0x009A76C0 is SetHeaderDir.
  - The global is SymbolHeaderDir. Its static init is 0x01224030.
  - 0x006F9BE0 and 0x004336BC are callers.

### crc (4 rows)
Rows: 00a381d0 00a38550 00a38670 00a38e50.
- On CCRCSymbolMap, Error, ParseEnum and ParseBuffer are protected and use the PDB parameter types.
- The static CurrentFilename is now used.
- Clear calls CVectorMap::DeallocateMemory.

### vws (4 rows)
Rows: 00412330 004126e0 00495890 00e81b40.
- `vector<CWideString, allocator<CWideString> >` is spelled with the explicit allocator, and _M_insert_overflow is protected. This makes 0x00412330's own Leaf the full IAE spelling.

### wsop (2 rows)
Rows: 00495fe0 009e90a0.
- 0x009E90A0 is re-authored as ?Open@CMemoryFile@@QAEXXZ. Its test was rewritten.
- 0x00495FE0 now uses `CWideString WorldToLoad` at +0x1E40 (PDB offset).
- Both rows call ??4CWideString@@QAEABV0@PBG@Z.

### misc (2 rows)
Rows: 004bb600 007066a0.
- 0x004BB600 uses the STLport list<CScriptThingInfo> over _List_base::clear 0x004BA810.
- 0x007066A0 calls CWorld::GetFrame (it was on CWorld_Calls) and CTCBase::PeekWorld 0x00686A80. The PeekWorld proof is in Ego_r.pdb, where 0x006B1E70 is 16 bytes after GetWorld 0x006B1E60, and the same pair sits at 0x02190D40 / 0x02190D60 in FableWin.pdb. Before this, the row spelled 0x00686A80 as CTCBase::GetWorld, but GetWorld is 0x00686A70.

### filt6 (8 rows)
Rows: 006156e0 006d3fb0 00774b40 00777000 007ee5a0 007ee660 009223a0 0093f930. This covers DECISIONS_W23_RESOLVED item 5.
- Re-identified from the callee addresses:
  - 0x006156E0 is `<HasTC, CThingMarker>`.
  - 0x006D3FB0 is `<HasTC, CThing>`.
  - 0x007EE660 is `<HasTC, const CThingVillage>`.
  - 0x0093F930 is `<In2DRange, Nearest, CThing>`.
  - 0x00777000 is `Bind2<IsNot, In3DRange>`.
  - 0x009223A0 is `Bind3<IsNot, IsNot, HasScriptName>`.
- The callers 0x007EE5A0 and 0x00774B40 were fixed.
- The own rows still use the CThingSearchToolsMethods view, which waits for the engine-header migration (D6).

### cstr (5 rows)
Rows: 00c0c0c0 00c17260 00c309e0 00c3bb50 00c3bcb0. This covers items 8 and 9 and the four 00c0dec0 views.
- 0x00C0C0C0 is ??4CString@@QAEABV0@ABV0@@Z.
- 0x00C17260 is the static LoadBank (D1).
- 0x00C309E0 is the dtor of a CString holder (D2).
- 0x00C3BB50 and 0x00C3BCB0 use `CLoadLog::AddError() <<` on CString.

### bcnc (25 rows)
Rows: 00415940 0044a140 00885f50 008f63c0 0098ab00 0098abf0 00994700 00996be0 009973c0 009cd480 009ce8d0 009e2d00 00a0bd80 00a0d290 00a0d410 00a38080 00a5f430 00a76370 00a7a7f0 00b0c9c0 00b61ff0 00b8da80 00bacc50 00bb99a0 00bc2280. The script is scripts/fam_bcnc24.py.
- The callers' local ~CBaseClassNonCopyable is now protected virtual (MAE). ??_G follows it from UAE to MAE.
- 10 rows were excluded (runs/excl_bcnc.txt; D8):
  - 8 implicit-dtor rows;
  - 0x00BC9C30 and 0x009CDF50, which DIFFER at HEAD.

## Outputs
- QUEUE_W24.tsv: respelled 19, respelled-partial 3, deferred 13, hold 3, done 5.
- BINDINGS.tsv: 59 RETIRE, 18 ADD, 8 KEEP, 0 CONFLICT.
- RENAMES.tsv: 13 rows.
- LABELS.tsv: 18 rows.
- DEFERRED.tsv: 461 lines.
- DECISIONS_W24.tsv: D1 to D8.

## Not reached
These stay in DEFERRED.tsv:
- From scope 2: the 23 vector<long>::_M_insert_overflow rows, the six callers of 0x007ACC50, the two c690 sort rows' leftovers, and the remaining 0x00412330 stand-in callers (004091c0 004128a0 005fdad0 0071ce80 009aef60 009af480).
- 0x0099B800's two in-TU callers: 009994a0 and 00999620.
- All of scope 3.
