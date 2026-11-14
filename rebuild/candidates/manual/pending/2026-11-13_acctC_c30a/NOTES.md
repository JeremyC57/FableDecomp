# agentC30 notes

## 0x007E15C0 NGraphicAppearance::CTCGraphicAppearanceMultipleStaticMeshes::OnAppearanceDraw -- DIFFER(969v963), parked (breadth first)
Draft differ/007e15c0.cpp (tmp/a7.cpp). Name: C29 NOTES (Ego_r order between OnAppearanceClear 0x007E1590 / GetMeshesUsed 0x007E1990).
arg1 (edi) is the CTCGraphicAppearance (C29 called it info): size = MainGraphic.RenderSizeX(+0x4C) * Scale(+0x34).
Structure decoded fully: loop1 push_back(AddPersistentPrimitive(CPrimitive(1, Mesh))); else-if ReasonsForDraw==1 RefreshPrimitivePos loop,
return; draw loop: skip Mesh==0; channel desc (override -> byte LightingChannel, else PeekLightingChannel dword); skip if
global 0x013756F0 (int, name unknown; also read by CTCGraphicAppearanceStaticMesh draw 0x0077BAC5) >= 0 && channel.ChannelIndex0;
fade override; CEnginePrimitiveStaticMesh 0x0077BD70 (landed ctor signature); MeshEffect = GetPMeshEffect; SetPrimitiveGraphics.
Closers found: C3DPositionInterpolator / C3DOrientationInterpolator ::GetInterpolated inline (FableWin names) -> retail evaluates
Interpolate before the pushes (overlay opt-ins); a named `bool` for IsAffectedByBulletTime puts that call first (retail order).
Residue: (1) retail keeps `this` in ebp all through and spills loop-1 index [esp+14]/offset [esp+10]; ours spills this.
(2) retail stores only channel +0x11/+0x12 = 0 (unconditional, before the override je); ours 3 bytes. Tried ternary w/ 3-arg ctor, assign.
(3) position FP: retail loads t first onto the FPU (fxch/fmul st(2)); ours homes the inline's float param. Tried manual expr, named t.
(4) a reference `entry` local v PDef->Meshes[i] each time: retail reloads begin 3x; no-ref variant longer (988/977).
Tried single shared `i` (no change).

## 0x007B57C0 CTCCarriedActionUseActivateQuest::OnActionUse(CThing&) -- RELOCATION_MATCH (src/ + test/ PASS, vland WIN), ~22 grades
Name: Ego_r ?OnActionUse@CTCCarriedActionUseActivateQuest@@UAEXAAVCThing@@@Z 0x007B59A0 right after GetSubDef<CActivateQuestDef> 0x007B5890
and CActivateQuestDef::Transfer; retail GetSubDef<CActivateQuestDef> 0x007B5680 precedes; same vslot / base call / `ret 4` as 0x007EF200.
Extent confirmed: 0x007B57C0..0x007B5AC8 (last insn `jmp 0x7b5a02` is the quest-path tail; 776 bytes).
What made it match: (1) GetSubDef BODY visible (include CDefinitionManager.h + GETSUBDEF_BODY + StatGetTypeName in the new CActivateQuestDef
opt-in) -> def kept in ebx, ScriptName CSE'd in edi (777 -> 758); (2) the GetSubDef result is unused, `if (!def.IsNull() && ...)`;
(3) FORCEINLINE_GETSTRING WITHOUT FORCEINLINE_CONVERSION and explicit `.GetString()` (GetString expanded: table call 0x009D49B0);
(4) nested ifs + a `done` flag for the shared reset block (the comma-expression / single && chain left the zero C3DVector temp in a
low frame slot: DIFFER(764v776)); (5) const PeekTC for the mode manager (vland RELOC_IDENTITY: retail const LowerBound 0x004365B0).
vland WARN: ~CCreatureAction_ActivateQuest -> 0x00629060 labelled ~CAggregate_00629060 (folded dtor body; coordinator label).
Descriptive names: none. IsPerformingRegionLoadReally body (RegionLoadStatus != 0) inferred from retail bytes + FableWin name.

## 0x007BC38A -- name proven, not authored: NThingShot::CTCShotDefinitionThrowPredictive::InitMovement (protected virtual, void())
Function start: follows `leave ; ret` of 0x007BC3xx with no padding, begins `push ebp ; mov ebp,esp ; sub esp,34h` (ebp frame, like Ego_r's
unaligned 0x007BB1D9); only reference is vtable 0x0123CE9C slot 33 (RTTI .?AVCTCShotDefinitionThrowPredictive@NThingShot@@). Catalog
label CTCPhysicsRigidBody_ApplyForceToward is wrong (NAME_FIXES). 208 FP instructions (predictive throw solve) -- not drafted.

## 0x00C271F0 -- name: CPhysicalPlayer::Play (libpk audio, descriptive; one name per address with landed caller 0x00C20A20), not authored
EH-frame start, `ret 18h`, called once from CSoundEvent::PlayPhysical 0x00C20AE3. Catalog label CTCInventory_OnItemEquippedHelper wrong.

## 0x00735FD0 CTCCombat::AttemptToStartParticle -- DIFFER(993v973), parked (C29 draft, differ/00735fd0.cpp)
Call sequence identical to retail. Residue = allocation from the first instruction: retail weapon -> ebp, GetSubDef bool -> bl, this -> edi
(then i), start_name CCharString in the weapon PARAM slot [esp+6Ch]; ours weapon -> ebx, bool spilled to the weapon param slot, frame
0x5C v 0x58. Tried (no change): named `bool has_def`, named CIntelligentPointer temp for FlourishParticles[i].

## 0x006A1A00 CTCCarrying::GetCarriedThingInterpolatedPosAndOrientation -- name proven (Ego_r 0x006C61A0 + FableWin 0x02216AF0, QBE_N
ABVCThing@@AAVC3DVector@@AAVCRightHandedSet@@_NM@Z, `ret 14h`); decoded to 0x006A1C62, not drafted (context). Shape: inline find over
CarrySlots (+0x0C, 12-byte {slot id, CIntelligentPointer}); CGameDefinitionManager::Get 0x0044C6B0 + GetDef<CCarrySlotDef> 0x006A3060;
CCharString from def+0x28 (GetString expanded + 0x0099EC30); PTCGraphicAppearance +0x64 null -> false; helper type b ? 0x10 : 0x31;
0x004C08C0 helper lookup (7 args); else physics +0x60: GetPosition(float) 0x0049D150 into pos, vslot 0x114 (facing) -> 0x004BBE10;
class-2 thing + 0x006A1330(slot id) + global byte 0x0137574C -> 0x00A22F80(rhs, 0.5f); then def+0x34 != "" with mode manager slot 10,
0x006622A0, second helper lookup and an FP cross product -> 0x00A22F80(..., 0.2f).

## Rows 2 / 5 / 6 / 7: not authored (context). 0x006B2CA0 is ~200 FP instructions (C29 notes: sun/moon rotation products).

### 0x007E15C0 follow-up (grades 15-27): best now differ/007e15c0.cpp = tmp/d4.cpp (969v963, 52 hunks v 71): a named `CPrimitive* primitive`
for the push_back argument and ONE `unsigned long i` shared by loops 1 and 3 (retail shares the [esp+14h] slot for both counters).
Still `this` spilled at entry. Also tried: named CPrimitive, named override bool, all-loops shared i (no change). Next ideas (not tried):
GetFullScale / GetScale spelling of size, PDef via an accessor (GetPDef()/PeekDef()), loop 1 via iterator with separate index,
fwsize on the FableWin CTCGraphicAppearanceStaticMesh::OnAppearanceDraw sibling (same draw block at 0x0077BA40..).
