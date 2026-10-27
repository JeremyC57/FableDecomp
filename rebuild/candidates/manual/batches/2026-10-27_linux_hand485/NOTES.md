# Hand chunk 485 (40 single-function rows, gap-scan heavy): 39 of 40 byte-exact

16 MATCH + 23 RELOCATION_MATCH, 0 retail_asm, 0 parked, 1 skipped. Every src/ row has a test/ that passes under g++
(tall.sh) and VC7.1 under Wine (tall.py). The read-only vland run shows 39/39 parity + behaviour PASS and no
RELOC_IDENTITY. Its RELOC_NAME warnings come from wrong callee labels; they are listed under Label corrections.

## What matched
Names: **proven** = PDB name set / RTTI-vtable / caller chain; **probable** = body + layout + neighbours with no direct
caller; **descriptive** = nothing names it (CUnproven*_<addr> types or an address-suffixed name).

| addr | name | grade | identity |
|---|---|---|---|
| 00404a60 | NUISystem::CIME::Callback_malloc(unsigned int) | RELOC | proven (PDB + CIME::Initialise) |
| 00405db0 | CActionInputControl::IsSameButton(const&) const | MATCH | proven (caller IsSameActionAndButton 0x0040EDA0) |
| 00405f70 | CRGBFloatColour::operator*=(float) | MATCH | probable |
| 004065f0 | CUserProfileManager::GetManualSaveBaseFileName() const | RELOC | probable (L"Save") |
| 0040db10 | std::allocator<CDateAndTime>::allocate | RELOC | probable (28-byte stride + neighbours) |
| 0040dd10 | std::_Construct<CUnprovenRecord16_0040dd10> | MATCH | descriptive element |
| 0040e3a0 | std::fill_n<unsigned int>(unsigned char*, ...) | MATCH | STLport shape (memset intrinsic) |
| 00415040 | CConsoleClasslessCommand::CallCommandFunction const | MATCH | proven (vtable slot 5) |
| 00415a10 | fabs(float) | MATCH | probable (VC7.1 <math.h> overload) |
| 0041b870 | NUISystem::CSprite::CSprite(const CSprite&) | RELOC | proven (vtables; base copy ctor 0x0052CCA0) |
| 0041cd13 | CAnimFilterNoGroup::operator()(const CAnimationEntry&) const | MATCH | proven (vtable slot 0), /Os |
| 00428c38 | CUnprovenPointer_00428c38 copy ctor | MATCH | descriptive, /Os |
| 0042db40 | CXMVPlayerBuffers::CXMVPlayerBuffers() | RELOC | proven |
| 0042fdb8 | std::_Rb_tree<CUnproven*_0042fdb8>::_M_empty_initialize | MATCH | descriptive types, /Os |
| 00432180 | CControlsDef::Alloc() | RELOC | proven (PDB, vtable 0x01230E84) |
| 004343f0 | CEngineDef::Alloc() | RELOC | proven (PDB, vtable 0x0123118C) |
| 00434660 | CConfigOptionsDefaultsDef::Alloc() | RELOC | proven (PDB, vtable 0x0123125C) |
| 00437710 | CPlayerGui::SetMoralityAsDisplaying() | MATCH | proven (PMorality +0x2DC + name set) |
| 0043ce90 | C3DVector::Initialise(float,float,float) | MATCH | probable (~400 identical unreferenced copies) |
| 0043dc10 | GFLimitUpper<float,float> | MATCH | proven (name set + body) |
| 0043dfa0 | std::__copy_backward_ptrs<CUnprovenPod_0043dfa0*> (__true_type) | RELOC | descriptive element |
| 0043f4f0 | std::fill_n<CUnprovenWord_0043f4f0*, unsigned int> | MATCH | descriptive element |
| 004405f0 | std::list<CUnprovenValue_004405f0>::insert | RELOC | descriptive value {CWideString, 3 longs} |
| 00441b90 | NPlayerGui::CDrawStatUpdates::~CDrawStatUpdates() | RELOC | proven (CPlayerGui ctor -> Reset 0x00444590 -> DeleteData 0x004437C0) |
| 00442190 | NPlayerGui::CDrawSpiritTargetingCursor::~CDrawSpiritTargetingCursor() | RELOC | proven (DeleteData 0x00443B00, Reset 0x00444C20) |
| 00442e60 | ??_GCDrawHighlightingTargetingCursor@NPlayerGui (non-virtual) | RELOC | probable |
| 00448df0 | vector<CCountedPointer<const CInputTypeBase>>::erase(first,last) | RELOC | probable (callees 0x00448300 / 0x00448860) |
| 0044bfa0 | CThing::GetTC<const CTCCoopSpirit> | RELOC | probable (interface 0xC7, const LowerBound 0x004365B0) |
| 0044f5f0 | std::fill_n<CUnprovenWord_0044f5f0*, int> | MATCH | descriptive element, /Os |
| 0046a130 | CAppearanceDef::CAppearanceDef() | RELOC | proven (Alloc tail 0x0046CDF4, vtable 0x012354DC), /Os |
| 0047b7b0 | std::_Vector_base<CHeroLogBook::CQuestObjectiveDesc>(n, a) | RELOC | probable (12-byte stride inside the vector<CQuestObjectiveDesc> run) |
| 0047b8a0 | vector<CHeroLogBook::CQuestObjectiveDesc>::_M_allocate_and_copy | RELOC | probable (calls bound __uninitialized_copy 0x0047B830) |
| 0047bde0 | CHeroLogBook::CQuestDesc::CQuestDesc(const CQuestDesc&) | RELOC | proven (Ego_r name + layout; callees 0x0099EC30 / 0x0047BA50) |
| 0047d478 | InGameMenu_ToggleLiveGui_0047d478() | RELOC | **descriptive** (unreferenced; /Os CTCInGameMenu TU) |
| 0049ae70 | std::__copy_backward<CTexture*, CTexture*, int> | RELOC | probable (CTexture::operator= 0x009FA1C0, between landed 0x0049AE30 / 0x0049B0F0) |
| 0049ca20 | CWorld::SetStartTime(double) | MATCH | probable (FableWin.pdb name; StartTime +0x180 per engine/CWorld.h) |
| 0049d8e0 | CWorld::GetEventDispatchTableEntry(long) const | RELOC | proven (Ego_r 0x004A6C40, PDB order next to ProcessEvent) |
| 004a96a0 | std::vector<CUnprovenElement8_004a96a0>::operator[] | MATCH | descriptive element |
| 004acab0 | CThing::GetTC<CTCRegionDisplay> | RELOC | proven (interface 0x8E) |

Totals: 17 proven, 12 probable, 10 descriptive. 8 of the descriptive rows are STLport templates whose shape is certain but whose element type, or caller, is unproven (0040e3a0 has builtin types but no caller). The other 2 are 00428c38 and 0047d478.

## Parked
(none)

## Skipped
- 004a1c41 `_global::Catch@004a1c41` (69 bytes): an EH catch funclet, not a function. It uses the parent's ebp frame
  (`[ebp-0x20]`, `[ebp+8]`), is referenced only from an EH HandlerType at .rdata 0x01369280, and ends with
  `mov eax,0x004A1C86 ; ret` (the continuation inside its parent). It can only be reproduced as the catch block of its
  parent function.

## Label corrections
- 0x004065D0, landed as "GetAutoSaveFileName": it returns L"Default", so it is more likely GetDefaultProfileName. This needs a recheck.
- 0x004376B0, landed as "CPlayerGui::SetCrimeAsDisplaying": the body touches +0x2CC, so it is SetMoneyAsDisplaying.
  Its landed source is a junk fastcall free function.
- 0x0052CCA0 is bound as `??0CDraggableInto@NUISystem@@QAE@ABV01@@Z`; the right name is
  `??0CChangingStateComponent@NUISystem@@QAE@ABV01@@Z` (it is CSprite's direct base, by RTTI).
- 0x00441B90 / 0x00442190 / 0x00442E60: the manifest label "Catch@0047e8cc" is wrong on all three. The reloc_bindings
  stand-ins are also wrong: `?base_dtor@CLandscapeBackgroundPatch@@QAEXXZ` (0x00441B90) and
  `@CLandscapeBackgroundPatch_scalar_dtor@4` (0x00442190).
- 0x00404A60: the binding `?Callback_malloc@@YIPAXI@Z` is a stand-in; the real name is `?Callback_malloc@CIME@NUISystem@@SIPAXI@Z`.
- 0x00432180 / 0x004343F0 / 0x00434660: the bindings `?CreateControlSchemeDef@@YAPAXXZ`, `?CreateEngineDef@@YAPAXXZ` and
  `?CreateConfigOptionsDefaultsDef@@YAPAXXZ` are stand-ins for the PDB `?Alloc@<Class>@@SIPAVCDefClassBase@@XZ`.
- 0x0047B830: the manifest label `_Uninit_copy<..._Vector_val<unsigned int>...>` is a Dinkumware name and wrong. It is
  `std::__uninitialized_copy<const CHeroLogBook::CQuestObjectiveDesc*, ...>`, which is already bound.
- 0x00448300: the label "Move_CountedPointer_00448300" is really `std::__copy<CCountedPointer<const CInputTypeBase>*, ..., int>`.
  0x00448860: the label "_Destroy_range<pair<CCharString,CCountedPointer<CEntityScriptBindingBase>>>" is really
  `std::__destroy_aux<CCountedPointer<const CInputTypeBase>*>`. vland lists both as RELOC_NAME warnings.
- 0x013B8790 (the global CPlayerGui*) has 57 binding spellings. I used the existing `?g_pPlayerGui@@3PAVCPlayerGui@@A`.
- 0x013B9288 has two spellings (`g_GameEventHandlers`, `GameEventDispatchTable`). I used the existing
  `?GameEventDispatchTable@@3PAVCGameEventDispatch@@A`.
- 0x004A95A0..0x004A9780: the neighbours' "_Dest_val<CCountedPointer<COptimisedPrimitive>>" labels are propagated.
  They do not name 0x004A96A0's element.

## Header debt
- engine/CConsoleCommandBase.h flattens the vptr (00415040 keeps a local base model).
- engine/CAnimFilterNoGroup.h has no virtual predicate slot (0041cd13 keeps a local model).
- engine/CSprite.h / CObserver.h flatten the vptrs of the three polymorphic bases (0041b870 keeps a local partial view;
  vland warns SHARED_HEADER_AVAILABLE CObserver).
- engine/CPlayerGui.h carries the Ego_r offset for PMorality (00437710 keeps a local partial view). 0047d478 uses the
  header through the Donor include.
- engine/CResource.h, CDefClassBase.h, CParentDefClassBase.h flatten the vptr (00432180 / 004343f0 / 00434660 /
  0046a130 write the def-base chain locally; vland warns SHARED_HEADER_AVAILABLE CResource on 0046a130).
- There is no header for CThing (GetTC rows use a partial view), CHeroLogBook::CQuestDesc / CQuestObjectiveDesc,
  NPlayerGui::CDraw*, CActionInputControl, CRGBFloatColour, C3DVector, CXMVPlayerBuffers, CAppearanceDef or
  CGameEventDispatch.
- engine/CWorld.h (0049ca20, 0049d8e0) and engine/CTexture.h (0049ae70) are used through the Donor include.

## Callee names
All call targets were checked against retail (q.py relocation compare + vland, no RELOC_IDENTITY). The declared names are:
malloc 0x00BFEA0E, free 0x00BFEA14, operator new 0x00BFEA1A, operator delete 0x00BFE9BC, memmove 0x00BFEAE6,
CWideString(const wchar*) 0x0099B6B0, CCharString(const&) 0x0099EC30, CResource() 0x009FBEC0, CSubDefClassBase()
0x0044C0C0, CAnimationSet() 0x005DD2E0, CBaseClassNonCopyable() 0x0099A2F0, CChangingStateComponent(const&) 0x0052CCA0,
CSprite::GetDataFromDef 0x0041AC20, CVectorMap LowerBound 0x0040F020 / const 0x004365B0, __copy 0x00448300,
__destroy_aux 0x00448860, __uninitialized_copy 0x0047B830, vector<CQuestObjectiveDesc>(const&) 0x0047BA50,
CPlayerGui::ToggleLiveGui 0x00438310, CTexture::operator= 0x009FA1C0.
