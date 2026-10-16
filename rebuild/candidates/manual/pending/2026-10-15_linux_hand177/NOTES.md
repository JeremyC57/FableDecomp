# Hand chunk 177 (CV_StatueMasterScript .. CVsyncCallbackTimer): 26 of 40 byte-exact

## What matched (26)
- CV_StatueMasterScript::GetStatuePointingPosition 0x00ED4420: sibling GetTimeOfDayFraction defined in the class (inlined); the 0.04 tolerance must be a literal (`fld [list] ; fadd [const]`), an `extern const float` gets its add operands swapped.
- CV_SwordInTheStoneScript ctor 0x00ED4AA0: landed quest-script ctor model (novtable arg base, CScriptThing pimpl member object, CTimer RegisterTimer).
- CV_TalentlessBardScript::ProcessSongsQuestComplete 0x00ED9B40: tag copy as a named CCharString local (`lea ecx ; push ecx`); retail vectors 12 bytes so QuestInfo +0x74, SongThingAvailable +0xd4 (byte-indexed).
- CV_TourGuideScript::WatchForClosingTime 0x00EE4A00: `while (!IsActiveThreadTerminating())` with the opening/closing test from g_pScriptDef +0x8e8/+0x8ec.
- CV_TrophyDealerScript::Init 0x00EE6D60: three CCharString named locals declared last-argument-first, GetActiveQuestName() (slot 655) inline as the first SetQuestCardObjective (slot 296) argument.
- vector<CBlurredAttitudesDef>::resize(n) 0x0067D610: inline default-ctor chain over CResource; derived def classes need IMPLICIT dtors (a user `virtual ~X() {}` keeps its vtable store alive across the free calls).
- CVectorMap<long, CIntelligentPointer<CThing>>::FindRandomElement 0x0086F510: count local before the inline generator step; hit arm as `if (first != last) {}` so `return 0` sits last.
- CThing::ComputeDistanceToMorphDummy 0x00661AA0: appearance pointer read into a named local AFTER the CCharString ctor (loads ecx before the pushes); `? false : true` bool across the string dtor.
- CVertexGroupBlock::AddGroupsFrom 0x00AD9260, CVertexGroupBlockOptimiser::InsertBlock 0x00AD92E0 / PaintCurrentBlockSegment 0x00AD9380: inline STLport map find returning the node, compared with the re-read header; loop bounds recomputed each pass.
- CVideoSys::Play/GetProgress/Process/TogglePause 0x00A3B130..0x00A3B4A0: COM stdcall vtables as the landed CVideoSys rows; Play's top-tested retry loop is `while (tries < 50 && (++tries, Run() != 0));`; GetProgress inlines GetCurTime/GetLength, `if (len == 0) return 0; return t / len;` for block order.
- CView3DBase::UpdateCameraMovementInfo 0x0069E870: implicit CCamera copies (`rep movsd`), C3DVector 3-float ctor temporary copied through integer registers.
- CTCCreatureOpinionOfHero::UpdateHitTolerance 0x006819D0: decay global read into a named local before the frame call; inlined GetToleranceToBeingHitRemaining; `return x <= 0.0f;` int-widened while the early exit is `mov al,1`.
- CTCQuestCard::GetMarkerRegionName 0x007021F0: named CCharString copy of TeleporterRegionName returned by copy (no NRVO); route test is `!= -1`.
- NUISystem::CTable ctor 0x005517E0: landed CZoomingComponent base model + STLport _Rb_tree base/derived ctor pair.
- CVillageDef ctor 0x004DFF04: `#pragma optimize("s", on)` (`or [m],-1` and `lea ecx` bases past +0x80).
- CVillageTaskCarryCrate::IsValid 0x00861430: `CTCCrateStack* stack = 0;` before the GetPItem call + out-parameter GetInterface(0xb5).
- CVillageTaskPatrol::GetNearestNode 0x00860A50: squared distance as `d = dz*dz; d += dx*dx; d += dy*dy;` (single sum expressions are reordered).
- CVirtualKeyboard::Confirm 0x00851920: `bool accept = !Done && GetProfileName().GetLength() > 0;` gives the frame-byte bool and the conditional temporary flag.
- CVolume::CopyFromVolume 0x00A67670: C3DBoxI->D3DBOX inline converter in retail store order (Top, Bottom, Left, Right, Front, Back).
- CVolumeTexture::ReleaseTexture 0x00A67F60 / CalcByteLength 0x00A67FF0: inline GetMemorySize `(PData ? GetAllocationSize(PData) : ByteLength) + 4`; CVolume by-value temporary with its inline dtor.

## Parked for Fable 5.1 (14)
- 00ed4b10 CV_SwordInTheStoneScript::Init DIFFER(122v122): timer index `mov edx,[esi+7Ch]` hoisted above the first flag stores | tried: direct SetTimer call, CTimer::SetTime helper, named locals | next: lane gap: scheduling
- 007f9660 CVectorCompare_BestForTargeting::GetCompareValueForPoint DIFFER(130v130): x87 squares X first where retail squares Y first | tried: X+Y+Z, Dot(*this), Y+X+Z | next: `s = dy*dy; s += dx*dx; s += dz*dz;` chain (the 0x00860A50 fix)
- 00cbd5d0 CVectorMap_EntryScriptBase::InsertEntry DIFFER(168v145): pair temporary words re-read from the frame after `inc [info]` where retail keeps them in ecx/edx/ebp/esi | tried: member-init copy ctor, local-pointer copy ctor, /Oa | next: build the pair words as by-value parameters of a __forceinline push helper
- 00ad8c30 CVertexGroupBlock::IsSubsetOf DIFFER(117v108): register allocation (retail other in ebp, other.begin in ebx reused as index, this.begin edi) | tried: index form, pointer walk, if-block, operand swap | next: lane gap: regalloc
- 00ad8ca0 CVertexGroupBlock::IsProperSubsetOf DIFFER(117v108): same as 0x00AD8C30 | tried: same | next: lane gap: regalloc
- 00ca9d00 CBase_00ca9d00::~CBase_00ca9d00 DIFFER(106v108): retail 8-byte EH frame (`sub esp,8`) vs `push ecx`; DeleteCriticalSection arg via eax not edx | tried: CBaseObject empty/4-byte, inline virtual ~CUnknown | next: make CBaseObject a DIRECT base of the class (probe on 0x00CAF480 gave `sub esp,8`)
- 008614b0 CVillageTaskCarryCrate::IsCreatureValidForTask DIFFER(112v117): retail keeps `true` in bl (`push ebx ; mov bl,1 ; cmp al,bl ; cmp [edi+18h],bl ; mov al,bl`) | tried: `== true` compares, bool local, optimize("s") | next: a non-constant bool source for the 1 (inline helper returning a bool member/argument)
- 0065dce8 CVillageTaskSecuritySweep ctor DIFFER(98v98): argument word copies interleave through edx in ours, strict eax/ecx in retail | tried: init list, body assignments, user operator=, user copy ctor | next: lane gap: scheduling
- 00a67530 CVolume::ReadOnlyLock DIFFER(151v141): D3DBOX zeros as immediates (retail shares xor eax); result copied via edx/ecx/esi in +4,+0,+0xc,+8,+0x10,+0x14 order with `out` in eax | tried: by-value return, named dimension locals, out reference | next: assign out fields one by one in retail order through a CLockedVolume& kept in eax
- 00a677d0 CVolume::Lock DIFFER(148v138): same as 0x00A67530 without the READONLY flag | tried: same | next: same
- 00caefb0 CBase_00caefb0::~CBase_00caefb0 DIFFER(113v118): retail null-checks `this` and the CUnknown pointer before ~CBaseObject, ours only the second | tried: CUnknown at +4 behind an interface | next: find the extra inline level (an intermediate class or explicit base conversion)
- 00caf480 CVtable_012bf8d4::~CVtable_012bf8d4 DIFFER(95v122): retail null-checks `this` before the CBaseObject dtor call, ours none | tried: base order swap, direct CBaseObject base, polymorphic novtable CBaseObject | next: same as 0x00CAEFB0
- 00a682e0 CVolumeTexture::InitialiseFromPreallocatedVolumeTexture DIFFER(112v131): retail tail-duplicates the second inline GetMemorySize's ByteLength arm | tried: ternary, multi-return, `g = g + size` | next: lane gap: tail duplication
- 009f7050 CVsyncCallbackTimer::GetInfo DIFFER(138v138): /Od frame order (retail info above the record pointer, no pad) | tried: pointer, reference, nested block | next: lane gap: /Od local allocation order

## Skipped
- none

## Label corrections
- 0x00661AA0 "CVertexBufferWin32::ComputeDistanceToMorphDummy" is a CThing member (GetPosition on this, PTCGraphicAppearance +0x64).
- 0x0067D610 "CVectorMap::resize" is vector<CBlurredAttitudesDef>::resize(n) (vtable 0x0125B8AC temporary).
- 0x006819D0 "CViewBase::UpdateHitTolerance" is CTCCreatureOpinionOfHero (inlines 0x00681940); 0x0049D870 "GFGetPreMainMemoryUsed" is CWorld::GetWorldFrame.
- 0x007021F0 "CViewBase::GetMarkerRegionName" is CTCQuestCard; 0x00686D20 "GetDisplayEngine" is the world-map accessor; 0x004FB150 "CWorld::DrawGetEnvironment" is CWorldMap::GetCurrentRegion.
- 0x005517E0 "CViewport::CViewport" is NUISystem::CTable::CTable (vtables 0x0124A224/0x0124A1FC/0x0124A1F4).
- 0x00CA9D00 "~CVertexShaderData", 0x00CAEFB0 and 0x00CAF480 "CVolume::ReleaseVolume" are strmbase-style CUnknown-family destructors (0x00CAA410 base, deleting dtor 0x00CAF460 in vtable 0x012BF8D4, callers 0x00CAF070/0x00CAFD90).
- 0x0099B220 "CCharString::GetLength" is CWideString::GetLength here; 0x00AD8AF0 "TreeNodes_HaveIntersection" is CVertexGroupBlock::HasIntersection; 0x00AD91F0 "AnimBlockGroup_AddBlock" is CVertexGroupBlock::AddGroup.
