# Hand chunk 509 (WinMainCRTStartup .. CPlayer): 18 of 20 byte-exact

All 18 grade RELOCATION_MATCH (gall.sh) and pass under g++ (tall.sh) and VC7.1 (tall.py). vland dry run: 18 wins,
0 rejects (warnings listed under Label corrections / Review at landing). No retail_asm rows.

## Proven
- 00401067 WinMainCRTStartup (PE entry point; crtexe.c built with _WINMAIN_, /O1)
- 004bc1a0 NGraphicAppearance::DrawRHSet (Ego_r.pdb name; parked)
- 004fdcb0 CWorldMap::IsSectionToHaveNavigationMap (Ego_r.pdb protected const member; "NULL" literal, InMap+IsInQuest filters)
- 0062cf90 CWorld::EACreatureBlock (Ego_r.pdb donor order of the EA run: 0x62CE60 EALoadGameFromInGameMenu, this, 0x62D380 EAControlledCreatureJump whose landed header names this address; builds CCreatureAction_StartBlocking, calls CTCCombat::IsBlocking/SetBlocking)
- 00635d70 CPlayer::UninitModeUseProjectileWeapon (CPlayer mode table Init/Uninit pair; removes the projectile-mode input processes)
- 0078bef0 CTCBerserk::CanBerserkBlock (callee; reached through the CTCBerserk 0x84 lookup in 0x62CF90)

## Probable
- 00431f90 std::vector<std::pair<unsigned long, CDefClassBase::CSubDefInfo> >::operator=
- 0047005b std::vector<CAppearanceModifierGraphics::CEntry>::_M_fill_insert
- 00470778 std::vector<std::pair<unsigned long, CSoundPair> >::_M_fill_insert
- 00470918 std::vector<std::pair<unsigned long, CSoundPair> >::_M_insert_overflow
- 00476ff7 std::vector<CEnvironmentThemeDef>::_M_fill_insert
- 00477e8e std::vector<CEngineLocalDetailObjectDef>::_M_fill_insert
- 004b8ff0 std::__find<const CCharString*, CCharString>
- 004d2760 std::vector<CSubtitledMessage>::_M_insert_overflow
- 0051d630 std::vector<CNavigationAreaUpdate::CNavigationLayerUpdate>::_M_fill_insert
- 0052b3d0 std::vector<CThingTypeInfo>::_M_insert_overflow
- 00624a30 std::vector<CTestQuest>::operator=
- 00625440 std::__introsort_loop<CTestQuest*, CTestQuest, int, CTestQuestNameCompare>
- 006315e0 std::vector<NUISystem::CUIStateDef>::operator=
- 00632ff0 std::vector<NUISystem::CUIStateDef>::_M_fill_insert
- 0043dbd0 IS_THING_ALIVE<CThingPlayerCreature*> (callee of 0x62CF90; see Review at landing)
(STL rows: STLport 4.6 algorithm shape exact; element type from the stride/size and the owning callers.)

## Descriptive
- 005239cd _global::CThingDeadCreature_SavePersistentState (catalog label of a fragment; not authored)

## What matched (18)
- WinMainCRTStartup: crtexe.c verbatim under /O1.
- STLport vector members (12 rows): operator=, _M_fill_insert, _M_insert_overflow. Shape: which helpers are inline,
  __forceinline (helpers with loops) or same-TU noinline bodies (so VC knows x_copy / a local does not escape and keeps
  registers), and the packed novtable def-class layouts that set the element size.
- std::__find: STLport four-way unrolled random-access find, __fastcall.
- std::__introsort_loop<CTestQuest>: needs the comparator body (0x6238A0) in-TU under auto_inline(off) so VC passes the
  argument slot itself instead of a 1-byte comparator copy.
- CWorldMap::IsSectionToHaveNavigationMap: CBasicString operator== body under auto_inline(off) keeps PStringData in ebp;
  each search in its own block ending `if (x) return true;`.
- CWorld::EACreatureBlock: same-TU noinline IS_THING_ALIVE body (creature stays in ebx, its slot is reused for the TC
  keys, 0x120 frame); the first GetPlayer result as a block local (GetPlayer is called before `push 2Fh`); const TC
  lookups test the bit through the bitset reference proxy (`test byte ptr`), not _M_test (`shr ; test`).
- CPlayer::UninitModeUseProjectileWeapon: eleven RemoveFromInterface calls in retail order, then the camera push.

## Parked for Fable 5.1 (1)
- 004bc1a0 NGraphicAppearance::DrawRHSet DIFFER(397v406): retail `sub esp,0D0h` with three distinct 0x40 CEnginePrimitiveLine slots (+0x68/+0x2C/+0xA4), ours `sub esp,58h` sharing one slot, so every esp offset and the colour-store schedule shift | tried: named locals, unnamed temporaries, temporaries with a user dtor on the base (4 grades) | next: give each line its own scope-crossing lifetime (e.g. all three named locals declared up front and drawn after construction), or a by-value argument copy that forces separate homes

## Skipped
- 005239cd: not a function start. It is a fragment of the real function at 0x005239C0 (379 bytes, to 0x00523B3B): it
  reads ebx it never set and pops registers it never pushed. Re-cut the catalog row to 0x005239C0.

## Label corrections
- 0x0078BEF0: catalog `CTCShotDefinitionBase::CanBeHitByMeleeStrike` -> `CTCBerserk::CanBerserkBlock` (called on the CTCBerserk interface 0x84 in 0x62CF90).
- 0x00487DC0: catalog `CPlayer::GetHeroCreature` (and invented free `@get_hero@4` bindings) -> `?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ`.
- 0x007331E0: `_global::GetMember_byte0x63_007331e0` -> `CTCCombat::IsBlocking() const`; 0x007337E0 `CTCCombat::SetBlocking()`.
- 0x00630DD0: `std::_Destroy<NUISystem::CUIStateDef*>`.
- 0x00401377: `_global::ReturnZero_ret0_00401377` -> CRT `_setargv` stub (`__setargv`, returns 0).
- 0x00631560 / 0x00633340 / 0x004D2100 / 0x0047725F / 0x00478104: `UninitCopy_* / UninitFillN_*` descriptive labels -> the
  STLport `__uninitialized_copy` / `uninitialized_fill_n` instantiations named in vland.log.
- 0x00430300: `Dtor_SharedBaseOf42_00430300` -> `CParentDefClassBase::~CParentDefClassBase`.
- 0x0043211D: `ReleaseFirstMember_0043211d` -> `vector<pair<ulong,CDefClassBase::CSubDefInfo>>::_M_clear`.
- 0x0051C590: `ContainerArray_CopyMultiDimensional` -> `std::__uninitialized_copy<CNavigationLayerUpdate*,...>`.
- Invented bindings in the catalog for this chunk's rows: 0x4D2760 (CRetailVector_CElement68 / CSubtitledMessageArray),
  0x52B3D0 (N0052b9d0::Element), 0x624A30 (CTestQuestVector), 0x632FF0 (CRetailVector_elem124::Insert); catalog names such
  as `vector_assign_questpair`, `vector_insert_12byte`, `vector_assign_conversation` are descriptive.

## Header debt
- 0x62CF90 / 0x635D70 model CTCCombat, CTCWeapon, CTCBerserk, CCreatureActionBase, CGameEvent, CAInputProcess,
  CGameCameraManager, CAIGameCameraBase locally (vland SHARED_HEADER_AVAILABLE); 0x632FF0 models CResource. Moving them to
  engine/<Class>.h needs the retail (not Ego_r) offsets: CThingManager pause flags +0x54/+0x55 (Ego_r +0x68/+0x69).
