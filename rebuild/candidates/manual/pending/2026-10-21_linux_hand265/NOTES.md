# Hand chunk 265 (CHybridMultifractal .. CModeDragonFireBeam): 23 of 40 byte-exact

## What matched (23)
- CMainGameComponent::InitGraphics 0x00416C8A: picks the texture formats (compressed DXT5 when supported and the global flag is set; otherwise opaque/alpha/bool-alpha), then the uncompressed and signed formats, and the bank size/mipmaps/dither. It creates "GBANK_MAIN" through CreateAndInitialize 0x9F83D0 and ends with SetGraphicBank 0x4BBFC0. Shape: a direct twin of the landed CNewFrontendGameComponent::InitialiseGraphics 0x0042DDB3, with each DisplayProgress string in its own block, in a size TU (`optimize("s",on)` + `("y",off)`).
- CMainGameComponent::Run 0x004189C2: the main loop. Starts the clocks and event-package I/O, sets the presentation interval or touches the current region, applies the user profile, then ticks until Quit. Shape: a size TU. A `double frameStart` local feeds RecordFrameTime. The post-loop store order `Running = false; *next = Next;` is retail's (store-order rotation). The bool argument comes from reading the global twice (`if (g) f(g,1,true)`), which gives `xor eax,eax; mov al,[g]`.
- NUISystem::CManager::SetInput 0x0041DF10 and CManager::CManager 0x0041E3F6: store the input type and fill the UI-key to engine-key map, and the front-end manager ctor. Shape: a local CManager model per the landed ~CManager 0x0041E641 / SetGraphicsBank 0x0042A9B7. Keys are const-ref temporaries.
- NUISystem::CMenuEntry::ProcessEvent 0x0053D200: menu entry input. Shape: the landed CClickable::ProcessEvent convention; the inline accessor call order is kept as written.
- NUISystem::CKeyRedefiner::ProcessEvent 0x00557EB0: turns a key event into a redefinition. Shape: the landed body of CanRedefineAction 0x00557960 is defined earlier in the TU under `#pragma auto_inline(off)`, so VC knows it preserves edx and keeps edx live across the call.
- CInputProcessMain::ProcessInput 0x00687FD0 (/GS), CInputProcessZTarget::ProcessInput 0x00688EB0, CInputProcessInventoryBase::ProcessInput 0x006894C0, CInputProcessUseRangedWeapon::ProcessInput 0x0068F9D0: input processors. Shape:
  - 0x00687FD0 has a char-array local, so it needs the /GS cookie (0x0139C8A8, check 0x00BFE9F9).
  - GetAsyncKeyState goes through the import 0x014402CC.
  - A named C2DVector local gives retail's `lea`.
  - A visible flag + `break` gives the shared exit.
- CMap::HasWaterAt 0x0081FB00, GetWaterSizeZOffsetAt(C2DCoordI) 0x0081FCC0, GetWaterSizeZOffsetAt(C3DVector) 0x0081FEC0, DrawGetSizeZAt 0x008F7630: water/ground heights. Shape:
  - Named-float plane locals.
  - A clamp macro and a single `float z`.
  - Constant multiplies split as retail evaluates them.
- CMiniMapDisplay::ConstructMinimapFilename 0x00829A30: "<file>_<region><nn>.tga". Shape: an inline by-value `ToString(long)` (local, Format, copy out; no NRVO). CWideString::Format 0x0099BA70 is a varargs __cdecl member, so `this` is pushed as the first stack argument. A discarded `regionName + number` temporary is kept.
- CMiniMapDisplay::DrawMap 0x0082BE60: draws the map sprite, the courtyard overlays and the markers. Shape:
  - C2DBoxF needs a user-declared copy ctor; without one, the by-value ToScreenBox result is copied to a second temporary and the call is hoisted.
  - `long layer = 0x44; if (!front) layer = 0x47;` gives store-and-overwrite (a ternary gives neg/sbb).
  - A discarded inline by-value getter with AddRef/Release members gives the unfolded `inc/dec [ecx+4]` pair.
- CIsOpinionSourceWithReaction::CIsOpinionSourceWithReaction 0x0095F9A0: the functor ctor.
- CMemoryAllocatorVariableSize::InsertFixup 0x00A64740 (MATCH): red-black insert rebalance. Shape: a cached `prev` in the rotate.
- CLandscapeBackgroundPatch::InvalidateEdge 0x00BE63F0 (engine header), CLandscapeBackgroundTreeNode::Render 0x00BF71D0, CLocalDetailPrimitiveMesh::Load 0x00BFA0F0: landscape edge invalidation, frustum-culled render and the primitive load. Shape: local models copied from the landed MarkTreeAsInvisibleLastFrame 0x00BF6A30.
- CMinionSpawner::Main 0x00DA5980 and CM_RestartMarker::Main 0x00EF34A0: entity scripts. Shape: the scoped filter object of the landed entity-script Mains, and `&*Messages.begin()`.

## Parked for Fable 5.1 (17)
- 00f36860 CHybridMultifractal::GetFractalHeight DIFFER(461v451): retail keeps `weight` live in st(0) after `fst` (`fcom` 0.001 with no reload, `fstp st(0)` at exit); ours uses `fstp` and reloads | tried: named weight/result locals, loop-test rewrites, clamp before/after the store | next: lane gap: x87 stack caching
- 00405020 NUISystem::CIMEFont::DrawTextW DIFFER(406v410): retail puts the address-taken `scale` in the dead `text` param slot and reads it once into ebp; ours gives it its own slot and re-reads it | tried: a scale copy after the position conversion (408v410, edi not ebp), scope changes | next: lane gap: frame slot packing
- 00687540 CMessageEventManager::AddMessage(type,...) DIFFER(486v486): originator/target live in esi/edi swapped (`lea esi,[edi+0x44]` v `add esi,0x44`) | tried: declaration order, the store order rotation, const-ref locals | next: lane gap: callee-saved register choice
- 00687730 CMessageEventManager::AddMessage(def,...) DIFFER(477v477): the faction-lookup key temporary goes into the dead `pos` slot in retail ([esp+0x78]) and the `def` slot in ours ([esp+0x74]) | tried: key as named local / temporary, def copied early | next: lane gap: frame slot packing
- 0095fb40 CIsOpinionSourceWithReaction::operator() DIFFER(443v451): frame 0x24 v 0x20 (retail shares the GetInterface key, the pointer temporary and the loop offset in one slot); owner compared from memory not edi; bool arg zero-extended v byte store | tried: early returns (these duplicate the epilogue), a single nested condition, an unsigned loop counter, `? true : false` | next: lane gap: frame slot packing
- 00558770 NUISystem::CKeyRedefiner::Initialise DIFFER(475v472): esi/edi swap (this v child-vector web) and `mov ecx,[esp+0x10]; mov ebx,ecx` v retail's `mov ebx,..; mov ecx,ebx` | tried: the CRedefinerList::Initialise 0x005566A0 model, a CCountedPointer<CList> local, `Children[size()-1]` | next: lane gap: callee-saved register choice
- 00537776 NUISystem::CList::CanMove DIFFER(480v480): frame 0x3c v 0x48, and the first ConvertX(...)+Offset loads Offset.x first | tried: member/free operator+, Offset-first sum, a rect struct copy (movsd), a scaleY local | next: an inline C2DVector ctor-from-two-floats sum in a separate scope; else lane gap: operand order
- 00ce0920 CMK_HS2_SOUL::Main DIFFER(453v470): retail calls the effect's IsAlive through the vtable (+0x12c) and calls ~CScriptThing 0x004AA840 out of line except for the last | tried: a non-inline dtor decl, the effect through a base pointer | next: model the effect as an incomplete-type virtual and give ~CScriptThing a TU-external body with one inline twin
- 00ce0b00 CMK_HS2_SOUL::Main DIFFER(456v470): as 0x00CE0920 | tried: as 0x00CE0920 | next: as 0x00CE0920
- 00ce0ce0 CMK_HS2_SOUL::Main DIFFER(456v470): as 0x00CE0920 | tried: as 0x00CE0920 | next: as 0x00CE0920
- 00ec42f0 CM_RTFERockTrollTrigger::Main DIFFER(412v412): retail places the early-exit block (destroy spawnPos, return) after the inline GetPos ternary; ours emits it inline after the IsDone test | tried: scoped filter, implicit name temporaries, discarded CreateCreature temporary, if/else inversion | next: lane gap: block placement
- 00c0b570 CAngleInterpolator::SetTarget DIFFER(417v435): retail keeps the `* 0.0f` terms of inverse*(r0,r1,0) (`fmul [0x0122DEDC]`) and homes r0/r1 in dead arg slots | tried: an explicit 0.0f vector component, an inline matrix*vector, named r0/r1 | next: the zero component as a non-const global/member load so VC cannot fold it
- 00a55df0 CMatrix3x4::operator* DIFFER(415v415): `mov ecx,0xc` (result copy count) is one instruction early inside the E32 sum | tried: inline operator*= over rep movsd copies, k-order products | next: lane gap: scheduling
- 00ef4320 CMeanBandit::Main DIFFER(492v420): retail calls the CScriptThing copy out of line for the filter and the Reset* args and inlines the Set* ones; ours inlines every copy | tried: by-value and const-ref arg forms, a named copy | next: a TU-external copy ctor declared plus an inline helper for the Set* sites
- 00a09f20 CMeshBankManager::OpenMeshBank DIFFER(488v485): found-path add-ref is mov/inc/mov through ecx->esi; retail `inc dword ptr [esi]` | tried: copy ctor init-list/body/info-local, unsigned RefCount, const-ref found, postfix ++, break+test | next: lane gap: callee-saved register choice
- 008db620 CModeDragonFireBeam::StartBeamEffect DIFFER(490v481): instruction stream matches; retail packs effectType, the pointer temporary, GetInterface's key and the second mouth name into one slot (R-0x2c) | tried: implicit const char* conversions, `effects.m_p->`, effects after the BeamActive check, a forceinline intelligent pointer | next: lane gap: frame slot packing
- 0041e1cd NUISystem::CManager::SetMetaLayer DIFFER(485v485): retail hoists the `or [ebp+8],-1` / `and [ebp+8],0` key stores of keys -1/0 above the previous value store | tried: clamp constants hoisted into edi/ebx, keys as const-ref temporaries in [ebp+8], statement order | next: lane gap: scheduling

## Skipped
- none

## Label corrections
- 0x00687CF0 / 0x004379C0: `_Cons_val` labels on list-node constructors.
- 0x009EA540 CThingFilter_IsUnconscious -> CConsole::ProcessInput.
- 0x00C0B570 CMatrix::UpdateInversePerspective -> CAngleInterpolator::SetTarget.
- 0x00C1D1C0 C3DMeshObject.. -> CMatrix3x4 inverse (__fastcall).
- 0x00A043A0 GetDefaultVal -> CInputEvent::GetAnaloguePosition.
- 0x00A03B40 DrawGetEnvironment -> CInputEvent::GetType.
- 0x00A0D390 CWomanToAttract::Init -> SetHandled.
- 0x00449970 Catch -> GetMainPlayer.
- 0x009E2C80 CMessageEventFilter.. -> GetFont.
- 0x00557960 CTCInventoryAbilities::GetCategory -> CKeyRedefiner::CanRedefineAction.
- 0x0099A380 / 0x0099A430: CBaseObject ctor / dtor.
- 0x00415B80 CNavigationLayer ctor -> CGraphicDataBankInit().
- 0x004BBFC0 SetCurrentThingManager -> SetGraphicBank (__fastcall).
- 0x00416202 operator_new -> frame-time recorder (thiscall, double).
- 0x0041627F duplicate BeginInputLoading -> BeginOutputSaving.
- 0x004FB150 DrawGetEnvironment -> CWorldMap::GetCurrentRegionIndex (+0x9c).
- 0x0099AED0 CCharString ctor -> CWideString::CWideString().
- 0x0099BA70 SetFromFormatV -> CWideString::Format (varargs __cdecl).
- 0x004C73D0 DoSizeof -> CThing position getter.
- 0x00643BEF operator&&<..> -> ToScreenBox (GFToScreen wrapper).
- 0x00669D30 CCRC::CalcAndLookup -> CSpecialEffectsDef::GetEffectFromString.
- 0x009BFF10 AddLearnedExpression -> CDisplayManager::ChangePresentationInterval.
