# Naming pass, wave 25 (one agent beside ONE chunk agent on the lane -- owner 2026-11-01: two concurrent agents)

Owner rules 2026-11-01, in force for every decision: consider how retail did it and recreate it faithfully; and **when a catalog name is
wrong, fix it**. Resolved decisions: `naming/2026-11-01_wave24/DECISIONS_W24_RESOLVED.tsv` (+ waves 13-23); canonical globals / members in
`naming/ESTABLISHED_SPELLINGS.tsv`; the class statics are reached through their PDB inline accessors (wave 18 README). Rules, mechanics and
hand-back as `briefs/BRIEF_W24.md` / `BRIEF_W23.md` (read both, the wave 24 README + AGENT_README + DEFERRED.tsv + QUEUE_W24.tsv and the wave 24
`scripts/`, reused as compose25.py etc.; mkindex.py needs an absolute output path). RENAMES.tsv paths repo-rooted; new_name the catalog's bare
spelling. Labels the chunks proved are in `ghidra_out/labels_chunk692..693_*.tsv` and `labels_naming_wave24_2026-11-01.tsv`. A hand-mangled spelling
in a NAME_FIXES row is a claim: the Leaf the compiler emits for the real text is authoritative. No modelling device retail never had. Never give a
second retail copy of one symbol that symbol's spelling (hold it). A landed source without a `// Leaf:` line is graded on its largest function:
add the Leaf before trusting a recheck verdict on it. Do not touch 0x00BC9C30 / 0x009CDF50 (chunk 693 repairs them).

## Scope, in this order (name-only unless noted; every family rechecked against HEAD, one commit per family; rechecks > ~900 rows in parts)
1. **The c692 queue rows and the DECISIONS_W24 items** (`$S/naming/w25/QUEUE_W25_input.tsv`; the coordinator appends c693 rows when that chunk
   lands): the OakValeRevisited ??_G 0x00EE8C10 (landed as a CLandscapeBackgroundPatch vector deleting dtor) and ctor 0x00EE8100 (outside NScript,
   wrong parameter types); the two CLostTraderTrader rows' callers; CPixelFormat::Initialise 0x009E3830 (many binders spell it a ctor);
   CVolumeTexture::GetLevelByteLength 0x00A681F0; vector<unsigned char>(n) 0x005136B0; PKCAutoDelete<...>::GetChecked 0x00C09920; ~CChunkedFileLoader
   0x00AA7540; CSimpleMemoryPool::ClearMemoryBlocks; CIndexBuffer / CVertexBufferWin32::Uninitialise; 0x00F35B30 IsActiveThreadTerminating vs the
   label DB's IsDone; 0x0139C8FC on the MFC spelling (_afxDataNil if it is the pointer, _afxInitData if it is the block -- read the bytes); the seven
   junk 0x00686A80 binders -> CTCBase::PeekWorld; the 0x00C2D9C0 owner (two sources or leave it); the eight implicit-dtor rows under the protected
   ~CBaseClassNonCopyable (per row: does the PDB class declare a dtor? check the retail vtable before any change).
2. **The per-row re-models**: the 23 vector<long>::_M_insert_overflow rows (size_type `I`), the six 0x007ACC50 callers, the six 0x00412330
   stand-in callers, the 0x0099B800 in-TU callers 009994a0 / 00999620, the c690 sort-row leftovers, the 14 ccsns rows.
3. **Wave 24 DEFERRED.tsv** (`DEFERRED_W24_INPUT.tsv`): PeekTypeList, the STLport set stand-in callers, 0x00CAF680, the STL stand-ins, the destructor
   families (0099a2e0 / 0099a410 / 00aa75c0 / 00f35b40 / 00cbd510) and the mega families (GetPItem, AddEvent, GetInstance, GetSubDef), then cg10.
Not in this wave: 0x00AD3A10, 0x007ADB50 / 0x007ADD50, 0x00613AE0 / 0x006196D0 callers (HOLD: two retail copies), the CBankFile rows and the
CThingSearchTools own-row view (engine-header migration), 0x007E5230 (pgui), the GetPWorldSeed / ConstantFPS rows (owner item), 0x00C28690,
0x0051B620, VIRTUAL_SCREEN_* (gated). 0x00440730 HOLD.

Hand back BINDINGS.tsv, LABELS.tsv, RENAMES.tsv, DEFERRED.tsv, QUEUE_W25.tsv (every queue row with its outcome), DECISIONS_W25.tsv, the
per-family rows files, ov_final/, logs, scripts and runs/reloc_bindings.after.tsv, as wave 24 did; leave no lv_* directory and no background job.
