# Chunk 686 (NA029) -- notes

Ten never-attempted rows with stand-in `_global::` labels. 6 matched (1 MATCH, 5 RELOCATION_MATCH, one of them
retail_asm), 4 parked with 12-18 probes each. Tests pass with g++ and VC7.1 /Od (tall.py). Retail bytes trailing padding
stripped by the oracle.

## Identities (two sources each)
- 0x00C68A50 `CEaxContext::SetEnvironment(unsigned long, unsigned long)` (libpk, descriptive member names as the landed
  00/c6 family): the 31-setter walk over `g_EaxReverbPresets` (0x0139CA40, 26 presets x 0x98 bytes, EAX 4.0 reverb
  property order) + the m_reverb/m_committed/m_dirty shape of the landed setters. Manifest `CDrawGuildSeal_SetLightProperty`
  is a propagated guess.
- 0x00A5BBA0 `NVectorMath::GFInterpolateVectors` (PDB name set; SSE loop retail ran as inline asm -- `// Retail-asm:` line;
  Flags /Zi). Copy in retail_asm/ with its test; src/ holds the same file so finish.py/vland cover it.
- 0x00A8EFA0 `C3DPrimitive2::ConvertToVBFormatStaticNonBump(CTVertexNormTex1*)` (PDB; vertex layout of the landed siblings).
- 0x00AC97D0 `QuadricOptimiserInternals::COptimisedVertex::RemapGroups(const std::map<unsigned long,unsigned long>&)`
  (PDB; Groups map at +0x9c, remap walked with _M_find; _M_erase 0x00AD14E0 of the same tree).
- 0x00CBDAD0 the STLport `__introsort_loop` instantiation over the (CCharString, CCountedPointer<CEntityScriptBindingBase>)
  pairs (objdump spelling in the Leaf line; callees 0x00CBA620 __median landed, 0x00CBB860 __unguarded_partition,
  0x00CBD0F0 __partial_sort, 0x00CB9C50 pair copy ctor; caller 0x00CBDFAD the binding-table sort).
- 0x00CA4570 libvorbis `vorbis_lsp_to_curve` (descriptive C name allowed for Vorbis; fastcall leaf `@vorbis_lsp_to_curve@32`,
  Flags /O2 /Oy /QIfist; the Vorbis constants 0x0128F604 / 0x0122DED8 / the fromdB table callees).
- Parked: 0x00B202D0 `CSplinePosParamData::TransformPosToMaintainShape`, 0x00A3A410 `CWADBank::AddFile`,
  0x007808B0 `CTCFlammable::InitialiseFirePoints`, 0x00B801F0 `vector<CTVertexSeaBackground>::_M_fill_insert` (see STATUS.tsv
  and the differ/ Grade lines).

## Label corrections
See NAME_FIXES.tsv (23 rows): the four manifest stand-ins of this chunk, the invented `IntrosortLoop_00cbdad0` binding,
`UpdateEntryFromFile` bound by the landed CreateWorldLevelWad, the CLight-labelled SetRoomHF/SetRoomLF, the nonexistent
`CMatrix3x4(const C3DVector&, float)` ctor bound by the landed C3DVector::Rotate (it is SetFromVectorAndAngle), the
CFlammableDef helpers landed under tree-owner names, `g_DefStringTable`, `DELTA_NOTIONAL_ZERO`.

## Header debt
- 0x00C68A50 models CEaxContext locally (no engine header; libpk class) -- same layout as the 29 landed setters.
- 0x00A3A410 uses `engine/CCountedPointer.h` (it fixed the register map); CBankFile/CDiskFileWin32 are the landed local models.
- 0x00AC97D0 / 0x007808B0 / 0x00CBDAD0 carry their own STLport 4.6 models (no shared STL header in the lane).
- 0x00CBDAD0 models CCountedPointer locally (vland WARN SHARED_HEADER_AVAILABLE): it is the landed 0x00CBD700 introsort model; moving it onto engine/CCountedPointer.h is a landing-time retype (the pair copy ctor is the out-of-line 0x00CB9C50 either way).

## Coordinator decisions wanted
- 0x00A3A410: the parked source keeps `__forceinline` on `vector::resize` (VC refuses to expand it twice next to the shared
  CCountedPointer header; 857v920 without it). Land-worthy only once the slot order is solved.
- 0x00B202D0: the normalisation is the division form (`scale = newLen / origLen; newDir /= newLen;` with
  `operator/=` as `*this *= 1.0f / s`), proven by the whole x87 prologue matching; the identity-store placement is open.
- retail_asm row 0x00A5BBA0 is delivered in both src/ (for finish.py) and retail_asm/.
