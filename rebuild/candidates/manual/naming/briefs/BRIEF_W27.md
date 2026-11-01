# Naming pass, wave 27 (one agent beside ONE chunk agent on the lane -- owner 2026-11-01: two concurrent agents)

Owner rules 2026-11-01, in force for every decision: consider how retail did it and recreate it faithfully; and **when a catalog name is
wrong, fix it**. Resolved decisions: `naming/2026-11-01_wave26/DECISIONS_W26_RESOLVED.tsv` (+ waves 13-25); canonical globals / members in
`naming/ESTABLISHED_SPELLINGS.tsv`. Rules, mechanics and hand-back as `briefs/BRIEF_W26.md` / `BRIEF_W25.md` (read both, the wave 26 README +
AGENT_README + DEFERRED.tsv + QUEUE_W26.tsv and the wave 26 `scripts/`, reused as compose27.py etc.; mkindex.py needs an absolute output path).
**Guard every scratch path variable** (`: "${W:?}"`, `: "${S:?}"`); write only under $S; no git at all. A hand-mangled spelling is a claim: the
Leaf the compiler emits is authoritative, and `objdump -t` the Leaf after any nesting or access change (wave 26: a Leaf naming no symbol is graded
on the largest function). A container re-model must keep each own row's parameter types (wave 26: 0x0098A9A0 CArray<long>). No modelling device
retail never had. Never give a second retail copy of one symbol that symbol's spelling (hold it).

## Scope, in this order (every family rechecked against HEAD, one commit per family; rechecks > ~900 rows in parts)
1. **The DECISIONS_W26 approvals** (DECISIONS_W26_RESOLVED.tsv): D1 the NScript entity Main rows (00d9f270 00ee76f0 00e0e2e0 00ef29e0 00d3a1a0
   00e8b020 00ef3000) nested in their script class with Main private virtual (EAE) on the vtable model, per row; D2 an opt-in retail layout of
   engine/CFileDataOutputStream.h with the CDataOutputStream vptr (FABLE_RETAIL_BANK_FILE precedent), then 0x00994480 Write (UAE) and its binder
   0x009A7720; D4 CPixelShader's private ctor / CShaderRenderManager::CreatePixelShader (AAE) with CShaderRenderManager as friend; D5 the access-only
   own-Leaf respells (DEFERRED.tsv "wave 26 finding" rows), then the virtual / parameter-type ones with two sources; D6 the two-target vector<long>
   re-models 008865c0 00f3f5a0 00f372b0 00774100 (each moved target PDB-proven, bindings moved together; 006bc4a0 only once its identity is
   proven); D7 004091c0 (as D6) and 0x004128A0 re-authored as the STLport member vector<CWideString>::push_back, retiring the allocator-less spelling.
2. **The open queue rows** (`$S/naming/w27/QUEUE_W27_input.tsv`: the wave 26 deferred and partial rows; the coordinator appends c695 rows when
   that chunk lands).
3. **Not reached in waves 24-26**: the 14 ccsns rows, the 0x0099B800 in-TU callers 009994a0 / 00999620, the c690 sort-row leftovers, then wave 26
   DEFERRED.tsv (`DEFERRED_W26_INPUT.tsv`: PeekTypeList, the STLport set stand-in callers, 0x00CAF680, the destructor families, the mega families,
   cg10).
Not in this wave (HOLD): 0x00AD3A10, 0x007ADB50 / 0x007ADD50, 0x005136B0, the 0x00613AE0 / 0x006196D0 callers (two retail copies); 0x0139C900 and
0x0143BB2C (no retail name); 0x00C2D9C0 owner; engine/CBankFile.h and its CBankFile_Retail binders 0x009D0380 / 0x009CEA70, CThingSearchTools
(engine-header migration); 0x007E5230; the GetWorldSeed / GetPRandomSeed / ConstantFPS rows (owner item); 0x00C28690; 0x0051B620;
VIRTUAL_SCREEN_*; 0x00440730; 0x00EF87D0 / 0x00EF5CB0 (keep their inline_depth sources, chunk 694 pilot).

Hand back BINDINGS.tsv, LABELS.tsv, RENAMES.tsv, DEFERRED.tsv, QUEUE_W27.tsv (every queue row with its outcome), DECISIONS_W27.tsv, the
per-family rows files, ov_final/, logs, scripts and runs/reloc_bindings.after.tsv, as wave 26 did; leave no lv_* directory and no background job.
