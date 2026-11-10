# chunk 900 notes (agent900)

## Job 1 -- DirectShow BaseClasses adjustor{4} thunks (vendor-library route)
- Vendor: `vendor_libs.tsv` row `strmbase-dxsdk-sum2004` (DX9.0c SDK Summer 2004 BaseClasses), fetched with
  `fetch_vendor_libs.py` into `$S/thirdparty` (archive sha256 ok, tree 5941d6f4...; fetch.log); objects built by `vendor_lib.py`
  with the row's flags (/O2 /W3 /MD /GS /EHsc /Gz /GF /Gy ...), cache `$S/thirdparty/obj/strmbase-dxsdk-sum2004-ce5ecc757e88a256/`.
  `FABLE_WINDDK_INC=/home/user/9176324/winddk/3790.1830/inc/wnet` was needed (the `$S/wine/winddk` default path is absent).
  The first amfilter.cpp build hung Wine at the 240 s timeout after writing the object (kept as amfilter_first.obj); a rebuild took 1.4 s.
- Identities (two sources each: the vendor object's vtable relocations and retail vtables):
  vendor `??_7CRendererInputPin@@6BIQualityControl@@@` = [CBaseInputPin W3 QI/AddRef/Release, Notify@CBaseInputPin, SetSink@CBasePin]
  = retail 0x012BF3AC [0x00CA6170, 0x00CA6180, 0x00CA6190, 0x00CA8E70, 0x00CA8560];
  vendor `??_7CDynamicOutputPin@@6BIQualityControl@@@` = [CDynamicOutputPin W3 thunks, Notify@CBasePin, SetSink@CBasePin]
  = retail 0x012BF748 [0x00CAA720, 0x00CAA730, 0x00CAA740, 0x00CA8590, 0x00CA8560].
- Sources: 899's hand models with a `// Vendor-proof:` line 2; all six RELOCATION_MATCH, tests PASS (g++ and VC7.1).
- `lib_verify.py --source` output (libverify.log), 6/6 MATCH:
```
00ca6170 MATCH strmbase-dxsdk-sum2004(BaseClasses/renbase.cpp) ?QueryInterface@CBaseInputPin@@W3AGJABU_GUID@@PAPAX@Z 10 bytes -- vendor strmbase-dxsdk-sum2004(BaseClasses/renbase.cpp) [/O2 /W3 /MD /GS /EHsc /Gz /GF /Gy /DNDEBUG /D_MBCS /D_LIB /D_WIN32_DCOM /DWINVER=0x400 /DWIN32 /DUNICODE] section 286 .text 10 bytes at 0x00CA6170, 4 masked relocation bytes, 1/1 relocations proven: +6:REL32:?QueryInterface@CBaseInputPin@@UAGJABU_GUID@@PAPAX@Z=0x00CA53C0[ok:lib]
00ca6180 MATCH strmbase-dxsdk-sum2004(BaseClasses/renbase.cpp) ?AddRef@CBaseInputPin@@W3AGKXZ 10 bytes -- vendor strmbase-dxsdk-sum2004(BaseClasses/renbase.cpp) [/O2 /W3 /MD /GS /EHsc /Gz /GF /Gy /DNDEBUG /D_MBCS /D_LIB /D_WIN32_DCOM /DWINVER=0x400 /DWIN32 /DUNICODE] section 287 .text 10 bytes at 0x00CA6180, 4 masked relocation bytes, 1/1 relocations proven: +6:REL32:?AddRef@CBaseInputPin@@UAGKXZ=0x00CA53F0[ok:lib]
00ca6190 MATCH strmbase-dxsdk-sum2004(BaseClasses/renbase.cpp) ?Release@CBaseInputPin@@W3AGKXZ 10 bytes -- vendor strmbase-dxsdk-sum2004(BaseClasses/renbase.cpp) [/O2 /W3 /MD /GS /EHsc /Gz /GF /Gy /DNDEBUG /D_MBCS /D_LIB /D_WIN32_DCOM /DWINVER=0x400 /DWIN32 /DUNICODE] section 288 .text 10 bytes at 0x00CA6190, 4 masked relocation bytes, 1/1 relocations proven: +6:REL32:?Release@CBaseInputPin@@UAGKXZ=0x00CA5510[ok:lib]
00caa720 MATCH strmbase-dxsdk-sum2004(BaseClasses/amfilter.cpp) ?QueryInterface@CDynamicOutputPin@@W3AGJABU_GUID@@PAPAX@Z 10 bytes -- vendor strmbase-dxsdk-sum2004(BaseClasses/amfilter.cpp) [/O2 /W3 /MD /GS /EHsc /Gz /GF /Gy /DNDEBUG /D_MBCS /D_LIB /D_WIN32_DCOM /DWINVER=0x400 /DWIN32 /DUNICODE] section 517 .text 10 bytes at 0x00CAA720, 4 masked relocation bytes, 1/1 relocations proven: +6:REL32:?QueryInterface@CDynamicOutputPin@@UAGJABU_GUID@@PAPAX@Z=0x00CA96F0[ok:lib]
00caa730 MATCH strmbase-dxsdk-sum2004(BaseClasses/amfilter.cpp) ?AddRef@CDynamicOutputPin@@W3AGKXZ 10 bytes -- vendor strmbase-dxsdk-sum2004(BaseClasses/amfilter.cpp) [/O2 /W3 /MD /GS /EHsc /Gz /GF /Gy /DNDEBUG /D_MBCS /D_LIB /D_WIN32_DCOM /DWINVER=0x400 /DWIN32 /DUNICODE] section 518 .text 10 bytes at 0x00CAA730, 4 masked relocation bytes, 1/1 relocations proven: +6:REL32:?AddRef@CDynamicOutputPin@@UAGKXZ=0x00CA9700[ok:lib]
00caa740 MATCH strmbase-dxsdk-sum2004(BaseClasses/amfilter.cpp) ?Release@CDynamicOutputPin@@W3AGKXZ 10 bytes -- vendor strmbase-dxsdk-sum2004(BaseClasses/amfilter.cpp) [/O2 /W3 /MD /GS /EHsc /Gz /GF /Gy /DNDEBUG /D_MBCS /D_LIB /D_WIN32_DCOM /DWINVER=0x400 /DWIN32 /DUNICODE] section 519 .text 10 bytes at 0x00CAA740, 4 masked relocation bytes, 1/1 relocations proven: +6:REL32:?Release@CDynamicOutputPin@@UAGKXZ=0x00CA9710[ok:lib]
```

## Job 2 -- splits
- 0x00C3EC10 (27-byte row) = CCodaSystem::CreateHardwareVoiceA (11 bytes, `mov ecx,[ecx+1d4h]; mov eax,[ecx]; jmp [eax+10h]`, B the same with [eax+14h];
  tail call into the audio context) + 5 int3 + NEW START 0x00C3EC20 CCodaSystem::CreateHardwareVoiceB (11 bytes). Vtable 0x012B5BC0
  = [0xc3f360, 0xc3e320, 0xc3e330, 0xc3e580, 0xc3ec10, 0xc3ec20, 0xc3f970, 0xc3ebb0]; Ego_r/FableWin `?CreateHardwareVoiceA/B@CCodaSystem@@UAEPAVCCodaVoice@@XZ`.
  CAudioContext vtable 0x012B8C18 slots 4/5 = 0xC56CD0 / 0xC56860. Both MATCH.
- 0x00F29FF0 (219-byte row) = CCutsceneDef::Copy (5 bytes, tail `jmp 0x00F2A000`) + 11 int3 + NEW START 0x00F2A000
  CCutsceneDef::operator= (203 bytes; `ret 4` at 0x00F2A0C8 ends at 0x00F2A0CB). Both RELOCATION_MATCH with the overlay.
- SPLITS.tsv rows written; oracle lines via split_oracle.py into `$S/batch/chunk900_oracle.tsv`.
- Lever for 0x00F2A000: implicit operator= with the parent-def forced-inline chain (FABLE_RETAIL_PDCB_FORCEINLINE_ASSIGN +
  CDEFCLASSBASE_IMPLICIT_ASSIGN + CRESOURCE_ASSIGN + STLP_VECTOR_ASSIGN) **plus** a new declaration-only CVectorMap::operator=
  (FABLE_RETAIL_CVECTORMAP_ASSIGN_OUTOFLINE). Without it 215v203 (CVectorMap expanded via ebx/ebp); with PDCB_IMPLICIT_ASSIGN
  instead 129v203 (whole parent def called as 0x00431ED0).

## Job 3 -- CTCInventoryWeapons
- New shared header hdr/engine/CTCInventoryWeapons.h (layout proven by compile-time asserts and the screens' member accesses; see hdr/README.md).
- 0x005C8459 ConstructScreen (console): drafted, parked `differ/005c8459.cpp`, DIFFER(3299v3235), frame-masked instruction share 0.865.
  Needs next:
  1. CDefPointer release shape: retail releases the loop's weapon-def pointer and the shot-def pointer as `dec dword ptr [ecx+4] ; jne`
     with no `Object = 0` store; ours (engine/CDefPointer.h dtor in this TU) keeps `mov ecx,eax ; add eax,4 ; dec [eax] ; cmp [eax],edi`
     + null store. Probably a different holder (a `CDefPointer` temporary vs a named local, or the release inlined from
     CDefPointerBase) -- compare with the landed 0x005F519F CTCInventoryAbilities::ConstructScreen (Codex) which has the same loop shape.
  2. Frame 0xE4 v 0xE0 and slot map: one local too many (likely the `guideBank` const ref or one of the named title pointers).
  3. Second title `SetValue(CCharString)`: ours loads the vtable into ebx before the string is built; retail after.
  4. `augmentations.size()` test: retail `sub ; sar eax,3 ; je`, ours `test ecx,mask` -- try `!augmentations.empty()` vs `size() != 0`.
  5. Local models still unproven: `CWeaponDef` (Type +0x28, Class +0x2c, Shot +0xa0) and `CUnprovenShotDef_005C8459` (Damage +0x54);
     the def owners need two-source names (Ego_r CWeaponDef? / CProjectileDef?) and shared headers.
- 0x005C9A95 ConstructScreenPC: NOT drafted (budget). Same header; flow (prior/899 NOTES + ann_005c9a95.txt): SetWeaponsInventory (CManager
  slot 15) / SetPlayerThing (slot 26), PC_UI_FRAME, PC_SUB_LIST, PC_DESCRIPTION_VIEWPORT, ConstructAugEntryPC 0x005C5F11, PC_TITLE_WEAPONS,
  PC_MENU_ENTRY_BRANCH_TEMPLATE, TEXT_GUI_MENUGUIDE_WEAPONS_MELEE_PC / _RANGED_PC, PC_SUBTITLE, PC_RING_PIC_MELEE / _RANGED,
  ConstructWeaponItemEntriesPC 0x005C8176, stores the frame at WeaponsList +0x17C. Start from differ/005c8459.cpp.
- 0x005B8743 CTCInventoryClothing::ConstructScreenPC: NOT drafted. ann_005b8743.txt annotated; build on the repo CTCInventoryClothing.h
  (Codex opt-ins _CALCULATE / _ARMOUR / _MENUENTRIES, fba8d6164) and the Codex model 0x005B4EBA CalculateArmorValues.

## Callee names used (LABELS.tsv)
CTCWeapon::GetDamage 0x005DAAA0, CTCWeapon::IsMeleeWeaponClass 0x005DA510, CTCInventoryItem::IsAugmentation 0x005D9870,
ConstructAugEntry 0x005C574E / PC 0x005C5F11, ConstructWeaponItemEntries 0x005C66B2 / PC 0x005C8176, CVectorMap op= 0x006313A0,
vector<CCharString> op= 0x00432EE9, CResource::operator= 0x00431F60, CGameDefinitionManager::Get 0x0044C6B0, CTCBase::GetDefinitionManager 0x006869D0.
CList vtable: 0x240 AddScrollingChild, 0x244 AddNonScrollingChild, 0x260 AddScrollingChildAndSetOffsets, 0x270 InitialiseOffsets;
CComponent: 0x0C FindChild, 0xEC AddChild, 0xF8 SetDeletion, 0x14C SetName, 0x170 MakeUnavailable; CManager 0x3C SetWeaponsInventory,
0x68 SetPlayerThing, 0x78 SetNumberAugmentations.

## Header debt
- 0x00C3EC10/20 use a local CAudioContext / CCodaSystem model (only +0x1d4 and two vtable slots). The landed 0x00C3EB60/80 call the
  same object (+0x1d4) as a `CEAXDevice` -- one retail object, two local names; a shared engine/CCodaSystem.h should settle it.
- CVectorMap ASSIGN comment wrong (see hdr/README.md). CEnginePrimitiveHandle.h fails to compile in the CTCInventoryWeapons TU.
- NAME_FIXES.tsv: 0x006313A0 (landed as CTavernTableDef::operator=) and 0x00432EE9 (landed under five vector-wrapper names).
