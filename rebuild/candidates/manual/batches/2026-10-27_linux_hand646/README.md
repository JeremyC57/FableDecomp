# Chunk 646 -- 3 of 10 landed (2026-10-31)

Cut from the parked-row pool (same-length residues). Landed via `coordinator/land_chunk.sh 646` (vland --land 3/3 RELOCATION_MATCH +
PASS, multi-check 0): NPlayerGui::CDrawNPCName::Draw 0x006572A0 -- the font call had the wrong signature: virtual slot 1 is
`C2DExtentsF GetStringExtents(const wchar_t*, bool) const` reached through the PDB inline CFontBank::GetStringWidth(const CWideString&),
so the `push 0` is the bool and the 8-byte return fixes the frame slot (plus PDB respells of Draw2DText, PeekCutSceneMainFont, GetTextBank,
GetCurrentWorldFrame, the const GetTextBySymbol, the CDrawBase members and the shared CCountedPointer.h); NTCScriptedControl::
CActionTalkBase::GetNextCameraTagIndex 0x00712E40 -- `rand() % 3` written in each branch lets VC7.1 hoist the identical call above the
`current_index == -1` test (retail's order and base registers), the static DefaultCameraTags indexed with STLport `*(begin()+n)`;
CCreatureActionBase::InitialiseActionEvents 0x006946C0 -- both callees that take the counted event (CCountedPointer<CActionEventBase>::Reset
as a noinline explicit specialisation carrying the shared header's body, and the push_back that copies it) given bodies in the same file turns
the release into retail's `dec [edi]; jne`; the PDB inline CWorld::GetGameCamera() fixed the ecx/eax roles. Seven rows stay DIFFER in
`pending/2026-10-27_linux_hand646/differ/` (00868e50 GetArmourWeightsFromThing one x87 operand swap, 00649e8b CDrawFishingGame::Draw two
CCharString temporaries share a slot, 009c8c00 CDataBank::NotifyEntryAdded scratch rotation, 00697490 IsPosCameraInsideThing pos load
after the last fstp, 008539c0 OnInitLeaf two scheduling swaps, 00687730 CMessageEventManager::AddMessage GetTC key in the dead def slot,
00537776 CList::CanMove frame 0x3c v 0x48). The new rows carry PDB spellings for callees that eight landed callers bound under stand-ins;
all eight were respelled and rechecked RELOCATION_MATCH (`verify_caller_respell.log`): the GF coordinate helpers (verified from the retail
bytes: 0x009E1DA0 GFAXToVX and 0x009E1DD0 GFAYToVY divide by the screen size, 0x009E1E10 GFVXToAX and 0x009E1E40 GFVYToAY multiply by
it; all `__fastcall float(float)`) in 0048dbc3, 009de140, 009db700, 0041a980; CPlayerGui::Draw2DText(const CFontBank&) in 0048f1f4,
00644424 (`#define CPlayerGui CPlayerGuiLayout` include pattern), 00646010; NAnimationEvents::CManager::GetEventNames / GetGameEventManager
and the STLport vector<CCharString> dtor in 00787f29. Lessons: check a callee's `ret N` and push count against the real signature before
blaming the frame packer; VC7.1 hoists an identical leading call out of both arms of an if; a counted-pointer release only takes the
`dec [edi]; jne` form when every callee that takes the pointer has a body in the same file (lets the shared CCountedPointer.h replace local
count-block models); PDB inline accessors fix register roles in member chains. 14 NAME_FIXES + 6 review rows queued (prefix c646):
GetCurrentWorldFrame 0x004167C8 (7 callers, 8 spellings, identity probable), g_pMainGameComponent at 0x00657240, PeekPControlledCreature,
GetMainPlayer by reference, NGameText::ConstructString, UpdateMiniGame namespace, GetGameEventManager (5 other callers), CThing::GetWorld
protected, NotifyEntryAdded, CWideString::operator= const return; descriptive globals 0x013B8390 g_pDisplayManager, 0x01380C30 /
0x013BAD00 (two spellings each), 0x01375550 g_FramesPerSecond, 0x013BA8BC (three spellings); header debt (CCreatureActionBase.h flattens
the vptr, no CDrawBase header, CPlayerGui.h/CFontBank.h flatten). Labels: ghidra_out/labels_chunk646_2026-10-27.tsv.
