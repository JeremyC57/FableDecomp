# agent899 (chunk 899) notes

Result: 25 of 26 tiny rows matched (11 MATCH, 14 RELOCATION_MATCH), every one on its first grade; 1 EXTENT_WRONG (0x00C3EC10);
the 3 large PC inventory rows parked with their identities settled (not drafted). Grade with `FABLE_HDR_OVERLAY=$A/hdr` (only
0x01224900 needs it).

## Identities proven (row -> what it is, lever)
- 0x00C10720 CCollectAllItems::Visit: vtable 0x012B2624 slot 0 (base 0x012B260C: _purecall, deleting dtor). `m_pList->Add(item)`,
  the list's slot 9 = CItemList::Add 0x00C09C20 (`m_pImpl->Add(this, item)`, ret 4). Class name = landed RunAllItems 0x00C10400.
- 0x00C1FD50 / FDF0 / FE90 / 0x00C202C0 `<Item>::Validate`: slot 3 of the four 9-slot item vtables 0x012B3594 / 0x012B35B8 /
  0x012B35DC / 0x012B3600 (the classes of the landed ctors 0x00C1FCC0 / 0x00C1FD60 / 0x00C1FE00 / CNamedItem 0x00C20230).
  `GetTable()->Validate()`: slot 5 is `lea eax,[ecx+8]` (the table member), the table's slot 9 is the landed CItemTable::Validate
  0x00C348F0. Each base (0x012B3528 / 354C / 3570 / 3474) has a byte-identical slot 3 (0x00C1FAB0 / FB90 / FC70 / 0x00C1F6D0, not mine).
- 0x00C37A00 / 0x00C37A30: slots 1 and 4 of the sound emitter vtable 0x012B5370 (dtor 0x00C37DA0 stores it; landed siblings
  slot 2 SetScalar 0x00C37A10, slot 5 SetPosition 0x00C37A40). `m_pOwner(+4)->DestroyInstance(this)` with the handle argument
  unused: VC7.1 overwrites the dead argument slot with `this` and tail-jumps. Names Release / Stop are descriptive (both destroy).
- 0x00C49610: CCodaFrameStreamer slot 1 (vtable 0x012B8138): `return m_doneQueue.GetCount();` (+0x1c CMailboxQueue).
- 0x00C53410 / 20 / 30: dirty-flag setters of the landed CUnprovenReverbParameters block (+4/+8/+0xC; bits 0x20/0x40/0x80). The
  bit of every setter in 0x00C53410..0x00C53A40 is 1 << the EAX 3.0 listener property id (0x100 = DECAYTIME on the float clamped
  to [0.1, 20]), so these are ROOM / ROOMHF / ROOMLF.
- 0x00C5DFD0 / E0 / F0 / 0x00C5E000: CKsObject (vtable 0x012B90D4, handle +4): ctor, IsValid (BOOL), GetHandle, WriteStream
  (0x002F8013 = IOCTL_KS_WRITE_STREAM). The landed rows spell this one class twice: CKsObject (SetProperty 0x00C5E050) and
  CVtable_012b90d4_00c5ec50 (dtor 0x00C5EC50, Reset 0x00C5E3F0, ??_G 0x00C5EC00) / CVtable_012b90d4_00c5ebd0 (dtor copy 0x00C5EBD0):
  same vtable, same handle member, same owner +0xc object (0x00C5E901 / 0x00C5EAD0). A rename wave should fold them into CKsObject.
- 0x00C77410 / 0x00C77430: CVtable_012bf0c8::StartBlend / StopBlend (the two store groups CommitAndProcess 0x00C774F0 expands).
- 0x00CA6170..90: CBaseInputPin::QueryInterface/AddRef/Release `adjustor{4}' (CRendererInputPin's IQualityControl table
  0x012BF3AC; IQualityControl +0x10, IPin +0x0C). 0x00CAA720..40: the same for CDynamicOutputPin (table 0x012BF748).
- 0x01224900: `dynamic initializer for 'CEngineInternalPrimitiveBase::CurrentInterpolationInfo'' (both PDBs).
- 0x01224970: `dynamic initializer for 'TableBuilder'' (both PDBs); CEngineInlineTableBuilder ctor 0x00BE2A80.

## EXTENT_WRONG / NOT_A_FUNCTION
- 0x00C3EC10 (27-byte gap row) = two functions: 0x00C3EC10 len 11 `CCodaSystem::CreateHardwareVoiceA` (vtable 0x012B5BC0 slot 4,
  cell 0x012B5BD0) and 0x00C3EC20 len 11 `CCodaSystem::CreateHardwareVoiceB` (slot 5, cell 0x012B5BD4), 5 bytes of int3 between.
  differ/00c3ec10.cpp holds both; each is byte-exact on its own 11 bytes (q.py: built 11 bytes = retail's first 11).
  +0x1d4 is g_AudioContext 0x0143BDB0 (CAudioContext, vtable 0x012B8C18): its slots 4/5 are 0x00C56CD0 / 0x00C56860 (landed as free
  `Create_00c56cd0` / `Create_00c56860`), which `new` the hardware voices 0x012B8D48 / 0x012B8CC8 (3D). The landed
  CCodaSystem::GetEAXProperty 0x00C3EB80 models the same member as `CEAXDevice* m_pEAXDevice`: it is that CAudioContext.

## Callee names (bound spellings used)
- ?DestroyInstance@CInstanceRegistry@@QAEXPAUCInstance@@@Z 0x00C2BAF0 (landed; its CInstance is this emitter: dtor 0x00C37DA0;
  the source casts `(CInstance*)this`. Three landed names for one class: CSoundEmitter_00c37ad0 / CInstance / CEntry_00c2bb10.)
- ?GetCount@CMailboxQueue@@QAEJXZ 0x00C49C30 -- NEW spelling (NAME_FIXES.tsv; was CV_StatueMasterScript::GetAngleBasedOnTimeOfDay).
- ??0CKsObject vtable symbol ??_7CKsObject@@6B@ (0x012B90D4); __imp__DeviceIoControl@32 (IAT 0x0143FEC0).
- ?QueryInterface/AddRef/Release@CDynamicOutputPin@@UAG... 0x00CA96F0/0x00CA9700/0x00CA9710 (landed, same spelling).
- ?QueryInterface/AddRef/Release@CBaseInputPin@@UAG... 0x00CA53C0/0x00CA53F0/0x00CA5510: the C++ final overrider (NAME_FIXES.tsv for
  the two landed rows: 0x00CA53F0 is landed as CBasePin::AddRef, 0x00CA5510 as a free func_00ca5510; vland may report RELOC_NAME
  until respelled).
- ??0CEngineInterpolationInfo@@QAE@XZ 0x0042FA30, ??0CEngineInlineTableBuilder@@QAE@XZ 0x00BE2A80 (both PDBs).
- Globals: ?CurrentInterpolationInfo@CEngineInternalPrimitiveBase@@2VCEngineInterpolationInfo@@A 0x0143B8E0 (NAME_FIXES.tsv: landed
  rows bind g_EngineInterpolationInfo / g_block_0143b8e0), ?TableBuilder@@3VCEngineInlineTableBuilder@@A 0x0143BA90 (class key unproven).

## Vendor code (strmbase)
0x00CA6170..90 and 0x00CAA720..40 are DirectShow BaseClasses (renbase.cpp / amfilter.cpp) thunks: by the owner's library rule they
could land as body-less `// Vendor-library: strmbase-dxsdk-sum2004(BaseClasses/<file>) <leaf> -- 10 bytes verified ...` stubs, but
lib_verify.py needs the vendor tree (FABLE_THIRDPARTY), absent in this lane. The sources here are the landed 0x00CAA770 hand model
(genuine C++, grade RELOCATION_MATCH); the coordinator may swap them for Vendor-library stubs after lib_verify.

## Header debt
- engine/CEngineInternalPrimitiveBase.h: new opt-in FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_CURRENTINTERPOLATIONINFO (hdr/, README).
- No shared header for CEngineInterpolationInfo (engine/CIEngine.h only forward-declares it); 0x01224900 carries the Ego_r layout.
- libpk / GameCODA classes used here have no shared header (CItemTable, CItemList, the item / pointer family, CSoundEmitter,
  CCodaFrameStreamer, CMailboxQueue, CUnprovenReverbParameters, CKsObject, CVtable_012bf0c8, CCodaSystem, CAudioContext): local models
  as the landed neighbours; a libpk/ header per class would remove dozens of copies.
- strmbase: the 300-line BaseClasses model is repeated per row (landed 0x00CAA770 pattern); a shared strmbase header would help.

## Parked large rows: what each needs next
All three are `#pragma optimize("s", on)` PC UI builders (prologue `push ebp ; lea ebp,[esp-74h] ; sub esp,0C8h`), no EH frame
(the TU is not /EHsc although ~16 CCountedPointer<CComponent> and two CWideString locals are destroyed at the end).
- 0x005C9A95 CTCInventoryWeapons::ConstructScreenPC (3065 B, slot 87): flow -- front-end manager SetInventory (+0x3C) / owner
  (+0x68); UI_MISC_THINGS_DEF lookup into a CDefPointer<CUIMiscThingsDef>; clear +0x180..+0x194 and the +0x198 component list;
  GetAllItems (0x005BF482) into a pair vector, split by GetSubDef<CWeaponDefPointer> + IsMeleeWeaponClass into melee / ranged pair
  vectors (CItemPairVector PushBack 0x005947B6) and augmentations (IsAugmentation, summing counts); PC_UI_FRAME child, PC_SUB_LIST,
  PC_DESCRIPTION_VIEWPORT (AddPositionChild = slot 124 +0x1F0), ConstructAugEntryPC 0x005C5F11 when augmentations exist,
  PC_TITLE_WEAPONS, the +0x170 component map cleared, two PC_MENU_ENTRY_BRANCH_TEMPLATE / PC_BUTTON_BASTARD menu entries (melee,
  ranged: CreateMenuEntryComponent with GetTextBySymbol of the misc-things def's +0x74 / +0x78 symbols plus " (n)" counts via
  GetXString 0x005CC772 / GFIntToWideString), PC_DESCRIPTION + TEXT_GUI_MENUGUIDE_WEAPONS_MELEE_PC / _RANGED_PC texts, PC_SUBTITLE x2
  with TEXT_GUI_MENU_MELEE / _RANGED, PC_RING_PIC_MELEE / _RANGED, then per category either slot 92 (+0x170, empty) or a PC_SUB_LIST
  filled by ConstructWeaponItemEntriesPC 0x005C8176 (9 args) with the active melee weapon's damage (hero TC 0x48 CTCWeapon::GetDamage
  0x005DAAA0 through the inline GetTC on the +0x134 intelligent pointer) / the active ranged weapon's def +0xA0 -> GetPDefFromGlobalIndex
  -> +0x54 (+0x141 pointer); stores the frame at +0x17C; returns it. ~30 distinct callees, 11 AddPositionChild, 8 AddChild,
  7 slot-145 calls. Needs a CTCInventoryWeapons header (retail offsets: +0x134 / +0x141 intelligent pointers, +0x170 map,
  +0x17C, +0x180..+0x194 six words, +0x198 list) before a draft is worth grading. Annotated listing: agent897 gen/ann_005c9a95.txt.
- 0x005C8459 CTCInventoryWeapons::ConstructScreen (3235 B, slot 77): the console twin of the above (author together; it calls the
  console ConstructWeaponItemEntries / ConstructAugEntry).
- 0x005B8743 CTCInventoryClothing::ConstructScreenPC (3838 B, non-virtual): sole caller 0x0041FBDD (PC-mode branch, else the
  virtual ConstructScreen). Same TU style; the landed CTCInventoryClothing::CreateMenuDescPC 0x005B3613 and ConstructClothingList
  0x005B70B8 are its neighbours / callees to copy the model from.

## Coordinator notes (landing 2026-11-10l)
- Landed 19 of the 25 matches. The six DirectShow BaseClasses adjustor thunks (0x00CA6170..90, 0x00CAA720..40) matched as C++
  but strmbase is third-party vendor code (EXTRA_RULES owner decision 2026-11-08: not chunk material; route = build the vendor
  source unmodified and prove it with libsig/lib_verify.py). Their sources sit in vendor_route/ as the reference for that route.
- Renamed at landing: 0x00C37A00 / 0x00C37A30 `Release` / `Stop` -> `ReleaseA` / `ReleaseB` (both bodies destroy the emitter; "Stop"
  did not describe the body; A/B as CCodaSystem::CreateHardwareVoiceA / B). 0x01224970: CEngineInlineTableBuilder is a `struct`
  (Ego_r struct_layouts @STRUCT), not `class`. All three re-graded RELOCATION_MATCH + PASS.
