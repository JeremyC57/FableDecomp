# NF393 hand-back (Account A naming agent, 2026-11-24): ruling R7 pilot on binder 0x00B217D0

**Current status: batches/b1 ACCEPT (see Part 2 at the end).** Part 1 is the pilot measurement that led to rulings R10 / R11; its
"not landed", "8 compiles" and "still owed" lines are superseded by Part 2. Part 1's evidence (work/wd_a1*, evidence/pk/) was built from
NF390's unchanged candidate (a copy is kept at work/probes/nf390_candidate.cpp); rows/00b217d0.cpp is now the Part 2 row.

No repo writes, no git. Work dir $S/w22/agentNF393. 8 VC7.1 compiles (vland pair: 2 rows + 2 tests; 1 probe; 3 census).
Lock 00b217d0 claimed (NF393) and released. The CEngineTextureMorph family and NF388 / NF391 rows were not touched.

## Result: step 4 (residue remains, not landed)
- Candidate: NF390's held/rows/00b217d0.cpp, unchanged (then at rows/00b217d0.cpp; now work/probes/nf390_candidate.cpp). It uses engine/CArray.h + FABLE_RETAIL_STLP_VECTOR_ASSIGN
  and the NParticleEngine GFInterpolateArray template. Left as is (no engine/CCRC.h swap, no local model) so the bins measure the CArray
  header alone. Both sides still warn SHARED_HEADER_AVAILABLE CCRC.
- vland at one LANDVERIFY_WORK path (work/wd_a1h, wd_a1n; logs evidence/vland_a1{h,n}.log):
  - HEAD: RELOCATION_MATCH PASS.
  - new: RELOCATION_MATCH PASS. The CArray SHARED_HEADER_AVAILABLE warning is gone.
- comdat_cmp + objsec by COMDAT symbol (evidence/objsec_00b217d0.comdat.txt, H_/N_sections.txt, *.comdatrel.txt):
  - The self ?InterpolateTo@CSplinePosParamData@NParticleEngine@@QAEXABU12@M@Z: bytes EQ, NoDuplicates on both sides. Only its 2 callee
    relocs are respelled:
    - +51 -> ??$GFInterpolateArray@K@NParticleEngine@@YI?AV?$CArray@K@@ABV1@0MABK@Z
    - +59 -> ??4?$vector@KV?$allocator@K@std@@@std@@QAEAAV01@ABV01@@Z
  - .rdata (`""` literal, 0.5f) EQ.
  - .drectve differs in two ways. Both have precedent in the fresh VC7.1 compile of landed 00b22310 (the neighbouring 0x00B22310 row, the caller of 0x00B217D0):
    - it gains /DEFAULTLIB:"libcp" (NF318 Q1, as 009b7870);
    - it gains the VC7.1 template /alternatename ??$GFInterpolateArray@K@... = ?GFInterpolateArray@NParticleEngine@@YI?AV?$CArray@K@@ABV2@0MABK@Z.
  - 11 NEW .text COMDATs by symbol (all selection Any; evidence/comdat_cmp_a1.txt), 10 under the rename: HEAD's local ~CArray goes GONE
    and pairs EQ with the new ~CArray.
- pk_all + pickany_vs_retail, HEAD v new (evidence/pk/):
  - HEAD: 1 copy, NOADDR.
  - new: 11 copies, all NOADDR, leafref n, reach n. With --addrs the ??4 forwarder reads EQ at 0x009B5A60 (NA_*).
  - pvr empty on both sides. No NEQ anywhere (no R5 shape). reachflip: 1 pair, 0 flips.

## Bins (evidence/bins.tsv)
- A (nf387 conditions):
  - `??4?$CArray@K@@QAEAAV0@ABV0@@Z` (19 B).
  - It is the only 19 B forwarder to 0x0045BC09 in the image (shapescan fwd19 count=1), so R4's two-forwarder block does not arise.
  - Its "every HEAD section pairs EQ" clause holds only if HEAD's ~CArray pairs under the rename (R11).
- B: none.
  - Retail expands the temporary's whole destructor chain inside the leaf (0x00B2182D..0x00B2183D: test, je, push, call free).
  - Any decl-only opt-in on the chain turns that into a call. Measured: FABLE_RETAIL_STLP_VECTOR_DTOR_OUTOFLINE grades DIFFER(110v118)
    (evidence/probe_b1_dtordecl.txt).
  - No header edit was made, so pin_asserts was not needed.
- C (R8): none. R8 covers local-class special members. Here every extra comes from the shared header, and most are written STLport bodies.
- D (residue), with sizes:

  | symbol | size |
  |---|---|
  | `??1?$CArray@K@@QAE@XZ` (rename-paired with HEAD's `??1?$CArray@K@NParticleEngine@@QAE@XZ`, same bytes + reloc; R11) | 14 |
  | `??1?$vector@KV?$allocator@K@std@@@std@@QAE@XZ` | 14 |
  | `??1?$_Vector_base@KV?$allocator@K@std@@@std@@QAE@XZ` | 14 |
  | `?deallocate@?$allocator@K@std@@QAEXPAKI@Z` | 18 |
  | `??1?$allocator@K@std@@QAE@XZ` | 1 |
  | `??1?$_STLP_alloc_proxy@PAKKV?$allocator@K@std@@@std@@QAE@XZ` | 1 |
  | `??$__destroy@PAKK@std@@YAXPAK00@Z` | 1 |
  | `??$__destroy_aux@PAK@std@@YAXPAK0ABU__true_type@0@@Z` | 1 |
  | `??$_Destroy@PAK@std@@YAXPAK0@Z` | 1 |
  | `??0__true_type@std@@QAE@XZ` | 3 |

- Why none of the D symbols has a retail home:
  - Byte-identical unreferenced pattern hits in Fable.exe (tools/shapescan.py, evidence/shapescan.txt). These are upper bounds: the
    regex is not anchored to function starts.
    - dtor14: 1369 hits, 1298 unreferenced.
    - dealloc18: 667 hits, 663 unreferenced.
    - lone ret: 2819 hits, 2154 unreferenced.
    - `mov eax,ecx ; ret`: 925 hits, 905 unreferenced.
  - R9 bars a set-level read: the candidates far outnumber the PDB names.
  - None of the referenced dtor14 / dealloc18 hits is bound to a vector / CArray<unsigned long> member (evidence/refjoin.tsv against the
    table and catalog; xrefs_unattrib.txt); no referenced ret1 / movret3 hit is bound to any residue name either. The 1 B / 3 B symbols
    rest on pk_all NOADDR plus the hit counts.
    - The 5 unattributed hits with real call/jmp references: 0x0040F2C0 (fishing-competition script), 0x004CFFB0 (debug speech
      script), 0x006CE220 (ground navigator), and two /Os tail jumps. Identifying their element type was not attempted: it needs
      caller-side type proof.
    - 0x0045BCE4 is vector<unsigned long>::_M_clear (bound), a different function.
  - 7 symbols have no PDB name at all (__true_type ctor, ~allocator, ~_STLP_alloc_proxy, ~_Vector_base, __destroy, __destroy_aux,
    _Destroy): FableWin's STL is Dinkumware. The FableWin matches for ~vector and deallocate are same-mangling coincidences.
- Already in the landed set (tools/samedef.py):
  - Fresh VC7.1 compiles of landed rows (evidence/samedef_r8_fresh.txt, census_r8.tsv):
    - 00ad9980 defines all 11 non-leaf new COMDATs, byte-and-reloc identical, selection Any.
    - 00b22310 (the neighbouring caller row) defines 6 of them.
    - 009b4b80 defines the __true_type ctor.
  - 447 census objects of NF390 / NF392 (samedef_census_dirs.txt) give the same result.
  - One counter-example, pre-existing: landed 009b4a90 (CArray<unsigned long>::DeallocateMemory) has a LOCAL namespace-std model whose
    allocator::deallocate is `{ free(__p); }` (repo source line 46, no null test), so its object (NF390 census, today) defines
    `?deallocate@?$allocator@K@std@@QAEXPAKI@Z` as a 14 B body (`8b442404 50 e8 .. 59 c20800`) while 20 others define the 18 B shared-header
    body. Two bodies under one pick-any name in the landed set already; so "the linked image gains nothing" is not established for
    deallocate, and R10 carves it out unless that local model is fixed. Follow-up for the coordinator.
- Rulings asked (RULINGS_NEEDED.tsv): R10 (a narrow residue test for shared-header pick-any copies already emitted identically by a landed
  object) and R11 (rename pairing for ~CArray).

## Proposed table changes (NOT applied; for the landing once R10 / R11 allow it)
- delete `??4?$CArray@K@NParticleEngine@@QAEAAU01@ABU01@@Z	0045bc09	00b217d0`. 0x0045BC09 is already bound as the vector spelling
  (witness 009b7650).
- replace `?GFInterpolateArray@NParticleEngine@@YI?AU?$CArray@K@1@ABU21@0MABK@Z	00b22f30	00b217d0` with
  `??$GFInterpolateArray@K@NParticleEngine@@YI?AV?$CArray@K@@ABV1@0MABK@Z	00b22f30	00b217d0` (Ego_r + FableWin name).
- Still owed before ACCEPT (not run: these are landing gates only):
  - a VC7.1 census of the rows that could reference either deleted spelling: the ~20 rows with an NParticleEngine-local CArray model
    (grep list in this NOTES' run: 00b1b380 00b158a0 00b17f70 00af15b0 00af1670 00af2160 00af1600 00af5380 00b21dd0 00b21940 00b218b0
    00b21850 00b215a0 00b214c0 00b21450 00b219a0 00b21630 00aebaf0 + 00854b90) and the 5 GFInterpolateArray rows (00b15bd0 00af3cf0
    00b22310 00b211b0 00b217d0);
  - apply_table --dry;
  - the /FORCE link gate (row + 0045bc09 + 00b22f30's owner if any + 00ad9980 / 00b22310 as definers).

## Name findings (NAME_FIXES.tsv)
- 0x00B22F30: the template spelling above.
- 0x0045BC09: the local stand-in spelling to retire.
- 0x00B217D0 self: the landed `?InterpolateTo@CSplinePosParamData@NParticleEngine@@QAEXABU12@M@Z` is wrong.
  - Ego_r.pdb has `?InterpolateTo@CSplinePosParamData@CPSCSplineBase@NParticleEngine@@QAEXABV123@M@Z`.
  - The table already binds that spelling at 00b217d0 (witness 00b22310).
  - Not respelled in this pilot. It changes the leaf symbol; queue it with the binder.

## Tools (tools/)
- New:
  - shapescan.py: byte-identical retail copies + reference counts.
  - refjoin.py: referenced hits v table / catalog.
  - samedef.py: same-named COMDAT definers + byte / reloc equality.
- Copied from NF392 / NF390 and renamed to NF393: nf393.py, probe393.py, census393*.py, comdats.py, comdatrel.py, pk374.py, pkt.py,
  runpair.sh, xrefs.py, hostof.py, disasm.sh (call it with `S` exported and the VA without 0x), and others.

## Part 2 (after the coordinator's rulings R10 = R1 tier 1, R11 = yes): batches/b1, ACCEPT
Lock 00b217d0 re-claimed as NF393, released at the end. 35 more VC7.1 compiles (43 in total):
- 1 probe;
- the vland pair (2 rows + 2 tests);
- an 11-row census and a 17-row census;
- 2 callee owners.

### The row
- rows/00b217d0.cpp is NF390's binder plus the self respell. It now uses three shared headers:
  - engine/CArray.h, as before;
  - engine/CCRC.h (the declaration-only default) replaces the local CCRC;
  - engine/CPSCSplineBase.h (FABLE_RETAIL_CPSCSPLINEBASE_PASS23) replaces the local CSplinePosParamData.
- The header edit is in hdr/engine/CPSCSplineBase.h. It adds one new default-off opt-in, FABLE_RETAIL_CPSCSPLINEBASE_POSPARAM_INTERPOLATETO:
  a declaration of the nested InterpolateTo inside the public section, add-only. pin_asserts.py: pinned 0.
- The probe reached RELOCATION_MATCH first time (evidence/probe_s1.txt).
- vland at one LANDVERIFY_WORK path with FABLE_HDR_OVERLAY=hdr: H and N both RELOCATION_MATCH PASS.
  - N has no WARN; H keeps its two SHARED_HEADER_AVAILABLE warnings (CArray, CCRC).
- The test is unchanged: it is a standalone behaviour model.

### Self respell
- Old: ?InterpolateTo@CSplinePosParamData@NParticleEngine@@QAEXABU12@M@Z. New: ?InterpolateTo@CSplinePosParamData@CPSCSplineBase@NParticleEngine@@QAEXABV123@M@Z.
- Sources for the new name:
  - Ego_r.pdb (00ae2d70) and FableWin.pdb (02f5e500);
  - the sole retail caller, a call at 0x00B22483 inside CPSCSplineBase::InterpolateTo 0x00B22310 (xrefs). Its fresh VC7.1 object
    UNDs exactly the new name.
- No other landed row defines the new name (self_symbols.tsv). Nothing references the old name (VC7.1 census). A grep of rebuild/ and
  ghidra_out/ finds it only in self_symbols.tsv and naming index files (status/OPEN.tsv row 00b217d0 split_fixable, which this batch
  closes; historical MULTINAME.tsv).
- RESULTS: old / new hold the SELF pair (the NF382 self-respell form) and carry no target marker. land_scripted.py then checks the graded
  symbol against new; a target marker would switch that check off. TABLE_CHANGES apply either way (land_scripted.py line ~167).
- The 00b22310 object's UND of the new name is a link fact, not a naming source.
- Header claims: no COORDINATION_CLAIMS / pending acctB entry names CPSCSplineBase.h; its Account B opt-ins are untouched.

### Table lines
- 0x0045BC09: ??4?$CArray@K@NParticleEngine@@ is deleted, not rewitnessed.
  - The VC7.1 census covers all 25 other rows with a local CArray model that mention NParticleEngine:
    - census_np.tsv: the 8 with a CArray<unsigned long> instance (incl. 00b20b10) and the 3 other GFInterpolateArray rows;
    - census_np2.tsv: the other 17;
    - census_r8 covers 00b22310.
  - None has a UND of it, of the old GFInterpolateArray spelling or of the old self.
- 0x00B22F30: not landed (no catalog row, no self). It has no other table line, so the action is replace: old non-template spelling to
  the template spelling, witness 00b217d0.
- apply_table --dry: delete 1, replace 1, 0 misses, multi-target 1 -> 1, none new. Also applied in a scratch root (work/troot).

### Gates
- objsec under the renames: the self section is EQ (NoDuplicates on both sides), ~CArray pairs EQ, .rdata is EQ. The 10 new pick-any
  COMDATs are each identical to landed 00ad9980's copy (evidence/samedef_b1_fresh.txt).
  - 00ad9980's copies come from engine/CArray.h -> stlport/fable_containers.h; it has no local std model (its source includes only
    shared headers).
- .drectve: libcp plus the GFInterpolateArray /alternatename, with 00b22310's directives beside it (evidence/drectve_b1.txt).
- pk_all + pvr, HEAD v new, on the shipped table (pk_b1/H_, N_) and the applied table (T_):
  - every copy reads NOADDR, leafref n, reach n; no NEQ; pvr empty;
  - the forwarder reads EQ at 0x009B5A60 with --addrs (NA_);
  - reach flips 0.
- /FORCE link of the row + 0045bc09 004014a0 + 00b22310 00ad9980 009b4b80, HEAD v new:
  - duplicates 0 -> 0;
  - the unresolved set shrinks 23 -> 21;
  - nothing HEAD-resolved is left unresolved. The GFInterpolateArray reference is unresolved on both sides under its old and new spelling,
    because 0x00B22F30 is not landed. 00b22310's UND of the new self is now resolved.

### Follow-ups for the coordinator
- The catalog / build-entry Module 'NParticleEngine::CSplinePosParamData' and the row's file name still say the unnested class.
  - Proposed label: NParticleEngine::CPSCSplineBase::CSplinePosParamData::InterpolateTo (apply_labels).
  - vland's name column shows the catalog module.
- 009b4a90's local deallocate (no null test) is already queued by the coordinator.
