# Chunk 629 -- 0 of 10 landed (2026-10-31)

Cut from the parked-row pool. No row byte-matched; all ten compile to the retail length and are parked in
`pending/2026-10-27_linux_hand629/differ/`, each rebuilt on the shared headers and PDB spellings (CBankFileAsyncData ctors on
CBaseClassNonCopyable + CMovableResource with the real std::set names; GFSerialiseVectorBinaryIn element corrected to
CTCInventoryMap::CUIRegionInfo; 0x00AD8AF0 is CVertexGroupBlock::SharesGroupWith, 0x00A358F0 C3DMeshObject::GetPositionMatrixFromRHSet).
Residues: one base/index order (006d8300), flag-store placement among pushes (009d5d30/009d5df0; /G5-/GB no effect), traits-object
slot (00619560), reload register (00699130), ebx/edi/esi rotation (00408820), x87 operand order (00a358f0), return-block order
(008930f0), ebx/ebp swap (00ad8af0), ebx/edi swap (0041726d). Rows got 4-14 grades each across sweeps. 10 NAME_FIXES (AddHeroStatExperience
enum parameter 0x0057A8DA, CThing::GetScriptThing const 0x004C7E50, GetDef<CControlsDef> 0x00410820, PeekWorldChecksum const,
CBaseClassNonCopyable ctor 0x0099A2F0, GetMapNumberFromScriptName const, ReadWithSrcChunkOverflow private, three row names) + 2 review
rows queued (prefix c629). Labels: ghidra_out/labels_chunk629_2026-10-27.tsv.
