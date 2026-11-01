# Naming pass, wave 22 (one agent beside ONE chunk agent on the lane -- owner 2026-11-01: two concurrent agents)

Owner rules 2026-11-01, in force for every decision: consider how retail did it and recreate it faithfully; and **when a catalog name is
wrong, fix it**. Resolved decisions: `naming/2026-11-01_wave21/DECISIONS_W21_RESOLVED.tsv` (+ waves 13-20); canonical globals / members in
`naming/ESTABLISHED_SPELLINGS.tsv`; the class statics are reached through their PDB inline accessors (wave 18 README). Rules, mechanics and
hand-back as `briefs/BRIEF_W21.md` / `BRIEF_W20.md` (read both, the wave 21 README + AGENT_README + DEFERRED.tsv + QUEUE_W21.tsv and the wave 21
`scripts/` -- compose21.py, hand21/, fam_gdst21.py -- reused as compose22.py etc.). RENAMES.tsv paths repo-rooted; new_name the catalog's bare
spelling. Labels the chunks proved are in `ghidra_out/labels_chunk689..690_*.tsv` and `labels_naming_wave21_2026-11-01.tsv`. A hand-mangled
spelling in a NAME_FIXES row is a claim: the Leaf the compiler emits for the real text is authoritative. A coordinator decision that rests on a
row's name is checked against that row's own landed Leaf first (wave 21 lesson: CDebugStream).

## Scope, in this order (name-only unless noted; every family rechecked against HEAD, one commit per family; rechecks > ~900 rows in parts)
1. **The DECISIONS_W21 families and the open queue rows** (`$S/naming/w22/QUEUE_W22_input.tsv`; the coordinator appends c690 rows when that
   chunk lands -- re-read before each scope step): the libpk error stream on CString (own rows 0x00C0DEC0 / 0x00C0DF00 / 0x00C0BA40; ~80
   stand-in rows, one mechanical family); 0x00B67EC0's three vectors re-modelled as std::vector<float> / <unsigned char> / <unsigned long> in one
   edit (moves 0x00441020 / 0x00485F3A / 0x00464ACE together); 00c44f80 -> CAmbientEmitter::ClearEvents; the c689 deferrals: 0x007ACC50's six
   callers, the 23 vector<long>::_M_insert_overflow rows (size_type `I`), the 0x00D34470 CQ_FireHeartScript ctor (signature + base model),
   the 0x00613AE0 / 0x006196D0 callers in the ~tree stand-in family; the GDefStringTable leftovers 005dcf70 / 008af850 / 00890f40.
2. **Wave 21 DEFERRED.tsv** (`DEFERRED_W21_INPUT.tsv`, 454 rows): PeekTypeList 0x0049C780 / 0x00947780 (33), HasTC 0x006A80A0 (11), the STLport
   set stand-in callers, 0x00CAF680, 0x00AD7400 / 0x00431606 / 0x00AD5B50 STL stand-ins, 0x00AD2F60 / 0x00AD39A0, 0x004303F0 / 0x004E108C,
   0x00CBB860.
3. **The destructor families** (0099a300 / 0099a2e0 / 0099a410 / 00aa75c0 / 00f35b40 / 00cbd510) where the bytes hold, then **the mega
   families** with per-row class / namespace re-models, biggest first: ~CCharString (126), GetPItem (90), AddEvent (23), GetInstance, GetSubDef;
   then the remaining cg10 targets (`CHATGPT_NAMING_QUEUE.tsv`).
Not in this wave: 0x00AD3A10 (HOLD: two retail copies of one symbol, per-module bindings needed), 0x007E5230 (pgui), the 13 GetPWorldSeed rows and
the 12 direct-read ConstantFPS rows (engine-header migration / owner item), 0x00C28690 (libpk name unknown), 0x0051B620 PushBackEmpty (unproven),
VIRTUAL_SCREEN_* (gated), the engine header changes (migration wave 10). 0x00440730 HOLD.

Hand back BINDINGS.tsv, LABELS.tsv, RENAMES.tsv, DEFERRED.tsv, QUEUE_W22.tsv (every queue row with its outcome), DECISIONS_W22.tsv, the
per-family rows files, ov_final/, logs, scripts and runs/reloc_bindings.after.tsv, as wave 21 did; leave no lv_* directory and no background job.
