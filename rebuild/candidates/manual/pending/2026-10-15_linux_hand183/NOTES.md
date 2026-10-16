# Hand chunk 183 (CAIStateGroup_MinionBehaviourGreetCreature .. CAIStateGroup_StealthSeekEnemy): 20 of 40 byte-exact

All 20 src/ rows grade RELOCATION_MATCH (gall.sh) and their tests PASS under tall.sh (g++) and tall.py (VC7.1).
Most sources share one AI model (thing bitset at +0x20, CVectorMap at +0x44, LowerBound 0x0040F020 / 0x004365B0,
CIntelligentPointer 8 bytes) expanded inline into each file.

## What matched (20)
- CAIStateGroup_MinionMotherProtectFamily / MinionReactToEnemy / MinionBehaviourGreetCreature / SeekScriptTarget
  IsRunnable (0x933C10, 0x937F70, 0x9575C0, 0x93BEF0): HasTC + entry-returning FindEntry (component used straight as
  `this` in the virtual call), CanCreatureSeeCreature __fastcall, `PX = thing` via the converting IP temporary.
- CBaseVideoRenderer::ResetStreamingTimes (0xCA52A0, catalog label MinionHasInformation::OnActivate is wrong): the SDK
  body in source order; the vfptr must be a plain `void*` member (a real vfptr is padded to 8 before the __int64s).
- CAIStateGroup_MinionGossip::IsStillValid (0x90CB80): GetInterface straight into members; a creature compared after a
  call is a NAMED local read before the call (held in a callee-saved register).
- CAIStateGroup_MinionHawkingFromStall::IsStillValid (0x94BDF0): same named-local rule; the second compare needs the
  getter result in a named local.
- CAIStateGroup_MinionKidGroupTag::Cleanup (0x9347B0): landed RandomFramesWithin inline, `frame + Random(2)`.
- CAIStateGroup_MinionFindPersonalSpace::IsSpaceInvader (0x92E890): `(float)fabs(d) <= 1e-4f` gives `test ah,41h ; jp`;
  HasTC must be `__forceinline` (it was emitted out of line otherwise); key lands in the dead argument slot unaided.
- CAIStateGroup_MinionReactToBurglary::Update (0x958000) and IsRunnable (0x957F20): a literal passed for a
  `const CCharString&` (IMPLICIT conversion temporary) is built before every other argument; `&&` statement keeps the
  temporaries' flags in ebx; a member pointer reached through an inline getter gives `lea ecx,[eax+168h]`.
- CAIStateGroup_MinionBehaviourGreetCreature::Update (0x9574D0): named CCharString in the inner block, result as a
  named long compared after the action dtor.
- CAIStateGroup_MinionReactToNoise::Update (0x91CCC0): rotated yield while-loop, landed do/while MoveTo loop.
- CAIStateGroup_MinionWalkToMarker::IsRunnable (0x921630): bool result declared OUTSIDE the block holding the two
  CCharStrings (kept in bl across both dtors).
- CAIStateGroup_NymphFloatAround::OnReplace (0x91C130): unused CBrainOutput local with the landed inline counted-pointer dtor.
- CAIStateGroup_OpinionReactionRoot::OnActivate (0x95F580): frame-rate global read into a local before the call;
  ThingReactingTo is the unaligned IP at +0x6b (pack(1), PDB layout).
- CAIStateGroup_OpinionReactionRoot::Update (0x95F6F0): CurrentUpdateFunc needs the multiple-inheritance PMF form
  (`#pragma pointers_to_members(full_generality, multiple_inheritance)`: `mov ecx,[edi+7Ch] ; add ecx,edi`).
- CAIStateGroup_ServeFood::OnReplace (0x9239C0): two body GetInterface calls share one slot; `-1` stores share `or eax,-1`.
- CAIStateGroup_StealthSeekEnemy::SetWhatsThatSound (0x93C600) / SetWhatsThatAnimation (0x93C6D0): implicit CCharString
  temporary; an inline const `GetCreature()` accessor fixes the PBrain load order.

## Parked for Fable 5.1 (20)
- 0096f180 CAIStateGroup_MinionBehaviourSad::FoundDeadSpouse DIFFER(188v188): frame: retail is 0x1c bytes with the intelligent-pointer temporary overlapping the dead filter slot (+0xc) and the out thing at +0x14; ours 0x20 with spouse +0x10, temp +0x14 | tried: inline search-tools expression, named tools local (fixed call order), 8-byte filter | next: lane gap: register/scheduling
- 00910100 CAIStateGroup_MinionListenToStory::IsRunnable DIFFER(183v186): retail returns through eax (`mov eax,1` / `xor eax,eax`, an int-width result despite the bool mangling) and puts the out thing in the key slot (+0xc) with the filter at +0x10 | tried: CCharString temporary, block-scoped named CCharString (fixed the Get/lea order), filter/marker declaration orders | next: lane gap: register/scheduling
- 00744d00 CTCPerceiveThing-family FindAlertEvent DIFFER(163v166): retail keeps the sender pointer's GetPItem in a frame word followed by a constant-false `xor al,al ; jne` (an inlined template test), owner in ebx/ebp, filter slots 8/0xc/0x10 | tried: flat && chain with Bind2 of two one-word filters | next: model the inlined IS_THING_ALIVE<CIntelligentPointer> template that yields the dead test
- 00958b20 CAIStateGroup_MinionReactToCrime::OnReplace DIFFER(219v224): the message-sender read is an inlined accessor leaving a dead frame store + constant-false `xor cl,cl ; jne` (same shape as 0x00744D00); also retail `dec dword [eax+0D0h]` in place and esi/edi swapped | tried: plain sender = GetPItem() form | next: recover the inlined CMessageEvent sender accessor (shared with 0x00744D00)
- 00958c00 CAIStateGroup_MinionReactToCrime::IsGuardOnLevel DIFFER(163v163): register swap only: retail holds the list node in edi and &node->value in esi, ours the reverse | tried: raw node for-loop, while loop, STL-style iterator struct | next: lane gap: register/scheduling
- 00958e20 CAIStateGroup_MinionReactToCrime::IsStillValid DIFFER(218v216): the opinion component goes to eax then `mov ecx,eax` after the argument pushes (retail loads ecx straight from the entry before them) | tried: HasTC+FindEntry for both (frame 0xc: handle not sharing the key word), named empty filter (drops the byte init), GetInterface out-param for the opinion (fixes frame), a T*-returning inline lookup | next: lane gap: register/scheduling
- 00930e20 CAIStateGroup_MinionReactToCriminal::OnReplace DIFFER(195v186): only the unlock: retail `and byte ptr [eax+0D4h],0FEh` in place, ours `mov cl,[eax+0D4h] ; add eax,0D4h ; and cl,0FEh ; mov [eax],cl` | tried: byte mask, 1-bit bitfield, #pragma optimize("s") (whole body changes) | next: lane gap: in-place RMW on a GetPItem() result (also 0x00958B20 `dec dword [eax+0D0h]`)
- 0092f240 CAIStateGroup_MinionReactToPlayerInteraction::InteractionFailed DIFFER(195v193): filter scheduling: retail stores Bind2.PFirst before the IsOriginatedByThing ctor call and IsOfType.Type after it, keeps the message in edi; later the creature's bitset base held in eax (ours ecx) | tried: filter temporaries (4-byte, then 8-byte second filter -> frame 0x14 fixed), named filter/Bind2 locals, named manager local | next: lane gap: register/scheduling
- 00938f70 CAIStateGroup_MinionSitOutside::IsRunnable DIFFER(240v238): only the random tail: retail takes `rotated % 150` (div ecx, remainder in esi) BEFORE the GFGetCurrentFrame call; ours sinks the div after the call | tried: GetInterface out-param vs register local (fixed: the preference test sits inside the HasTC branch), named remainder local, operand order 225 + r + frame, re-reading *seed | next: lane gap: div sinking past a call
- 0092e130 CAIStateGroup_MinionSleep::IsRunnable DIFFER(229v229): register swap: retail keeps this in esi and the CTCSleep in edi (edi pushed after the HasTC test), ours the reverse | tried: nested-if form (235 bytes), early-return form (size fixed), GetInterface out-param | next: lane gap: register/scheduling
- 00985f40 CAIStateGroup_OpinionReactionRoot::WifeClothingUpdate DIFFER(201v201): frame-slot swap only: retail puts the lookup key and the category out value in one word (+0xc) and the CCharString line at +8; ours shares the key with the line | tried: && chain, early return, declaration order, HasTC+FindEntry | next: lane gap: register/scheduling
- 0095e350 CAIStateGroup_ReactToDeadLeader::IsRunnable DIFFER(217v219): call order only: retail calls the world getter 0x008FCFA0 BEFORE pushing &filter (`mov eax,[eax+60h] ; lea edx ; push edx ; mov ecx,eax`), ours pushes first | tried: named filter (frame 0xc), filter temporary (fixed frame and the ~flags test), named manager local (reorders the creature loads), block-scoped named filter | next: lane gap: register/scheduling
- 00965210 CAIStateGroup_RecoverStealableItem::FindPStealableItemOutsideTargetPointRange DIFFER(170v165): frame 0x4c v 0x38: retail builds the In3DRange temporary in the slot the Bind4 block later reuses (dead Z store at +0x34) and copies it into the unary_negate at +0x1c, the empty HasFreeActionPoint filter is not zeroed, the out object sits in the dead first-argument slot | tried: all-temporaries call (one try) | next: named filters without value-init; lane gap: dead-argument-slot / overlapping temporaries
- 0097b780 CAIStateGroup_ScorpionKing::Cleanup DIFFER(235v236): two spots: the combat manager goes eax then `mov ecx,eax` (retail `mov ecx,[eax+4Ch]`), and the trap re-index is `mov ecx,[edi+1Ch] ; add ecx,ebp` where retail has `mov eax,[edi+1Ch] ; lea ecx,[eax+ebp]` (also the loop-align filler `lea ecx,[ecx]` vs `lea esp,[esp]`) | tried: inline call chain (pushes the creature first), named creature local, named combat-manager local (keeps the creature in esi), named trap local (drops the 4th GetPItem) | next: an inline element accessor for the re-index
- 0093bdf0 CAIStateGroup_SeekEnemy::Cleanup DIFFER(169v169): argument scheduling: retail loads the component into ecx between `mov edx,[ebx+4]` and `mov eax,[edx+18h]` (ours loads PBrain->PCreature through ecx/edx first) | tried: FindEntry form, GetInterface out-param | next: lane gap: register/scheduling
- 0093c230 CAIStateGroup_SeekScriptTarget::Cleanup DIFFER(169v169): argument scheduling: retail loads the component into ecx between `mov edx,[ebx+4]` and `mov eax,[edx+18h]` (ours loads PBrain->PCreature through ecx/edx first) | tried: FindEntry form, GetInterface out-param | next: lane gap: register/scheduling
- 00922fc0 CAIStateGroup_ServeFood::WalkTo DIFFER(175v175): lane gap: dead-argument-slot temporary -- retail keeps the look-at X/Y in the 0xC frame and stores Z into the dead pos.X argument slot ([esp+14h]); ours needs a 0x10 frame (or, with a value ctor, evaluates Z first) | tried: operator+ via value ctor (right-to-left order), field-by-field named sum | next: lane gap: register/scheduling
- 00927df0 CAIStateGroup_SetupWares::IsRunnable DIFFER(173v173): tail merge only: the two `return false` jumps (no shop / condition false) land on retail's SECOND SetStockState arm's `xor al,al` tail (0x927E93), ours on the first arm's | tried: arm returns, if/else arms, early-return chain | next: lane gap: register/scheduling
- 00945bd0 CAIStateGroup_SitForMeal::Update DIFFER(219v219): register only: retail tests the presence byte through eax (`mov eax,[eax+18h] ; test byte [eax+28h],40h`), ours through ecx | tried: while/return loop (231 bytes), do/while loop + chair branch order (all else fixed), raw bitset test | next: lane gap: register/scheduling
- 0093c420 CAIStateGroup_StealthSeekEnemy::Cleanup DIFFER(184v184): argument scheduling only (same as 0x0093BDF0): retail `mov ecx,[eax+4] ; mov eax,[edi+4] ; mov edx,[eax+18h]`, ours loads PBrain->PCreature through ecx/edx before the component | tried: FindEntry inline, GetInterface for 0x45 (fixed that call), named component local, named creature local | next: lane gap: register/scheduling

## Skipped
- none (no row spans two functions; nothing in the chunk was already landed).

## Label corrections
- 0x00CA52A0 is CBaseVideoRenderer::ResetStreamingTimes (CTextureRenderer vtable 0x0129D08C slot 47), not
  CAIStateGroup_MinionHasInformation::OnActivate.
- 0x00744D00 sits in the CTC region and calls CTCPerceiveThing::SetAsDirectlyAwareOfPerceivedThing on `this`; the
  MinionReactToAlert attribution is doubtful.
- 0x004C73D0 is the thing position getter (not CVertexBufferWin32::DoSizeof); 0x00798D80, 0x0049D9D0, 0x00487DD0 are
  getters (not _Cons_val<...>); 0x008FCF40 is the state-group finish/hand-back call (not CXMVPlaybackInfo);
  0x008FCFA0 is a second world getter (PBrain->+0x1c->+0x24, not CPlayer::GetCurrentMode); 0x0065914B is a village
  crime query (not CBankFileAsyncData::StartRead); 0x00658D5D is a village time test distinct from the curfew test
  0x00658DB3; 0x007B9090 / 0x007B85F0 are table-component calls (not CTCActionUseAcceptBoast / CVillageTaskCarryCrate);
  0x00787C07 is a trap reset (not CXMVPlaybackInfo); 0x0066B280 is a sound-component play call.
