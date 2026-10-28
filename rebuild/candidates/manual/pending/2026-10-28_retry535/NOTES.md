# Hand chunk 535 (CPersistContext .. CEnginePrimitiveOcclusionManager): 0 of 10 byte-exact

Retry chunk: 10 rows parked earlier (2026-10-27 hand521 / hand519). I regraded every row under RTM 13.10.3077
(1 real grade each). Then I explored the residues compile-only: about 150 source and flag variants for the family and
about 130 for 0x00B2F760. All variants were scored against the retail instructions with a local
`tools/exp.py` + `tools/cc.py` harness (in this directory). None closed a row. All 10 rows are parked again in
differ/ with updated `// Grade:` lines.

## What matched (0)

## Parked for Fable 5.1 (10)
- 00476024 CPersistContext::TransferVectorOfSubComponents<CQuickAccessItemButtonGuiDef> DIFFER(254v255): ours `mov eax,[ecx+4] ; add eax,4 ; xor edx,edx ; ... cmp eax,7FFFFFFFh`, then edx through the fast path; retail `lea edx,[eax+4] ; xor eax,eax ; ... cmp edx,7FFFFFFFh`, then eax through the fast path (the 64-bit position's lo/hi words have swapped registers) | tried: RTM regrade; compile-only (no grade) sweeps of: Read spellings (named / inline / nested `__int64`, `(unsigned long)` / `unsigned __int64` / implicit casts, `size + StreamPos`, old-pos local, `StreamPos = StreamPos + size`, inline GetPosition, update-order permutations, if/else / early-return fast path, `unsigned char*` buffer); IsValidPosition spellings (static / member / const-ref / two-return / `<0x80000000` / `0i64` / LONG_MAX / reversed / unsigned-long parameter); ReadSLONG via ReadType / direct / reference-out traits; save-arm count types and size() forms (`long`/`int`/cast/split/byte difference/udiv); operator[] / size() / loop forms (36-way grid); switch forms (default first, case lists, Mode local / cast / getter / enum values, case-return); `if` forms (layout moves); shared inline arm / loop helpers; resize as default argument / explicit `T()`; implicit instantiation from a caller; pragma placement; flags /O1 /Os /Og /Ob1 /Ob2 /Oi /Op /Ow /G5 /G6 /G7 /arch /Gr /GR /GX /Gy /GF /Zi /J /Zp8 /Ge /Gi- /GZ /QIfist; QFE-4035 compiler; a 40-way random grid; dead `__int64` locals | next: lane gap: register tie (evidence: (1) of the ~800 inline-Read sites in the retail exe, the hi word is never in edx: 506 `lea ecx,[eax+4];xor eax,eax`, 147 `lea edx`, 84 `add eax,4;xor ecx,ecx`, 0 `add eax,4;xor edx,edx`; (2) our RTM build picks hi=edx only when the function also contains the save arm's `cdq ; idiv` (non-power-of-two element). Removing the idiv, the write, either loop, or using /O2 imul gives retail's lea form. Only `/Oa` / `optimize("a")` or folding the hi compares away (`(__int64)p` inside IsValidPosition with an `unsigned long` parameter) gives retail's allocation, and both change other bytes. If someone retries, the one untested route is an LTCG (/GL + link /LTCG) build of the def TU.)
- 0047652b CPersistContext::TransferVectorOfSubComponents<CAbilityButtonGuiDef> DIFFER(254v255): same inline-Read lo/hi register swap | tried: RTM regrade (1 grade); the family sweeps above | next: lane gap: as 00476024
- 004769d0 CPersistContext::TransferVectorOfSubComponents<CLensFlareElementDef> DIFFER(254v255): same | tried: RTM regrade (1 grade); family sweeps | next: lane gap: as 00476024
- 00478ae5 CPersistContext::TransferVectorOfSubComponents<CAbilityTrackLevelDef> DIFFER(254v255): same | tried: RTM regrade (1 grade); family sweeps | next: lane gap: as 00476024
- 00479466 CPersistContext::TransferVectorOfSubComponents<COpinionDeedReactionDef> DIFFER(244v245): same (variant B, out-of-line element ctor/dtor) | tried: RTM regrade (1 grade); family sweeps | next: lane gap: as 00476024
- 004798ab CPersistContext::TransferVectorOfSubComponents<CEngineLocalDetailLayerDef> DIFFER(244v245): same | tried: RTM regrade (1 grade); family sweeps | next: lane gap: as 00476024
- 00479ef4 CPersistContext::TransferVectorOfSubComponents<CAppearanceModifierScalingDef> DIFFER(254v255): same | tried: RTM regrade (1 grade); family sweeps | next: lane gap: as 00476024
- 0047a2f5 CPersistContext::TransferVectorOfSubComponents<CAbilityTrackDef> DIFFER(244v245): same | tried: RTM regrade (1 grade); family sweeps | next: lane gap: as 00476024
- 004f9c07 CPersistContext::TransferVectorOfSubComponents<CCardPositionsDef> DIFFER(244v245): same | tried: RTM regrade (1 grade); family sweeps | next: lane gap: as 00476024
- 00b2f760 CEnginePrimitiveOcclusionManager::CEnginePrimitiveOcclusionManager DIFFER(101v95): `LastCameraDirection = C3DVector(1,0,0)` keeps the 12-byte frame temporary, but ours reloads temp.X and temp.Z (`lea eax,[esi+14h]`, copy through ecx/edx). Retail reloads only temp.X, forwards the zero Y/Z (`mov edx,eax`), copies through edx alone with `lea ecx,[esi+14h]`, sinks the dead temp Y/Z stores to the end, and stores LastCameraPosition in Z,Y,X order | tried: RTM regrade (the parked file did not compile: C2512, because C3DVector() was missing; restored); compile-only grids over default / (MMM) init-list or body / user copy ctor / user or implicit operator= / pack(1) or natural packing / temp vs named local vs memberwise vs Set vs reference-taking operator= and ctors / static XAxis() by-value return / user dtor / int or double arguments / memcpy operator= / union array / every-member-in-body orders. Every user operator= or named-local form folds the temporary (0x44 body); every implicit-operator= form gives the same 101-byte body | next: retail's partial store-to-load forwarding (X reloaded, Y/Z forwarded) looks like an optimizer-state difference; try an LTCG build, or a C3DVector header with more inline members (C3DVector.h from FableWin's inline set) in case the class shape changes how the aggregate copy is lowered.

## Skipped (0)

## Names
### Proven
- The 9 family rows: `CPersistContext::TransferVectorOfSubComponents<T>` (Ego_r/FableWin template name; TLC drops the name
  argument), T in address order: CQuickAccessItemButtonGuiDef, CAbilityButtonGuiDef, CLensFlareElementDef,
  CAbilityTrackLevelDef, COpinionDeedReactionDef, CEngineLocalDetailLayerDef, CAppearanceModifierScalingDef,
  CAbilityTrackDef, CCardPositionsDef. Evidence (hand521, rechecked): size() stride, default element vtable RTTI,
  out-of-line resize / ctor / dtor callees, single callers.
- Every relocation in the 00476024 leaf was checked with `objdump -dr` against both PDB name sets. The PDB-exact ones:
  `?CheckCRC@CPersistContext@@IAEXPBD@Z` (0x00404500), `?WriteSLONG@CDataOutputStream@@QAEXJ@Z` (0x00993EB0),
  `?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z` (0x00993CA0), `??0CParentDefClassBase@@QAE@XZ` (0x00430370) and
  `??1CParentDefClassBase@@UAE@XZ` (0x00430300). The remaining ones are the STLport `std::vector<T>::resize(unsigned int, const T&)` and the
  vtable / string literal.
- 0x00B2F760 `??0CEnginePrimitiveOcclusionManager@@QAE@XZ`: vtable 0x012A124C (RTTI, base CEngineComponent), Ego_r.pdb.
  Callee 0x00B59710 is `??0CEngineComponent@@QAE@XZ`: it stores vtable 0x012A2544 (RTTI CEngineComponent) and sets
  GlobalPointerAddress, RenderEnabled and KeepAlive, which matches the Ego_r layout.
- 0x004F9CFC is `??1CCardPositionsDef@@UAE@XZ`. Both PDBs spell only UAE. The landed 0x004F9DF8 binds QAE (NAME_FIXES).
### Probable
- (none)
### Descriptive
- (none)

## Label corrections
- 0x00476024 .. 0x004F9C07 (9 rows): the catalog label `TransferVectorOfSubComponents<CAbilityButtonGuiDef>` is propagated.
  Only 0x0047652B is really CAbilityButtonGuiDef (NAME_FIXES gives the leaf of each of the other eight).
- 0x00B2F760: the catalog label `CCompressedMemoryDataInputStream::CCompressedMemoryDataInputStream` is wrong. The row is the
  CEnginePrimitiveOcclusionManager ctor.
- reloc_bindings carry many invented spellings for the shared callees (27 for 0x00404500, 18 for 0x00993EB0, 12 for
  0x00993CA0, 10 for 0x00430300, 3 for 0x00430370, 3 for 0x00B59710). NAME_FIXES.tsv lists them with the PDB spelling.

## Header debt
- engine/CDataInputStream.h / CMemoryDataInputStream.h have no methods and no inheritance. The family uses a local stream
  model (as hand521 did).
- engine/CEnginePrimitiveOcclusionManager.h flattens CEngineComponent and the C3DVector members, so 0x00B2F760 uses a
  local model. KeepAlive is now `unsigned char` per Ego_r.

## Review at landing
- Nothing lands from this chunk (src/ is empty), so there is no vland conflict to settle.
- NAME_FIXES.tsv: 16 rows (7 callee addresses, 9 catalog row names).
