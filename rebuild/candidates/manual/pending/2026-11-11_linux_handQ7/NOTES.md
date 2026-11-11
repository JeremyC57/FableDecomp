# Chunk Q7 notes (agent Q7, 2026-11-11)

Grading: every grade with `FABLE_HDR_OVERLAY=$A/hdr` (tools/gg.sh exports it). RTM cl 13.10.3077, base flags `/O2 /Oy /W3`.

## Matched (src/ + test/, RELOCATION_MATCH, tests PASS under g++ -std=c++03 and VC7.1 /Od)
| row | name | lever |
|---|---|---|
| 0x0047E651 | CTCInGameMenu::VideoOptionsChanged | `(GetBackBufferWidth() * 0.5f) - ...` PAREN tuple (push ecx after the X fsubp) |
| 0x0047F30F | CTCInGameMenu::VideoOptionsNotAccepted | same |
| 0x0047E9AE | CTCInGameMenu::GlobalUpdate (/GS) | same + real hero-path callees and shared engine/ThingAlive.h (noinline) |
| 0x00697490 | CGameCameraManager::IsPosCameraInsideThing | shared engine/C3DVector.h (hand762 base) + `axis *= (1.0f / height)` |
| 0x0086BFB0 | CRandomAppearanceMorph::TransferBinaryIn | shared headers; FABLE_RETAIL_STLP_VECTOR (visible _M_fill_insert); overlay opt-ins |

## Identities / names proven this pass (LABELS.tsv)
- 0x00697490 takes `const CThing&` (Ego_r `QBE_NABVCThing@@ABVC3DVector@@@Z`), not a pointer; callees CThing::GetPos 0x004C73D0,
  NGraphicAppearance::CTCGraphicAppearance::GetDummyObject 0x004C08C0 (references, const), g_NormaliseEpsilon 0x0129BA3C.
- 0x0047E9AE GlobalUpdate is virtual (`UAEX_N@Z`); the model keeps the vptr in a 4-byte CTCBase base (a plain vptr + double member
  aligns the members 4 bytes high). Hero: CTCBase::GetPlayerManager (protected, reference) -> CPlayerManager::GetMainPlayer
  (non-const, reference, 0x00449970) -> CPlayer::GetPControlledCreature -> IS_THING_ALIVE<CThingPlayerCreature*> 0x0043DBD0.
- Dialog rows: CObservable slots RemoveObserver (+0x14) / AddConcurrentExclusiveObserver (+0x24), CObserver::ObserveAllEvents
  (+0x18); the screen getters are NUISystem::CUIScreenUnitConverter::GetBackBufferWidth/Height (static __fastcall).
- 0x00A1A7E0 is CNavigatorAStar::GetIntersectionPointsBetweenNodes (pair<C2DVector,C2DVector>, const refs, private const).

## Rework on shared headers (coordinator request, 2026-11-11)
The four rows that carried local models (0x0047E651, 0x0047F30F, 0x0047E9AE, 0x00697490) were rewritten on the shared headers
only; each still grades RELOCATION_MATCH and its test passes under g++ and VC7.1 /Od. No local struct / class model remains in a
row source; the free-function declarations left in rows are GFIntToWideString (0x00405F00, Ego_r spelling; no shared header
declares it), the two landed callbacks VideoOptionsChangedConfirmed / VideoOptionsNotAcceptedConfirmed, and `extern const float
g_NormaliseEpsilon`. Every relocation symbol the four objects emit was checked against the PDB spellings (objdump of the graded
.obj): e.g. ??0?$CCountedPointer@VCAction@NUISystem@@@@QAE@PAVCAction@NUISystem@@@Z, ?ReleaseData@?$CCountedPointer@VCAction@NUISystem@@@@QAEXXZ,
?push_back@?$vector@PAVCComponent@NUISystem@@...@std@@QAEXABQAVCComponent@NUISystem@@@Z, ?push_back@?$list@PAVCActionParam@NUISystem@@...,
?AddData@CGameEvent@@QAEXJPBX@Z, ?GetTextBySymbol@CDataBank@NGameText@@QBE?BVCWideString@@ABVCCharString@@@Z, ??H@YI?AVCWideString@@PBGABV0@@Z,
?GetDummyObject@CTCGraphicAppearance@NGraphicAppearance@@QBE_NABVCCharString@@JKAAVC3DVector@@AAVCRightHandedSet@@M@Z.
The earlier made-up / local spellings are gone: CActionPointer -> CCountedPointer<NUISystem::CAction>; CComponentBase -> the shared
NUISystem::CComponent (CUIInterface primary base); SetOkCallback / SetCancelCallback -> CYesNo::SetYesCallBack / SetNoCallBack (142/143);
SetPosition -> CComponent::ChangePosition (31); SetActive -> ChangeState(unsigned long) (48); GetState -> GetTargetStateIndex (87);
AddAction -> AddActionOnLeftUnclicked (80); SetText / SetWideText -> CText::SetValue (145 / 144); CWideString / CDataBank ->
fable_string.h CWideString / NGameText::CDataBank; CMemoryBuffer::Copy -> CGameEvent::AddData (AddSLONG / SetPlayer inline);
GetInterface -> CThing::GetTC(ETCInterfaceType, T**); IntToWideString -> GFIntToWideString; the local GAME_EVENT_LOAD_GAME enum ->
the literal 60 (no EGameEventType enumerator dump exists). Old local-model drafts are kept in p/rw/*_localmodel.cpp.
Header opt-ins the rows define are all existing ones except the four new overlay opt-ins (hdr/README.md).
Tests: the three CTCInGameMenu tests are tgen/rw.py + tgen/mk.py output (the earlier test model respelled to the PDB names, then
the row body verbatim, then recording stubs); 0x00697490's test is the row body verbatim over a portable model.

## Near misses and what each needs next
- 0x00A1CC30 CNavigatorAStar::ConstructFinalRoute: RELOCATION_MATCH with `float radius = (PInterface->GetRadius(false));`, but only
  with the earlier pass's fake same-TU body for GetIntersectionPointsBetweenNodes (declaration-only: 664v632) and placeholder
  statistics globals -> not landable. Next: the real GetIntersectionPointsBetweenNodes body same-TU noinline; name the globals.
- 0x006AD700 ControlledMove (dl=2), 0x0047D296 PCSetQuitToFrontEnd (dl=2), 0x00B9D730, 0x00872110, 0x008539C0, 0x00853670
  (dl=4 each), 0x00BCF040 (dl=2 + a masked reloc), 0x00A1BA10 / 0x00698170 / 0x008F3310 / 0x0095B9E0 (one slot each): see each
  differ/ line 2.
- 0x00927F30 (dl=18) and the stack-slot rows 0x0047E196 / 0x00563480 / 0x00813D00 were regraded only.

## Header debt
- Six overlay headers (hdr/README.md) must land with the rows: CRandomAppearanceMorph.h + CVectorMap.h (0x0086BFB0),
  CTCInGameMenu.h (VIDEODIALOGS: the three CTCInGameMenu rows), CUserProfileManager.h (DISPLAYDIALOG) + CTCInventory.h
  (REVERTVIDEOCHANGES) (0x0047E9AE), CGameCameraManager.h (INSIDETHING: 0x00697490). Each is the current repo header plus additions
  only; CGameCameraManager.h / CTCInventory.h were rebased on the agentFAM05 additions that landed at 18:01 and regraded.
- engine/CRandomAppearanceMorph.h has no default ctors for CBodyPartMesh / CSkeletalMorph on the default path (PDB has them).

## Descriptive names
- InRange (0x00697490 local inline helper, from the hand762 base; no PDB name found) -- descriptive.
