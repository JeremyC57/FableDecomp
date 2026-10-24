# Hand chunk 352 (std::count_if<HasBuildingAsHomeOrWork> .. CCountedComponentHolder_007550c0): 40 of 40 byte-exact

Every row grades MATCH (3) or RELOCATION_MATCH (37) for the file in src/ (gall.sh, final run). Every test passes under
both g++ (tall.sh) and VC7.1 (tall.py). Lengths in STATUS.tsv are the retail bodies with trailing int3 padding stripped
(the chunk `length` column includes the padding for 00512050, 00870c50, 00ac6210, 00aecb80, 004a7710, 00633830,
00633880, 004ac800, 009bde90, 007d26e0, 00739df0, 00768bc0, 00ab34c0, 00696560 and 006f48a0; finish.py strips it the
same way). No row held two functions.

## What matched (40)
- **std::count_if over list<CIntelligentPointer<CThingAICreature> >** (006600ec HasBuildingAsHomeOrWork, 0065f6a6
  HasGuardProperty, 006c1c70 HasBuildingAsWork): village-people counters. All are `long __stdcall` (`ret 0Ch`) with
  `// Leaf: count_if`. The two unaligned loops (jmp to the bottom test) need `#pragma optimize("s", on)`.
  - 0065f6a6 passes the predicate a converted CIntelligentPointer<const CThing> by value.
  - 006c1c70 needed the creature's inline accessor `GetWorkBuilding() const { return WorkBuilding.GetPItem(); }`, which
    gives `lea ecx,[eax+170h]`; chaining `->WorkBuilding.GetPItem()` gives `add eax,170h ; mov ecx,eax`.
- **CPlayer** (00633830 UninitModeParalysed, 00633880 UninitModeBoastUI): mode-exit handlers that detach each mode's
  input handlers. Identified from the PlayerModeInfo table initialisers at 0x0121B4xx/0x0121B6xx. Modelled through
  `engine/CPlayer.h` with a methods carrier.
- **CTCCombat::FrameUpdate** (00736ae0): vtable slot 9, through `engine/CTCCombat.h`.
- **CIVCountedPointer<N3DPrimitive::CTriangleList>::Attach** (00655230, 00cd23da): release the old pointer, take the new
  one. Uses the landed 0x0042FC66 form plus `#pragma optimize("s", on)`; the tell is `and dword ptr [esi],0`.
- **Implicit destructors emitted through a qualified call** (`p->T::~T();` in a helper, `// Leaf: ??1T`):
  - 004420d0 NPlayerGui::CDrawMeleeActionButtonEffects (ctor 0x00654258 is landed).
    - CDrawBase's vtable 0x0125A52C has only ClearPrimitivesIfNecessary and PrepareForLoad, so no vtable stores.
    - The two vectors of virtual-dtor elements need STLport's inline `_Destroy(first, last)` with by-value bounds. A
      loop on `_M_finish` re-reads it after every virtual call (117v115, attempt 1).
  - 006f48a0 CDefendingCombatantInfo (Ego_r.pdb layout, retail drops 4 bytes per container). Its members:
    - a CDefPointer;
    - a CIntelligentPointer<CThing> (direct ~CBaseIntelligentPointer 0xA01C10);
    - the CCombatWheel ring vector (out-of-line tag DestroyRange 0x6F44E0, tag in the `push ecx` slot);
    - the STLport attackers tree (clear, then free the head).
  - 00a326d0, 00af65b0, 004b86b0, 006fb460, 00a6f390, 00a70130: std::pair destructors; the leaf needs the exact mangling.
  - 009f88c0 CGraphicDataBankInfo, 00485cf1 CPersistContextReadSection.
- **User/empty destructors of member-only records** (members destroyed in reverse; CCountedPointer re-reads Info after
  the decrement; CIVCountedPointer/CDefPointer release through slot 1, then zero):
  - 004ac800 COpinionReactionManager (ctor 0x007004B0 is landed; layout from Ego_r.pdb). Its members:
    - an inline list destructor (`clear()` 0x43F230 out of line, then `if (_M_node) free`);
    - a fully out-of-line list destructor 0x4AB420;
    - two CIntelligentPointers and the def pointer.
  - 00ab34c0 and 00aecb80: counted-pointer-owned records with vector destructors in the landed 0x006FD040 form
    (`__fastcall` DestroyRange with an SNonscalarPtrIteratorTag temporary, then `if (start) free`). 00aecb80 ends with
    the out-of-line ~CStringKeyedTable_006fd040. Both matched first try.
  - 004ae3e0, 007550c0, 0056d190, 00a61b10: descriptive records.
  - 009bde90/009bdfa0 CSubTitle.
  - 00abefa0 C3DMeshMaterial::CFilenames and 00ac6210 CMapFilenames (strings, then the counted-pointee base).
  - 00739df0 CAnimFilterOf3Groups (vtable 0x0122FFCC).
  - 00b2e6c0 CSurfaceCompositor (inline CRenderStateCaptureBlock dtor, CBase root).
- **Scalar deleting destructor** 00512050 CMapInfo: non-polymorphic `??_G`, written `void Destroy(CMapInfo* p) { delete p; }`.
- **Constructors**: 004a7710 CTestQuest (the implicit default ctor emitted through placement new) and 00768bc0
  (descriptive quest/thing record).
- **Deleter** 00696560 `CCountedPointer<T>::DeleteData`: a static `__fastcall` template member with an explicit
  instantiation.
- **Dispatch** 00870c50 NCreatureAction::CActionEventSound::Dispatch (slot 1, vtable 0x0125C11C): in retail it only builds
  and destroys two empty CCharString locals.
- **STLport one-argument resize** (`resize(n) { resize(n, T()); }`):
  - 00657037 vector<CIntelligentPointer<CParticleEmitter> >: CDraw-region pragmas `y` off + `s` on.
  - 007d26e0 vector<CShoveHaPennyRegionDef>: CResource <- CDefPointeeBase (inline virtual dtor) <- CDefClassBase chain.
  - 0086ee50, 00728f80: descriptive vectors of vectors / keyed records.

## Parked for Fable 5.1 (0)

## Skipped
- None. No row held two functions; all extents agree with the disassembly (the function ends at its `ret` and the rest is
  int3 padding).

## Label corrections
All 35 `_Dest_val<...>` labels in this chunk were propagated across byte-identical bodies. They name a different entity
except for the pair rows noted below.
- 00633830 `_Dest_val<CEditLevelMerger::CConflict>` -> CPlayer::UninitModeParalysed
- 00633880 `_Dest_val<CEditLevelMerger::CConflict>` -> CPlayer::UninitModeBoastUI
- 004a7710 `_Dest_val<CEditLevelMerger::CConflict>` -> CTestQuest::CTestQuest
- 00870c50 `_Dest_val<CMacro>` -> NCreatureAction::CActionEventSound::Dispatch
- 00abefa0 `_Dest_val<CMacro>` -> C3DMeshMaterial::CFilenames::~CFilenames
- 00ac6210 `_Dest_val<CMacro>` -> C3DMeshFileEngineMaterialChunk::CMapFilenames::~CMapFilenames
- 00512050 `_Dest_val<CMapInfo>` -> CMapInfo::`scalar deleting destructor'
- 00aecb80 `_Dest_val<CMapInfo>` -> destructor of a counted record that holds CStringKeyedTable_006fd040 at +0 (class unrecovered)
- 004ac800 `_Dest_val<CIVCountedPointer<CTriangleList>>` -> COpinionReactionManager::~COpinionReactionManager
- 00655230, 00cd23da `_Dest_val<CIVCountedPointer<CTriangleList>>` -> CIVCountedPointer<N3DPrimitive::CTriangleList>::Attach
- 009bde90, 009bdfa0 `_Dest_val<CIVCountedPointer<CTriangleList>>` -> CSubTitle::~CSubTitle
- 00657037 `_Dest_val<pair<CCharString,CScriptInfo>>` -> vector<CIntelligentPointer<CParticleEmitter> >::resize
- 007d26e0 `_Dest_val<pair<CCharString,CScriptInfo>>` -> vector<CShoveHaPennyRegionDef>::resize
- 0086ee50, 00728f80 `_Dest_val<pair<CCharString,CScriptInfo>>` -> STLport vector resize(n) (descriptive element types)
- 00485cf1 `_Dest_val<pair<CCharString,CSymbolInfo>>` -> CPersistContextReadSection::~CPersistContextReadSection
- 0056d190, 00a61b10, 00768bc0 `_Dest_val<pair<CCharString,CSymbolInfo>>` -> descriptive record dtor / CInputManagerDX
  device-list member dtor / record ctor
- 00736ae0 `_Dest_val<pair<CCharString,CSymbolInfo>>` -> CTCCombat::FrameUpdate
- 00739df0 `_Dest_val<pair<CCharString,CSymbolInfo>>` -> CAnimFilterOf3Groups::~CAnimFilterOf3Groups
- 009f88c0 `_Dest_val<pair<CCharString const,CCountedPointer<C3DSkeletalMorphTarget>>>` -> CGraphicBankManager::CGraphicDataBankInfo::~CGraphicDataBankInfo
- 00a326d0 (same label) -> std::pair<long,C3DMeshVolume>::~pair
- 00ab34c0 (same label) -> destructor of a counted record {CCharString, vector<CCountedPointer>, CCountedPointer} (class unrecovered)
- 00af65b0 (same label) -> std::pair<NParticleEngine::EPSCType,NParticleEngine::CComponentInstance>::~pair
- 00b2e6c0 (same label) -> CSurfaceCompositor::~CSurfaceCompositor
- 004420d0 `_Dest_val<pair<EAnimComponent,CIVCountedPointer<CAnimComponentBase>>>` -> NPlayerGui::CDrawMeleeActionButtonEffects::~CDrawMeleeActionButtonEffects
- 006f48a0 (same label) -> CDefendingCombatantInfo::~CDefendingCombatantInfo
- 00696560 (same label) -> CCountedPointer<T>::DeleteData (per-type deleter)
- 004ae3e0, 007550c0 (same label) -> descriptive record destructors
- Labels right in substance: 004b86b0/006fb460 (pair<CCharString,CSymbolMap::CSymbolInfo>::~pair), 00a6f390/00a70130
  (pair<CCharString,C3DMeshVolume>::~pair) and the three count_if rows (STLport count_if with those predicates).
- Callee labels found wrong on the way:
  - 0x00443AA0 "CTCMapwho::OnDie" = CCountedPointer<NPlayerGui::CDrawMeleeActionButtonEffects> deleter.
  - 0x004AD600 "CTCMapwho::OnDie" = CCountedPointer<COpinionReactionManager> deleter.
  - 0x004ACA70 "CLandscapeBackgroundPatch::`vector deleting destructor'" = COpinionReactionManager::`scalar deleting destructor'.
  - 0x006EFC70 "FreeTree_CTreeOwner" = map<unsigned __int64,CThingCreatureBase*>::_M_erase (CDefendingCombatantInfo::Attackers).
  - 0x004AB420 "CGuiControlTreePane::SortTreeRecursively" = list<CReactionMatch*>::~list.
  - 0x0043F230 "CGuiControlTreePane::SortTreeRecursively" = list<CIntelligentPointer<const CThing> >::clear.
  - 0x006F4A60 (unlabelled) = CCombatantInfo::~CCombatantInfo.
