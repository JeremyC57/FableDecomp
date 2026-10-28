# Chunk 530 lessons (GOTCHAS one-liners)

- VC7.1 keeps edx live across a call only when the callee is defined EARLIER in the same TU: a `__declspec(noinline)` body of the real callee (CalculateFadeOut) reproduces it (0x00B814B0/0x00B81640).
- x87 term order inside one expression is canonicalized: no source order changes it, but the FableWin inline `CMatrix3x4::operator*(const C3DVector&) const` gives retail's order (0x00B814B0); in a different frame context (0x00BCEE20) it still did not.
- VC7.1 reassociates `a / b * c` into `c / b * a` and `a*b*c/d` into `a/d*...`: when retail keeps a factor on the x87 stack from the first instruction, it is a PARAMETER scaled in place (`tolerance *= d / (r * S)`), not a fresh local (0x00BCF2E0).
- An unrotated loop (`cmp ; jae` at the top + `jmp` back) came from a bound inside an inline predicate (`HasLOD(lod) { return lod < 4 && ... }`), not from the loop statement form; while/for/for(;;) all rotate the same (0x00BCF2E0).
- Early `return true` + final `return true` sharing one epilogue in retail = one `if (!cached) { ... } return true;` block (0x00BCF2E0).
- `x > eps ? x : eps` gives `test ah,41h ; je`; `if (x <= eps) x = eps` gives `jp` (0x00BCF2E0).
- Inline accessor forms decide load order: `PeekMeshBank()` reads the manager before the index (0x00B9DFC0); a resource fetched into a local before a CCharString temporary is held in a callee-saved register across the string ctor (0x00B8B630).
- A by-value return through user copy constructors (C3DVector, C3DBoundingSphere) gives retail's frame temporaries (0x00B9DFC0); a counted pointer's operator= that copies the source's Info/Data into locals before the compare keeps them in ebx/ebp across the release (0x00B41E50).
- A local handle whose class has a virtual dtor calling virtuals is NOT inlined by VC7.1 unless the dtor is `__forceinline` (0x00BAE160).
- Refcount release shape `mov edi,[ecx+4] ; lea eax,[ecx+4] ; dec edi ; mov edx,edi ; test edx,edx` = pointee `long Release() const { long count = --IVRefCount; if (count == 0) Delete(); return count; }` (0x00BAE160); `return --IVRefCount` tested by the caller reloads the pointer.
- Store order of a run of member assignments follows source order through VC's load/store pairing: retail's loads (T,A,B) / stores (A,B,T) came from source order T, A, B (0x00BAE160).
- Flattened engine headers carry `void* __vftable` as data: deriving from one and declaring a virtual adds a SECOND vptr (all members +4). Model polymorphic classes locally (0x00B9DFC0 graded build).
- An inline helper that wraps an `__asm` helper (QuantiseLandscapeHeight over GFFloatToLongNear) is called out of line unless it is `__forceinline` too; write one call site so VC tail-merges as retail (0x00B439C0).
- Two adjacent 16-byte getters can carry the same catalog label: check the member offset each reads against the PDB layout (0x00B4A6A0 Layer2DToSortDistance vs 0x00B4A6B0 Layer2DToDistance).
- FableWin.pdb names many methods landed files call "descriptive" or "unknown" (CBankFileAsync::GetAsyncEntry, CEnginePrimitiveManagerSprites::AddSprite): grep `ghidra_out/fablewin_pdb_names.tsv` by return type / parameter shape before inventing.

# Review at landing
- 0x00B4A6A0: landed callers 0x00BE0DE0, 0x00BE1BA0, 0x00BE0CD0 call it Layer2DToDistance; it is Layer2DToSortDistance (NAME_FIXES.tsv).
- 0x0042B467 (_Rb_tree<CCharString,...>::_M_find) is COMDAT-shared by every CCharString-keyed tree; the landed leaf uses CUnprovenValue_0042b467 as mapped type, 0x00B41E50 instantiates it with `long` (map<CCharString,long> from the OpenRetailStaticMap PDB signature). Same address, two instantiation spellings -- pick one at landing.
- Global spellings for 0x01436E78 (g_pPrimitiveRendererSharedData here; landed: g_pEngineRenderSettings, g_pFadeSettings, g_pPrimitiveRenderer, ...), 0x01436E80, 0x01436E98, 0x01436EA4 vary across landed files: choose one per address.
- 0x00B9DFC0: differ file holds the corrected model; one regrade should give RELOCATION_MATCH.
- 0x00A25E70 GetMesh: the landed SetupMesh2D 0x00BCF430 declares the pointer/non-const form; respell to the PDB reference/const form.
- vland dry run: 10 WINS, no RELOC_CONFLICT / RELOC_DUPLICATE. Remaining warnings: RELOC_NAME on 0x0042B467 (catalog `StdMap_FindLE`) and 0x00BACDD0 (catalog `_Cons_val<...>`; it is CEnginePrimitiveManager::AddPrimitive) -- catalog labels to correct; SHARED_HEADER_AVAILABLE for polymorphic local models whose headers flatten the vptr (CDataInputStream 0x00B41E50, CEngineComponent 0x00B8B630, CIVCountedPointeeBase 0x00BAE160) and for the earlier-session rows 0x00B67510 (CVertexBufferWin32, CIndexBuffer, CTexture) and 0x00B814B0 / 0x00B81640 (CMeshDataBank, C3DMeshInfo, CEngineInternalPrimitiveBase, CEnginePrimitiveRenderer). With LANDVERIFY_STRICT_HEADERS=1 these six rows would be rejected; 0x00B41E50 and 0x00BCF2E0 now derive from their non-polymorphic headers (regraded RELOCATION_MATCH).
