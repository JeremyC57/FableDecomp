# Chunk 364 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- If a memberwise record ENDS in a memberwise C3DVector, VC7.1 strength-reduces a second src/dst pointer pair at &vector+8. A flat
  record of 5 or 6 dwords puts that pair at +0xC instead. Model the nested `struct { ...; C3DVector v; }` (0076f780, 00543810).
- C3DVector is byte-packed (alignment 1), so `pair<C3DVector,bool>` is 13 bytes and copies with `add ecx,0Dh`. The Ego_r.pdb
  `_Pair_base<C3DVector,bool>` size confirms it (006efec0).
- `__uninitialized_copy` with an unaligned start and no loop-head padding comes from a size TU: it needs `#pragma optimize("s", on)`.
  Without it you get alignment padding (005ba146, 38v31), and a 12-byte POD copies through ebx instead of `movsd` x3 (005924d0, 005790c3).
- Copying a CRGBColour through an inline `Set(r, g, b, a)` in its copy ctor reproduces retail's byte-shuffle loads and the
  `[esp+18h]` spills (006cac20, 00720270).
- A CVectorMap member inside an implicit copy has three parts: a call to the vector copy ctor, nothing for the empty comparator byte,
  and a byte move for Dirty. CVectorMap = 12-byte vector + Compare (+0xC) + Dirty (+0xD) (00674fa0 via 0x00673E10).
- An out-of-line STLport `_Destroy(first, last)` is a `__fastcall` with a plain `ret`. Write it as
  `__destroy_aux(first, last, __false_type())` over an inline template `__destroy_aux` (00675310, 007094d0).
- A member vector's `_Destroy` that dispatches to an OUT-OF-LINE `__destroy_aux` homes its temporary tag in the `push ecx` frame word
  (`lea eax,[esp+0Bh]`). Declare that `__destroy_aux` external (0051cd80 -> 0x00519EF0).
- The STLport list copy ctor inside `__uninitialized_copy` matches fully inline with no noinline stand-in:
  - the `pos` iterator has a user copy ctor and is built in its argument slot;
  - the const_iterators have implicit copies;
  - the tag is homed in the dead `result` slot (007705a0).
- PDB-unique struct sizes and layouts are enough to name an anonymous element type. Search Ego_r.pdb by size and member-type
  sequence (CTestQuest 00623700, CConversationLine 006e65a0, CGameMapCell 006b7b70).

Review at landing:
- These twin pairs are byte-identical bodies with per-address namespaces (`FableCopy_<addr>` / `FableDestroy_<addr>`):
  - 005ba146/005f79bd
  - 005790c3/005924d0
  - 00673c60/00673cf0
  - 006b7b70/006b7bb0
  - 006e1d80/00768fc0
  - 006e65a0/006e68a0
  - 0076f780/0076f8e0

  Keep one leaf per address.
- All rows use local models for the STLport containers (CRetailVector, pair, list) and for CCharString, C3DVector, CRGBColour and
  CIntelligentPointer. Check them against `rebuild/include/engine/<Class>.h` for C3DVector, CRGBColour, CBaseIntelligentPointer
  and CCharString, and swap in a header where it compiles to the same bytes.
- Element identities that rest on inference:
  - 006a3130: CThingGraphicArgPasser, inferred from the TU neighbour ctor 0x006A2F00. The other 9-byte {CIntelligentPointer, bool}
    record in the PDB is the alternative.
  - 006efec0: pair<C3DVector,bool> versus the same-shaped CThingRespawnInfo. The pair is preferred because the PDB has a vector of it.
  - 006b6f50: an anonymous SRecord30.
  - 00527bb0 and 00786da0: SNamedRecord0C.
  - 00566430: STripleStringRecord10.
  - 00720270: SColouredRecord18.
  - 0076f780: SKeyedRecord18.
  - 00543810: SKeyedRecord1C.

  These are descriptive names; the sizes and layouts are verified by grading.
- Landed 0x007674A0 (CTCObjectAugmentations::ClearAllParticleEffects) has a correct model: vector<vector<ParticleAndEndPoints>>.
  0076bd70 and 0076bdd0 now use its field names (StartDummy, EndDummy, ParticleType, Unknown0c). No error was found in landed sources.
- No catalog row holds several functions, and no row needed more than 4 grades.
