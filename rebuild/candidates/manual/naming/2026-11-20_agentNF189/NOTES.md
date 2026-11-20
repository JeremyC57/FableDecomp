# NF189 notes (round 10, returned STL / counted pointer / string template instances, second pass)

Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log): 77 entries: 9 done, 2 done-already, 7 rejected, 5 blocked (q4774 / q6206 /
q6207: work done, one row left; q5223 / q5991: identity decision needed), 54 open (40 not started; 14 census-only, no row touched,
census in their APPLIED note). STATUS.tsv: 18 ported (6 of them self renames), 105 not-a-binder (probed REFS rows that already bind the
claim), 40 blocked (rows left, census level, never graded). recheck_final.log (one vland run over the 18 changed rows): 1 MATCH +
17 RELOCATION_MATCH, every test PASS.

Census source: a fresh HEAD probe of every REFS row for 66 entries (work/probe.tsv, tools/probe189.py, 424 rows; tools/census.py
summarises it). The very large families (q4451 / q6492, q4456, q5991, q5992, q12197, q5215, q5216 / q12134, q12135, q6521) were not
re-probed: their census comes from NF181 / NF185 or from reloc_bindings.tsv (every bound symbol has one witness line).

## Out-of-order units (one unit each, done with the first queue line)
- q4386 + q12196 + q12200 (0x005D66BA); q4394 + q12191 (0x009F03B0); q5216 + q12134 + q12135 + q12137 (cut-scene cast map);
  q4451 + q6492; q4456 / q12197 census-only lines; the q5223..q5236 counted-pointer claims were triaged together (thunk scan).

## Decisions / flags for the coordinator
- **Size-pragma scaffolding (EXTRA_RULES 2026-11-20), two new opt-ins in stlport/fable_map.h** (hdr/README.md has the three checks):
  `FABLE_RETAIL_STLP_TREE_FIND_LONG_LONG_SIZE_BODY` (q4385, row 0x006D0630, owner 0x005937E1) and
  `FABLE_RETAIL_STLP_MAP_FIND_ULONG_LONG_SIZE_BODY` (q4386, row 0x005D66BA, owner 0x005D8548). Each pick-any copy is byte-identical to
  its owner (cmpbody + q.py at the owner address). 0x005D66BA carries its own row-head `optimize("y", off)` / `("s", on)`; it includes
  stlport/fable_map.h before them, so the header's `optimize("", on)` reset comes first and cannot clobber them (the row grades the same
  with and without the macro). Please confirm that include-order reading of the third condition. Queue the owners' ports onto the header.
- **q12200 (0x005D8548 map::find vs tree find<K>)**: settled without a rename: bytes cannot tell them apart, nothing references the
  address, and the q4386 port no longer depends on it. A D1-style leaf-only respell is left to you.
- **Counted-pointer destructor claims**: q5224 q5225 q5227 q5228 q5233 q5235 q5236 rejected (44-byte size-TU bodies reached by 5-byte
  E9 jmp thunks = the destructor; the body is ReleaseData, FableWin.pdb has each ReleaseData). NAME_FIXES lines filed for each.
  q5223 (0x004057A0) and q5991 (0x00CE1000) are the 53-byte /O2 form with NO thunk (976 identical bodies in the exe): dtor vs ReleaseData
  cannot be told from bytes. Decision needed before anyone moves 155 rows of q5991 onto ??1.
- **Narrowing self renames with a binder left** (task rule: the self Leaf counts as a binding; nobody binds the old leaf):
  0x00579EA3 (q4774; 0x00579FAA still binds a list_T stand-in), 0x00AADD80 / 0x00AADE80 (q6206 / q6207; 0x00AAD5D0 still binds CArray
  stand-ins). Each address goes from three spellings to two; none grows. Entries left `blocked` with the row named.
- **Callee respells that keep the spelling count**: 0x00579D97 (now bound by 0x00579EA3 as the tree's `_M_erase`; its own landed row is
  the junk LTextTreeWalkThrough::BuildTreeArray, self-recursive) stays at two spellings. 0x00A99FF0 / 0x00AADFC0: every binder now on
  the self Leaf (one spelling each).
- **Stale reloc_bindings lines** for 0x0099EFB0 (q12197): witnesses 0x009E6FA0 / 0x004D0F80 / 0x008DB030 recompiled here and all bind
  ??4CCharString@@QAEABV0@ABV0@@Z.
- No stand-in type was cast to a real type. No new local model of a shared-header class: every change removed local models or
  respelled inside an existing one.

## Local classes on a shared base / in-place respells (list per the coordinator rules)
- 0x005EC990: header-less local `CRetailHandleVector` keeps its token (0x0048E0A2 is declared with it, as landed 0x00643EB2 binds it)
  and now derives from std::vector<CEnginePrimitiveHandle> (narrow rule). Landed sibling shape: none needed (no header for the class).
- 0x00A98BE0 / 0x00A98C80 / 0x00A98D20 / 0x00A98DC0: in-place respell inside the row's existing local CCountedPointer template (a
  `static DeleteData` declaration added, the free deleter stand-in dropped). No definition of the member in those objects; no class
  token added. They still model CCountedPointer / CChunkedFileChunk locally (pre-existing; a port onto the shared headers is the
  follow-up, together with their self rename to Register*SubChunkType, NAME_FIXES).
- 0x00AADD80 / 0x00AADE80: only the explicit instantiation's element type changed inside the existing local STLport model.

## New findings outside my entries (NAME_FIXES.tsv)
- 0x00A98BE0 / 0x00A98C80 / 0x00A98D20 / 0x00A98DC0 are C3DAnimFileLoader::Register{Root,Object,Helpers,MovementVector}SubChunkType in
  both PDBs (not Add*); binders 0x00A999B0 / 0x00AA4710.
- 0x00A9CF40 is CCountedPointer<CChunkedFileChunk>::Reset (FableWin + body); 0x009EA3A0 is CConsoleCommandBase::GetFormattedName (both
  PDBs); 0x009F03B0 is CCodeSectionManager::DeregisterSection (both PDBs, NF184).
- 0x00579FAA (landed as std::list_T_00579faa::~list) is the map<EHeroTrainableStatType,SExpSpendUndo> tree destructor (clear, then free
  the header); one source (the body), not filed.

## Pick-any checks (every newly defined symbol, owners189.py + cmpbody / q.py at the owner)
- 0x006D0630: only new symbol ??$find@J@ (owner 0x005937E1): identical. _M_find<long> copy present at HEAD too (DIFFER 56v56 at
  0x00592F92 before and after: HEAD debt, unchanged).
- 0x005D66BA: map<unsigned long,long>::find (owner 0x005D8548): identical; ??8_Rb_tree_base_iterator (table line 0x0041C820 =
  CPixelFormat::IsEquivalent from 0x00CBFB7D's binder): grades MATCH there anyway; the rest are STLport inlines with no owner.
- 0x00A9CF40 / 0x00AADFF0: DeleteData<CChunkedFileChunk> / DeleteData<CWaveFile> copies identical to their owners (the first try with a
  forward-declared CChunkedFileChunk gave an 8-byte incomplete-type delete; fixed by including the shared class header).
- 0x00BBEAF0 / 0x00BC34D0 / 0x00BC4D90, 0x00A80A20, 0x00AADFF0: new STLport / counted-pointer inlines, none with a table line or Leaf.
- 0x004C00B0, 0x005EC990, the four C3DAnimFileLoader rows: defined-symbol sets unchanged in kind.

## Funclets / flags / descriptive names
- No landed EH funclet (rebuild/src/compiled/01/21) includes any changed row. No `// Flags:` line added.
- Descriptive-by-library spellings (no PDB holds STL instances): 0x00579EA3 tree clear, 0x00A80A20 _List_base clear, 0x00AADD80 /
  0x00AADE80 vector<short>. No made-up names.

## Locks claimed beyond rows.txt (release at landing)
- 00a98be0 00a98c80 00a98d20 00a98dc0 00bc34d0 00bc4d90

## Resume pointer (40 open entries)
- Next in queue order: q6240 (0x00442870), q6245, q6320, q6332 / q6341, q6333 / q6342, q6366, q6377, q6500, q6513, q6521, q11655 ...
  q12195 (see APPLIED.tsv). Their census rows are already in work/probe.tsv (tools/census.py <entry>).

## Not run
- No link test of the new owner / pick-any pairs (0x005937E1 + 0x006D0630, 0x005D8548 + 0x005D66BA, 0x00A99FF0 + 0x00A9CF40,
  0x00AADFC0 + 0x00AADFF0) with `LINK=` cleared. The copies are byte-identical (cmpbody), but the link itself was not exercised.

## Rule slips
- Three Bash calls exited non-zero (the tools/xcp.py assertion runs while writing the q4553 transformer).
- The census probe compiled every REFS row of 66 entries without a HELD / lock screen first (read-only compiles in my pS dir; no row
  of HELD.txt or another agent's lock was edited or graded for hand-back).
- Units were taken out of strict queue order (listed above); entries q12196 / q12200 / q12191 / q12134 / q12135 / q12137 / q12197 were
  settled with their earlier queue line.
- Self renames that leave one binder on a stand-in (0x00579EA3, 0x00AADD80, 0x00AADE80) follow the task's "no landed row binds the old
  self leaf" rule but conflict with the brief's "every REFS row ends on the new spelling" wording: please rule; drop them from the landing
  if the stricter wording applies (each is independent of the rest of the hand-back).
