# Hand chunk 593 (STLport vector/sort helpers, particle list, LZO, libpk): 9 of 10 byte-exact

All nine matched rows grade RELOCATION_MATCH. Each test prints `<addr>_TEST PASS` under g++ (tall.sh) and VC7.1 (tall.py).
Every `// Leaf:` was checked with `objdump -t` on the graded object. No retail_asm rows; no `__asm` anywhere.

## What matched

| Address | Bytes | Name | Proof of name |
|---|---|---|---|
| 0x00BDFF50 | 316 | `std::vector<C3DBoundingSphere>::_M_fill_insert` | caller resize 0x00BE0140 binds this exact spelling; callees 0x00BDFE70 / 0x00BDF880 bound over C3DBoundingSphere; 16-byte stride = Ego_r.pdb C3DBoundingSphere |
| 0x00AEB190 | 318 | `NParticleEngine::CParticleEmitterList::Insert` (private) | Ego_r.pdb / FableWin.pdb name set (the only member taking (list&, const CCountedPointer&, long)); its only caller FindVisible 0x00AEB2D0 |
| 0x00AF2010 | 318 | `std::vector<NParticleEngine::CParticleEmitterInfo>::_M_fill_insert` | caller resize 0x00AF2170 binds it; callees 0x00AF1E90 / 0x00AF1C10 / 0x00AF1AA0 bound over CParticleEmitterInfo; 12-byte stride |
| 0x00B85B90 | 319 | `std::__chunk_insertion_sort<CSortElement*, int, less<CSortElement> >` | caller __merge_sort_with_buffer 0x00B86260 binds it; STLport 4.6 _algo.c verbatim |
| 0x00B85CD0 | 320 | `std::__rotate_adaptive<CSortElement*, CSortElement*, int>` | only caller __merge_adaptive 0x00B862C0 (bound, call 0x00B8636B); STLport 4.6 verbatim |
| 0x00C11AC0 | 320 | `CSoundEventManager::FindFirstEntryState` (descriptive) | class: vtable 0x012B2748 slot 17, stored by ~CSoundEventManager 0x00C13470; method name descriptive |
| 0x00C06F30 | 326 | `swd_init` | LZO 1.08 lzo_swd.ch static (neighbours swd_initdict 0x00C06DE0, swd_insertdict 0x00C06E50, caller init_match 0x00C075C0) and the catalog |
| 0x00B4A2C0 | 327 | `std::vector<CEngineLightingManager::CThemeChannel>::_M_fill_insert` | caller resize 0x00B4A4C0 (Ego_r.pdb); callee _M_insert_overflow 0x00B4A0E0 names it its only caller; 112-byte stride = Ego_r.pdb CThemeChannel |
| 0x00AF7570 | 328 | `std::vector<std::pair<NParticleEngine::EPSCType, NParticleEngine::CComponentInstance> >::_M_fill_insert` | resize 0x00AF7890 path binds it; _M_insert_overflow 0x00AF7460 (chunk 585) names it its caller; 24-byte pair |

## Parked

- **0x00C1A870 `PKCDefaultFileManager::ReleaseFile(unsigned long key)`** (descriptive). Best source:
  `differ/00c1a870.cpp`, DIFFER(325v321).
  - Residue: frame 0x14 vs 0x10. Retail homes the trace CString in the dead `value` out-slot (both at frame+4, the
    CDebugBreak byte at frame+3); VC7.1 here gives them separate slots. All esp offsets then shift by 4.
  - The extra slot also moves `&m_files` from retail edi to ebp. The count test CSEs `lea esi,[file+4]` where retail keeps
    the record in esi.
  - Tried: `(void**)&file` (343), the `void*` local (325, kept), value in a sibling block (332), an inline typed Lookup
    helper (337), the count in a local (326).
  - Next step: this looks like the known lane gap of short-lived EH locals homed in one dead slot (chunk 589 0x00C4AEB0).
    Untried: the trace block as an inline assert helper, so its CString lives in an inlinee scope.
  - Test `test/00c1a870.cpp` passes under both compilers.

## Skipped

None.

## Label corrections (every catalog label in this chunk was wrong)

| Address | Catalog label | Correct |
|---|---|---|
| 0x00BDFF50 | std_vector_insert_fill_at_16byte | std::vector<C3DBoundingSphere>::_M_fill_insert |
| 0x00AEB190 | List_InsertSorted_ByVisibility | NParticleEngine::CParticleEmitterList::Insert |
| 0x00AF2010 | Vector_ResizeN | std::vector<NParticleEngine::CParticleEmitterInfo>::_M_fill_insert |
| 0x00B85B90 | IntroSortInner | std::__chunk_insertion_sort<...> |
| 0x00B85CD0 | Merge_RotateElements | std::__rotate_adaptive<...> |
| 0x00C11AC0 | Combat_IterateAnimationsForCombo (propagated) | CSoundEventManager::FindFirstEntryState (descriptive) |
| 0x00C1A870 | FileManager_CloseHandleOnException | PKCDefaultFileManager::ReleaseFile (descriptive) |
| 0x00C06F30 | swd_init | swd_init (right) |
| 0x00B4A2C0 | Vector_Resize_Struct0x70 | std::vector<CEngineLightingManager::CThemeChannel>::_M_fill_insert |
| 0x00AF7570 | Vector_InsertRangeResize | std::vector<pair<EPSCType, CComponentInstance> >::_M_fill_insert |

swd_init is the one label that was right.

## Callee names

- **STLport helpers**: every helper uses its real `std::` template spelling (`__uninitialized_copy`,
  `__uninitialized_fill_n`, `uninitialized_fill_n`, five-argument `__copy_backward`, `fill`, `_M_insert_overflow`,
  `__insertion_sort`, `__rotate`).
  - Where a landed placeholder spelling exists, NAME_FIXES.tsv gives the correction: 0xBDF580, 0xAF1B30, 0xAF6E10,
    0xB48EF0, 0xB497B0, and the 0xB85870 caller binding.
- **0x00C196A0 `CObjectMap_00c196a0::Find`**: its own landed spelling. Five callers bind other placeholders (NAME_FIXES).
- **0x00C119D0**: slot 18 of CSoundEventManager, landed as `CCombatStateTable_00c119d0::ProcessAnimationState`.
  - FindFirstEntryState calls it virtually as `GetEntryState(tag, entry)` (descriptive).
  - Class correction in NAME_FIXES.
- **libpk, matching chunks 589/590**: PKCProtectionInstance 0x00C0FFC0 / 0x00C0FFE0, CString::operator<< 0x00C0DEC0 /
  0x00C0DF00, ~CString 0x00C0BA40, CDebugBreak 0x00C16750, `_afxPchNil`, `g_pfnAssertHook`.
- **0x00C1AE50 `PKCTRRedblackMap::Find`**: the ctor 0x00C1AE20 and QueueRead spelling. NAME_FIXES covers the
  `PKCTRRedblackMap_00c1ae50` / `COpenFileMap_00c1a070` placeholders.
- **0x00C1B7D0 `PKCTRRedblackMap::Remove`** (descriptive). The forge label `CGameMap_RemoveByKey` is wrong.

## Header debt

- `engine/CEngineLightingManager.h` has no nested `CThemeChannel`; ThemeChannels is an opaque `unsigned char[0xc]`.
  0x00B4A2C0 carries a local 112-byte nested model (the landed 0x00B4A4C0 / 0x00B4A0E0 model).
- `engine/CParticleEmitter.h` has two problems:
  - It is flattened (vptr as data, the +0x40 bitfield word opaque) and in the global namespace.
  - It cannot name `NParticleEngine::CParticleEmitter::Priority` (ushort:3 at +0x40) or DistanceToCam (+0x154).
  - 0x00AEB190 uses a local pack(1) model, as the landed FindVisible does.
- No engine headers exist for:
  - NParticleEngine::CParticleEmitterInfo, CComponentInstance, CCountedPointer / CCPPointerInfo, C3DBoundingSphere /
    C3DVector (both with user memberwise copy ctors here);
  - the libpk classes (CSoundEventManager, PKCDefaultFileManager, PKCTRRedblackMap, CString, PKCProtectionInstance);
  - LZO's lzo1x_999_t / swd (taken from the parked swd_accept file, chunk 588).
- 0x00AF2010 uses the shared `engine/CIVCountedPointeeBase.h` (after `rebuild_abi.h` and `FABLE_RETAIL_DEF_CLASS_BASE`).
  0x00AF7570 uses `engine/CBaseClass.h`.
- `engine/CEngineSubPrimitiveRenderer.h` exists, but it has no nested `CSortElement` (SortBuffer is an opaque
  `unsigned char[0xc]`). 0x00B85B90 / 0x00B85CD0 therefore carry a local class with the nested 8-byte CSortElement, as the
  landed stable_sort siblings do. vland warns SHARED_HEADER_AVAILABLE for both; including the header would redefine the
  class.

## vland (read-only)

- WINS 9, no RELOC_IDENTITY, no DIFFER among src/ rows.
- RELOC_NAME warnings, all label-catalog spellings:
  - 0x00AF1C70 `CIVCountedPointer_Attach_00af1c70` is `std::fill` (NAME_FIXES).
  - 0x00B497B0 `UninitFillN_00b497b0` (NAME_FIXES).
  - 0x00BDF880 `UninitCopy_SElement16_00bdf880`: the reloc binding is already the real template.
  - 0x00AF6980: the label is the same template, demangled.
  - 0x00BDF600: labelled `_Move_backward<pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo>*>`.
    The family-pass-5 label says `std::__copy_backward<C3DBoundingSphere*, C3DBoundingSphere*, int>`, and this row's
    match proves the C3DBoundingSphere caller. If the body is ICF-folded with a 16-byte CPersistInfo pair, one name
    must win at landing (review item).

## Descriptive names (libpk has no PDB or RTTI)

- CSoundEventManager::FindFirstEntryState, and the slot-18 GetEntryState.
- PKCDefaultFileManager::ReleaseFile and PKCTRRedblackMap::Remove.
- Classes in 0x00C11AC0: CUnprovenEntryTable_00C11AC0 (slots 4 GetEntryCount / 5 GetGroupCount / 6 GetEntry),
  CObject_00c196a0 (slot 5 GetEntryTable) and CUnprovenOwner_00C11AC0.
- Vtable slots with no evidence are `UnprovenSlotNN`.
