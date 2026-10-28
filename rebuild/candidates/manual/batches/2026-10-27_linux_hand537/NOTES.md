# Hand chunk 537 (_global .. CWorld): 2 of 8 byte-exact

Retry chunk (8 rows parked DIFFER earlier). Lane: VC7.1 RTM cl 13.10.3077, BASE_FLAGS /O2 /Oy /W3.
Two rows land (both RELOCATION_MATCH, no retail_asm); six stay parked with every name brought to the exact PDB spelling.

## What matched (2)
- **CSubtitleRenderer::Clear(bool) 0x00492BAB** (672 bytes, `/GS /Zc:wchar_t`) -- clears the three primitive-handle vectors
  (`for_each` + `mem_fun_ref`), and unless keeping the text resets captions/overrides, ends the player mode it added,
  force-clears the hero's CTCHeroReceiveItems (interface 0xCB) display, posts the text-finished messages, unpauses and shows
  the next queued text. SHAPE: the queue is reached through `reinterpret_cast<CTextInfoQueue&>(TextInfoQueue)` *in each
  expression* (`empty()`, `front()`, `pop()`), not through a `CTextInfoQueue& queue` local: the local made VC7.1 keep start
  in edi across the branch (retail: `mov ecx,[eax+10h] ; cmp ecx,[eax]` then re-reads start for front()) and swapped the
  edi/ebx roles of L""/"" for the whole body. Real STLport 4.6 deque/queue/_Deque_iterator, bitset<274>::test,
  CVectorMap::LowerBound, CGameEvent::AddData (engine/CGameEvent.h), CThing::GetTC returning `CTCBase&` (PDB) all hold.
- **CWorld::GetPathNameForFileName 0x0049EF50** (553 bytes, `/GS /Zc:wchar_t`) -- scans U:\* directories for an entry
  whose split name has an empty extension and the requested base name. SHAPE: the extension test is the STLport inline
  `operator==(const basic_string&, const _CharT*)` (with `__char_traits_base::length` folding to 0 for L"") and the
  CWideString == CWideString compare goes through two `const CWideStringData*` locals; with `wchar_t dirName[262]` /
  `name[262]` and /Zc:wchar_t the frame-slot residue of the parked file disappears. engine/CWorld.h via the layout-define
  pattern.

## Parked for Fable 5.1 (6)
- 0040cec0 CUserProfileManager::CUserProfileManager DIFFER(984v977): 50-name fill temp/ref slots ([esp+20h]/[esp+18h] v 18h/1Ch), defaults release `dec [eax+4]` + reload + `cmp [ecx+4],ebx` (retail `mov ecx,[esp+1Ch] ; dec dword ptr [ecx+4] ; jne`), min-size edx load order, CDisplayModeDesc copy register order | tried: written next step (landed `if (Object) Object->Release()` with delete-in-Release, Release()+GetRefCount()==0, local copy, direct --IVRefCount, long count, != 0 early return; named-local AddDeviceResetCallback; `vector(n, const T& = T())` = 994, worse), real names incl. CSystemManager::GetDisplayManager() (fixes the display-manager fetch: ndiff 80 -> 71) | next: lane gap: regalloc/frame-slot
- 0047e651 CTCInGameMenu::VideoOptionsChanged DIFFER(752v752): ChangePosition float-slot `push ecx` scheduled before the X `fsubp` (retail after) | tried: C2DVector ctor/copy forms, temp orders, RTM+QFE (earlier); real PDB names, same-TU GetBackBuffer*/GetUIScale bodies, temp vector, dialog local, named x/y, `/2.0f`, negation, compound `-=` | next: lane gap: scheduling
- 0047e9ae CTCInGameMenu::GlobalUpdate DIFFER(860v860): the same push ecx/fsubp swap; everything else identical | tried: as 0047e651 (shared model) | next: lane gap: scheduling
- 0047f30f CTCInGameMenu::VideoOptionsNotAccepted DIFFER(636v636): the same push ecx/fsubp swap | tried: as 0047e651 | next: lane gap: scheduling
- 00489d40 CPlayer::CreateCharacter DIFFER(815v815): frame slots only -- pos/forward homed at 1Ch/28h (retail 28h/1Ch); holy-site out-local shares up's slot (retail the name temp's +0Ch); offset temp shares forward (retail up). Registers now agree | tried: ~30 probes earlier; written next step (outer-scope holy-site out-local, debug vectors in a sibling block), 30 declaration permutations, debug-branch and offset-vector forms; real names + named number/mainNumber locals fixed the esi/edi pair | next: lane gap: frame-slot
- 0049be90 CParticleEnvironmentInteraction::IsParticleCollidingAt DIFFER(806v808): retail keeps hitPos in edi, iterator in ebx and the hit flag in the filter temporary's byte; ours iterator edi, flag bl, hitPos re-read | tried: filter temp forms (earlier); real names (virtual const, CWorld::PeekWorldMap, HasPhysicsMesh template, reference args) + user ctor on the filter (else VC zero-inits the temp: +1 instruction), outPos/thing locals, outer iterator, inlined hit test | next: lane gap: regalloc

## Skipped
None.

## Label corrections
(manifest/catalog labels proven wrong; full list with wrong spellings in NAME_FIXES.tsv, 34 rows)
- 0x00403C60 MemCmp_Unsigned16 -> std::__char_traits_base<wchar_t, wint_t>::compare
- 0x00999850 CAFile::FindNextFileSequential -> CAFile::ScanNextFile(const CWideString&, CFileScanInfo&, wchar_t*)
- 0x0041CC14 / 0x0041CC2B CTCLook::GetHeadAngularVelocityMaxXY / GetScreenWidth/Height -> NUISystem::CUIScreenUnitConverter::GetBackBufferWidth / GetBackBufferHeight
- 0x0099A2F0 CBase::CBase -> CBaseClassNonCopyable::CBaseClassNonCopyable (protected)
- 0x0040D2A0 CTBaseSingleton<CUserProfileManager>::Get -> CUserProfileManager::Get (static __fastcall)
- 0x0040A740 one binding says Apply -> it is CUserProfileManager::Save (Apply is 0x0040BC80)
- 0x00405F00 CGuiControlNumber::SetSLongValue -> GFIntToWideString
- 0x009F1760 CMemoryBuffer::Copy -> CGameEvent::AddData(long, const void*) (no CMemoryBuffer class in either PDB)
- 0x0049C490 GetPBestThingInRoughArea<...> -> CThingSearchTools::GetAllThingsInRoughArea<const CThing, vector<const CThing*>, CThingFilter_HasPhysicsMesh>
- 0x009C02D0 BuildVideoCapabilityTree -> CDisplayManager::EnumerateDisplayModes(CArray<CDisplayModeDesc>&, C2DExtentsI, C2DExtentsI)
- 0x009C0190 CMainGameComponent_AddComponentToList -> CDisplayManager::AddDeviceResetCallback
- 0x004083E0 CManager::GetIconsDef -> CUserProfileManager::GetConfigOptionsDefaultsDef
- 0x0040F4A0 Std_BitSet_Allocate -> _Deque_base<ETutorialCategory>::_M_initialize_map (probable)
- 0x004124C0 Std_Vector_Insert_Range_16Bytes -> vector<CDisplayModeDesc>::_M_insert_overflow (probable)
- 0x0040DDC0 _Copy_impl<C2DLineF*> -> std::__uninitialized_copy<CDisplayModeDesc*> (already bound)
- 0x0043DBD0 IS_THING_ALIVE<CThingBuilding*> -> IS_THING_ALIVE<CThingPlayerCreature*> (SETTLED; ICF-folded instances)
- 0x00487DC0 CPlayer::GetHeroCreature -> CPlayer::GetPControlledCreature() const (vland RELOC_NAME)
- 0x0099B800 CCharString::AssignFromWide -> CWideString::operator=(const wchar_t*) (vland RELOC_NAME)
- virtual slots (no relocation; parked names were wrong): CObservable slot 5 = RemoveObserver (not AddObserver), slot 9 =
  AddConcurrentExclusiveObserver (not SetFocus); CComponent slot 48 = ChangeState(unsigned long) (not SetActive),
  slot 87 = GetCurrentStateIndex; CObserver slot 6 = ObserveAllEvents (not TakeFocus_slot6); CYesNo 142/143 =
  SetYesCallBack / SetNoCallBack; CText 143/144/145 = SetValue(unsigned long, const CWideString&, bool) /
  SetValue(const CWideString&, bool, bool) / SetValue(const CCharString&, bool, bool); CWorld slot 13 = PeekWorldMap
  (the 0049be90 parked file called it the environment's GetWorldMap; slot 12 is GetWorldMap).

## Names
### Proven (exact PDB spelling + a second source: caller shape, RTTI, donor order, body, or a byte match)
- 0x00492BAB `?Clear@CSubtitleRenderer@@QAEX_N@Z`; 0x0049EF50 `?GetPathNameForFileName@CWorld@@QBE?AVCWideString@@ABV2@@Z`
- all 34 rows of NAME_FIXES.tsv except 0x0049C490 (probable, see below)
- callees landed in src with PDB spellings: `?SetText@CSubtitleRenderer@@QAEXABVCFontBank@@ABVCWideString@@1111KM_N2@Z`,
  `?SetButton1OverrideText@...` / `?SetButton2OverrideText@...`, `?AddMessage@CMessageEventManager@@...`,
  `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@...`, `?test@?$bitset@$0BBC@@std@@QBE_NI@Z`, `?pop_front@?$deque@VCSetTextInfo@...`,
  `?RemoveMode@CPlayer@@QAEXW4EPlayerMode@@@Z`, `?GetMainPlayer@CPlayerManager@@QAEAAVCPlayer@@XZ`,
  `?StartFileScan@CAFile@@SI_N...PA_W@Z`, `?IsPathFile@CAFile@@SI_NABVCWideString@@@Z`, `?SplitPath@CAFile@@SIXABVCWideString@@PAV2@111@Z`,
  `??H@YI?AVCWideString@@ABV0@PB_W@Z`, `?AddGameEvent@CMainGameComponent@@QAEXABVCGameEvent@@@Z`
- parked-row callees: `?PeekWorldMap@CWorld@@UBEABVCWorldMap@@XZ` (vtable 0x012390F0 slot 13), `?GetDisplayManager@CSystemManager@@QAEPAVCDisplayManager@@XZ` (inline),
  CIVCountedPointeeBase AddRef/Release/GetRefCount/Delete, `CDefPointer::Object`, CCountedPointer `Data`/`Info`,
  `?OnPostDeviceReset@CUserProfileManager@@MAE_NXZ` (returns bool), `??RCThingFilter_HasPhysicsMesh@@QBE_NABVCThing@@@Z`,
  `??$GetTC@VCTCInventory@@@CThing@@QAE_NPAPAVCTCInventory@@@Z` (inline), `?AddSLONG@CGameEvent@@QAEXJ@Z` (inline),
  `?GetTextBank@CMainGameComponent@@QBEABVCDataBank@NGameText@@XZ` (inline), CThing `Dead : 1` (+0x91 bit 0, IS_THING_ALIVE body)
### Probable
- 0x0049C490: the const-CThing instance (in both PDB name sets) rather than FableWin's non-const twin; the body cannot tell them apart.
- 0x0040F4A0 `?_M_initialize_map@?$_Deque_base@W4ETutorialCategory@@...@IAEXI@Z`, 0x004124C0
  `?_M_insert_overflow@?$vector@VCDisplayModeDesc@@...@IAEXPAVCDisplayModeDesc@@ABV3@ABU__false_type@2@I_N@Z`,
  0x00485DBC vector<CComponent*>::_M_insert_overflow (IAE): STLport 4.6 source spellings, no PDB row for these instances.
- CText slot 145 = SetValue(const CCharString&, bool, bool) / 144 = (const CWideString&, bool, bool): reverse-declaration slot
  order + argument types at the call sites.
### Descriptive
- 0x013BCA24 `?g_EmptyWideString@@3VCWideString@@B` (no PDB global; five bound spellings, see Review at landing)
- 0x013B86A0 `?g_pMainGameComponent@@3PAVCMainGameComponent@@A` (no PDB global; the most-bound spelling)
- enum constants TCI_HERO_RECEIVE_ITEMS (0xCB), MESSAGE_EVENT_TEXT_FINISHED_8/_2 (0x12/0x11), GAME_EVENT_PAUSE (9),
  GAME_EVENT_LOAD_GAME (0x3C): enumerator spellings unknown; they reach no symbol.
- vtable filler slots (v04.., o00.., b00..) in the parked UI / CWorld models: unused slots, names not needed for the bodies.

## Review at landing
- 0x00686D40 is an ICF-folded no-`this` getter (`mov eax,[013B8A1C] ; mov eax,[eax+30h] ; ret`) bound under 6 spellings
  (CTCBase::GetPlayerManager IAE, CTCInventoryBase, CTCTeleporter, GetPlayerManager2, a free function). The parked
  0047e9ae uses `?GetPlayerManager@CTCBase@@IAEAAVCPlayerManager@@XZ`; one retail address must get one name.
- 0x013BCA24: five spellings bound (`g_EmptyWideString` const, `g_emptyWideString` const/non-const, two `_013bca24`
  suffixed); 0049ef50 uses `?g_EmptyWideString@@3VCWideString@@B`.
- 0x0043DBD0 is ICF-folded across IS_THING_ALIVE<T*> instances; SETTLED says CThingPlayerCreature*, which 0x00492BAB uses.
- The parked 0047e651/0047f30f/0047e9ae carry same-TU noinline bodies of real retail callees under their real names
  (STLport `vector<CComponent*>::push_back` 0x00486024 + `_M_insert_overflow` 0x00485DBC; `IS_THING_ALIVE` 0x0043DBD0 in
  0047e9ae): without the visible bodies VC7.1 spills the pushed/tested local (ndiff 2 -> 33/10).
