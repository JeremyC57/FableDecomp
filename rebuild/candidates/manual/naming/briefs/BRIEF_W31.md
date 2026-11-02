# Naming pass, wave 31 (one agent beside ONE chunk agent on the lane -- owner 2026-11-01: two concurrent agents)

Owner rules 2026-11-01, in force for every decision: consider how retail did it and recreate it faithfully; and **when a catalog name is
wrong, fix it**. Resolved decisions: `naming/2026-11-02_wave30/DECISIONS_W30_RESOLVED.tsv` (+ waves 13-29); canonical globals / members in
`naming/ESTABLISHED_SPELLINGS.tsv`. Rules, mechanics and hand-back as `briefs/BRIEF_W30.md` / `BRIEF_W29.md` (read both, the wave 30 README +
AGENT_README + DEFERRED.tsv + QUEUE_W30.tsv and the wave 30 `scripts/`, reused as compose31.py etc.; mkindex.py needs an absolute output path).
**Guard every scratch path variable** (`: "${W:?}"`, `: "${S:?}"`); write only under $S; no git at all. The Leaf the compiler emits is
authoritative (a bare-name Leaf like `DoDieFX` is not a symbol: fix it). When a family edits a shared header, recheck every landed includer (real
`#include` lines) and list them in runs/hdrdep.rows.txt; for a generated header give the gen_class_headers.py HEADER_MEMBERS line. No modelling device
retail never had. Never give a second retail copy of one symbol that symbol's spelling (hold it). Do not add new invented spellings to an address
that has a PDB name: reach the PDB member (a protected base member is callable from a derived member).

## Scope, in this order (every family rechecked against HEAD, one commit per family; rechecks > ~900 rows in parts)
1. **Header families** (DECISIONS_W30 D2 / D5): fable_string.h gains ??0CCharString@@QAE@ABV0@@Z (0x0099EC30), then the four blocked setters;
   engine/CTCBase.h gets a retail polymorphic branch (opt-in, as CScriptThing.h's FABLE_RETAIL_NSCRIPT), then the local CTCBase models move to it.
2. **The queued rows** (`$S/naming/w31/QUEUE_W31_input.tsv`): the ~CTCBase 0x00686830 binders (21), the remaining ~CScriptThing binders (32, by reason
   in wave 30 `runs/deferred30_manual.tsv`, minus the 17 two-view rows on HOLD), the five transitional spellings of DECISIONS_W30 D3 -> PDB forms, the
   libpk spellings -> CVtable_012bf050 (virtual dtor) / CVtable_012bf0c8 (D7), the c697-c701 families (CSoundEmitterLH, CBaseIntelligentPointer members,
   ScorpionKing / ranged-target setters, CShaderDataBank::CloseShaders / Close, the CArenaCreatureDef neighbours, CEngineWorldMap leftovers). The
   coordinator appends c702 rows when that chunk lands.
3. **DECISIONS_W29 D2 / D6 / D3** (the 41 entity-script ??_G rows, CSpawnedFunc2.h, 0x0072DA40 / 0x00A399E0), then ccsns, 009994a0 / 00999620, the
   c690 sort rows and wave 30 DEFERRED.tsv.
Not in this wave (HOLD): the 17 two-view CScriptThing rows (inline_depth owner question); the 121 CActiveEntityScript<T> ??_G rows; 0x00AD3A10,
0x007ADB50 / 0x007ADD50, 0x005136B0, 0x005359A0, 0x0040E350 (ICF / two retail copies), the 0x00613AE0 / 0x006196D0 callers; 0x006BC4A0; 0x0139C900 and
0x0143BB2C; 0x00C2D9C0 owner; engine/CBankFile.h and every CBankFile-family binder (incl. 0x009D58A0 / 0x009CE360 / 0x009D6490); CThingSearchTools;
0x007E5230; LoadFileList; the ConstantFPS owner item; 0x00C28690; 0x0051B620; VIRTUAL_SCREEN_*; 0x00440730; 0x00EF87D0 / 0x00EF5CB0.

Hand back BINDINGS.tsv, LABELS.tsv, RENAMES.tsv, DEFERRED.tsv, QUEUE_W31.tsv (every queue row with its outcome), DECISIONS_W31.tsv, the
per-family rows files (+ runs/hdrdep.rows.txt), ov_final/, logs, scripts and runs/reloc_bindings.after.tsv, as wave 30 did; leave no lv_* directory
and no background job.
