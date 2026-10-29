# Hand chunk 577 (NScript CQ_AmbushTradersScript .. std::vector<CConversation>): 10 of 10 byte-exact

All ten rows grade RELOCATION_MATCH (gall.sh) and pass their tests under g++ and the lane's VC7.1 (tall.sh / tall.py). The
read-only vland run reports 10 RELOCATION_MATCH + PASS, no RELOC_IDENTITY; the two remaining warnings are catalog labels of
callees (RELOC_NAME 0x00D34E80 / 0x00E54C10 labelled `CRetailVector_poly0x34/0x5c::~...`, really the vectors' `_M_clear`;
NAME_FIXES.tsv). No retail_asm, no flags, no pragmas. Every call target was checked against retail's (tmp/rel.py: COFF
relocations of the leaf mapped onto the retail bytes; every target equals the declared callee's retail address) and every
string literal is at the retail address of the same offset.

## What matched (10)
- **Quest / level-script slots (3):**
  - 0x00DEC8A0 NScript::CQ_SecretPassageScript::Init: five `Game->AddQuestRegion("Q_SecretPassage", <region>)` (the landed
    CQ_HangingTreeEvilScript::Init model). First grade.
  - 0x00E7E210 NScript::CV_FishingCompetitionScript::OnPersist: Transfer<CCharString> through the two-argument inline
    (GetDefaultVal<CCharString> 0x0040E1E0), then float / bool / long Transfers with defaults in the dead `context` slot.
    First grade.
  - 0x00E7A410 NScript::CV_ExposeMayorScript::DoMission (spawned thread): `if (StageNumber == 0) { term; WAIT(IsLevelLoaded
    "BowerstonePosh"); PhaseOne(); frame; term; }`, PhaseTwo .. PhaseFive each followed by a frame and a termination test,
    WAIT(StageNumber == 5), EndMission. First grade.
- **Threads with an unrotated wait (1):** 0x00F033F0 NScript::CQ_AmbushTradersScript::WatchForAllGuardsDead: two region WAITs,
  then the GuardAliveCount wait in the chunk 576 do-while form (`do { if (GuardAliveCount == 0) { term; AllGuardsDead = true;
  term; return; } NewScriptFrame(); } while (!term);`); `while (GuardAliveCount != 0)` rotated it (347v344). Two grades.
- **Entity-script Init (2):** 0x00E5DE80 NScript::CV_BodyGuardScript::CBodyGuard::Init (eleven interface calls, by-value Me
  copies built in their slots with both vtable stores, `SetWanderCentrePoint(Me, Me.GetHomePos())` with the C3DVector
  result in the argument slot, `AngerTimer = 0` through the inline CTimer::operator=), 0x00DDE7F0
  NScript::CQ_PrisonWardenGameScript::CSecretBook::Init (an if / else-if chain over fresh GetDataString() temporaries and
  the inline CCharString::operator==, then SetDataString("NULL")). Both first grade.
- **Script helpers (2):**
  - 0x00E5E240 NScript::CV_BodyGuardScript::GetHenchmanTypeFromString(const CCharString&): four early-return `==` tests;
    retail keeps PStringData in ebx across them, which needs CBasicString<char>::Compare's body in the TU under
    `#pragma auto_inline(off)` (the chunk 576 0x00F088E0 lesson; declared only: 267v261). Two grades.
  - 0x00DCEDF0 NScript::CQ_OrchardFarmRaidScript::CCrateTeamMember::GetNearestCrateToMe(): nearest-crate scan with
    GetDistanceBetweenThings 0x00CBE512; a named CScriptThing local (out-of-line CBaseClass ctor/dtor, `__forceinline`
    operator= and dtor, compiler-generated copy ctor for the return). Needed STLport's `operator[] = *(begin() + n)` and the
    counted-pointer assignment reading other.Info before other.Data (351v350 otherwise). Four grades.
- **STLport vector::operator= (2):** the landed vector<CLipSyncPhoneme> 0x00A4A520 template with each instance's own helper
  inlining: 0x00D34D40 vector<CFireHeartPatternDef> (_M_allocate_and_copy, _M_clear, __copy_ptrs, _Destroy, _Construct
  out of line; `__forceinline` __uninitialized_copy; __false_type without a user ctor, so the shared __copy_ptrs tag is
  zero-stored) and 0x00E54AA0 vector<CConversation> (`__forceinline` __copy_ptrs / __copy / __uninitialized_copy, inline
  placement _Construct, element operator= / copy ctor out of line). Two grades each.

## Names and their evidence (all proven; none descriptive)
- 0x00DEC8A0: sole reference = slot 3 (Init) of the RTTI-less vtable 0x012DDC30 stored by the landed CQ_SecretPassageScript ctor
  0x00DEC7F0 (slots: 0 dtor 0x00DF10B0, 1 RegisterMain 0x00DECA10, 2 Main 0x00DECAD0, 4 OnPersist = CScriptBase default
  0x00CBD4E0); Ego_r ?Init@CQ_SecretPassageScript@NScript@@UAEXXZ; the quest-name literal is the factory registration's.
- 0x00E7E210: slot 4 of vtable 0x012EA478 (landed CV_FishingCompetitionScript ctor 0x00E7E110 / Alloc 0x00E81EB0); Ego_r
  OnPersist UAE; every key string = the Ego_r member at retail offset + 0x14.
- 0x00E7A410 / 0x00F033F0: the CSpawnedFunc member-pointer store (`mov [edi+34h], <row>`) right after the thread-name
  literal ("DoMission" 0x012CB73C at 0x00E75E28; "WatchForAllGuardsDead" 0x012F7CAC at 0x00F02B19); Ego_r
  ?DoMission@CV_ExposeMayorScript@NScript@@QAEXXZ (every callee a landed CV_ExposeMayorScript Phase*/EndMission member) and
  ?WatchForAllGuardsDead@CQ_AmbushTradersScript@NScript@@QAEXXZ (consumes GuardAliveCount, lowered by the landed
  EntityCountDecrease 0x00F088E0).
- 0x00E5DE80 / 0x00DDE7F0: slot 2 (Init) of the entity vtables 0x012E701C (landed CBodyGuard ctor 0x00E5DDE0; slots 4/5 the
  landed OnPersist / OnPredicateFail) and 0x012DCC6C (landed CSecretBook::Alloc 0x00DDE720); Ego_r
  ?Init@CBodyGuard@CV_BodyGuardScript@NScript@@EAEXXZ / ?Init@CSecretBook@CQ_PrisonWardenGameScript@NScript@@EAEXXZ, each
  right after its Alloc in the donor order. Entity slot order from the landed CTyler::Init 0x00E836F0.
- 0x00E5E240: both callers (0x00E5E0DC, 0x00E5FDBB, CBodyGuard code) pass ParentClass in ecx and Me.GetDataString() by
  reference and store the long into HenchManType; the only CV_BodyGuardScript member of that type in both PDBs is
  ?GetHenchmanTypeFromString@CV_BodyGuardScript@NScript@@QAEJABVCCharString@@@Z.
- 0x00DCEDF0: sole caller 0x00DCE549 (CCrateTeamMember entity code, result slot pushed); the only CCrateTeamMember member
  returning a CScriptThing in both PDBs is ?GetNearestCrateToMe@...@AAE?AVCScriptThing@@XZ (private); it reads the same
  ParentClass->CrateList (+0x54 = Ego_r 0x68 - 0x14) as the landed IsThingCarryingCrate 0x00DCED10.
- 0x00D34D40 / 0x00E54AA0: the callees are the landed vector<CFireHeartPatternDef> / CConversation helpers (strides 0x34 /
  0x5C); Ego_r + FableWin `??4?$vector@V<T>@@V?$allocator@V<T>@@@std@@@std@@QAEAAV01@ABV01@@Z`.
- Callees (spellings checked with objdump -t and against reloc_bindings.tsv): CCharString ctor/dtor (`??0CCharString@@QAE@PBDJ@Z`,
  `??1CCharString@@QAE@XZ`), `?Compare@?$CBasicString@D@@KIJPBD0@Z` 0x00411570, `?IsActiveThreadTerminating@CScriptBase@@QAE_NXZ`
  0x00CB7940, `?GetDistanceBetweenThings@@YIMABVCScriptThing@@0@Z`, `??0CBaseClass@@QAE@XZ` / `??1CBaseClass@@UAE@XZ`,
  `??3@YAXPAX@Z`, `??$GetDefaultVal@VCCharString@@@CPersistContext@@KI?AVCCharString@@XZ`, `??$Transfer@<T>@CPersistContext@@...`
  (CCharString / M / _N / J), `?g_pGameScriptInterface@@3PAVCGameScriptInterface@@A` (0x0143E8F8, chunk 573's descriptive
  spelling: no PDB data symbol), the vector helpers (`_M_allocate_and_copy` / `_M_clear` IAE, `__copy_ptrs`, `_Construct`,
  `_Destroy` __fastcall), `??0CConversation@@QAE@ABV0@@Z`, `??4CConversation@@QAEAAV0@ABV0@@Z`, the CV_ExposeMayorScript
  Phase*/EndMission QAE members. Interface slots from ghidra_out/cgsi_slots_aligned.tsv (7, 11, 12, 89, 235, 237, 271, 360,
  477, 478, 481, 506, 601, 763, 766, 769); CScriptThing slots 3 GetDataString, 4 SetDataString, 7 GetHomePos.

## Header debt
- engine/CDefClassBase.h: RESOLVED at landing (owner review 2026-10-29). The local `virtual ~CDefClassBase(); _body[0x24]`
  stand-in in 0x00D34D40 / 0x00E54AA0 is replaced by the shared headers' retail opt-in (FABLE_RETAIL_DEF_CLASS_BASE):
  CDefClassBase : CDefPointeeBase : CResource : CIVCountedPointeeBase with all 25 retail vtable slots; the elements
  declare their real overrides (dtor, Transfer, Copy, GetSizeofClass). Both rows regrade RELOCATION_MATCH + PASS.
- engine/CGameScriptInterface.h / CScriptThing.h: not used (flattened vptr); the interface is CGameScriptInterfaceBase with the
  needed virtual slots, as every landed quest row does. CScriptBase / CEntityScriptBase / CLevelScriptBase have no headers.

## Parked for Fable 5.1 (0)

## Skipped
None (every extent checked: first instruction a prologue, last a `ret`/`ret 4`, no int3 inside).

## Label corrections
- 0x00E5E240 `GetAlignmentFromCode` -> NScript::CV_BodyGuardScript::GetHenchmanTypeFromString
- 0x00D34D40 `std_vector_Conversation_Assign` -> std::vector<CFireHeartPatternDef>::operator= (the element is not CConversation)
- 0x00DDE7F0 `Script_Set_Color_Green` -> NScript::CQ_PrisonWardenGameScript::CSecretBook::Init
- 0x00E7E210 `sub_00e7e210` -> NScript::CV_FishingCompetitionScript::OnPersist
- 0x00F033F0 `sub_00f033f0` -> NScript::CQ_AmbushTradersScript::WatchForAllGuardsDead
- 0x00DCEDF0 `Main_CheckThingDistance` -> NScript::CQ_OrchardFarmRaidScript::CCrateTeamMember::GetNearestCrateToMe
- 0x00E54AA0 `std_vector_CConversation_Assign` -> std::vector<CConversation>::operator=
- 0x00E7A410 `sub_00e7a410` -> NScript::CV_ExposeMayorScript::DoMission
- 0x00E5DE80 `sub_00e5de80` -> NScript::CV_BodyGuardScript::CBodyGuard::Init
- 0x00DEC8A0 `sub_00dec8a0` -> NScript::CQ_SecretPassageScript::Init
- Callees (NAME_FIXES.tsv): 0x00D34E80 / 0x00E54C10 are the vectors' `_M_clear` (not `~CRetailVector_poly0x34/0x5c`),
  0x00CDB7C0 / 0x00CDB790 are `_Destroy<CFireHeartPatternDef*>` / `_Destroy<CConversation*>` (not Destroy_00cdb7c0/790).
