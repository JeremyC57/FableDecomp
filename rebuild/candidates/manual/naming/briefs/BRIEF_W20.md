# Naming pass, wave 20 (one agent beside three chunk agents on the lane)

Owner rules 2026-11-01, in force for every decision: consider how retail did it and recreate it faithfully; and **when a catalog name is
wrong, fix it**. Resolved decisions: `naming/2026-11-01_wave19/DECISIONS_W19_RESOLVED.tsv` (+ waves 13-18); canonical globals / members in
`naming/ESTABLISHED_SPELLINGS.tsv`; the class statics are reached through their PDB inline accessors (wave 18 README). Rules, mechanics and
hand-back as `briefs/BRIEF_W19.md` / `BRIEF_W18.md` (read both, the wave 19 README + AGENT_README + DEFERRED.tsv + QUEUE_W19.tsv and the wave 19
`scripts/` -- compose19.py, hand19/, fam_q18.py -- reused as compose20.py etc.). RENAMES.tsv paths repo-rooted; new_name the catalog's bare
spelling. Labels the chunks proved are in `ghidra_out/labels_chunk684..689_*.tsv` and `labels_naming_wave19_2026-11-01.tsv`. A hand-mangled
spelling in a NAME_FIXES row is a claim: the Leaf the compiler emits for the real text is authoritative (wave 19 lesson).

## Scope, in this order (name-only unless noted; every family rechecked against HEAD, one commit per family; rechecks > ~900 rows in parts)
1. **The c686 queue rows** (`$S/naming/w20/QUEUE_W20_input.tsv`; the coordinator appends c687 / c688 / c689 rows as those chunks land --
   re-read before each scope step): 0x00CBDAD0's callers (0x00CBDFAD binds `IntrosortLoop_00cbdad0`) -> the `__introsort_loop` instantiation in its
   Leaf line; CFlammableDef::GetAllFireHelpers 0x00781B90 / GetEffectAtHelperPoint 0x007817F0 (landed under tree-owner stand-ins -- own-row
   respells, byte-identical only); CTCGraphicAppearance::GetAllHelperPointNames 0x004BDA30; CEaxContext::SetRoomHF / SetRoomLF 0x00C632E0 /
   0x00C63350 (landed under CLight labels); CMatrix3x4::SetFromVectorAndAngle 0x00A55F90 (landed 0x00A13C60 binds a nonexistent ctor);
   CSplinePosParamData::TransformFromPosition 0x00B21850; CCharString::operator< 0x009E6530; the map<CCharString,long> _M_find / insert_unique /
   _M_erase rows 0x0042B467 / 0x00431606 / 0x0043020A; the 0x007E6EC0 filter-class verification (DECISIONS_W19 item).
2. **Wave 19 DEFERRED.tsv** (`DEFERRED_W19_INPUT.tsv`, 440 rows): PeekTypeList 0x0049C780 / 0x00947780 (33 rows), HasTC 0x006A80A0 (11), the
   STLport set stand-in callers, 0x00CAF680, the c685 per-row leftovers (0x00AB2BD0, 0x00BEC680's std::map argument, 0x00A36EC0 out-of-line
   ~CCountedPointer<const C3DMesh2>), the CODA globals 0x0143BB58 / 0x0143BB48 (one GameCODA SDK spelling each), 0x01238C8C the CScriptThing vtable.
3. **The destructor families** (0099a300 / 0099a2e0 / 0099a410 / 009fc550 / 00aa75c0 / 00f35b40 / 00cbd510) where the bytes hold, then
   **the mega families** with per-row class / namespace re-models, biggest first: ~CCharString (126), GetPItem (90), AddEvent (23), GetInstance,
   GetSubDef; then the remaining cg10 targets (`CHATGPT_NAMING_QUEUE.tsv`).
Not in this wave: 0x007E5230 (pgui, DECISIONS_W19), the 13 GetPWorldSeed rows and the 12 direct-read ConstantFPS rows (blocked on the engine-header
migration / owner item), 0x00C28690 (libpk name unknown), VIRTUAL_SCREEN_* (gated), the engine header changes (migration wave 10). 0x00440730 HOLD.

Hand back BINDINGS.tsv, LABELS.tsv, RENAMES.tsv, DEFERRED.tsv, QUEUE_W20.tsv (every queue row with its outcome), the per-family rows files,
logs, scripts and runs/reloc_bindings.after.tsv, as wave 19 did; leave no lv_* directory and no background job.
