# NF192 notes (round 10, engine / render / resource / data-stream names, second pass)

## q4201..q4204 (streaming-font glyph cache statics 0x013BDA34 / 38 / 3C / 40) + 0x013BDA24
- Names: Ego_d.pdb PUB32 anonymous-namespace statics of the streaming-font TU (`?glyphTextureCacheGlyphDataIndices@?A0xfe46a155@@3PAKA`,
  `...GlyphData@...@@3PAXA`, `...GlyphDataCache@...@@3PAVCStreamingGlyphData@@A`, `...GlyphAdjustment@...@@3PAVCStreamingGlyphAdjustment@@A`,
  `?textureCacheMem@...@@3PAEA`); second source: the 0x009A3640 InitializeStreamingFontTextureCache body (new[] element sizes / order,
  landed RELOCATION_MATCH on these spellings). Ego_r / FableWin carry none of them (said so in the rows rather than citing them).
- The `?A0x` hash is per TU path (each row emits its own, so two rows never emit the same full symbol). gate.py does NOT normalise it:
  its `collapse()` only merges a spelling that is a strict prefix of another (a Leaf line cut at `?A0x`). So by the gate these moves are
  count-neutral, not closures: 0x013BDA24 / 34 / 38 / 3C go 2 -> 2 spellings and 0x013BDA40 4 -> 4 (stand-ins out, per-TU hashes of the
  one Ego_d name in; no CREATED split). The queue's `?A0xd4716bce` is just another TU's hash.
- COORDINATOR DECISION (same question as q11864 / 0x0122A600 GFontTextureCache): do per-TU hashes of one anonymous-namespace PDB name count
  as one spelling? If yes these entries are closed; if not, nothing a row can write unifies them (the hash comes from the compile path), and
  the alternative is one TU text for all users. RENAMES s2 writes `?A0x<tu>` for the hash because it differs between my compile path and the
  landing path (the vland object of the landed row 0x009A3640 binds `?A0x837960d0` while its Leaf line carries Ego_d's `?A0xfe46a155`).
- 0x013BDA24 (`textureCacheMem`) moved in the same edit (outside the entries; both referencers are mine; NAME_FIXES line).
- Tests of 0x009A1660 / 0x009A14E0 are standalone host copies (static globals of their own); unchanged.

## q4362 (0x00AC0C00 C3DMeshFileMaterialChunk::CreateMap) and q4795 (0x00AFF0A0 CPSCRenderMesh::Initialise)
- Both ports also fix the self leaf (0x00AC0C00 also the catalog name, CEngineWorldMap::CreateNewBankMap -> C3DMeshFileMaterialChunk::CreateMap):
  RENAMES s1. Neither address is referenced by a landed row (exe scan + reloc table).
- 0x00AC0C00 vtable: the 11 slot relocations of the shared-header ??_7C3DMeshFileMapChunk@@6B@ copy name the functions at the retail
  slot addresses of 0x0129EE34 (dumped: 0x00AC1960 ??_E, 0x00AC1290, 0x00ABFE80, 0x00AA76A0, 0x00AA7610, 0x00AA76C0 const / 0x00AA76B0
  mutable GetAsSuperChunk, 0x00AC03F0, 0x00ABFF70, 0x00ABFFB0, 0x00AA8460, matching the HEAD row's slot comments); the owner 0x00ABFE50
  defines a 36-byte stand-in (?v0@CChunkedFileSuperChunk... names; cmpbody: differs from HEAD 0x00AC0C00's 44-byte copy as well), so the
  HEAD-counterpart rule applies.
- 0x00AC0C00 newly binds `?DeleteData@?$CCountedPointer@VC3DMeshFileMapChunk@@@@SIXPAX@Z` (FableWin.pdb 0x0347E160) at 0x00AC1460, replacing
  the stand-in `?DeleteMapChunk@@YIXPAX@Z`; the 0x00AC1460 self row is a bare-Leaf `DeleteData(AutoTinyVirtualObject*)` stand-in
  (spelling count at 0x00AC1460 unchanged: 2 -> 2). Its AddSubChunk call now binds the 0x00AA84F0 self Leaf (0x00ACD5A0 still binds the
  stand-in `?AddSubChunk@CChunkedFileChunk@@QAEXABV?$CCountedPointer@UCChunkedFileChunk@@@@@Z`: outside the slice, not changed).
- New header-less local classes (allowed: no header anywhere under rebuild/include, landed sibling 0x00B03CA0 has the same shape, bases
  from shared headers): CEnginePrimitiveParticleMeshGroup, CEnginePrimitiveParticleLineGroup, NParticleEngine::CPSCRenderMesh (0x00AFF0A0).
- Tests of both rows are unchanged standalone host copies (they model CMatrix3x4 / CIEngine / C3DMeshFileMapChunk locally at HEAD: debt,
  not changed here).
- Follow-ups (not in the slice): 0x00AC1960 / 0x00AC1460 / 0x00ABFE50 rows are stand-ins whose bodies are the shared-header
  ??_GC3DMeshFileMapChunk / CCountedPointer<C3DMeshFileMapChunk>::DeleteData / C3DMeshFileMapChunk() (byte-identical copies checked).

## q5193 (0x00C266E0): deferred (vendor)
- The ctor is CPooledResource::CPooledResource() (descriptive libpk name): it stores the CPooledResource family vtable 0x012B3FDC, and
  libpk/CPooledResource.h already documents it (FABLE_RETAIL_STREAMED_REQUEST_CTOR_PASS23 "NativeC266E0"). A class-token rename in place
  graded RELOCATION_MATCH PASS but is a new local model of a shared-header class (vland WARN SHARED_HEADER_AVAILABLE), so it was withdrawn
  (draft in work/c266e0/). The proper port needs an inline CPKVoiceParams() opt-in (0x00C10A30 is expanded in this ctor): deferred under
  the vendor light-renaming rule.

## q5164, q6242, q6313, q6367, q6370, q5209 (see APPLIED / STATUS for each row)
- q5209: the five CRightHandedSet(const C3DVector&) binders moved onto engine/CRightHandedSet.h `_ANGLES` + engine/C3DAngles.h; 0x00894950
  uses `FABLE_RETAIL_CRIGHTHANDEDSET_RIGHT_COMPONENTS` only so the (unused) GetRight compiles where C3DVector.h was already included without
  operator^ (no code effect); the three setters add `FABLE_RETAIL_C3DVECTOR` for the same reason.
- q6242: self leaf and two binders respelled IN PLACE in their pre-existing row-local C3DPrimitive2 models (advisor in-place rule: same class
  token, no new symbols naming the class beyond the leaf); 0x00A931F0's lock read `released` and was claimed.
- q6370: 0x00861A80 ported onto engine/NAnimationEntryBodge_CAnimEntryLite.h with the NF192 inline-accessor opt-in (the first, in-place
  namespace respell matched too but defined accessor COMDATs naming the respelled class; withdrawn, draft work/00861a80_inplace.cpp).
- q5164: 0x00B3A850 / 0x00B3A8C0 (byte-identical ~list / ~_List_base twins, no caller) need a decision on which is which before they move.
  Data for it: in the 0x00B3ADC0 object the COMDAT sections come clear (sec 9) < ~_List_base (sec 11) < ~list (sec 13), and retail has
  clear 0x00B3A760 < 0x00B3A850 < 0x00B3A8C0, which suggests 0x00B3A850 = ~_List_base, 0x00B3A8C0 = ~list (one TU, one source).
- 0x00B3ADC0 keeps the strong ??1CEnginePrimitiveAttachmentManager@@UAE@XZ copy it had at HEAD (cmpretail: 62 B equal to 0x00B39F70's
  body): a pre-existing duplicate with the owner 0x00B39F70 (LNK2005 debt, not new).
- q6367: 0x00CDBD00's class is unproven (unreferenced deleting-destructor shape; not TeleportData, which has no vtable); needs a decision.

## Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log)
- Entries (50): 10 done, 3 done-already, 4 blocked (q4251 4 rows left, q5164 2 rows left, q6204 1 row left, q6367 1 row left), 1 deferred
  (q5193 vendor), 32 open (not reached: every queue line from q6437 onward).
- Rows: 16 ported (all RELOCATION_MATCH + PASS in recheck_final.log), 14 not-a-binder, 8 blocked (0x00BDC510 0x00BE7D70 0x00BE8920
  0x00BF3B60 0x00B3A850 0x00B3A8C0 0x00C266E0 0x00CDBD00).
- Self renames (RENAMES s1): 0x00AC0C00 (also catalog), 0x00AFF0A0, 0x00A8EDA0, 0x0073A690 (leaf); none is referenced by a row that keeps the
  old spelling. No EH funclet includes any changed row (grep of rebuild/src/compiled/01/2*).
- Flags lines: none added. Header opt-ins: 4 (hdr/README.md). Pick-any checks: 0x00AC0C00 (3 COMDATs vs retail bytes, equal),
  0x00B3ADC0 (3 STLport COMDATs vs retail bytes, equal); the other rows define only their leaf plus shared-header inlines.

## Rule slips
- Three failing compiles: the 0x00723760 /Od preflight failed (C2676) and the same Bash call still graded the three setters at /O2 (all
  SRC_FAIL in seconds, no spin). Afterwards the preflight and the grade went in separate calls.
- Three Bash calls exited non-zero (greps with no match as the last command: a build-flags grep, the 0x00A8EDA0 flags grep, one more).
- 0x00C266E0: a class-token rename to CPooledResource was graded and hand-written before the libpk/CPooledResource.h header was found
  (census grep cut by `head`); withdrawn from rows/ before hand-back (draft kept in work/c266e0/).
- Self-row old leaves in RENAMES s1 were checked against the HEAD objects (work/head/*.syms): 0x00AC0C00, 0x00AFF0A0, 0x00A8EDA0 agree.
- Defined-symbol diffs HEAD -> new (work/head vs work/new, work/b3adc0): only the leaf changes, shared-header inlines, and the pick-any copies
  listed in STATUS; no newly defined non-leaf symbol is another landed row's Leaf except `clear` 0x00B3A760 (retail bytes equal).
- Locks: 006a5d90 claimed then released (not edited); 00a931f0 and 00861a80 read `released` and were claimed; none of my rows is in
  UNMATCHED_ROWS.tsv as acctB.
