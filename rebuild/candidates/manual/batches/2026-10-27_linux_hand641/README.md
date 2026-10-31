# Chunk 641 -- 3 of 10 landed (2026-10-31)

Cut from the parked-row pool. Landed via `coordinator/land_chunk.sh 641` (vland --land 3/3 RELOCATION_MATCH + PASS, multi-check 0,
two new binding rows: CThing::CheckUID 0x004C7BC0 (single-source name, review) and the GetAllThings<CThing, vector<CThing*>,
CThingFilter_InMap> const instance).
- CGameScriptInterface::GetAllThingsInLevel 0x008A8AF0 -- the same-TU search stand-in takes &filter AND the filter is a temporary in
  the call (the temporary shares a slot with the later script-thing local); real STLport vector model, CThingFilter_InMap(const
  CWorldMap&, long), CWorld::GetWorldMap returning CWorldMap&, CScriptThing::IsNull (slot 77) inline on the local.
- CThingManager::DebugVerifyUniqueIDs 0x0051E630 (private, const std::vector<CThing*>&) -- PDB return types first (CThing::GetDefName
  returns CDefString, CDefStringTable::GetString(long) const, inline CDefString::GetString with a named result), then a user copy
  constructor on CDefString fixed the register rotation; GDefStringTable, slot 10 GetInstantiationName, 4-byte STLport list.
- NScript::CQ_HobbeCaveScript::CBanditInRoomSample::Main 0x00D910B0 -- a CTimer local from engine/CTimer.h (ctor/dtor/GetTime/
  operator= inline; its destructor at each exit gives retail's hoist and cross-jump into the loop exit) and the generated-script else
  arm `if (IsActiveThreadTerminating()) return; RemoveThing; return;` fixing the edx/eax exit copies; PDB field names
  KilledSideRoomHobbes (+0x4E), HC_BanditCallsForHelpDelay (+0xCB0).
Seven rows stay DIFFER in `pending/2026-10-27_linux_hand641/differ/` (00712900 CActionPlayAnimation::Update eax/ecx creature read,
00956e70 CAIStateGroup_BriarRose::Update ebx/edi tie, 006d4e60 IsCreatureValidFollower key-address rotation, 0047ed76 SetToSave
push ecx vs fsubp order (GOTCHAS family), 00a22ca0 CRightHandedSet::GetAngleXZ one x87 swap, 00a1ad40 CNavigatorAStar::GetPosInDirection
two x87 swaps -- duplicated return in the distance exit fixed prologue/frame/hoist/cold exit, PDB C3DVector::operator+=(const C2DVector&)
fixed the step adds --, 009e5c90 CSymbolMap::LoadFromFile SIB base/index order). Lessons: script timers are a CTimer local, never
explicit Register/Deregister; the generated-script else-arm shape sets the exit-copy registers; a duplicated return in a loop exit
reproduces a cold `fstp st(0); jmp` tail; search stand-ins need both &filter and a temporary filter; PDB return types first, then a user
copy ctor on a small by-value class; a named float local fixes a trailing multiply order.
15 NAME_FIXES + 3 review rows queued (prefix c641): IsActiveThreadTerminating 0x00F35B30 (helper_f35b30-style callers),
IsDistanceBetweenThingsUnder 0x00CBE2FF, CDefStringTable::GetString, GDefStringTable 0x013CA828, GetUIScale const, NCreatureMode::
IsIncapacitated 0x0085F0C0, IsPerformingAnyAction const, GetRandomAnimFromName const ref, AddQueuedAction, SetCurrentAction,
GetMapNumberFromScriptName const, GetSubDef<CBriarRoseDef> CDefPointer<const> param, GetScriptThing const; g_pGameScriptInterface /
pGlobalScriptDef established spellings kept. Labels: ghidra_out/labels_chunk641_2026-10-27.tsv.
