# Naming pass, wave 29 (one agent beside ONE chunk agent on the lane -- owner 2026-11-01: two concurrent agents)

Owner rules 2026-11-01, in force for every decision: consider how retail did it and recreate it faithfully; and **when a catalog name is
wrong, fix it**. Resolved decisions: `naming/2026-11-01_wave28/DECISIONS_W28_RESOLVED.tsv` (+ waves 13-27); canonical globals / members in
`naming/ESTABLISHED_SPELLINGS.tsv`. Rules, mechanics and hand-back as `briefs/BRIEF_W28.md` / `BRIEF_W27.md` (read both, the wave 28 README +
AGENT_README + DEFERRED.tsv + QUEUE_W28.tsv and the wave 28 `scripts/`, reused as compose29.py etc.; mkindex.py needs an absolute output path).
**Guard every scratch path variable** (`: "${W:?}"`, `: "${S:?}"`); write only under $S; no git at all. The Leaf the compiler emits is
authoritative; `objdump -t` it after any nesting or access change. When a family edits a shared header, recheck every landed includer of it (in
parts for large sets). No modelling device retail never had. Never give a second retail copy of one symbol that symbol's spelling (hold it).

## Scope, in this order (every family rechecked against HEAD, one commit per family; rechecks > ~900 rows in parts)
1. **The CSpawnedFuncBase family** (DECISIONS_W28 D5, chunk 696 decisions 2 / 3): engine/CSpawnedFuncBase.h with retail's implicit dtor and real
   CCharString / CCountedPointer members (retail ~CSpawnedFuncBase 0x00CDD4C0 calls ~CCountedPointer 0x00CDD4F0 out of line); the 14 sources declaring
   an invented virtual ~CSpawnedFuncBaseRuntime move to CSpawnedFuncBase; recheck all ~394 includers in parts.
2. **D7**: the 191 sfb / esb own rows (wave 28 `runs/sfb.rows.txt`, `runs/esb.rows.txt`) re-authored as the compiler's own scalar deleting destructors
   (the CTB_Bard 0x00ED7DB0 precedent), each with a Leaf naming its emitted ??_G (today those Leafs name no symbol).
3. **The open queue rows** (`$S/naming/w29/QUEUE_W29_input.tsv`: wave 28 deferred / partial rows incl. the ~CScriptThing 0x004AA840 binders (~66 rows),
   the ~CTCBase 0x00686830 binders (21), the c697 destructor families (LHNetworkLib group, ~CEngineInternalPrimitiveOcclusionBase virtual), the
   GetPRandomSeed binders of 0x0049D810 -> ?GetWorldSeed@CWorld@@QBEPAKXZ; the coordinator appends c698 rows when that chunk lands).
4. **Not reached in waves 24-28**: ccsns, 009994a0 / 00999620, the c690 sort-row leftovers, then wave 28 DEFERRED.tsv.
Not in this wave (HOLD): 0x00AD3A10, 0x007ADB50 / 0x007ADD50, 0x005136B0, 0x005359A0, 0x0040E350 (ICF / two retail copies), the 0x00613AE0 /
0x006196D0 callers; 0x006BC4A0; 0x0139C900 and 0x0143BB2C; 0x00C2D9C0 owner; engine/CBankFile.h and its binders, CThingSearchTools; 0x007E5230;
LoadFileList (in neither PDB); the ConstantFPS owner item; 0x00C28690; 0x0051B620; VIRTUAL_SCREEN_*; 0x00440730; 0x00EF87D0 / 0x00EF5CB0.

Hand back BINDINGS.tsv, LABELS.tsv, RENAMES.tsv, DEFERRED.tsv, QUEUE_W29.tsv (every queue row with its outcome), DECISIONS_W29.tsv, the
per-family rows files, ov_final/, logs, scripts and runs/reloc_bindings.after.tsv, as wave 28 did; leave no lv_* directory and no background job.
