# 0x00402510 GFMain -- matched (RELOCATION_MATCH, /Gr /GS); open points for the coordinator / Account A (B24C4, 2026-11-24)
(B24C's version: b24c4/old_b24c/ESCALATE.md.)

- Item (1) "use the landed spelling for each data address" was issued while B24C called six of them descriptive. They are NOT
  descriptive: ghidra_out/egod_carved_names.tsv (Ego_d.pdb PUB32) has ?CreateCompiledDefs@ / ?CreateChecksums@ /
  ?UsingCompiledDefs@CDefinitionManager@@1_NA, ?m_harddiskErrorCallback@CAFile@@0P6IXPBD@ZA and ?m_errorCallback@ /
  ?m_harddiskErrorCallback@CThreadedFileManager@@... . The row follows item (1) anyway (landed g_b* / DebugMessage /
  HardDiskErrorCallback spellings, written by the PDB-named inline setters in the header opt-ins), and NAME_FIXES.tsv carries the
  PDB spellings for A's respell. When A respells, the opt-in storage lines (CAFile.h _SETCALLBACKS, CDefinitionManager.h
  _STARTUP_SETTERS, CThreadedFileManager.h PASS27 static) are where the class statics go back.
- Two addresses already had two landed spellings: 0x0138E189 (g_bDefsCompiling / g_bCheckDefDependencies) and 0x013CA7D8
  (g_bDefsLoaded / g_bLoadBinaryDefinitions). The row uses g_bCheckDefDependencies and g_bLoadBinaryDefinitions (the readers in
  CheckDependencies / Compile); both are wrong per Ego_d.
- 0x013B864F: the row keeps ?GDisableCapsLock@@3_NA (Ego_d). The landed ?g_KeyRedefinerFlag@@3EA is unsigned char and cannot bind
  CPersistContext::Transfer(const char*, bool&, bool) without a cast; the only data DIFF left in the relocation check.
- vland RELOC_NAME lines: three are Ego_r-proven wrong manifest labels (0x00415530 GetLanguageFolder, 0x009BC8A0 SetNoHangulWordWrap,
  0x00402130 TestDirForValidFilePermissions); 0x009D81E0 only swaps the address-suffixed placeholder ClearPendingFlag_009d81e0 for
  the descriptive DisableVTuneCollection (no PDB name exists). The fifth, ??1CAFile@@UAE@XZ -> 0x0099A300 (label
  CBaseClassNonCopyable::~CBaseClassNonCopyable), is the ICF-folded body of both destructors (GOTCHAS nf104; reloc_bindings.tsv
  already binds ??1CAFile there): it comes from the declaration-only ~CAFile that removes a NEQ pick-any copy.
- Landed 0x00403B10 (CSystemManagerInit ctor) models the +0x38 block as a local `SStartupSizes`; the shared header now calls the type
  CSoundManagerInit (the landed leaves of 0x00991910 / 0x00A3EF00) and the member SoundManagerInit (landed 0x009A6610). Porting
  0x00403B10 onto engine/CSystemManagerInit.h would need an inline CSoundManagerInit constructor in its store order (not added: no
  row here uses it).
