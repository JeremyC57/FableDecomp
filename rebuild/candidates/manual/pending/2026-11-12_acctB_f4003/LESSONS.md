# Chunk 4003 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00CD52D0, 24.7 KB, matched at the first whole-function compile) A long registration table is one basic block of
  near-identical statements: decode every call site into one line per statement (tools/map.py), hash the instruction shape of
  each statement to find the repeating unit and its deviations (21 shapes here), and generate the whole source from the map.
  The deviations that look like source differences mostly are not: the eax / ecx / edx rotation of address temporaries and the
  five units whose member stores stay ahead of the argument pushes (0x00CD5745, 0x00CD7AB2, 0x00CD7BE4, 0x00CD7D16,
  0x00CD7E48) are c2's scheduler (round-robin renaming, region cuts) and come out by themselves when every statement has
  retail's spelling. Only the operand differences (which allocator, NULL, true / false) are source.
- (0x00CD531E) FableWin.pdb keeps out-of-line copies of constructors retail inlines: `??0CScriptInfo@@QAE@ABVCCharString@@J_N
  P6I..@ZP6I..@Z` gives the parameter order (name, id, master, alloc, data alloc), and retail's store order equals it although
  the members are declared in another order (AllocFunc, MasterScriptDataAlloc, Name, ID, MasterScript): the body is
  `: Name(name) { ID = id; MasterScript = master_script; AllocFunc = alloc_func; MasterScriptDataAlloc = ...; }` (a full
  initialiser list would store the two allocators before the Name copy call). Look for the FableWin COMDATs right after a
  function for the inline helpers it uses (here also ?StatGetMasterScriptDataAlloc@<master script>).
- (0x00CD5315) `CScriptInfo("Name", ...)` with an implicit const char* -> CCharString conversion re-takes the temporary's
  address with `lea` for the copy; one frame word serves all 479 string temporaries and one 0x14-byte slot all 161
  CScriptInfo temporaries (frame 0x18).
- (0x00CD532D) A constant used in 127 statements (the Sunnyvale data allocator) is kept in ebp for the whole function, 1 in
  edi and 0 in ebx; nothing in the source asks for it (an inline static returning the address is enough).
- (0x00CB5AC0) An assert-only member whose body is compiled out still costs its by-value argument at every call site
  (`push ecx ; mov ecx,esp ; push -1 ; push <text> ; call ctor ; mov ecx,esi ; call`): 156 CCharString builds for an empty
  CheckSectionIsNotAlreadyRegistered.
- A relocation-masked diff plus one-to-one symbol pairing does not prove that a symbol is the RIGHT name for its address: the
  166 allocator names here rest on the PDB class of each registered script name (C<Name>Script) and on the landed Leaf lines
  (119 agree); script classes have no RTTI in retail, so the vtable route gives nothing.
- tools/rel.py (agent4001's) aligned instructions by masked text and so skipped every relocated `push 0` of the object against
  retail's `push 0x012F7A9C`; when both listings have the same instruction offsets and sizes, pair by index (fixed in my copy).
- Lane: a first RTTI probe printed 161 lines by mistake; pipe every probe through a counter first.

Review at landing:
- The row needs the overlay headers (hdr/README.md): engine/CScriptInfo.h opt-in FABLE_RETAIL_CSCRIPTINFO_CTOR and the new
  engine/CScriptInfoManager.h.
- NAME_FIXES.tsv has 10 landed spellings to respell: 0x00CD52D0 / 0x00F2A0F0 (protected, bound public by the landed Init),
  0x00CB5AC0 (CheckSectionIsNotAlreadyRegistered, landed under a propagated STL label; vland prints WARN RELOC_NAME for it),
  0x00CDBD20 (CQ_SunnyvaleMasterData::Alloc, landed as pSunnyvaleDataAlloc_func), 0x00E9C300 (CV_HeroDuelScript::Alloc, landed
  as a stand-in), and five script allocators landed with `CGameScriptInterface&` instead of `CGameScriptInterfaceBase&`.
- 42 of the 161 script allocators and 0x00CD4A60 (CalextestData::Alloc) are not landed / not catalogued yet; LABELS.tsv
  names them all.
- The row declares the 161 script classes locally with their static Alloc only (NOTES.md "Header debt").
- EScriptActiveStatus has no recovered enumerators; the row passes `(EScriptActiveStatus)0`.
- refs/script_recovery/native_catalog.json: its "allocator" labels are propagated Ghidra names and "allocatorAddress" is
  null; map/allocs.tsv (address, registered name, class) can fill both.
