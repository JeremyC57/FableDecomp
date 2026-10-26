# sweep3: landed-errors sweep 3, 2026-10-25/26 (complete)

Hand-over for the coordinator. Every RENAMES/FIXES row is regraded fresh (MATCH/RELOCATION_MATCH) and its test printed
PASS under VC7.1 before it was appended (`tools/row.py`, or `tools/batch.py` = the same grade in parallel plus a check that
the `// Leaf:` names exactly one defined object symbol). Nothing under the repo was written; no git.

## Counts
| Output | Rows | Notes |
|---|---|---|
| A RENAMES.tsv | 1,494 | 575 script `Alloc` factories, 343 other NScript rows, 244 `GetInterfaceType`, 149 `GetSubDef<T>`, 77 STLport templates, 15 `X::Copy`, 14 `GetAllThingsInRoughArea`, 77 other |
| B FIXES.tsv | 333 | 188 by the 1d `_Rb_tree` access pass, 54 AccurateNormalise callers, 91 other |
| C LABELS.tsv | 72 | non-landed callees only (4 removed when their addresses got landed, D59) |
| D UNRESOLVED.tsv | 79 | 70 NScript-convention rows held (65 with unverified class attribution, 5 propagated-label twins), 9 others (3-attempt drops, rejected claims, family-wide items) |
| BINDINGS_DROP.tsv | 1,178 | reloc_bindings rows whose recorder no longer emits the symbol, or that bind it to the wrong target |

Grades over A+B: 303 MATCH, 1,524 RELOCATION_MATCH; every row has its test PASS. No address is in both A and B, none of
A/B is in D, and LABELS holds no landed address. Every renames/ and fixes/ source has its row (and every RENAMES row its
test); there are no stray files. Purity grep over renames/ and fixes/ (`__asm`, `naked`, `_emit`, `volatile`, `_STL`): no hits.
309 RENAMES sources (234 NScript, 15 thing-search template instances, 5 others) have no mangled `// Leaf:`; g.grade
derived their leaf from the name, and the batch check found exactly one defined symbol for it.

## Method
- Queue order as PROMPT.md: (1) FINDINGS_LOG from line 622 to the end (re-read at the end: still 633 lines, chunk 483 the
  last, processed); family pass 1 items 1a, the CScriptThing binding 1b, the three binding clashes 1c; (2) the sweep2 owner
  items, including the NScript convention and the deferred 1d `_Rb_tree` access pass; (3) CRetailTree stand-ins: only the
  trees that queue 1/2 rows touched (pending-object map, bank maps, CComponent map, unsigned long map) were converted.
  The family is too big for this sweep: 1,126 files (see Owner items).
- Coordinator additions: (a) the four wrong Family pass 2 bindings, and (b) chunk 482's landed-wrong items. Then the
  remaining FINDINGS line 631 (agent951 Family pass 2) label corrections that name landed rows.
- Identity evidence, at least two per claim: the Ego_r/FableWin PDB name sets (as name sets only); the PDB member order
  against retail body order (accessor runs); Ego_r layouts (`struct_layouts_egor.tsv`, with the retail CThing shift of
  -0x10 from ThingTypeFlags onward); RTTI type descriptors read through vtables (`work/cam/vtname.py`,
  `tools/rtti_vt.py`); xrefs (`$S/w23/xref.py`); the bytes (`$S/w17/xdis.py`); the landed callers' and callees' bindings
  (`work/fp2/bindchk.py` compares each caller's undefined symbol with the symbol its callee row defines).
- STL models are STLport 4.6 in `std` (`_tree.h` / `_vector.h` / `_list.h` / `_deque.h` access and spellings). Where
  STLport's own text was the only source for an access level, libstdc++ 13 (same SGI lineage, on disk) was the second
  source. Where no second source exists (vector `_M_insert_overflow` / `_M_clear` access) the change was not made.
- Bindings: `tools/pbdrop.py` recompiles each A/B source and compares its object symbols with every reloc_bindings row
  that address recorded (DROP = no longer emitted; CONFLICT = the table binds the symbol elsewhere). Scans:
  `work/pbdrop_full1.tsv` (all rows at the time), then `pbdrop_new.tsv` / `pbdrop_new2.tsv` / `pbdrop_new3.tsv` for rows
  added or changed later. The newest scan of an address supersedes the older ones (`work/mkdrop.py`), and the hand
  drops (`work/manual_drops.tsv`, wrong-target rows) are merged in with their own reason.

## Progress
- 1b CScriptThing binding: done (RENAMES 0x8CFF40, FIXES 6 callers + 6 F014 members).
- 1a Create_CVtable family: 575 RENAMES (557 x 145-byte + 18 x 153-byte) = NScript::<Script>::<Entity>::Alloc.
- 1a set/faction: RENAMES 0x6B94C0 CFaction::IsEnemyOfFaction, 0x4BAA30 std::set<CCharString>::find, 0x76C7D0
  CTCEnemy::IsEnemyOfFaction (was misnamed IsAlliedWithFaction); FIXES 0x6B9460.
- 1a bank cluster: RENAMES 0x9A8090 RegisterBank, 0x9AB120 / 0x9AAF70 `_M_insert`, 0x9A9AD0 pair copy ctor,
  0x9AA9E0 `_M_create_node`, 0x9AA520 `_Construct`; FIXES 0x9A7F80 FindContainedBank; LABELS 0x9AA560, 0x9AAA30.
- 1a map<CCharString,bool> / TransferMap: RENAMES 0x7ADB30 std::map<CCharString,bool>::find, 0x7AE3D0
  CPersistContext::TransferMap<bool,CCharString> (real STLport map; `_M_find` callee 0x7ACC90), 0x757160 the 2-argument
  TransferMap<std::pair<long,long>,CCharString>; FIXES 0x7556B0 CTCHeroRememberCrimes::OnSerialise, 0x9CCDC0 (thiscall callee);
  LABELS 0x756D00, 0x755640, 0x755680.
- 1a C3DAnimationStats: RENAMES 0xA4EAC0 HasEvent, 0xA4EAE0 GetEventTime (multimap<CCharString,float> TimingEvents +0x20).
- 1a script ::Main: LABELS 0xDA34B0 CQ_MinionClifftopChaseScript::Main, 0xF024C0 CQ_AmbushTradersScript::Main; 0xDCC770 is
  already catalogued as CQ_OrchardFarmRaidScript::Main (only the NScript convention applies).
- 1a vector<CThing*> claims / CThingMask47 (the thing-search cluster):
  - FIXES 0x518850 (vector<const CThing*>), 0x5198B0 (vector<CThing*> + CThingFilter_HasTCOfLeafType<CTCActionUseScriptedHook>).
  - Mask 0x47 = CThingBuilding, type list 3 = CThingBuilding, type list 4 = CThingVillage (ctor evidence). FIXES 0x66E5A0,
    0x4AD700, 0x6D4600 (was CThingBuilding, is CThingVillage), overflows 0x4AC530 (vector<CThingBuilding*>), 0x4AC610
    (vector<const CThingBuilding*>), 0x6D4320 (vector<CThingVillage*>), 0x959260 (vector<const CThingDeadCreature*>).
  - RENAMES 0x4AD220 std::vector<CThingBuilding*>::push_back, 0x512F60 GetPFirstThing<CFilter_Bind2<InMap,IsInQuest>,const
    CThingBuilding> (was HasScriptName/InMap/CThingGameObject47), 0x49C770 CThingSearchTools::PeekClassList (was
    CVertexBufferWin32::DoSizeof; 85 search callers), 0x6A7590 CThingFilter_ObjectHasProperty::operator() (was MarkerHasProperty,
    split from its byte-identical neighbour 0x6A75B0 by the callers' masks), 0x702FF0 CThingObject::GetProperties (was vfunc_48).
  - RENAMES 14 x FindThingsInRoughArea -> CThingSearchTools::GetAllThingsInRoughArea (no PDB FindThingsInRoughArea; every
    row matched to its Ego_r/FableWin instance; 3-parameter template, real std::list/std::vector, PDB filter signatures; also
    In2DRangeB -> In3DRange at 0x7A63D0 and MarkerHasProperty -> ObjectHasProperty at 0x91ADB0).
  - LABELS 0x4AD260, 0x6D44B0, 0x959340 (push_back twins), 0x4AD780 GetAllLeafThings<const CThingBuilding,...,HasTC>,
    0x6A7C20 CThingFilter_IsInQuest::operator().
- 1c vector<unsigned char>::_M_insert_overflow clash (0x467831 vs 0x485B6C): 0x467831 is vector<bool> (one byte per element,
  STLport bool fill) -- RENAMES 0x467831 / 0x4676EF / 0x467686 / 0x4676BB / 0x53CB4F (std::vector<bool>::*), LABELS 0x46780E
  __uninitialized_copy<bool*,bool*>; 0x485B6C stays vector<unsigned char> (caller back_insert_iterator 0x9E98C0).
- 1c basic_string<char>::_M_range_initialize clash (0x5672D0 vs 0x567D00): member-template instances. RENAMES 0x567D00
  `<char*>`, 0x5672D0 `<const char*>`; FIXES 0x568440 (`<const char*>`, char_traits class), 0x55F090 / 0x55F140 (`<char*>`,
  template range ctor), 0x568CE0 (copy ctor `<char*>`). Same pattern found for append: RENAMES 0x567400 append<char*>,
  0x567D60 append<const char*> (was CNetString_Retail::Append); FIXES 0x569690 operator+ and 0x5693B0 assign rebuilt on the
  real basic_string class template (RELOC_CONFLICT on ?append@basic_string@std@@... gone). RENAMES 0x565360 / 0x565A30 /
  0x99C550 BasicStringRaise ("OnInitialActivate") -> std::_String_base<char|unsigned short,...>::_M_throw_out_of_range /
  _M_throw_length_error.
- 1c __destroy_aux<pair<__int64,list<C2DLineF>>*> clash (0xA83420 vs 0x519EF0): two 16-byte element families. 0x51 TU =
  CNavigationAreaUpdate::CSwitchableBlockage, 0xA8 TU = pair<unsigned __int64, list<C2DLineF> >. RENAMES 0x519EF0, 0xA83420,
  0x51C1F0 (vector<CSwitchableBlockage>::_M_insert_overflow), 0x5199E0 (CSwitchableBlockage copy ctor), 0x519FF0 (__copy<const
  CSwitchableBlockage*,...>), 0x51A880 (__uninitialized_fill_n), 0x51C850 (push_back); LABELS 0x51A810, 0x51AC00.
- Chunk 477-481 callee LABELS (17): 0x6FE7C0 GetAllThingsInRoughArea<CThingGameObject,...MinAllowedRadius/OverlappingSphere>,
  0x70B150 vector<CEngineTextureDecal>::erase, 0x6D8BF0 CTCHeroExperience::GetOnHitSound, 0x769650 _Rb_tree<EObjectAugmentationType,
  ...CObjectAugmentationSoundSet>::_M_erase, 0xA14040 C3DVector::RotateXY, 0x7FA610 __unguarded_insertion_sort<CThing**,
  CThingCompare_BestForTargeting2>, 0x681850 IsFactionEnemyToHeroOverriddenOnOpinion, 0x8F28A0 vector<CCard*>::_M_range_insert<CCard**>,
  0x87AA10 / 0x87AFF0 NMaths::GetRootsNumerically<CTanMethod / CCotMethod>, 0x45EF2D vector<float>::operator=, 0x5E9B20
  CTCTavernGame::SetState, 0x6C39A0 CArray<C3DSkeletalBoneMorph>::~CArray, 0xA14440 C3DVector::AccurateNormalise, 0xA14480
  C3DVector::GetMagnitudeAndNormalise, 0x663340 CThingCreatureBase::GetPHeroCombatDef.
- GetSubDef family (chunks 480/481 named 5; done family-wide): all 151 landed GetSubDef bodies are explicit specialisations of
  the member template `CDefClassBase::GetSubDef<T>(CDefPointer<const T>*) const`; T = the class-name literal each body builds,
  every T present in the Ego_r/FableWin name set and unique. RENAMES 149 (142 address-named stand-ins + 7 catalogued
  `CDefTree::GetSubDef<T>`), FIXES 0x787AE0 (catalog right, source a stand-in) and 0x65F604 (the model: its manager callee
  0x9ADA10 is the protected non-const GetPDefFromGlobalIndex overload, not the const one -- see Decisions).
- CThing accessor run (chunk 481 claimed 0x4C7990 = "CThing::GetDefinitionManager"; no such PDB symbol): RENAMES 0x4C7990
  PeekDefinitionManager, 0x4C79D0 GetWorldMap, 0x4C79E0 PeekWorldMap, 0x4C79F0 PeekPlayerManager, 0x4C7A00 GetPlayerManager
  (all CThing), 0x51DF20 CThingManager::PeekMapAt (was a second "GetMapAt"); FIXES 0x51DEE0 (class mangling, PeekMapAt slot);
  LABELS 0x4C79B0 CThing::GetMapAt, 0x4C79C0 CThing::PeekMapAt. 0x4C79A0 (between PeekDefinitionManager and GetMapAt; reads
  defmgr+0xD0) is PeekGraphicBank or PeekGlobalDefinition (FableWin has both there) -- left as is.
- Chunk 477/478 "Landed wrong" + callee items (this session):
  - FIXES 0x7530D0 (callee 0x752B00 = CTCLook::SetHeadLooking, private members, PDB signatures) + LABEL 0x752B00;
    0x768310 / 0x7681D0 (callee 0x766C00 = SetupAugmentationSounds, private; OnSetInLimbo virtual).
  - CTCBase run (Ego_r/FableWin order = retail body order, see Decisions): RENAMES 0x6869C0 StatGetDriverType, 0x6869D0
    GetDefinitionManager (was "CTCHeroStats::GetDisplayEngine"; 389 call sites), 0x686D20 PeekWorldMap, 0x686D30 GetWorldMap,
    0x686D40 GetPlayerManager, 0x686D50 PeekPlayerManager (was GetPlayerManager), 0x686D60 PeekComponent, 0x686D70
    PeekSampleBank, 0x686DA0 GetPWorldSeed, 0x51E480 / 0x51E490 CThingManager::Peek/GetThingSearchTools; FIXES 0x686DB0
    GetPLocalSeed (was a free function); LABELS 0x686D80 / 0x686D90 CTCBase::Get/PeekThingSearchTools; RENAME 0x686830
    ~CTCBase (source was a `struct C` stand-in).
  - CEngineTextureMorph vector cluster (chunk 477 "mislabelled STL copy"): RENAMES 0x5BBDF1 vector<CEngineTextureMorph>::vector,
    0x5BBC93 __uninitialized_copy<const CEngineTextureMorph*,CEngineTextureMorph*>, 0x5BA5AD _Vector_base<CEngineTextureMorph>
    (was the invented SRecord20), 0x5B9DE7 allocator<CEngineTextureMorph>::allocate; LABEL 0x5BB9FA the element copy ctor.
  - RENAME 0x70ADB0 _Rb_tree<long, pair<const long, CEngineTextureDecal>, ...>::_M_erase (private, 5-argument STLport).
  - Deed log: RENAMES 0x6C5630 CTCHeroOpinionDeedLog::PeekPDeedEffectsDef (chunk 478's "GetDeedEffectsDef" has no PDB
    symbol), 0x6C55E0 SetVillageForCrimes, 0x6C76B0 CDefinitionManager::GetDef<COpinionDeedEffectsDef>.
  - RENAME 0x4AB490 std::list<CThing*>::~list (CTCCreatureItemLevitationBase::LevitatingItems).
  - Sound player (sweep2 owner item 0x66ADD0): RENAMES 0x66AD90 ReleaseAttachedLoopingSound, 0x66ADD0 StopAttachedSound
    (catalog "StopSample" has no PDB symbol), 0x66AE60 IsAttachedSoundLoadedAndPlaying.
- Chunk 477-481 remainder (this session, after the context break):
  - RENAME 0x7688B0 std::uninitialized_fill_n<CDefPointer<const CThingBaseDef>*, unsigned int, ...> (was
    UninitializedFillN_IVCountedPointer); FIX 0x7AF980 std::fill element CIntelligentPointer<CThing> -> <CThingAICreature>
    (+ LABELS 0x7AF6C0 CTCBoss::AddEffect, 0x7AF710 CTCBoss::AllocateHelperCreatures, 0x7B00C0 / 0x7AFE20
    vector<CIntelligentPointer<CThingAICreature> >::resize / _M_fill_insert). Twin 0x7B83D0 (CThingObject) checked: right.
  - Re-path RENAMES (catalog name already right, source a junk stand-in, pass pattern not `<addr>_TEST PASS`): 0x84F1B0
    NCameraModes::CBase::GetObservedThing, 0x4047A0 CStringParser::~CStringParser (compiler-generated: no vptr re-store),
    0x414EA0 std::vector<CCharString>::~vector.
  - 0xA14440 = C3DVector::AccurateNormalise family: FIXES 54 landed callers (53 by generator + 0xA0C2B0 CCamera::PointAt by
    hand): `void Normalise()` -> `C3DVector& AccurateNormalise()`, C3DVector declared `class` (the PDB key). 0x486F60 only
    mentions the address in a comment (left).
  - 0x869020 SetAsModifiedByArmour: FIX (five anonymous-namespace armour helpers under their Ego_r names); RENAME 0x868B40
    GetToughnessDamageReductionFactorForThing (was CTCBase_CalculateEffectiveDamageMultiplier).
  - Interface 0xAD = CTCBoss: FIX 0x8C8960 (CTCBanditKingWeapons -> CTCBoss::NextWeapon, static
    CThingCreatureBase::CreateWeaponForThingFromDef). This led to the GetInterfaceType family: RENAMES 244 TC vtable slot-21
    rows `X::vfunc_21` / gapscan -> `X::GetInterfaceType` (237 with the PDB symbol, 7 TLC-only classes by slot + body).
  - PositionMesh family: FIXES 0x8EFA00, 0x8EF2F0, 0x7D60C0, 0x7D2EF0, 0x7D7F10, 0x7D4E30, 0x7D7FD0 (static __fastcall
    CTCTavernGame::PositionMesh(CThing*, const C3DVector&, const CRightHandedSet&, const C3DVector&, float, float)).
    0x8F06F0 CCard::FrameUpdate has a source file but no catalog row (not landed); its edit is in work/pm/008f06f0.cpp.
  - Checked, no row: 0x569140 / 0x567120 / 0x569E30 / 0x5E1420 / 0x440990 (already STLport templates with the right
    elements), 0x99EAE0 ~CCharString, 0x8F9780 ~LSocket, 0x73A6E0 (label already CTCCreatureModeManager::GetCurrentMode;
    body = *PModes.front()).
- Family pass 2 bindings (coordinator item a):
  - CBaseClass family (0x99A2D0 ctor, 0x99A2E0 `??1CBaseClass@@UAE@XZ`, 0x99A2F0 `??0CBaseClassNonCopyable@@IAE@XZ`,
    0x99A300 `??1CBaseClassNonCopyable@@MAE@XZ`): RENAME 0x8385B0 CThingNoise::Create (was `_global::Create`; the static
    member, CTCParamsPhysicsBase hierarchy from RTTI), FIXES 0x418B7A (NonCopyable protected ctor/virtual dtor -> MAE),
    0x4C4160 (`virtual ~CBaseClass()` -> UAE).
  - 0x91F740 = CThingCompare_Nearest::operator()<const CThingObject> (LABEL): FIXES 0x91FA00 / 0x91FB30 / 0x920160 /
    0x9204F0 / 0x9206A0 (sort family element `const CThingObject*`, was CThing*).
  - 0x478876 = CAbilityTrackLevelDef::operator= (vtable Copy thunk): RENAME 0x478C2D std::vector<CAbilityTrackLevelDef>::erase
    (was vector<CBalverineBattleDef>).
  - 0x42C058 tree model: RENAME 0x42C058 = equal_range of the CTCInventoryTrade map<NUISystem::CComponent*,
    NUISystem::CComponent*> (was _Rb_tree<unsigned long, pair<const unsigned long, long> >); RENAME 0x5D84F2 = the real
    map<unsigned long, long>::_M_lower_bound (was CMemoryAllocatorVariableSize_FindNodeLinear); LABELS 0x42AE90 / 0x42AEB5 /
    0x42CDE0 / 0x42B854 (CComponent tree) and 0x5D8446 / 0x5D8676 (unsigned long tree).
  - Found on the way: RENAME 0x478751 std::_Destroy<CAbilityTrackLevelDef*> (was DestroyRange_Elem0x48; manifest
    vector<CIntelligentPointer<CThing const>> propagated); RENAME 0x4537E9 CTCNoiseDef::operator= (catalog said
    CBalverineBattleDef::operator=; Copy thunk in .?AVCTCNoiseDef@@ slot 19).
- Queue 2 catalog lags: same-path RENAMES 0x431F10 CDefClassBase::operator=, 0xC0BCC0 CString::SafeStrlen, 0xCAF580
  CCmdQueue::~CCmdQueue (path re-derived), and two more found by the operator= scan, 0x460657
  CSpecialAbilitiesDivineWrathDef::operator= / 0x471A77 COpinionDeedEffectsDef::operator= (both catalogued as
  CParentDefClassBase::operator=). 0x121AEC0: catalog and source agree (descriptive name, Leaf _$E1); only the manifest's
  `$E2` lags -> owner item, no row.
- Chunk 482 landed-wrong:
  - Pending-map tree helpers: RENAMES 0x98FE70 _M_find, 0x98FCA0 _M_upper_bound, 0x9901D0 _M_lower_bound, 0x9905D0
    erase(first, last), 0x98FD40 _M_erase -- all std::_Rb_tree<CThreadedFileManager::CPendingObjectSortKey, ...> on one
    STLport 4.6 template (one CPendingObjectSortKey::operator< for all three walks).
  - 0x98E770 (Dinkumware-shaped pending map): UNRESOLVED (std::map model 91v91, one push/lea swap; 3 attempts).
  - CParsedItem getters: RENAMES 0x9B9870 GetSymbol (was a second "GetString"), 0x9B9880 GetString (re-path), 0x9B9890
    GetInteger, 0x9B98A0 GetFloat; FIXES 0x998170 / 0x997E90 (callee is GetSymbol), 0xA63DB0 (callee is GetString), and the
    callers binding invented accessor names: 0xA63CC0 / 0xA640D0 / 0xA64130 / 0xA63D20 (SParsedItem::GetAsLong/GetAsFloat,
    PDB access + class keys), 0xA63EE0 (ParsedItem / ParserMethods wrapper), 0x9EB2C0 (const + class keys, virtual
    ~CBaseClass).
  - 0x9AF1E0 CDefinitionManager::CompileString (FIXES: protected PDB access via the layout-rename include pattern).
  - CAIBrain output: RENAME 0x88B8F0 CAIBrain::SetOutput (was CQuestManager::RemoveAvailableQuestCardType); FIXES 0x97D8B0
    (callee SetBrainOutput -> SetOutput, class keys) and 0x88BCB0 (`output.Merge(Output)` -> `output = Output`, the callee is
    CBrainOutput::operator= 0x88C840).
- Sweep2 owner item, NScript namespace: 414 catalog rows named C[VQ]_*Script::... without NScript (326 global sources, 88
  already wrapped). Generator work/ns/gen.py wraps the class in `namespace NScript`, struct -> class + public:, forward
  decls, out-of-line qualifiers, mangled leaves; 411 graded MATCH/RELOCATION_MATCH + PASS (3 had no class definition).
  Then label checks (label_trust, catalog uniqueness, the source's own Identity/propagated notes, address locality):
  - RENAMES kept: 336 + 0x00D6EF20 (restored once its catalog twin was resolved) + 4 by hand: 0xD643A0 / 0xD67270
    CScorpionHome::Main and 0xD9D4F0 CQ_HobbeToothWhisper::Init nested in their parent script class (Ego_r spells them so,
    private virtual), 0xE398F0 = NScript::CV_BordelloScript::CV_BordelloScript (catalog CQ_HangingTreeGoodScript ctor was
    propagated; the source already modelled Bordello).
  - 0xEDCB80: CScriptThing class-keyed in that source too (its Leaf already spelled V); 6 sources whose class sat outside an
    existing `namespace NScript` block were regenerated (generator depth check).
  - Held (UNRESOLVED, 65 rows once 0xD6EF20 was restored): the class attribution is not verified (manifest MISLABELLED_FAMILY with the manifest label
    unchanged and no Identity line; duplicate catalog names; sources that call their own label propagated; rows outside
    the script region 0xCF0000-0xF30000: 0xB2FA40, 0xC12040). The generated sources stay in work/ns/src/.
  - 5 rows dropped as propagated labels: 0xC393F0 / 0xC395B0 / 0xC39770 / 0xC39930 / 0xC49C30 ("CV_StatueMasterScript::
    GetAngleBasedOnTimeOfDay", five byte twins in the 0xC39xxx mailbox-queue run) -> UNRESOLVED.
  - The 3 rows with no class: 0xCA5030 = CRendererInputPin::BeginFlush (IPin vtable slot 15; renbase.cpp), 0xD17E70 =
    NScript::CQ_BountyHuntScript::RemoveBoastQuestInfo (FableWin; retail order; Ego_r members), 0xE9FEC0 =
    NScript::CV_KnotholeGladeGatesScript::InitialiseVariables (FableWin; no ResetMissionState exists in either PDB).
    Companions found on the way: 0xCA8E10 CBaseInputPin::BeginFlush, 0xCA50D0 CRendererInputPin::EndFlush.
- CSystemManager (sweep2 owner item, callers of 0x9A4EC0 GFGetSystemManager): RENAME 0x9A4FB0
  CSystemManager::IsApplicationInputFocus (was GetCanItemBeQuickAssigned; `mov al,[ecx+0Ah]` = Ego_r ApplicationHasInputFocus
  +10); FIXES 0x4162B5 (class key -> `YIPAVCSystemManager`) and 0xAB4830 CMouseDX::UpdateAcquired (__fastcall, class key,
  IsApplicationInputFocus). The other 12 callers of 0x9A4EC0 and 0x9C9070 / 0x9C7A40 / 0x9CC410: deferred, owner item.
- RENAME 0x4774AB std::_Vector_base<CEngineLocalDetailObjectDef>::~_Vector_base (was the descriptive FreeMember0 /
  catalog CActiveFile::OnReadFinished; only caller CEngineLocalDetailLayerDef::~CEngineLocalDetailLayerDef).
- NTCScriptedControl::CActionFollowCreature (sweep2 owner item): FIXES 0x713EE0 Update, 0x715710 OnActivate, 0x7158F0
  OnCleanup -- class in its namespace with Ego_r members (PTCScript, Terminated), virtual slots, callee 0x713D50 bound as the
  private SetAsSneaking (`AAEX_N@Z`; it was an invented global SetSneaking).
- std::list<NUISystem::CComponent*> (sweep2 owner items "0x42ABAF / 0x42AB8A list stand-ins" and "0x42ABCA leaf U?$allocator"):
  RENAMES 0x42A008 _M_create_node (was CRetailList_node0xc_dword::Buynode), 0x42AB8A insert (was a stdcall Std_List_Insert),
  0x42ABAF list(const allocator_type&) (was a free function `list`); FIX 0x42ABCA (allocator class-keyed, Leaf V?$allocator).
  All four on one STLport 4.6 model (class allocator, struct _List_node_base/_List_node, iterator with a user copy ctor,
  protected _M_create_node); relocations bind _malloc and ?_M_create_node@... as retail does.
- 1d `_Rb_tree` access pass (sweep2 owner item; done): every landed source mentioning `_Rb_tree` was compiled (1,378) and
  its object symbols demangled; 190 emitted or called `_M_insert` / `_M_copy` / `_M_erase` / `_M_create_node` /
  `_M_clone_node` as public (`QAE`). FIXES 188 (generator work/acc/gen.py: the member goes under STLport 4.6's access and
  the previous access is restored after it; `struct _Rb_tree` -> `class` with `public:` and a public base; full mangled
  symbols in Leaf lines rewritten). The other 2 (0x9AAF70 / 0x9AB120) are sweep3 RENAMES whose new sources already used it;
  the two sweep3 RENAMES 0x98FD40 / 0x9905D0 were regenerated and regraded in place. Checks: every one of the 188 regrades
  MATCH/RELOCATION_MATCH with its test PASS, and an old-vs-new symbol diff of each object shows only the access-letter
  (and U->V `_Rb_tree` key) changes, nothing else, and no member left public. On the way:
  - 27 bare or ambiguous Leaf lines (`_M_create_node`, `??0_Rb_tree`, `?erase@?$_Rb_tree@`...) made exact: the one
    defined symbol that contains the old leaf and grades against the retail bytes (0x9905D0 = the two-iterator erase).
  - 8 tests that link the source object's callees got the same access in their own `_Rb_tree` model.
  - 0x4E4BEE: EObjectAugmentationType and CWeaponTrailGraphicSet had been declared inside `namespace std`; Ego_r spells them
    global (`W4EObjectAugmentationType@@` x388, `VCWeaponTrailGraphicSet@@` x70). Moved out, class-keyed, tree given its
    five STLport parameters (same row).
  - Not in scope, still stand-ins (queue 3): non-template `_Rb_tree` models in invented namespaces at 0x756250
    (`X_00756250::std`), 0x9EFB80 (`CacheTree::std`), 0x801CB0 / 0xADBA40 (`RecutTree_*`), 0x7AD060 (`FableTree_*`),
    0x4F97B3 (`NPairLongMapTree`), 0x4F9960 (global `_Rb_tree`), 0x5E2580; their access is fixed, their names are not.
  - Vector `_M_insert_overflow` / `_M_clear` access (the other half of sweep2's deferred item): not applied. Only one
    source says protected (STLport 4.6 `_vector.h`); libstdc++ has no such members to cross-check and the PDB sets are
    Dinkumware. Owner item.
- Family pass 2 label corrections (FINDINGS line 631, agent951 G0xx groups that name landed rows; work/fp2):
  - RENAMES 14 `std::vector<T>::resize` (catalog "CArray<pair<...>>::push_back" / invented): 0x464093 EWeaponClass,
    0x4658DF CDefString, 0x468FA5 CRGBColour, 0x469851 ECreatureGeneratorGenerateType, 0x46CB84 EActionRegisteredType,
    0x4721B3 EOpinionAttitudeType, 0x4E8705 EVillageTaskType, 0x4E8A87 CDefIndex, 0x47662A CAbilityButtonGuiDef, 0x476ACF
    CLensFlareElementDef, 0x479595 COpinionDeedReactionDef, 0x4799C6 CEngineLocalDetailLayerDef, 0x479FF3
    CAppearanceModifierScalingDef, 0x47A407 CAbilityTrackDef. Each T comes from the def's Transfer member offset
    against its Ego_r layout and from the landed erase / `_M_fill_insert` callees.
  - RENAMES 7 `std::__uninitialized_copy<std::pair<K, CCountedPointer<T> >*, ...>` (0x448280, 0x6B8DA0, 0x827EC0,
    0xA13580, 0xA7F920, 0xAE9860, 0xB8EDE0), 3 `fill_n` (0x4482C0, 0x6B8DE0, 0xAE98A0), `__destroy_aux` 0xA80E40
    (CNavConnectorNode) and the catalog lag 0x4488B0; LABELS 9 (fill_n 0xA7F960 / 0x827F00 / 0xB8EE20 / 0xA135C0,
    `__destroy_aux` 0x4AD9E0 / 0x825AE0 / 0xB8F470 / 0x519050 / 0xAE9D50).
  - FIXES 0xA79290 (pair declared `struct`: binds `U?$pair` as the PDB does), 0x493B5B CMessageEventManager::RemoveMessage
    (real STLport list<CMessageEvent>, class key, PDB `QAEXPBVCMessageEvent@@@Z`); RENAME 0x882C40
    `std::_List_base<NScreenEffect::CScreenEffectPriorityStack::CScreenFilterEntry>::clear`.
  - G030 "_Destruct callees": FIXES 8 scalar deleting destructors (0x4E2DCB, 0x4E32A6, 0x65ED3D, 0x65EDF0, 0x65EEA0,
    0x65EF35, 0x65EFD8, 0x903367). Each is now the compiler-generated `??_GX@@UAEPAXI@Z` of a class whose virtual destructor is
    the landed `??1X@@UAE@XZ` row, emitted through a `new X` factory. They had been `__fastcall` stand-ins calling extern
    `X_Destruct` / `OperatorDelete`.
  - G022 / G058 vtable Copy thunks: RENAMES 15 `X::Copy` (`?Copy@X@@UAEXPBVCDefClassBase@@@Z`, the virtual
    `*this = *static_cast<const X*>(other)` that tail-jumps to X::operator=): 0x462969 CSimBuildingDef, 0x47852E
    CAppearanceModifierScalingDef, 0x478871 CAbilityTrackLevelDef, 0x4E0EB7 COverheadDisplayDef, 0x4E1287
    CWallMountEffectsDef, 0x4E1315 CTeleporterDef, 0x4E1A59 CHasNameDef, 0x4E4857 CBalverineBattleDef, 0x4537A0 CPhysicsDef,
    0x461ED3 CLightningDef, 0x4E01D9 CDoorDef, 0x4E02ED CSpotLightDef, 0x4E0707 CStockItemDef, 0x4E0A67 CShipDef, 0x4E10C1
    CTavernGameCoinBaseDef.
  - G070 thing search: FIX 0x7FD110 (filter CThingFilter_HasScriptData -> CThingFilter_HasScriptName, operator() on
    `const CThing&`); LABELS 0x914330 GetPBestThing<HasScriptName, Nearest, CThingMarker>, 0x6A7E30 / 0x6A7E70
    HasScriptName / HasScriptData operator(); 0x913F30 -> UNRESOLVED (family-wide model, see Owner items).
  - FIX 0x768120 (AttachAugmentationParticles / SetupAugmentationSounds private, PDB `AAEXXZ`).
  - CThing accessor run, continued: RENAMES 0x4C7A10 CThing::PeekPlayer, 0x4C7A30 CThing::GetPlayer; FIX 0x85F8D0 (binds
    PeekPlayer and AccurateNormalise).
- Chunk 483 items that name other landed rows:
  - CCamera getters: RENAMES 0xA0BE80 GetFOVFlags, 0xA0BE90 GetHorizontalFOV, 0xA0BEA0 GetVerticalFOV, 0xA0BEB0 GetFOV;
    FIXES 0x6977F0 / 0x697F50 (callee 0xA0BEB0 is GetFOV); LABELS 0xA0BEC0 GetZoom and the angle run 0xA0C380 GetAngleXY,
    0xA0C3E0 GetAngleYZ, 0xA0C450 GetAngleXZ, 0xA0C570 SetAngleXY, 0xA0C6F0 SetAngleYZ.
  - Deque: RENAMES 0x48C820 `std::_Deque_base<C2DVector>::_M_destroy_nodes`, 0x48CAF0 `std::_Deque_base<C2DVector>::~_Deque_base`
    (catalogued as CBankFileAsyncData::FreeBuffer); FIX 0xA0F040 deque<C2DVector>::erase on the real `_Deque_base`.
  - FIX 0x9B5DB0 (callee 0x9B3630 is the member template `_M_range_insert<const unsigned char*>`).
  - RENAME 0xA04380 CInputEvent::GetMousePos; LABELS 0xA043D0 GetMouseMovement, 0xA043F0 GetHeldMouseMovement.
  - RENAME 0x9BE100 std::list<CSubTitle>::pop_front (catalog lag; the source was already right).
  - LABELS 0xA112F0 CMatrix3x4::TransformPlane, 0xA0DB60 GFInitMaths, 0xA0DCD0
    CJoystickPositionTracker::HasPerformed360ClockwiseMove, 0x9E5C60 CSymbolMap::UpdateDirty, 0x9E1650
    std::vector<CQuickDrawTriInfo>::_M_clear.
  - UNRESOLVED 0x9DB810 AddTri / 0x9DC030 Add2DTri (the `__false_type` overloads: family-wide, see Owner items).
  - The chunk's own rows (0x9BD910, 0x9E6A70, 0x9F1D20, 0x9E2590, 0xA0C830, 0xA043A0) are already catalogued under the
    names chunk 483 gives (0x9F1D20 keeps its descriptive `InitializeMemorySystem`; the chunk says the name describes
    its caller). 0x9FA620 is already labelled CTexture::GetNextValidDimensions.
- FIX 0x485CF1 ~CPersistContextReadSection (last manual-drop review): the local CStringParser model's destructor is virtual
  (both PDBs `??1CStringParser@@UAE@XZ`), so its Parser member call binds the UAE spelling that RENAME 0x4047A0 defines.
- LABELS hygiene: the 23:19 catalog update landed 0x91F740, 0x5D8676, 0x42CDE0 and 0x42B854. Their new sources already
  model the identities sweep3 had labelled (`operator()<const CThingObject>`; the map<unsigned long, long> equal_range;
  the two CComponent-map erases), so those 4 LABELS rows were removed (copy in work/LABELS.before_landed_check.tsv).

## Decisions
Each paragraph says what was decided, the evidence, and the rows it touches. The full per-row evidence is in the TSV
evidence columns.

**D1. 0x8CFF40 is CGameScriptThing::CreateNullThing, not a CScriptThing constructor.** The bytes are a static fastcall
that returns a CScriptThing by value and forwards to CGameScriptThingManager::GetNullScriptThing 0x704730 on the quest
manager's PGameScriptThingManager. `?CreateNullThing@CGameScriptThing@@SI?AVCScriptThing@@XZ` is the only static fastcall
CScriptThing producer in Ego_r. The real default constructor is 0x6E7B40. Rows: RENAME 0x8CFF40; FIXES of its 6 callers
and the 6 F014 members that used `CMember_006e7b40`; drop of `??0CScriptThing@@QAE@XZ -> 0x8CFF40`.

**D2. The 575 `Create_CVtable_XXXX` rows are `NScript::<Script>::<Entity>::Alloc`.** 567 are proven by the PDB Alloc
symbol of the bound/registered entity class, with the Ego_r object size equal to retail's `new` size. The 8 irregular
rows are proven individually (elimination among the shared-factory doors at 0xDD90C0, plus the vtable / RegisterScripts
entries). The 18 rows of 153 bytes also match their constructor member at +0x1C.

**D3. Container wrappers are named by the container member that retail inlines.** 0x4BAA30 is `std::set<CCharString>::find`
and 0x7ADB30 is `std::map<CCharString,bool>::find`: each is a thin `_M_t.find` with `_Rb_tree::find` inlined around the
landed `_M_find`, and the emitted leaf is the set's or map's find symbol. Retail has no ICF (D20), so one body per
instantiation.

**D4. 0x76C7D0 is CTCEnemy::IsEnemyOfFaction (it was misnamed IsAlliedWithFaction).** It null-tests PFaction (+0x18) and
tail-jumps to 0x6B94C0, which is CFaction::IsEnemyOfFaction: it tests EnemyFactions at +0x10, while 0x6B9460 tests
AlliedFactions at +4. Its twin 0x76C7F0 tail-jumps to the allied test. FIX 0x6B9460 replaces the flat CCharStringSet
stand-in with the STLport `set<CCharString>` that both bodies use.

**D5. The bank maps are real STLport maps.** `map<CCharString, CBankFileManager::CBankInfo>` is at 0x9AB120, with the pair
copy constructor 0x9A9AD0 (CCharString plus 4 CWideString plus a word). `map<CCharString, NBankFileManager::CContainedBank>`
is at 0x9AAF70, with `_M_create_node` 0x9AA9E0 (malloc 0x28). They replace the invented SBankRecord / SRecord20 /
SBankEntryInfo types. FIX 0x9A7F80 FindContainedBank takes its PDB signature.

**D6. TransferMap template arguments are `<V, K>`.** The PDB's `TransferMap<V,K>` takes `map<K,V>`, so the landed
`<CCharString,bool>` was swapped. Each address gets one instantiation: 0x7AE3D0 `<bool,CCharString>`, and 0x757160 is the
2-argument `<pair<long,long>,CCharString>` whose 3-argument overload 0x756D00 is a label. 0x7AE3D0's callee 0x7ACC90 is
`_Rb_tree::_M_find`, not lower_bound.

**D7. C3DAnimationStats::HasEvent / GetEventTime** work through TimingEvents (`multimap<CCharString,float>`, +0x20) and its
`_M_find` 0xA54310. The PDB names both functions. The catalog's CNavQuadTree::IsActionPoint was propagated.

**D8. 0x518850 holds `vector<const CThing*>` and 0x5198B0 holds `vector<CThing*>`.** Their overflow callees settle it:
0x49C370 is `vector<const CThing*>::_M_insert_overflow`, while 0x436AC0 is the one for `vector<CThing*>`. The Ego_r
`GetAllThings` instance names agree. 0x5198B0's filter is `CThingFilter_HasTCOfLeafType<CTCActionUseScriptedHook>`.

**D9. Mask 0x47 (the invented CThingMask47) is CThingBuilding.** Type list 3 is buildings and type list 4 is villages,
from the constructors. THING_TYPE_BUILDING is 0x40 and the CThingGameObject mask is 7. The only None-filter
`GetAllThings` / `GetAllLeafThings` PDB instance with that mask is `<CThingBuilding, ...>`. Rows: FIXES 0x66E5A0, 0x4AD700,
0x6D4600 (that one is CThingVillage), the overflow rows, and RENAME 0x512F60.

**D10. The 1c clashes are three pairs of different instantiations.**
- 0x467831 is `vector<bool>` (one byte per element, STLport bool fill), while 0x485B6C stays `vector<unsigned char>`.
- 0x567D00 and 0x5672D0 are the `<char*>` and `<const char*>` instances of the member template `_M_range_initialize`.
  The same holds for `append`.
- 0x519EF0 is in the CSwitchableBlockage family (0x51 TU), while 0xA83420 is `pair<unsigned __int64, list<C2DLineF> >`.

Each caller now binds its own instance.

**D11. `GetAllThingsInRoughArea`, not `FindThingsInRoughArea`.** No PDB has FindThingsInRoughArea. All 14 rows match their
Ego_r/FableWin `CThingSearchTools::GetAllThingsInRoughArea<T, Container, Filter>` instance, built on real std::list /
std::vector and the PDB filter signatures.

**D12. GetPDefFromGlobalIndex has two overloads.** The public const QBE is at 0x9AD9E0 and the protected non-const IAE at
0x9ADA10, 0x30 apart as in both PDBs. Const GetDef<T> calls the const one, while GetSubDef calls 0x9ADA10. Row: FIX
0x65F604, the model for the GetSubDef family.

**D13. GetSubDef is one member template, applied family-wide.** All 151 landed bodies are explicit specialisations of
`CDefClassBase::GetSubDef<T>(CDefPointer<const T>*) const`. Each T is the class-name literal its body builds, and every T
is in the PDB name set and unique. Rows: 149 RENAMES and FIXES 0x787AE0 / 0x65F604.

**D14. The CThing accessor runs are named by PDB order.** The Ego_r/FableWin order is PeekDefinitionManager, GetMapAt,
PeekMapAt, GetWorldMap, PeekWorldMap, PeekPlayerManager, GetPlayerManager, PeekPlayer, GetPlayer. This equals retail
0x4C7990..0x4C7A30, and each body's const-ness and callee agree. No `CThing::GetDefinitionManager` exists, so chunk 481's
claim was corrected to PeekDefinitionManager. 0x51DF20 is CThingManager::PeekMapAt, the const twin 0x40 after GetMapAt.
0x4C79A0 is ambiguous (PeekGraphicBank or PeekGlobalDefinition) and was left alone.

**D15. The CGameDefinitionManager cluster.**
- 0x44C6AF ErrorCallback, 0x44C6B0 Get (static, `SIAAV1@XZ`), 0x44C6B6 IsAllocated and 0x44C6C2 the constructor (a label)
  sit in Ego_r order with Ego_r sizes 1/6/12.
- 0x415FBC is CMainGameComponent::PeekDefinitionManager, a 5-byte tail jump in both Ego_r and retail.
- 0x42B040 is `GetDef<CThingObjectDef>`, from the name set, the exact binding of landed 0x706370 and its CThingObject callers.

**D16. No ICF in retail.** Byte-identical bodies are distinct functions (e.g. 0x6A7590 / 0x6A75B0, and 0x811340
CTCEntityEvents::HasEventOfType, whose catalog name IsModeActive belongs to its byte twin 0x73A8A0). Each address is named from its callers and its PDB neighbours, never
from a twin's label.

**D17. AnimExists is a base/override pair.** 0x662CC0 is `CThingCreatureBase::AnimExists` (public virtual UBE) and 0x830F20
is `CThingAICreature::AnimExists` (MBE override). Both sit in vtable slot 73 (CThingPlayerCreature 0x12457FC and
CThingAICreature 0x127293C respectively).

**D18. Some claims were rejected or left unresolved by the evidence.**
- 0x662010 reads the creature def's field +0xF0, not the AI brain, and no PDB accessor exists, so it is not
  GetCurrentStateGroupType.
- 0x7AEF60 is CTCBoss::ClearEffects: no KillDependentThings exists, and the constructor calls this body.
- 0x83FB20 Open-versus-SetConnectionDetails could not be settled from two sources.
- The CStringParser Skip run identities are proven (0x9B9BA0 SkipPastString, 0x9B9C00 SkipPastWholeString), but both
  failed to grade within 3 attempts.
- 0x98E770 (the real map model) failed on one VC7.1 push/lea swap after 3 attempts.

All of these are in UNRESOLVED.

**D19. The CTCBase accessor run is named by Ego_r and FableWin order.** Retail 0x6869B0..0x6869E0 is HandleMessage,
StatGetDriverType, GetDefinitionManager, EndSerialising, and 0x686D20..0x686DA0 is the Peek/Get run. This overturns
chunk 389's 0x686D50 (it is PeekPlayerManager, and 0x686D40 is GetPlayerManager). The CThingManager
Peek/GetThingSearchTools pair (0x51E480 / 0x51E490) cross-checks it. 0x686DB0 GetPLocalSeed is a member, not a free
function.

**D20. PDB names win over FINDINGS paraphrases.**
- 0x6C5630 is PeekPDeedEffectsDef: no GetDeedEffectsDef exists.
- 0x66ADD0 is StopAttachedSound: no StopSample exists. Its siblings follow by order and emitter callees.
- 0x7688B0's element is `CDefPointer<const CThingBaseDef>`.
- 0x7AF980's element is `CIntelligentPointer<CThingAICreature>` (RTTI of vtable 0x123E7E0).

**D21. The CEngineTextureMorph vector cluster** (0x5BBDF1, 0x5BBC93, 0x5BA5AD, 0x5B9DE7) has one element type. The proof is
the element copy constructor 0x5BB9FA and the caller CEnginePrimitive2DMorphedAnimatingMesh, whose PDB constructor takes
`vector<CEngineTextureMorph>`. This replaces the invented SRecord20. 0x70ADB0 is the map<long, CEngineTextureDecal> tree
erase. 0x4AB490 is `list<CThing*>::~list`, on LevitatingItems (+0xC).

**D22. ~CStringParser (0x4047A0) is compiler-generated and virtual.**
- Retail does not re-store the vptr: 35 bytes, where a user-written destructor is 41.
- It is a COMDAT in an early TU.
- Both PDBs spell it `??1CStringParser@@UAE@XZ`, because CStringParser derives from CBaseClass.

So the RENAME has no user destructor. FIX 0x485CF1 makes its local CStringParser model's destructor virtual: vptr plus
0x24 bytes, still 0x28. The member call then binds the UAE spelling, and the manual drop of the QAE binding stands.

**D23. GetInterfaceType family.** TC vtable slot 21 returns the interface constant, the slot just before
GetClassTypeName. 237 rows have the PDB symbol. 7 TLC-only classes are proven by slot and body. Interface 0xAD is CTCBoss
(0x7AFCD0), so FIX 0x8C8960 replaces the invented CTCBanditKingWeapons with CTCBoss::NextWeapon.

**D24. 0xA14440 is C3DVector::AccurateNormalise, and C3DVector is class-keyed.** Ego_r has only AccurateNormalise and
GetMagnitudeAndNormalise out of line after RotateXY. The body is fsqrt, reciprocal, scale and return this. 54 landed
callers were fixed: `void Normalise()` became `C3DVector& AccurateNormalise()`, with `class C3DVector`.

**D25. The armour helpers (0x869020) use their Ego_r names in an anonymous namespace,** following the repo convention. The
hash is unstable, so the leaf of the graded row does not depend on it. 0x868B40 is
GetToughnessDamageReductionFactorForThing.

**D26. PositionMesh is the static `CTCTavernGame::PositionMesh(CThing*, const C3DVector&, const CRightHandedSet&, const
C3DVector&, float, float)`, a __fastcall static.** The 5th parameter is a float (x87 loads). Its 7 landed callers are fixed.

**D27. Exact leaves.** Where a generated or landed `// Leaf:` was bare or matched 0 or 2 symbols, the leaf became the one
defined symbol that contains the old leaf and grades against the retail bytes (`finish.py`, `work/acc/leafup.py`).

**D28. The CBaseClass destructors.**
- 0x99A2E0 is `??1CBaseClass@@UAE@XZ`.
- 0x99A300 is `??1CBaseClassNonCopyable@@MAE@XZ` (protected virtual), with the protected constructor
  `??0CBaseClassNonCopyable@@IAE@XZ` at 0x99A2F0 (vtable 0x129A7C4).
- Rows: FIXES 0x418B7A and 0x4C4160; RENAME 0x8385B0 CThingNoise::Create, whose CTCParamsPhysicsBase hierarchy comes from
  RTTI.
- The two wrong `??1CBaseClass` bindings are dropped.

**D29. The 0x91F740 sort family is over `const CThingObject*`.** 0x4A8D80 is `operator()<CThing>` and 0x91F740 is
`operator()<const CThingObject>`. The 5 sort rows (0x91FA00..0x9206A0) take that element.

**D30. Catalog lags are same-path or re-path RENAMES.**
- Same path, where the source was already right: 0x431F10, 0xC0BCC0, 0xCAF580, 0x460657, 0x471A77, 0x4488B0,
  0x9BE100, 0x8385B0.
- Re-path, where the source was a junk stand-in: 0x84F1B0, 0x4047A0, 0x414EA0.
- 0x121AEC0 has only a manifest lag (`$E2`), so it is an owner item with no row.

**D31. Tree helpers use the real STLport 4.6 five-parameter `_Rb_tree` in `std`, with one key `operator<` per TU.** The
pending-object map helpers 0x98FE70 / 0x98FCA0 / 0x9901D0 / 0x9905D0 / 0x98FD40 share one CPendingObjectSortKey
comparison. The same model is used for the bank, CComponent and unsigned long trees.

**D32. CParsedItem getters by body order.** Retail 0x9B9870..0x9B98A0 is GetSymbol, GetString, GetInteger, GetFloat, in PDB
order. The catalog had two "GetString" entries, and 0x9B9870 is GetSymbol. The callers that bind the wrong getter or
invented accessors are fixed.

**D33. CAIBrain::SetOutput (0x88B8F0) uses a local model.** engine/CAIBrain.h has the Ego_r offsets: retail is 4 lower
(Output +0x44, OutputValid +0x8C). 0x88BCB0's `Merge` was CBrainOutput::operator= 0x88C840.

**D34. CDefinitionManager methods use the layout-rename include pattern** (the engine header under a layout name, with the
class deriving from it) instead of the `CDefinitionManager_Methods` wrapper. The protected PDB access then mangles as
retail does. Row: 0x9AF1E0.

**D35. All CV_/CQ_ script classes live in `namespace NScript`, class-keyed,** as both PDBs spell them, consistent with the
Alloc family.
- Applied to 411 generated rows, keeping only those whose class attribution passes label_trust, catalog uniqueness, the
  source's own identity notes and address locality.
- 70 rows are held in UNRESOLVED.
- Nested entity classes and private virtuals follow Ego_r (0xD643A0 / 0xD67270 / 0xD9D4F0).
- 0xE398F0 is the CV_BordelloScript constructor (the catalog name was propagated), which frees 0xD6EF20 to keep
  CQ_HangingTreeGoodScript.

**D36. The three class-less rows were identified directly.**
- 0xCA5030 is CRendererInputPin::BeginFlush (IPin slot 15, renbase.cpp), with companions 0xCA8E10 and 0xCA50D0.
- 0xD17E70 is CQ_BountyHuntScript::RemoveBoastQuestInfo.
- 0xE9FEC0 is CV_KnotholeGladeGatesScript::InitialiseVariables.

**D37. CSystemManager.** 0x9A4FB0 is IsApplicationInputFocus (`mov al,[ecx+0Ah]` = ApplicationHasInputFocus +10). The
callers 0x4162B5 / 0xAB4830 get the class key and `YIPAVCSystemManager`. The other 12 callers of 0x9A4EC0 and
0x9C9070 / 0x9C7A40 / 0x9CC410 are deferred: each needs a bespoke remodel, so they are owner items. 0x686A80's 62
callers are deferred for the same reason (they should bind `?PeekWorld@CTCBase@@IBEABVCWorld@@XZ`).

**D38. Sweep2 owner items left as owner items.**
- The CSoundBankRecord family has no such PDB class: the PDB has CSoundBankEntry, so the element must be identified
  first.
- 0x6DA140 belongs to the CRetailTree queue-3 family.

**D39. 0x4774AB is `std::_Vector_base<CEngineLocalDetailObjectDef>::~_Vector_base`.** Its only caller is
~CEngineLocalDetailLayerDef. The descriptive FreeMember0 and the catalog's CActiveFile::OnReadFinished were wrong.

**D40. NTCScriptedControl::CActionFollowCreature** gets its namespace, Ego_r members and virtual slots. Callee 0x713D50 is the
private SetAsSneaking. Rows: FIXES 0x713EE0 / 0x715710 / 0x7158F0.

**D41. `std::list<NUISystem::CComponent*>` uses one STLport model.** Its pieces are a class allocator, the node structs, an
iterator with a user copy constructor and a protected `_M_create_node`. Rows: RENAMES 0x42A008 / 0x42AB8A / 0x42ABAF, FIX
0x42ABCA.

**D42. The 1d `_Rb_tree` access pass was applied.** STLport 4.6 `_tree.h` and libstdc++ 13 `stl_tree.h` (same SGI lineage)
both make `_M_insert` / `_M_copy` / `_M_erase` private and `_M_create_node` / `_M_clone_node` protected.
- That gives 188 FIXES plus 2 regenerated sweep3 RENAMES.
- An old-versus-new symbol diff shows only the access letters (and the U-to-V `_Rb_tree` key).
- The vector `_M_insert_overflow` / `_M_clear` access was not applied: only one source exists, so it is an owner item.

**D43. 0x4E4BEE.** EObjectAugmentationType and CWeaponTrailGraphicSet are global in Ego_r (`W4EObjectAugmentationType@@`,
`VCWeaponTrailGraphicSet@@`), so they moved out of `namespace std`. The tree got its five STLport parameters.

**D44. Family pass 2 vector helper identities come from the call graph plus Ego_r layouts.** The proof is each def's
Transfer member offset against its Ego_r layout, plus the landed erase / `_M_fill_insert` / overflow callees around each
body. Rows: resize 14, `__uninitialized_copy` 7, `fill_n` 3, `__destroy_aux` 2 and 9 labels.

**D45. `std::pair` is a struct** (every PDB pair symbol is `U?$pair`; EXTRA_RULES). Row: FIX 0xA79290.

**D46. CMessageEventManager::RemoveMessage uses the PDB spelling** `QAEXPBVCMessageEvent@@@Z` (class key), on the real
`list<CMessageEvent>` whose erase is 0x4937CB. Row: FIX 0x493B5B.

**D47. CScreenFilterEntry is 0x3C bytes:** CStackEntry, then param[0x20], then reset[0x0C]. The list constructors malloc a
0x44-byte node (8-byte link plus 0x3C), so 0x882C40 is `_List_base<CScreenFilterEntry>::clear`.

**D48. G030: the 8 `_Destruct` stand-ins are the compiler-generated scalar deleting destructors.** Each calls the landed
`??1X@@UAE@XZ`, then MSVCR71 operator delete 0xBFE9BC on bit 0. The PDB names `??_GX@@UAEPAXI@Z`. The source emits it
through a virtual destructor declared out of line plus a `new X` factory, with `optimize("s")` as the TU requires.

**D49. The vtable Copy thunks are virtual `X::Copy(const CDefClassBase*)` bodies** that tail-jump to X::operator=. Their
vtable slot in each `.?AVX@@` vtable, the PDB name `?Copy@X@@UAEXPBVCDefClassBase@@@Z` and the jump target (the row's own
operator=) agree. 0x47852E is RELOCATION_MATCH: it jumps to 0x47848C.

**D50. CThing::PeekPlayer (0x4C7A10) and GetPlayer (0x4C7A30)** continue the D14 run in PDB order. Their callees are the
const and non-const CPlayerManager::GetPlayer overloads, and PlayerNumber sits at retail +0x90 (Ego_r +0xA0, minus 0x10).
FIX 0x85F8D0 binds PeekPlayer.

**D51. The CCamera getters follow PDB order.** The order is GetFOVFlags, GetHorizontalFOV, GetVerticalFOV, GetFOV, GetZoom,
which is retail 0xA0BE80..0xA0BEC0 at 0x10 spacing, with Ego_r field offsets +0x28..+0x34. GetFOV's body is byte-identical
to GetHorizontalFOV (no ICF, D16), so the two callers of 0xA0BEB0 bind GetFOV.

**D52. `_Deque_base` holds `_M_destroy_nodes` as a protected member,** in both STLport 4.6 `_deque.h` and libstdc++ 13. deque
derives from it protected. 0x48CAF0 (catalogued CBankFileAsyncData::FreeBuffer) is `~_Deque_base`: it calls 0x48C820 on
the node range and then frees the map.

**D53. The forward-iterator `_M_range_insert` is a member template** (STLport `_vector.h`, and both PDB name sets spell
`??$_M_range_insert@PBE@...`). Row: FIX 0x9B5DB0.

**D54. CInputEvent members follow PDB order.** GetMousePos (returns C2DVector by value, hidden result), then
GetJoystickPosition 0xA043A0, then GetMouseMovement 0xA043D0 and GetHeldMouseMovement 0xA043F0. The return sizes agree.
MousePos at +0x18 is a descriptive field name.

**D55. 0x7FD110 searches by ScriptName.** Retail calls 0x6A7E30, which compares the CCharString at +0x74, the retail
ScriptName (Ego_r +0x84). HasScriptData 0x6A7E70 reads +0x78. Landed 0x5ED860 already binds the HasScriptName operator
there.

**D56. Binding drops.** A reloc_bindings row is dropped only when its recording address is an A/B row whose new source no
longer emits the symbol, or when a hand check proves the target wrong (11 rows, work/manual_drops.tsv). CONFLICTs are
not drops:
- 168 resolve once the wrong-target rows in this file are dropped.
- 46 are data-addend artifacts: the same global taken at +4 / +8 (`g_MapwhoLevels`, `g_SearchStamps`,
  `_g_StaticTree013B8A8C`, ...).
- 2 are the CIntelligentPointer<T> vtable invented names (Owner items).

**D57. Family-wide model debts are deferred rather than fixed row by row:**
- the CRenderManager2D `__false_type` vectors (0x9DB810, 0x9DC030);
- the CThingSearchToolsMethods pattern behind 0x913F30;
- the CRetailTree stand-ins (D58).

Fixing one row would leave its siblings binding the old spellings.

**D58. CRetailTree scale (queue 3).** 1,126 landed files still use CRetailTree*/invented tree stand-ins. Only the trees that
queue 1/2 rows touched were converted (D31). The rest is listed for the owner, with the pending-map conversion as the
recipe.

**D59. LABELS rows whose address got landed meanwhile are removed** when the new source already carries the identity
(0x91F740, 0x5D8676, 0x42CDE0, 0x42B854).

## Dependency chains (apply together)
A caller FIX binds the symbol that its callee RENAME defines. Applying one without the other leaves an unresolved or
mismatched binding.
- 1b: RENAME 0x8CFF40 + the 6 caller FIXES + the 6 F014 FIXES (CScriptThing::CScriptThing 0x6E7B40).
- Factions: 0x6B94C0 + 0x76C7D0 + FIX 0x6B9460 (one set<CCharString> model; `_M_find` 0x4B9370).
- Bank maps: 0x9A8090, 0x9AB120, 0x9AAF70, 0x9A9AD0, 0x9AA9E0, 0x9AA520, FIX 0x9A7F80 (+ LABELS 0x9AA560 / 0x9AAA30).
- TransferMap: 0x7AE3D0 + 0x7ADB30; 0x757160 + FIX 0x7556B0 (+ LABELS 0x756D00 / 0x755640 / 0x755680).
- Thing search: FIXES 0x518850 / 0x5198B0 with the overflow FIXES 0x4AC530 / 0x4AC610 / 0x6D4320 / 0x959260, and
  0x66E5A0 / 0x4AD700 / 0x6D4600 with RENAMES 0x4AD220 / 0x512F60 / 0x49C770 / 0x6A7590 / 0x702FF0 and the 14
  GetAllThingsInRoughArea RENAMES.
- 1c vector<bool>: 0x467831 / 0x4676EF / 0x467686 / 0x4676BB / 0x53CB4F together.
- 1c basic_string: RENAMES 0x567D00 / 0x5672D0 / 0x567400 / 0x567D60 / 0x565360 / 0x565A30 / 0x99C550 with FIXES 0x568440 /
  0x55F090 / 0x55F140 / 0x568CE0 / 0x569690 / 0x5693B0.
- 1c CSwitchableBlockage: 0x519EF0 / 0xA83420 / 0x51C1F0 / 0x5199E0 / 0x519FF0 / 0x51A880 / 0x51C850.
- GetSubDef: the 149 RENAMES + FIXES 0x787AE0 / 0x65F604.
- CThing accessors: 0x4C7990 / 0x4C79D0 / 0x4C79E0 / 0x4C79F0 / 0x4C7A00 / 0x4C7A10 / 0x4C7A30 with 0x51DF20 + FIX 0x51DEE0,
  FIX 0x85F8D0 (PeekPlayer) and 0x6869D0 (tail-jumps to PeekDefinitionManager).
- CGameDefinitionManager: 0x44C6AF / 0x44C6B0 / FIX 0x44C6B6 / 0x415FBC.
- AnimExists: 0x830F20 + 0x662CC0.
- CTCBase run: 0x6869C0 / 0x6869D0 / 0x686D20..0x686DA0 + FIX 0x686DB0 + 0x51E480 / 0x51E490 (labels 0x686D80 / 0x686D90);
  0x686830.
- Deed log: 0x6C5630 + 0x6C55E0 + 0x6C76B0.
- CEngineTextureMorph: 0x5BBDF1 / 0x5BBC93 / 0x5BA5AD / 0x5B9DE7.
- 0x7530D0 (label 0x752B00); 0x768310 / 0x7681D0 / 0x768120 (0x766C00 / 0x7677E0 private).
- 0x7AF980 with labels 0x7AFE20 / 0x7B00C0 / 0x7AF710; 0x8C8960 with the GetInterfaceType row 0x7AFCD0.
- AccurateNormalise: the 54 caller FIXES together (and 0x85F8D0).
- PositionMesh: FIXES 0x8EFA00 / 0x8EF2F0 / 0x7D60C0 / 0x7D2EF0 / 0x7D7F10 / 0x7D4E30 / 0x7D7FD0.
- 0x869020 + 0x868B40.
- CBaseClass: FIXES 0x418B7A / 0x4C4160 + RENAMES 0x8385B0 / 0x4047A0 + FIX 0x485CF1, with the `??1CBaseClass` /
  `??1CStringParser@@QAE` drops.
- Sort family: FIXES 0x91FA00 / 0x91FB30 / 0x920160 / 0x9204F0 / 0x9206A0 (+ the 0x91F740 binding drop).
- 0x478C2D + 0x478751 + Copy 0x478871 (0x478876 = CAbilityTrackLevelDef::operator=); 0x4537E9 + Copy 0x4537A0.
- CComponent / unsigned long trees: 0x42C058 + 0x5D84F2 (+ the wrong-target `_M_lower_bound` / `_M_upper_bound` drops).
- Pending map: 0x98FE70 / 0x98FCA0 / 0x9901D0 / 0x9905D0 / 0x98FD40.
- CParsedItem: 0x9B9870 / 0x9B9880 / 0x9B9890 / 0x9B98A0 + FIXES 0x998170 / 0x997E90 / 0xA63DB0 / 0xA63CC0 / 0xA640D0 /
  0xA64130 / 0xA63D20 / 0xA63EE0 / 0x9EB2C0.
- CAIBrain: 0x88B8F0 + 0x97D8B0 + 0x88BCB0.
- NScript: the whole family together (+ 0xE398F0 with 0xD6EF20; 0xCA5030 with 0xCA8E10 / 0xCA50D0).
- CSystemManager: 0x9A4FB0 + 0xAB4830 + 0x4162B5.
- CActionFollowCreature: 0x713EE0 + 0x715710 + 0x7158F0.
- list<CComponent*>: 0x42A008 / 0x42AB8A / 0x42ABAF / 0x42ABCA.
- 1d: every access FIX together with the sweep3 RENAMES it calls (0x9AAF70 / 0x9AB120 / 0x98FD40 / 0x9905D0); the 125
  superseded public-spelling bindings go with them.
- fp2: resize 0x468FA5 is consistent with 0x469012 only once that row is re-keyed (owner item); 0xA79290 + RENAME
  0xA80E40; 0x493B5B + its erase callee 0x4937CB; the uc / fill_n / destroy RENAMES with the landed overflows that call them.
- G030: the 8 sdd FIXES with the landed `??1X@@UAE@XZ` rows they call.
- Copy thunks: the 15 RENAMES with the family-pass-2 operator= rows they jump to.
- Camera: 0xA0BE80 / 0xA0BE90 / 0xA0BEA0 / 0xA0BEB0 + FIXES 0x6977F0 / 0x697F50 (0xB314E0 already binds GetFOVFlags /
  GetVerticalFOV correctly).
- Deque: 0x48C820 + 0x48CAF0 + FIX 0xA0F040.
- 0x9B5DB0 with the landed 0x9B3630 (`_M_range_insert<const unsigned char*>`).
- 0x7FD110 with LABEL 0x6A7E30 and the landed 0x5ED860 binding.

## Owner items
Header and convention debt (needs regeneration, not per-row fixes):
- The 957 generated engine headers all use `struct`, while the PDB keys most classes `class`. This is the root of the
  CRGBColour family (365 sources), and of the CPlayer / C2DVector / CThing / CWideString / CTexture / CQuickDrawTriInfo
  mismatches. One case: CPlayerManager::GetPlayer 0x449910 / 0x4498C0 carry malformed Leafs and U keys.
- Headers with the wrong layouts:
  - engine/CAIBrain.h has the Ego_r offsets; retail is 4 lower.
  - engine/CStringParser.h is 0x30; retail is 0x28 (STLport list).
  - engine/CGameCameraManager.h puts PObservedThing at 0x110; retail is 0x100.
  - engine/CActiveFile.h and engine/CCompressorZlib.h use Dinkumware layouts.
  - The CBankFile.h retail variant is opaque.
- The CThingSearchToolsMethods pattern (79 sources): retail symbols are const CThingSearchTools members, and the 0x7FD110
  callers bind HasTrackName / HasDefGlobalIndex spellings. 0x913F30 is held for this.
- The CRenderManager2D vector family: AddText 0x9DB700 and the other Add* rows; 0x9E1750 (QuickDraw namespace);
  0x9DE890, 0x9E11A0, 0x9E1870; callers 0x9DC500 / 0x9DDFF0 / 0x9DBFF0. 0x9DB810 / 0x9DC030 are held for this.
- Vector `_M_insert_overflow` / `_M_clear` access: only STLport's own text says protected.
- Queue 3: 1,126 CRetailTree*/invented-tree sources. Non-template `_Rb_tree` stand-ins in invented namespaces remain at
  0x756250, 0x9EFB80, 0x801CB0, 0xADBA40, 0x7AD060, 0x4F97B3, 0x4F9960, 0x5E2580, and also 0x6DA140, 0x70AF90 / 0x70B0E0,
  and the pending-map destructors 0x9904F0 / 0x990BD0 / 0x991280.
- The ~13.9k-row source/catalog name lag (heuristic) and the manifest `$E2` lag at 0x121AEC0.

Callers that bind invented spellings (each needs a family fix):
- 0x431F10 / 0x431ED0, about 50 names.
- 0x6869D0 (76 symbols, 124 sources) and 0x686D20..0x686DA0; 0x686A80, 62 callers (should be
  `?PeekWorld@CTCBase@@IBEABVCWorld@@XZ`).
- 0x4C7990, 89 sources with "CThing::GetDefinitionManager".
- About 15 spellings at 0x4C7A10 / 0x4C7A30 / 0x449910 / 0x4498C0.
- The GetSubDef callers at 0x669580 (16 spellings).
- 0xA14040 RotateXY (28 sources say RotateAboutZ / RotateInPlace).
- 0x45EF2D vector<float>::operator= (43 sources say CFloatVector::operator=).
- About 90 sources use invented names for the CIntelligentPointer<T> vtables. The real names are, for example,
  0x1238C6C `??_7?$CIntelligentPointer@VCThing@@@@6B@`, 0x1237F1C `<const CThing>` and 0x1231C4C `<CThingPlayerCreature>`.
  These are the two remaining non-addend CONFLICTs.
- 0x6637D0, the `X_006637d0` namespace (an anonymous namespace in the PDB), with 9 bindings.
- 0x4115A0, six invented names.
- 0x9A4EC0's other 12 callers and 0x9C9070 / 0x9C7A40 / 0x9CC410.
- 0x904030's constructor binding (`PAUCThing_Retail` versus the PDB's `AAVCThing@@M_N`).
- 0x9038D0 (the real `?GetScriptThing@CScriptGameResourceObjectScriptedThing@@UBE?AVCScriptThing@@XZ`).
- 0x0143E90C, which still appears as g_pQuestGlobals in 0xD17EA0 and others.
- The 0xA01B10..0xA01C10 intelligent-pointer helpers.
- 0x70A570 binds map members (they should be `_Rb_tree` members).
- 0xA14510 versus 0xA14540 (C2DVector).
- 0xA0EB40 std::copy: it uses `struct C2DVector` with an ambiguous Leaf, so it DIFFs on the C2DVector key.

Rows and items not done:
- 0x469012: a U-keyed leaf and a one-parameter vector, which needs the CRGBColour class-key decision first.
- 0xA7DDA0 UpperBound (FableWin const QBE) is an invented stand-in.
- The remaining G071 / G073 rows:
  - remove_if 0x65DDDB / 0x65DE48 / 0x65DF0F / 0x65DF94 / 0x65E09E;
  - 0x64639C and 0x64A2DB;
  - 16 list destructors and 6 ListSentinel constructors;
  - 0xB0CA90 / 0xB0F990, 0xDA2F90 / 0xDA2F70, 0x832190 and 0x4AD650.
- G037 0x487DC0; G070 0x93CDB0 / 0x961080 / 0x92C260; the other agent951 per-family lines G014–G073.
- 0x4EDF44 and the G067 `_M_fill_insert` callees (0x4766C2 and others; not landed, label candidates).
- 0x8F06F0 CCard::FrameUpdate is not landed. Its PositionMesh edit is in work/pm/008f06f0.cpp, and its binding to
  0x5EABC0 stays.
- 0x4C79A0 is PeekGraphicBank or PeekGlobalDefinition.
- 0x869020's anonymous-namespace hash is unstable.
- 0x66F9B0 is a catalog lag only (the source is already SetVelocity).
- 0x9F1D20 keeps its descriptive name. The chunk 483 word-wrap helpers (0xA10310, 0xA10350, 0xA10240, 0xA10170,
  0xA102B0) are descriptive only.
- The 70 held NScript rows (work/ns/src/): leaf debt on 0xD2F7F0 / 0xE1A990 / 0xEE2DB0, plus sweep2's simplified
  list<T> leaf in 0x42B4C3.
- 0x993B10 (seen while fixing 0x485CF1): it stores the .?AVCDataStream@@ vptr (0x129A69C), so it is
  CDataStream::~CDataStream. It is catalogued as CChunkedFileChunk::~CChunkedFileChunk, labelled
  ~CMemoryDataInputStream, and bound under 4 spellings. Its own row and those callers need one decision.
- Chunk 477-481 parked-row labels that are not in ghidra_out (0x6FDA20 IsThingCollision, 0x733B70, 0x733CF0, 0x74E140,
  0x6E94D0, 0x7AA880, 0x7BBC9F, 0x779EF0, ...).
- The comment-only `CScriptThing(); // 0x008CFF40` declarations in about 20 CGameScriptInterface_* sources.

## BINDINGS_DROP.tsv
1,178 rows, built by `work/mkdrop.py`:
- the DROP rows of the newest pbdrop scan of each address (1,159 + 7 + 11 + 1 from full1 / new / new2 / new3);
- the 11 hand drops, whose wrong-target reasons replace the scan's;
- every one of the 125 1d stale public-spelling bindings, marked "1d access: superseded by the A/I spelling".

Every row exists in reloc_bindings.tsv with that symbol, target and recorder. The table's 216 CONFLICTs are reviewed in
D56. None is a drop.
