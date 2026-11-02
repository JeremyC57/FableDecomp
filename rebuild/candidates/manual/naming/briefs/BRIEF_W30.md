# Naming pass, wave 30 (one agent beside ONE chunk agent on the lane -- owner 2026-11-01: two concurrent agents)

Owner rules 2026-11-01, in force for every decision: consider how retail did it and recreate it faithfully; and **when a catalog name is
wrong, fix it**. Resolved decisions: `naming/2026-11-02_wave29/DECISIONS_W29_RESOLVED.tsv` (+ waves 13-28); canonical globals / members in
`naming/ESTABLISHED_SPELLINGS.tsv`. Rules, mechanics and hand-back as `briefs/BRIEF_W29.md` / `BRIEF_W28.md` (read both, the wave 29 README +
AGENT_README + DEFERRED.tsv + QUEUE_W29.tsv and the wave 29 `scripts/`, reused as compose30.py etc.; mkindex.py needs an absolute output path).
**Guard every scratch path variable** (`: "${W:?}"`, `: "${S:?}"`); write only under $S; no git at all. The Leaf the compiler emits is
authoritative. When a family edits a shared header, recheck every landed includer (match real `#include` lines, not comment mentions) and, for a
generated header, give the coordinator the gen_class_headers.py HEADER_MEMBERS line. No modelling device retail never had. Never give a second
retail copy of one symbol that symbol's spelling (hold it).

## Scope, in this order (every family rechecked against HEAD, one commit per family; rechecks > ~900 rows in parts)
1. **The queued binder families, largest first** (`$S/naming/w30/QUEUE_W30_input.tsv`): the ~CScriptThing 0x004AA840 binders (~66 rows, ten
   spellings -> ??1CScriptThing@@UAE@XZ from engine/CScriptThing.h), the ~CTCBase 0x00686830 binders (21), the c697 / c698 / c699 destructor and member
   families (LHNetworkLib group, ~CEngineInternalPrimitiveOcclusionBase virtual, 0x00A9D3A0 = ~C3DMeshFilePrimitiveChunk, PeekPThingWithUID,
   CTCBase::OnDie, the STLport dtors, the ??_G stand-ins), and one descriptive libpk spelling per class (0x00C74450 / 0x012BF0C8). The coordinator
   appends c700 rows when that chunk lands.
2. **DECISIONS_W29 D2 / D6**: prove the member destructors 0x00CD4470 / 0x008AC970 / 0x00412130 (0x007E74D0 is landed as
   ~CScriptGameResourceObjectScriptedThingBase), then the 41 esb entity-script ??_G rows with per-row entity models, with CSpawnedFunc2 promoted to
   engine/CSpawnedFunc2.h. D3: 0x0072DA40 / 0x00A399E0 only where retail's ctor and ??_G came from the same object.
3. **Not reached in waves 24-29**: ccsns, 009994a0 / 00999620, the c690 sort-row leftovers, then wave 29 DEFERRED.tsv.
Not in this wave (HOLD): the 121 CActiveEntityScript<T> ??_G rows (DECISIONS_W29 D1); 0x00AD3A10, 0x007ADB50 / 0x007ADD50, 0x005136B0, 0x005359A0,
0x0040E350 (ICF / two retail copies), the 0x00613AE0 / 0x006196D0 callers; 0x006BC4A0; 0x0139C900 and 0x0143BB2C; 0x00C2D9C0 owner;
engine/CBankFile.h and its binders (incl. ~CBankFileAsync 0x009D6490), CThingSearchTools; 0x007E5230; LoadFileList; the ConstantFPS owner item;
0x00C28690; 0x0051B620; VIRTUAL_SCREEN_*; 0x00440730; 0x00EF87D0 / 0x00EF5CB0.

Hand back BINDINGS.tsv, LABELS.tsv, RENAMES.tsv, DEFERRED.tsv, QUEUE_W30.tsv (every queue row with its outcome), DECISIONS_W30.tsv, the
per-family rows files (+ runs/hdrdep.rows.txt), ov_final/, logs, scripts and runs/reloc_bindings.after.tsv, as wave 29 did; leave no lv_* directory
and no background job.
