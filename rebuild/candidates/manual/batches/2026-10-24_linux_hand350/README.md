# Hand chunk 350 (_Copy_backward<CTCInventoryMap::CUIMiniMapMarker*> .. _Copy_impl<CTCSmokeGenerator::CGenerationPos*>): 38 of 40 byte-exact

All 38 matched sources grade MATCH / RELOCATION_MATCH under RTM cl 13.10.3077 (gall.sh) and their tests print
`<addr>_TEST PASS` under g++ (tall.sh) and the lane's VC7.1 (tall.py). The rows are COMDAT-folded STL helper bodies,
so the propagated manifest labels name only one of the element types; every model below is sized from the bytes and
named from Ego_r.pdb where a TU neighbour or caller pins the type, otherwise descriptive.

## What matched (38)
- **SGI `__copy_backward(first, last, result, const random_access_iterator_tag&, Distance*)`** (/Gr __fastcall, `ret 0Ch`,
  `for (n = last - first; n > 0; --n) *--result = *--last; return result;`):
  - POD elements -- 0x00A314E0 pair<unsigned long, C3DDummyObject> (14-dword `rep movsd`), 0x00A522B0 / 0x00A94A00
    C3DVectorWord (6 bytes: dword + word), 0x006B7A00 7-byte record (`unsigned char[7]`: dword + word + byte).
  - CCharString / CWideString out-of-line operator= members -- 0x00613390 CUIMiniMapMarker, 0x00CB62F0
    pair<CCharString, CScriptInfo>, 0x006BEAA0 pair<long, CRegisteredMusicEntry>, 0x00A31060 0x40 record
    (13-dword block + name + index + flag).
  - CCountedPointer (the landed 0x0084F130 inlined operator=, source fields read first) -- 0x00CBAE30
    pair<CCharString, CCountedPointer<..>>, 0x00A70AB0 pair<CCharString, C3DMeshVolume>, 0x00A33050 / 0x00A70A10
    pair<unsigned long, C3DMeshVolume>, 0x00AF6E10 pair<EPSCType, CComponentInstance>.
  - CIVCountedPointer (intrusive, release through virtual slot 1, clear, adopt) -- 0x005E0A00
    pair<EAnimComponent, CIVCountedPointer<CAnimComponentBase>>, 0x00A43720 CDataBankTextEntry, 0x00AF1B30
    CParticleEmitterInfo.
  - CIntelligentPointer (SetPItem(other.GetPItem())) -- 0x006A2EB0 pair<long, CIntelligentPointer<CThing>>.
  - out-of-line member operator= -- 0x009929C0 (0x24 pair, record op= 0x00992630), 0x008893D0 (0x30 record, vector
    op= 0x00888EE0 + a double copied by fld/fstp).
  - polymorphic second member: 0x00769EA0 pair<ul, CParticleUpdateParamDesc<ul,ul>> and 0x00AF6E10 skip the word at +4
    (the vtable the implicit operator= never copies) -- the PDB layouts (vtable +0) confirm the types.
- **SGI `__copy`** (same front end, `*result = *first; ++first; ++result;`): 0x00A52120 / 0x00A52260 / 0x00A744B0
  C3DVectorWord, 0x00722C90 CTCSmokeGenerator::CGenerationPos (C3DVector with a user copy ctor -> pointer-register copy),
  0x00769B90 (two strings + three SEPARATE floats: a C3DVector member copies through pointer registers instead),
  0x00769C00, 0x00781720 (strings, words, intelligent pointers), 0x00992A10 / 0x00A5E830 (out-of-line record /
  vector operator=; 0x00A5E830's holder is polymorphic, +4 skipped), 0x006F0640 (byte + C3DVector at +1: a
  `#pragma pack(1)` 13-byte block with a memberwise user operator=; the implicit one block-copies 13 bytes).
- **STLport `__copy_backward_ptrs(first, last, result, const __false_type&)`** (`ret 8`, the SGI __copy_backward
  inlined: result loaded first, source addressed as result + (last - result)): 0x00A95A70 (0x20 POD), 0x0047130E
  (8-byte pair, /Os TU: inner helper `__forceinline`), 0x004ED2F1 (0x48 record, /Os + `optimize("y", off)` frame,
  inner helper and the record's memberwise operator= `__forceinline`).
- **SGI `__unguarded_linear_insert(last, T val, Compare comp)`** (labelled _Copy_backward): 0x008074A0 pair<C3DVector,long>
  with the 0x2C-byte CCompareTargetedPositionsForFirstPersonTargeting (`a > b` on GetCompareValueForPoint 0x007F9660),
  0x005E07F0 pair<EAnimComponent, CIVCountedPointer<..>> with CKeyPairCompareLess (callee destroys `val`).
- **STLport `__uninitialized_copy(first, last, result, const __false_type&)`**: 0x00CB60F0 pair<CCharString, CScriptInfo>
  (placement new with null test, STLport pair user copy ctor, CCharString copy ctor 0x0099EC30).
- **copy into an output iterator**: 0x008ED4D0 / 0x008ED510 -- the def-name inserter (operator= 0x008ED3B0 / 0x008ED440)
  is returned through a hidden pointer (ecx; first in edx) because it has a user constructor; the loop is written in the
  out-of-line function itself.

## Parked for Fable 5.1 (1)
- 006c1ed0 _global::CopyIf_HasBuildingAsHomeOrWork DIFFER(241v240): `mov ecx,eax ; add ecx,168h/170h` where retail has `lea ecx,[eax+168h/170h]` before the building GetPItem calls; callee-saved pushes after the empty-range test (retail pushes ebx/ebp/esi/edi first), ebx/ebp and esi/edi swapped | tried: plain loop + inlined STLport list push_back, prev read once into a local, CBaseObject-derived creature + static_cast operator->, derived CIntelligentPointer<CThingBuilding> members + `*dest++` | next: an inline `GetHomeBuilding()/GetWorkBuilding()` accessor returning a reference (the lea form), a named `const CCreaturePointer& p = *first;` local, or the predicate taking a `CThingAICreature*` fetched once

## Skipped
- 00564d5d: wrong bounds -- the row starts mid-function. The function begins at 0x00564D50 (`mov ecx,[esp+0Ch] ; test ecx,ecx
  ; jne 00564D5D ; mov eax,[esp+4] ; ret`), 42 bytes, a cdecl memcpy-like `(dest, src, n)` that returns dest (rep movsd +
  rep movsb); the catalogued 0x00564D5D is the `n != 0` branch target. No manifest row exists for 0x00564D50.

## Label corrections
- 0x008074A0 `_Copy_backward<CWaterSeaGenerator::CVertexDef*>` -> `__unguarded_linear_insert<pair<C3DVector,long>*, pair<C3DVector,long>, (anon)::CCompareTargetedPositionsForFirstPersonTargeting>`.
- 0x005E07F0 `_Copy_backward<NGameText::CDataBankTextEntry*>` -> `__unguarded_linear_insert<pair<EAnimComponent,CIVCountedPointer<CAnimComponentBase> >*, .., CKeyPairCompareLess<..> >`.
- 0x005E0A00 `_Copy_backward<NGameText::CDataBankTextEntry*>` -> `__copy_backward<pair<EAnimComponent,CIVCountedPointer<CAnimComponentBase> >*>`.
- 0x00AF1B30 `_Copy_backward<NGameText::CDataBankTextEntry*>` -> `__copy_backward<NParticleEngine::CParticleEmitterInfo*>`.
- 0x00AF6E10 `_Copy_backward<pair<long,CCountedPointer<CNavConnectorNode> >*>` -> `__copy_backward<pair<NParticleEngine::EPSCType,NParticleEngine::CComponentInstance>*>`.
- 0x00A314E0 `pair<CCharString,C3DDummyObject>` -> `pair<unsigned long,C3DDummyObject>` (0x38; the caller 0x00A33800 landed by chunk 330).
- 0x00A522B0 / 0x00A94A00 `pair<float,CCharString>` -> 6-byte C3DVectorWord copy_backward; 0x006B7A00 -> 7-byte record; 0x009929C0 -> 0x24 (long, record) pair.
- 0x00A95A70 / 0x0047130E / 0x004ED2F1 -> STLport `__copy_backward_ptrs` (0x20 POD, 8-byte unsigned-key pair, 0x48 record).
- 0x00769B90 and 0x00781720 are FORWARD `__copy` bodies (labelled _Copy_backward); 0x00769C00 / 0x006F0640 are not CGenerationPos; 0x00992A10 / 0x00A5E830 are not CSkeletalMorph; 0x008893D0 holds no CCharString.
- 0x00CB60F0 `_Copy_backward<pair<long,CRegisteredMusicEntry>*>` -> `__uninitialized_copy<pair<CCharString,CScriptInfo>*>`.
- 0x008ED4D0 / 0x008ED510 `_Copy_impl<CArray<CBodyPartMesh>*>` -> copy into the persist def-name output iterator (0x008ED3B0 / 0x008ED440 are its operator=, not vector push_back).
- Callees: 0x00992630 (`CGameCameraManager::UpdateCleanupFlags`) is a 0x20-byte record operator=; 0x00A5E4D0 and 0x00888EE0 are vector operator=; 0x00431F10 (`CThingBuildingDef::operator=`) is the operator= of a CDefPointeeBase-derived 0x28-byte def (type unverified).
