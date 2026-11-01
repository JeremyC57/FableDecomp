# Naming pass, wave 23 (one agent beside ONE chunk agent on the lane -- owner 2026-11-01: two concurrent agents)

Owner rules 2026-11-01, in force for every decision: consider how retail did it and recreate it faithfully; and **when a catalog name is
wrong, fix it**. Resolved decisions: `naming/2026-11-01_wave22/DECISIONS_W22_RESOLVED.tsv` (+ waves 13-21); canonical globals / members in
`naming/ESTABLISHED_SPELLINGS.tsv`; the class statics are reached through their PDB inline accessors (wave 18 README). Rules, mechanics and
hand-back as `briefs/BRIEF_W22.md` / `BRIEF_W21.md` (read both, the wave 22 README + AGENT_README + DEFERRED.tsv + QUEUE_W22.tsv and the wave 22
`scripts/` -- compose22.py, hand22/, fam_cstr22.py -- reused as compose23.py etc.). RENAMES.tsv paths repo-rooted; new_name the catalog's bare
spelling. Labels the chunks proved are in `ghidra_out/labels_chunk690..691_*.tsv` and `labels_naming_wave22_2026-11-01.tsv`. A hand-mangled
spelling in a NAME_FIXES row is a claim: the Leaf the compiler emits for the real text is authoritative. **No modelling device retail never
had** (wave 22: an inline forwarding overload that makes a pointer-calling template bind the PDB reference operator was rejected even though the
bytes matched): re-model the caller to do what retail did.

## Scope, in this order (name-only unless noted; every family rechecked against HEAD, one commit per family; rechecks > ~900 rows in parts)
1. **The DECISIONS_W22 families and the open queue rows** (`$S/naming/w23/QUEUE_W23_input.tsv`; the coordinator appends c691 rows when that
   chunk lands -- re-read before each scope step): the generic CThingSearchTools templates (GetPFirstThing / GetPBestThing / GetPFirstLeafThing
   and the CFilterBind2 / CFilterBind3 adaptors) re-modelled to call `filter(*p)` on the PDB `operator()(const CThing&)` of every filter they bind
   (HasTC 0x006A80A0's 10 rows 0x004A9E50 / 0x005C0D9D / 0x00615750 / 0x0065F744 / 0x00782670 / 0x00799C40 / 0x00965E80 / 0x004A9DA0 / 0x0093F8A0 /
   0x006186F0, and the In2DRange / InMap / ... filters the same templates bind -- check each filter's PDB operator first; rows rechecked
   together); the CAmbientEmitter own rows (ctor 0x00C44EB0, 0x00C61C70; 0x00C44C50 keeps its stand-in -- no invented name); the four libpk
   CString own rows 0x00C0AD10 / 0x00C0DF50 / 0x00C0E0F0 / 0x00C0E360 re-authored on CString with int Mid / Right; the per-row STLport re-models
   (0x007ACC50's six callers, the 23 vector<long>::_M_insert_overflow rows on size_type `I`, the ~tree family callers of 0x00613AE0 / 0x006196D0);
   the remaining 00c0dec0 views, the two c690 sort rows' leftovers.
2. **Wave 22 DEFERRED.tsv** (`DEFERRED_W22_INPUT.tsv`, 461 rows): PeekTypeList 0x0049C780 / 0x00947780 (33), the STLport set stand-in callers,
   0x00CAF680, 0x00AD7400 / 0x00431606 / 0x00AD5B50, 0x004303F0 / 0x004E108C, 0x00CBB860, the 14 ccsns rows that need re-authoring.
3. **The destructor families** (0099a300 = ~CBaseClassNonCopyable per chunk 690 / 0099a2e0 / 0099a410 / 00aa75c0 / 00f35b40 / 00cbd510) where
   the bytes hold, then **the mega families** with per-row class / namespace re-models, biggest first: GetPItem (90), AddEvent (23), GetInstance,
   GetSubDef; then the remaining cg10 targets (`CHATGPT_NAMING_QUEUE.tsv`).
Not in this wave: 0x00AD3A10 (HOLD: two retail copies of one symbol), 0x009CEC60 / 0x009CBD30 / 0x009CBD50 (CBankFile header), 0x007E5230 (pgui),
the 13 GetPWorldSeed rows and the 12 direct-read ConstantFPS rows (engine-header migration / owner item), 0x00C28690 (libpk name unknown),
0x0051B620 PushBackEmpty (unproven), VIRTUAL_SCREEN_* (gated), the engine header changes (migration wave 10). 0x00440730 HOLD.

Hand back BINDINGS.tsv, LABELS.tsv, RENAMES.tsv, DEFERRED.tsv, QUEUE_W23.tsv (every queue row with its outcome), DECISIONS_W23.tsv, the
per-family rows files, ov_final/, logs, scripts and runs/reloc_bindings.after.tsv, as wave 22 did; leave no lv_* directory and no background job.
