# Chunk 1023 notes

Every grade exported `FABLE_HDR_OVERLAY=$A/hdr` (see hdr/README.md for each opt-in). Grades were wrapped in `timeout 240`;
diagnostic compiles (tools/odis.py, tools/scan.py, c2prio) are not grades.

## Rows
| Row | Identity | Result | Lever |
|---|---|---|---|
| 0x006E3EC0 | `?Initialise@NSpeechGainManager@@YIXABVCWorld@@@Z` (Ego_r / FableWin; caller 0x004A76C0) | RELOCATION_MATCH (6) | FABLE_FORCEINLINE CResponseTimes(float, float) with an Inline-budget note (retail expands the ctor 3x but calls vector::erase; /d2inlT scan: no threshold splits them); release delta `frame * (-1 / release)` |
| 0x007CB8B0 | `?OnSerialise@CTCHeroReceiveItems@@MAEXAAVCPersistContext@@_N@Z` (vtable 0x0126EBFC slot 1) | RELOCATION_MATCH at 508 (2) | SPLITS.tsv: the catalog's 608 bytes cover 0x007CBAB0 / 0x007CBAE0 too; graded with tools/gsplit.py (q.py / gall.sh compare against 608 and print DIFFER(508v594): expected) |
| 0x0061A6A0 | `?OnCreate@CTCInventoryQuests@@UAEXXZ` (vtable 0x012585B4 slot 4) | RELOCATION_MATCH (3) | same-TU GetSubDef<CCreatureDef> body (no capture), do-while item loop, block-scoped `CWideString text = L"..."` |
| 0x00B67270 | `?BuildBlendingTables@CEngineLandscapeRenderer@@AAEXXZ` (Ego_r private) | RELOCATION_MATCH (4) | texel index `y * pitch + x * bpp + b` (strength-reduced into retail's reused slots), normal.Y from the invariant expression in the inner loop, PMem read before Pitch; GFFloatToLongFloor retail-asm helper (Retail-asm line) |
| 0x00417747 | `?UpdateForcedTickSpeed@CMainGameComponent@@QAEXXZ` | DIFFER(600v596), parked (0 grades here; 3 were left) | retail `fld [m] ; fld1 ; fdivrp st(1)` reciprocal: 20+ /O1 micro probes (probe/recip/r*.cpp) found no C++ spelling; only std::_Pow_int's loop-carried register float gives it. Needs an owner ruling or a second retail site for a shared helper |
| 0x0074B77C | not a function | NOT_A_FUNCTION (0) | interior of the landed 0x0074B770 CTCCreatureGenerator::InitialiseCreatureGenerator (GOTCHAS boundary note 0x74B77C -> 0x74B770); drop the row |
| 0x00C1C460 | libpk listener-space source update; owner unproven (`CUnprovenSoundWorld_00c1c460::UpdateSourcePositions`, descriptive) | DIFFER(445v445), parked (1 grade) | same-TU statics with private conventions (needs `/Gr`, the module's flag, for callee-pop), if/else assignment, coda assigned from Copy(). Residue: i / slot register swap (c2prio: slot prio 44 > i 34) + one 3-dword copy schedule |

## Callee names used
- 0x006E3EC0: vector<unsigned short>::_M_fill_insert 0x005149F0, _List_base<NSpeechGainManager::CActiveDialogueInfo>::clear 0x006E4B90,
  vector<CResponseTimes>::erase 0x006E4EA0 / _M_fill_insert 0x006E5240 (see NAME_FIXES.tsv for the landed misspellings),
  CMainGameComponent::GetConstantFPS (inline).
- 0x007CB8B0: as listed in its source (vector<CCharString>::reserve 0x004BA7A0, CTCBase::GetDefinitionManager 0x006869D0, ...).
- 0x0061A6A0: NHeroInformationScreens::CManager::RegisterScreen 0x0066EB11, CDefClassBase::GetSubDef<CCreatureDef> 0x00485815,
  NInventory::CTCInventoryBase::SetPDef 0x005BDB4D / OnCreate 0x005C0121.
- 0x00B67270: CPixelFormat::Initialise 0x009E3790 / GetByteColourDepth 0x009E3800, CTexture::Initialise 0x009FA280 / Lock 0x009FA450 /
  Unlock 0x009F9DE0, C3DVector::AccurateNormalise 0x00A14440, CEngineLandscapeMeshBuilder::GetMappingDirectionBlend 0x00BE2B70.
- 0x00C1C460: SwapYZ 0x00C1BFB0, BuildListenerMatrices 0x00C1BF00 (static, descriptive), C3DVector::Cross 0x00C25860 / DotProduct 0x00C256A0 /
  Copy 0x00C1BFF0, InvertMatrix3x4 0x00C1D1C0, TransformHelperPoint_00c1c3f0 0x00C1C3F0, TransformPointThroughMirroredFrame_00c1c330
  0x00C1C330, PKCProtectionInstance 0x00C0FFC0 / ~ 0x00C0FFE0 over g_soundLock 0x0143BAFC, PKCFrallocStatic<CPlaybackSlot>::Get 0x00C13700,
  CQuadHolder::GetValue 0x00C34600, CODA_SetSourcePosition 0x00C33FD0 (stdcall; landed as SetVectorA with a camera model),
  GlobalMethodTailThunk_00c33690 0x00C33690.

## Name notes (not proven enough for NAME_FIXES)
- 0x00C1C460 is bound three ways in landed callers (`?Refresh@CWorld@@QAEXXZ` in 0x00C1C620, `?Step@CTCInventoryMagic_00c1c6a0@@QAEXXZ`,
  `?Update@CTarget_00c10a90@@QAEXXZ`); all descriptive. This chunk used `CUnprovenSoundWorld_00c1c460::UpdateSourcePositions`; the
  coordinator should pick one spelling for all four sites. The landed 0x00C1C620 "CWorld" is that same libpk object (+0x18 Paused).
- 0x00C33FD0 is landed as `?SetVectorA@@YGXPAUCameraConstants@Row00c41c40@@PBUVec@2@@Z`; here it takes (CODA source handle, position)
  -- the landed camera model looks propagated.
- 0x013B8C64 / 0x013B8C68 (landed g_testQuestIndex / g_testQuestCount) are the quest-card item selection index / count
  (0x0061A1A0 IncrementQuestCardItemSelection, landed as vfunc_71 of CTCInventoryQuests, vtable slot 71, walks them); the
  established spellings were kept.

## Header debt
- engine/NSpeechGainManager.h and engine/CQuestCardItem.h are new headers (move to rebuild/include/engine).
- libpk/PKCFrallocStatic.h is new; the landed accessor rows 0x00C13700 / 0x00C13620 still carry local copies.
- The libpk C3DVector out-of-line members (Cross / DotProduct / Copy) are landed with local class models in three different
  spellings (x/y/z vs X/Y/Z); FABLE_RETAIL_C3DVECTOR_LIBPK_OUTOFLINE is the shared-header form.

## Next steps for parked rows
- 0x00417747: owner ruling on the `fld1 ; fdivrp` reciprocal (no C++ spelling found), or find a second retail site to justify a
  shared helper.
- 0x00C1C460: 9 grades left. Make i outrank slot in c2's priority (c2prio --blocks on differ/00c1c460.cpp shows the terms), then
  re-check the 3-dword copy schedule at 0x00C1C595 (it may follow the register change).
