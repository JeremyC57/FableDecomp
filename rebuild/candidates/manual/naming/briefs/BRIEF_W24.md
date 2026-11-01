# Naming pass, wave 24 (one agent beside ONE chunk agent on the lane -- owner 2026-11-01: two concurrent agents)

Owner rules 2026-11-01, in force for every decision: consider how retail did it and recreate it faithfully; and **when a catalog name is
wrong, fix it**. Resolved decisions: `naming/2026-11-01_wave23/DECISIONS_W23_RESOLVED.tsv` (+ waves 13-22); canonical globals / members in
`naming/ESTABLISHED_SPELLINGS.tsv`; the class statics are reached through their PDB inline accessors (wave 18 README). Rules, mechanics and
hand-back as `briefs/BRIEF_W23.md` / `BRIEF_W22.md` (read both, the wave 23 README + AGENT_README + DEFERRED.tsv + QUEUE_W23.tsv and the wave 23
`scripts/` -- compose23.py, hand23/, fam_filt23.py -- reused as compose24.py etc.; mkindex.py needs an absolute output path). RENAMES.tsv paths
repo-rooted; new_name the catalog's bare spelling. Labels the chunks proved are in `ghidra_out/labels_chunk691..692_*.tsv` and
`labels_naming_wave23_2026-11-01.tsv`. A hand-mangled spelling in a NAME_FIXES row is a claim: the Leaf the compiler emits for the real text is
authoritative. No modelling device retail never had (no forwarding overloads, no pragma regions retail's source lacked): re-model the caller to do
what retail did. Never give a second address the spelling another copy already carries (two-retail-copies rule, wave 23 GOTCHAS): hold it.

## Scope, in this order (name-only unless noted; every family rechecked against HEAD, one commit per family; rechecks > ~900 rows in parts)
1. **The c691 queue rows and the DECISIONS_W23 per-row work** (`$S/naming/w24/QUEUE_W24_input.tsv`; the coordinator appends c692 rows when that
   chunk lands -- re-read before each scope step): CDefStringTable::SaveTable 0x009D4CE0 (landed as SaveBinary), CDefinitionManager::Compile /
   ClearSymbols, SetGlobalScriptDef, the NBankFileManager header-dir accessors and global, vector<CWideString> _M_insert_overflow (IAE) /
   operator=, the CCRCSymbolMap helpers, CVectorMap::DeallocateMemory, CWorld::GetFrame, CWideString::operator= 0x0099B800; the six wrong-filter
   rows (re-identify the filter each calls first); 0x00C17260 (decide LoadBank vs the own row from two sources, or leave it); 0x00C0C0C0 re-authored
   as CString::operator=; the four remaining 00c0dec0 CString views.
2. **The per-row re-models**: the 39 ~CBaseClassNonCopyable callers still on stand-ins; the c689 STLport rows (0x007ACC50's six callers, the 23
   vector<long>::_M_insert_overflow rows on size_type `I`, the ~tree family callers of 0x00613AE0 / 0x006196D0); the two c690 sort rows' leftovers.
3. **Wave 23 DEFERRED.tsv** (`DEFERRED_W23_INPUT.tsv`): PeekTypeList 0x0049C780 / 0x00947780, the STLport set stand-in callers, 0x00CAF680,
   0x00AD7400 / 0x00431606 / 0x00AD5B50, 0x004303F0 / 0x004E108C, 0x00CBB860, the 14 ccsns rows; then the destructor families (0099a2e0 / 0099a410 /
   00aa75c0 / 00f35b40 / 00cbd510) and the mega families (GetPItem, AddEvent, GetInstance, GetSubDef), then the cg10 targets.
Not in this wave: 0x00AD3A10 and 0x007ADB50 / 0x007ADD50 (HOLD: two retail copies of one symbol), the CBankFile rows 0x009CEC60 / 0x009CBD30 /
0x009CBD50 and the CThingSearchTools own-row view (engine-header migration), 0x007E5230 (pgui), the GetPWorldSeed / ConstantFPS rows (owner item),
0x00C28690 (libpk name unknown), 0x0051B620 PushBackEmpty (unproven), VIRTUAL_SCREEN_* (gated). 0x00440730 HOLD.

Hand back BINDINGS.tsv, LABELS.tsv, RENAMES.tsv, DEFERRED.tsv, QUEUE_W24.tsv (every queue row with its outcome), DECISIONS_W24.tsv, the
per-family rows files, ov_final/, logs, scripts and runs/reloc_bindings.after.tsv, as wave 23 did; leave no lv_* directory and no background job.
