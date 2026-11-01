# Chunk 688 (NA031, 641-776 band) -- 5 of 10 landed (2026-11-01)

Five rows landed (`verify.log`; bindings multi-check 0): the STLport `vector<CTVertexLandscapeBackground>::_M_fill_insert` 0x00B6CE00 (the
landed water twin's text), libvorbis 1.0.1 res0.c `_2class` 0x00CA0890 (vendor text), the __fastcall `ReadFromFile(CVertexCompressionParams&,
CDataInputStream&)` 0x00AA5460, CStereoDelayLine::Process 0x00C75140 (libpk, descriptive) and ~COpinionReactionManagerDef 0x00678460 (RTTI;
`iterator end = L.end()` hoisted in the destructor list loops). All ten labels were stand-ins; 18 labels applied
(`ghidra_out/labels_chunk688_2026-10-27.tsv`, incl. the CLevelLoader::CObject family landed under CTCMapwho / SMappedRecords stand-ins, the
five map `_M_erase` instantiations of COpinionReactionManagerDef's members, and ~CResource landed as ~CMemoryBlock).

Decisions at landing (coordinator, under the owner delegation): 0x00C88E60 CDspOps::FilterGain4 carries a hand SSE kernel (`movss/unpcklps/
shufps 44h` gather, `subps x,x` zeroing: VC7.1 cannot emit either) and is a retail_asm-grade source once its 681v681 fill residue (x87 operand
order of un-parenthesised coefficient sums in the unrolled copies) is solved -- parked with the `__asm` form in differ/ (never in src/); the
landed caller spellings (0x00C75140 as `PostStage::Process`, Blend's parameter constness) are queued c688. Header debt (COptimisedPrimitive
opaque arrays, CWorldMap without virtual slots, CLevelLoader PDB-size list, no generated CLevelLoader::CObject / CLevelFiles /
CEnginePrimitiveLine headers) -> migration wave 10. Slot not refilled (owner 2026-11-01: two concurrent agents).

Parked: CDspOps::FilterGain4 0x00C88E60 (681v681, 53 probes), CCombFilter::Blend 0x00C76120 (623v641, 12), NShape::DrawPrimitiveLineOverLandscape
0x00761280 (733v733, 12: one spill store moved), COptimisedPrimitive::FindAdjacentFaces 0x00AC9A60 (743v745, 16: ebx/edi role swap), CWorldMap::
LoadLevelFromFiles 0x00501150 (756v756, 8: the chunk-149 empty-tag-in-live-arg-slot lane gap). Best sources in
`../../pending/2026-10-27_linux_hand688/differ/`. NAME_FIXES.tsv: 16 rows (queued c688). Lessons in LESSONS.md.
