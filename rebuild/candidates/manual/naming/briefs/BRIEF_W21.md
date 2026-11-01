# Naming pass, wave 21 (one agent beside ONE chunk agent on the lane -- owner 2026-11-01: two concurrent agents)

Owner rules 2026-11-01, in force for every decision: consider how retail did it and recreate it faithfully; and **when a catalog name is
wrong, fix it**. Resolved decisions: `naming/2026-11-01_wave20/DECISIONS_W20_RESOLVED.tsv` (+ waves 13-19); canonical globals / members in
`naming/ESTABLISHED_SPELLINGS.tsv`; the class statics are reached through their PDB inline accessors (wave 18 README). Rules, mechanics and
hand-back as `briefs/BRIEF_W20.md` / `BRIEF_W19.md` (read both, the wave 20 README + AGENT_README + DEFERRED.tsv + QUEUE_W20.tsv and the wave 20
`scripts/` -- compose20.py, hand20/, fam_fastcmp20 -- reused as compose21.py etc.). RENAMES.tsv paths repo-rooted; new_name the catalog's bare
spelling. Labels the chunks proved are in `ghidra_out/labels_chunk686..689_*.tsv` and `labels_naming_wave20_2026-11-01.tsv`. A hand-mangled
spelling in a NAME_FIXES row is a claim: the Leaf the compiler emits for the real text is authoritative.

## Scope, in this order (name-only unless noted; every family rechecked against HEAD, one commit per family; rechecks > ~900 rows in parts)
1. **The DECISIONS_W20 families** (`$S/naming/w21/QUEUE_W21_input.tsv`, the w20 rows; the coordinator appends c689 / c690 rows as those
   chunks land -- re-read before each scope step): CSoundSystem for every landed row that models vtable 0x012B1E24 as CAudioOwner;
   CPKVoiceParams for 0x00C10A30 / 0x00C62530 / 0x00C621B0, the setters and CPKVoiceTable::Play (six rows together); CDebugStream for the libpk
   error-macro stream (0x00C29FC0 and its users); the PDB `set<unsigned long>` copy-ctor spelling at 0x00AD3A10 in its six _Rb_tree-form callers;
   the GDefStringTable mechanical family: every TU spelling `?g_DefStringTable@@3UCDefStringTable@@A` or a variant -> the established
   `?GDefStringTable@@3VCDefStringTable@@A` (94+ TUs; name-only, one family commit, rechecked in parts).
2. **Wave 20 DEFERRED.tsv** (`DEFERRED_W20_INPUT.tsv`, 456 rows): 0x00C73AF0's caller 0x00C62920 parameter type (`const unsigned short*`),
   the StoreSound*Allocator own rows (00c16690 / 00c166d0 / 00c166b0 carry CHoverable_ProcessEvent stand-ins), 0x00AD7400 / 0x00431606 /
   0x00AD5B50 STL stand-ins, 0x00AD2F60 / 0x00AD39A0 (identity from callers), 0x004303F0 / 0x004E108C ~CMemoryBlock stand-ins, 0x00CBB860 /
   0x00CBA8E0 own rows, then PeekTypeList 0x0049C780 / 0x00947780 (33), HasTC 0x006A80A0 (11), the STLport set stand-in callers, 0x00CAF680.
3. **The destructor families** (0099a300 / 0099a2e0 / 0099a410 / 00aa75c0 / 00f35b40 / 00cbd510; 009fc550 is now ~CResource) where the bytes
   hold, then **the mega families** with per-row class / namespace re-models, biggest first: ~CCharString (126), GetPItem (90), AddEvent (23),
   GetInstance, GetSubDef; then the remaining cg10 targets (`CHATGPT_NAMING_QUEUE.tsv`).
Not in this wave: 0x007E5230 (pgui), the 13 GetPWorldSeed rows and the 12 direct-read ConstantFPS rows (engine-header migration / owner item),
0x00C28690 (libpk name unknown), 0x0051B620 PushBackEmpty (unproven), VIRTUAL_SCREEN_* (gated), the engine header changes (migration wave 10).
0x00440730 HOLD.

Hand back BINDINGS.tsv, LABELS.tsv, RENAMES.tsv, DEFERRED.tsv, QUEUE_W21.tsv (every queue row with its outcome), the per-family rows files,
logs, scripts and runs/reloc_bindings.after.tsv, as wave 20 did; leave no lv_* directory and no background job.
