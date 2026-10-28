# Chunk 510 lessons

## GOTCHAS one-liners
- STLport `vector::operator=` passes the source as `(const_pointer)` to `_M_allocate_and_copy` and `__uninitialized_copy`. Its out-of-line copy helper is therefore the PBV instantiation, and the PAV byte twin belongs to `_M_fill_insert`/`resize`. Keep STLport's casts instead of following a landed PAV name (008f6a90, 0x8F67C0 vs 0x8F6800).
- STLport `__find_if` over a CFilter_Bind2 of reference-holding filters needs no trick for retail's hoisted key load. VC hoists `*F1` out of the unrolled loop and reloads it in the 3/2/1 remainder by itself (006fb240).
- A gap-scan row that starts on a flags-dependent `jne` and pops registers it never pushed is the tail of a Ghidra-split function. Disassemble the 0x10-0x20 bytes before it to find the real entry, then SKIP the row (0x0074B77C is the tail of 0x0074B770).
- Two helper C3DVector out-locals, one per call site (function scope plus loop scope), give retail's 0x1C frame. A block-scoped first helper shares the loop's slot (00780430).
- `if (A && B(CCharString("x")))` and `bool b = A && B(...)` both put the temporary's construction flag in ebx and the result in a frame byte. Retail's frame-word flag plus bl result is a different source form, still unfound (007805d0).
- Ego_r.pdb sorts over CCharString-keyed scalar pairs only use `pair<CCharString, unsigned long>` (`K`); `pair<CCharString, long>` appears only in CPersistContext_TransferableVector. A landed sort family spelled with `J` may be the `K` instantiation.
- The lane sometimes leaves an empty `D:\Documents\FableTLC/rebuild/build/landverify` directory tree inside the agent's `src/`. finish.py then fails with IsADirectoryError, so delete the tree before finish.py.

## Review at landing
- 0x009C6D60 is landed as the non-template `?__destroy_aux@std@@YIX...`. 009c73e0 reaches it through the STLport `_Destroy` -> `__destroy` -> `__destroy_aux` template (declaration only).
- 0x0086CC50 is spelled with a one-parameter `vector<pair<long,CSkeletalMorph> >` (no allocator argument) in reloc_bindings; 0086cd70 uses the full two-parameter spelling.
- Const vs non-const helper names:
  - 0x008AC140 keeps its bound PBV spelling (008ad5f0 casts to const_pointer).
  - 0x008F67C0 should become PBV (008f6a90). The landed 0x008F67C0 source still carries PAV, so vland reported a RELOC_CONFLICT before the fix; after the fix this row references PBV.
- 0x006D4750 `_M_insert_overflow` is declared public (QAE) in 006d4850 to keep its bound spelling. STLport has it protected (IAE), and 0085eaa0 uses IAE for its own instantiation.
- vland RELOC_CONFLICT on 006d4850: `__copy_backward<pair<CCharString,long>*, ..., int>` points at 0x006D3610 here and at 0x009B1E20 in the landed `__linear_insert` 0x009B5990. 006d4850's pair type is PDB-proven (TransferableVector<pair<CCharString,long> >::Resize caller). 0x009B5990/0x009B1E20 are probably the `pair<CCharString, unsigned long>` / `CKeyPairCompareLess<CCharString, unsigned long>` instantiation (the only CCharString-keyed scalar sort in Ego_r.pdb), so the landed 0x9B5990 family is the one to respell.
- 0x00487DC0: these sources declare CPlayer::GetPControlledCreature; the catalog says GetHeroCreature.
- 007805d0 (parked) uses the Ego_r.pdb template spelling of GetSubDef<CSpecialEffectsDef> 0x00669580, which is not yet bound.
