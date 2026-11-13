# Chunk 2009 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (chunk 2009, 0x00745120) The frame packer gives a NAMED 1-byte local a 4-byte node but keeps a 1-byte TEMPORARY at size 1 (density
  1000 x weight): an empty filter object passed as a temporary (`GFFilter_Bind3(CThingFilter_HasPhysicsMesh(), ...)`) sorts in front
  of everything else and lands at a byte offset ([esp+33h]); retail's 4-aligned [esp+78h] between the line filter and the mesh is a
  named `CThingFilter_HasPhysicsMesh filter;` (density 250). Read with the phook packer trace (tmp/c2/pk.py), no grade needed.
- (chunk 2009, 0x00745120) An address-taken record declared at function scope conflicts with every slot in the packer: moving the
  CMeshIntersectionData out of the loop let the view-cone filter share the CPhysicsMeshObject's slot instead of the record's
  (frame 0xD0 -> 0xC8, all slots at retail's offsets).
- (chunk 2009, 0x00745120) Same-TU setters defined out of line in the row's TU (SetThingLineOfSight 0x00744A00 etc.), not in the class
  body: VC7.1 expands each constant-argument call (the `if (flag) { Remove; flag = 0; }` remnants) and calls the variable-argument
  ones, exactly retail's split; no inline device needed.
- (chunk 2009, 0x00745120) STLport std::min / std::max select through a reference (`lea eax,[a] / [b] ; fld [eax]`); retail's
  `jp ; fld [a] ; jmp ; fld [b]` arms are value ternaries. A box whose four __ftol2 results are stored straight into it in the order
  TLX, BRX, TLY, BRY is filled member by member (named longs + the 4-long ctor keep the results in registers).
- (chunk 2009, 0x00745120) Visible noinline vector::reserve (FABLE_RETAIL_STLP_VECTOR_RESERVE) and GetAllThingsInRoughArea
  (FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREABODY) bodies tell VC7.1 the local result vector is not captured: end() stays in ebx through
  the loop and the 0 constant moves to ebp (declaration only: 1470 v 1538 bytes, ratio 0.73).
- (chunk 2009, 0x00952890) Which release + epilogue copy survives cross-jumping at a function's end: a final
  `if (!IsStillRunnable()) Terminate();` (falling into the natural exit) keeps the END copy; retail's middle copy owns the tail when
  the final check is `if (!IsStillRunnable()) { Terminate(); return; }` with the same-TU predicate visible (VC7.1 expands it at that
  join site only and calls it at the in-branch checks). The plain `!alive || IsStopping()` with an explicit return reshuffles the
  Terminate merges instead.
- (chunk 2009, 0x00952890) FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY keeps the GetSubDef<T> out-handle uncaptured: the def stays in edi
  for the whole function and its release has no null test (`dec dword ptr [edi+4] ; jne`), as retail.
- (chunk 2009) The hand886 phook.py packer hook runs unchanged through c2dbg.probe (tools/decomp_pipeline/linux_lane/lane_tools/c2probe):
  slot list with members, size and IL weight in ~10 s per compile; it identified the 0x00745120 frame lever without spending a grade.

Review at landing:
- Rows 0x0097FEA0 and 0x007A0D90 were not attempted (0 grades; extents verified, outlines in NOTES.md). Re-queue them.
- Overlay headers (hdr/README.md): 3 new headers + opt-ins in 9 copied repo headers; re-copy HEAD and re-apply before merging.
- NAME_FIXES.tsv: 11 entries (CPlayerGui Add/RemovePerceivingThing CThing&, 0x006A7390 folded operator(), vector reserve, the
  GetAllThingsInRoughArea instantiation vs the landed stand-in self row 0x00746560, three catalog labels, two JackDragon private
  spellings, ~CCreatureAction_DragonRearUpFull vs the landed `DestVal_114_D` self row).
- vland RELOC_CONFLICT on GetExtendedSightRadius (landed 0x00864620 binds 0x007448F0; filed by chunk 2004 too).
- Both rows define same-TU members whose COMDATs land in the row objects (GetFOV, the three line-of-sight setters, IsStillRunnable,
  GetSubDef<CJackDragonDef>, the reserve / search template bodies); only the leaf is graded.
