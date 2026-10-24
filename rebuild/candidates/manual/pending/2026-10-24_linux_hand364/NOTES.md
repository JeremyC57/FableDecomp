# Hand chunk 364 (std::__destroy_aux<CNavigationLayerUpdate> .. std::__uninitialized_copy<SNamedRecord0C>): 40 of 40 byte-exact

All 40 rows carry the same propagated Dinkumware label, `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,...> >,
unsigned int*, allocator<unsigned int> >`. Retail's STL is STLport 4.x. Each body is identified by its `ret`, its element stride
and its callees:
- 32 rows are `std::__uninitialized_copy(first, last, result, const __false_type&)`: `ret 8`, a placement-new null test, and the
  advanced result returned in eax.
- 6 rows are `std::__destroy_aux(first, last, const __false_type&)`: `ret 4`.
- 2 rows are out-of-line `std::_Destroy(first, last)`: plain `ret`.

All are `__fastcall` (/Gr TUs). Grades are 17 MATCH and 23 RELOCATION_MATCH; tall.sh (g++) and tall.py (VC) both PASS 40 of 40.
No row needed more than 2 grades before its final form. Twin bodies each get their own namespace (`FableCopy_<addr>` /
`FableDestroy_<addr>`), so every address keeps one distinct mangled leaf. Every leaf was checked against objdump of `$S/wine/q/<addr>/<addr>.obj`.

## What matched (40)
- **Plain and POD records**
  - 005ba146 and 005f79bd: 4-byte value, one dword per element.
  - 005790c3 and 005924d0: 12-byte POD, `movsd` x3.
  - All four are size-optimised TUs (unaligned start, no loop-head padding) and need `#pragma optimize("s", on)`. Without it
    005ba146 gains alignment padding (38v31) and 005924d0 copies through ebx.
  - 006b7b70 and 006b7bb0: CGameMapCell, 7 bytes (dword + word + byte). PDB-unique size and layout.
- **pair<key, C3DVector> and nested C3DVector records**
  - 006e1d80 and 00768fc0: pair<ulong,C3DVector>.
  - 006efec0: pair<C3DVector,bool>, 13 bytes. C3DVector is byte-packed, so the flag sits at +0xC and the stride is `add ecx,0Dh`.
  - 0076f780 and 0076f8e0: a 24-byte {key, 2 words + C3DVector} record.
  - 00543810: a 28-byte {key, 3 words + C3DVector} record.
  - Shape: the C3DVector copy is memberwise (user copy ctor). When the record ENDS in that nested vector, VC strength-reduces a
    second src/dst pointer pair at &vector+8. A flat 5/6-word record puts it at +0xC and differs.
- **CRandomAppearanceMorph cluster (0x673xxx..0x676xxx)**
  - 00673c60 and 00673cf0: pair<ulong,CTextureMorph>.
  - 00673ca0: pair<long,CSkeletalMorph>.
  - 00674fa0: CBodyPartMesh. The CVectorMap member copy is a call to the vector copy ctor 0x00673E10; the empty comparator
    (+0x14) is skipped and the Dirty byte (+0x15) is moved.
  - 00675310: `_Destroy` over CVectorMap<ulong,CTextureMorph>, 16 bytes, plain `ret`. It inlines `__destroy_aux(first, last, __false_type())`.
  - 00675850: `__destroy_aux` over CArray<CBodyPartMesh>; the inner record is 0x18.
  - All use STLport pair's explicit memberwise copy ctor.
- **String-bearing records (out-of-line CCharString / CWideString copy ctors)**
  - 00527bb0 and 00786da0: SNamedRecord0C.
  - 00566430: three strings and two flags.
  - 00623700: CTestQuest (six strings + colour index; identified by its PDB-unique layout).
  - 006e65a0 and 006e68a0: CConversationLine (PDB-unique layout).
  - 0076f700: pair<CCharString,CCharString>.
  - 006be700: pair<long,CMusicManager::CRegisteredMusicEntry> (CWideString copy 0x0099B720).
  - Shape: `T* cur = result;` with the copy ctor declared out of line.
- **Colour records**
  - 006cac20: CCompositeBlendDesc.
  - 00720270: a 24-byte {4 words, 2 CRGBColours} record.
  - Shape: copying a CRGBColour through inline `Set(r, g, b, a)` gives retail's byte-shuffle loads with the `[esp+18h]` spills.
- **Intelligent pointers**
  - 006a31d0: pair<long,CIntelligentPointer<CThing>>.
  - 006a3130: NScreenEffect::CScreenEffectPriorityStack::CThingGraphicArgPasser (9 bytes, `pack(1)`).
  - Shape: a `__forceinline` copy ctor calls the base ctor 0x00A01B10, stores the vtable, then runs `SetPItem(other.GetPItem())`.
- **Other inline copy chains**
  - 0067c9f0: CBlurredAttitudesDef (0x48 bytes).
  - 006e1f80: pair<ulong,CParticleUpdateParamDesc<ulong,ulong>> (key, vtable, two words).
  - 006fbae0: pair<ulong,CSmallVector<NAnimationEvents::CEvent,32>>.
- **List copy**
  - 007705a0: list<CWeaponTrailEntry>, the same model as the landed CEnginePrimitiveWeaponTrail ctor 0x007703B0.
  - The STLport list copy ctor inlines fully: the proxy is zeroed, the sentinel is malloc'd and self-linked, then
    `insert(begin(), x.begin(), x.end())` runs.
  - The by-value `pos` iterator (user copy ctor) is built in its argument slot. The const_iterators are pushed directly. The
    empty tag is homed in the dead `result` slot.
- **Vector-of-vector destroys**
  - 0051cd80: CNavigationLayerUpdate. The implicit dtor runs SwitchableBlockages' `_Destroy`, which dispatches to the
    out-of-line `__destroy_aux<CSwitchableBlockage>` 0x00519EF0 with a temporary tag in the `push ecx` word. LineList (C2DLineF)
    only frees.
  - 007094d0 (`_Destroy`, plain `ret`) and 00770670 (`__destroy_aux`): vector<POD>.
  - 006b6f50: vector<SRecord30>.
  - 0076bd70: vector<CTCObjectAugmentations::ParticleAndEndPoints>. Field names follow the landed 0x007674A0 model.
  - 0076bdd0: vector<AttackParticleAndEndPoints>.
  - Shape: a local STLport vector model (start/finish/eos) whose dtor runs DestroyRange and then `free(_M_start)`.

## Parked for Fable 5.1 (0)

## Skipped
None. No row holds a second function: the extra bytes in every catalog length are int3/nop tail padding only, and the bounds agree
with the scanner-verified extents.

## Label corrections
- All 40 rows: the propagated `_Uninit_copy<_Vector_const_iterator<_Vector_val<unsigned int,allocator<unsigned int> > >,unsigned
  int*,allocator<unsigned int> >` should be relabelled with the function and element names in STATUS.tsv:
  - `std::__uninitialized_copy<E>` (`ret 8`)
  - `std::__destroy_aux<E>` (`ret 4`)
  - `std::_Destroy<E>` (plain `ret`: 00675310, 007094d0)
- 0x00519EF0 `_Move_backward<pair<unsigned long,CSoundPair>*>` is `std::__destroy_aux<CNavigationAreaUpdate::CSwitchableBlockage>`.
  It has 16-byte elements and clears a list<C2DLineF> at +8; the caller is 0x0051CD80.
- 0x00673E10 "std::vector_CopyRange" is the `vector<pair<unsigned long, CRandomAppearanceMorph::CTextureMorph> >` copy constructor.
  It is called for the CVectorMap member by 0x00674FA0.
- 0x0076FD40 "LinkedList_InsertAll" is `list<CWeaponTrailEntry>::_M_insert_dispatch(iterator, const_iterator, const_iterator,
  const __false_type&)` (`ret 10h`), called from 0x007705A0.
- 0x006753F0 "Vector_Assign_16byte_CFeatDef" is the `vector<CVectorMap<unsigned long, CRandomAppearanceMorph::CTextureMorph> >`
  assignment (CRandomAppearanceMorph::BodyPartTextureMorphs), not a CFeatDef helper. It calls 0x00675310.
