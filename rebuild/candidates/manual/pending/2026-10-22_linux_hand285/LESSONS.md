# Chunk 285 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Parameter order of a `__forceinline` helper orders VC7.1's inlined live-range copies. In 00b47630 (advisor C1), `static __forceinline void SetState(CTrackedRenderState&, unsigned long value, CRenderStateManager* mgr)` with the manager LAST gives retail's `mov ecx,esi ; mov edi,edx`. Try reordering helper parameters when a row has swapped movs only.
- `const long& x = obj.Member;` fixes more than load order:
  - SIB base/index order: in 00b9e950, `const long& layer = mesh.Layer;` gives `[ecx+edx+0x320]`.
  - Load-after-store placement: in 00bd1f70, `const long& primitiveType = mesh.Type;`.
- A pointer read through a smart-pointer member each time (`meshPointer.Object->...`) must stay a member access. A named data local caches it in a register, where retail reloads (00b9e950).
- A large inline matrix helper (PreMultiply) needs `__forceinline` to inline at all (00b47630). Forcing a full matrix-product operator* does NOT dead-store-eliminate the unused elements; 00bd6810 grew to 1671 bytes.
- VC7.1 canonicalises commutative x87 fmul/fadd terms by first reference, so per-element source order of a matrix product has no effect (00b47390, 15 forms). Park such rows.
- CRGBFloatColour:
  - `Set(r,g,b,a)` gives direct constant stores, while a ctor temporary goes through the frame (00b482a0).
  - A user bytewise copy ctor on CRGBColour lets constants propagate into byte stores (00bd6810).
- CalcSWLighting family (A3): the facing dot is a C3DVector member `Dot` (`Y*o.Y; += Z*o.Z; += o.X*X`) used as `-DiffuseVector.Dot(n)`. Further levers:
  - a G/B spill via `CRGBFloatColour sum = ...; CRGBFloatColour c(sum.R, sum.G, sum.B, sum.A);`;
  - the lambert dot as Y,X,Z;
  - `*this *= FastInvSqrt(lsq)`.
  - The Normalise reciprocal spill (`fstp [r] ; fmul [r]`) is still unsolved after 24+ spellings: a lane-gap candidate.
- STLport `GetData` on vector<unsigned short>: `size() > 0 ? _M_start : (T*)this` gives retail's `cmp eax,edi ; ja` (zero in a register). Plain `size() ?` gives `jne`. On vector<char> the sub flags give `jne` either way (009ae390).
- A frame-size miss of exactly N bytes with matching slot offsets for the lower locals usually means a callee-constructed object is larger than modelled. In 009ae390, CCompressor's body is 0x54, not 0x40: +0x14 of frame.
- An STLport tag argument (`__true_type()`) passed by address with no zero store in retail means the tag struct has a user (empty) ctor. Without one, VC7.1 value-initialises it (`mov byte ptr [esp+x],0`) (009ae390).
- A pair copied into vector storage from a temporary keeps both words in registers only when the copy reads both fields into named values before storing. `*_M_finish = x` and placement-new copy reload the second word after the first store (009ae390).

Review at landing:
- 00bd6b90 / 00bd6df0: an earlier draft redeclared the global `operator new(unsigned int)`. It was removed and both still RELOCATION_MATCH; re-grade at landing.
- 00b9e950 / 00bd1f70 declare a CLASS-scope `static void* __fastcall operator new(unsigned int)` (0x00BEB540, the sub-primitive pool). It is a member allocator, not the global one; check that the lander accepts it.
- 00babb50: line 1 names CEnginePrimitiveManagerRepeatedZSprites (the catalog says 2DSprites). Check the leaf binding and the module attribution.
- 00b47630 came from advisor C1 ($S/w22/retry_shared/advisor/00b47630/final.cpp); it took more than 4 grades.
- Local models next to existing engine headers, all for documented layout reasons (12-byte retail vectors, CThing/CDefClassBase deltas):
  - CEngineLightingManager rows: a local model, because ThemeChannels is a 12-byte vector, so the colours sit 4 below the PDB.
  - 00b8d780: CEngineOcclusionManager members are shifted by the 12-byte vectors.
  - 00babb50: RepeatedZSprites PrimitiveSortList is 12 bytes.
- The CompressPrimitive class (009ae390) is doubtful (members at +0x88 / +0x94); verify the owner before landing it.
