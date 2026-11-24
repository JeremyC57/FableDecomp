# Landing nf371_b2 (2026-11-24): seven CCountedPointer<T>::Reset rows on their PDB names

Hand-back: `pending/2026-11-24_nf371_handback/` (b2_held is the measured run dir). Rows, each FableWin.pdb exact:
0x00447D90 Reset<const CInputTypeBase>, 0x00A29D20 Reset<CMeshDataBankEntry>, 0x00A299C0 Reset<C3DMesh2>, 0x00AA4460
Reset<QuadricOptimiserInternals::COptimisedMesh>, 0x00B3E510 Reset<CLoadingMemoryFailureHandler>, 0x00CD43A0
Reset<CScriptDefinitionManager>, 0x00F39CA0 Reset<CRenderTarget>. All RELOCATION_MATCH PASS before, on the landed tree and again after
the catalog renames; objsec equal under the renames; both pick-any checkers SAME, 0 reach flips; the seven `?Destroy_<addr>` table lines
are now the PDB DeleteData names.

Rulings (advisor decision, owner delegated):
- **R2, yes:** the unreferenced pick-any constructor copy changes from the stand-in `??0SCountBlock@@QAE@PAX@Z` to the header's
  `??0CCPPointerInfo@@QAE@KP6IXPAX@Z0@Z`; it is leaf=n with 0 NEQ in all seven (evidence/pk_gate) and equals what header-form includers
  already emit.
- **R1, form b for 0x00A299C0 and 0x00F39CA0:** the sibling explicit instantiation (GOTCHAS nf80's form for a template member), not a
  strong specialization beside 0x00A24520's / 0x00F38990's pick-any copies (two new LNK2005). The variant adds an unreferenced 53-byte
  pick-any ReleaseData copy, leaf=n, 0 NEQ (evidence/pk_gate_inst). The other five keep the strong specialization as measured.
Catalog: the seven Reset rows and their seven DeleteData owners (0x00447660, 0x00A28370, 0x00A281E0, 0x00AA4310, 0x00B3E080, 0x00CD42E0,
0x00F38920, nf366's) get CCountedPointer<T> Modules and names, which clears the RELOC_NAME warnings; file names stay. self_symbols updated.
Still on stand-ins (queued): binders 0x00A26D40 and 0x00B3B240 (ports), 0x00AA4210 (T3 nf366).
