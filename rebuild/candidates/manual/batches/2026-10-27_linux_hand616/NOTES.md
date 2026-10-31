# Hand chunk 616 (CAIStateGroup_MinionGuardHeardYellForHelp .. CWorld): 2 of 10 byte-exact

All ten rows came in PARKED with a same-length best attempt. 2 now match; 8 stay parked, each with a re-proven
spelling and its residue measured again.

## What matched (2)
- **CSoundEventManager** (libpk) -- 0x00C11D00 `??0CSoundEventManager@@QAE@PAUCAudioOwner@@MMMMMPAUCSteeringWorldEntry@@@Z`
  (RELOCATION_MATCH 507v507, /EHsc). The constructor builds the owner pointer, a listener frame (position 0, front +Y, up +Z),
  the +0x2C CSoundEventSet from three floats, the fader (1000 / owner rate), clears the state block, then news the two helper
  queues, CAudioResources, and the CTimerHost (50 Hz, stack 0x3000, priority 15).
  Shape: the earlier STransform5 + STransform4 members fixed the store order wrongly. Three `CVec3 {X,Y,Z; CVec3(x,y,z)}`
  members built in the init list (`Position(0,0,0), Front(0,1,0), Up(0,0,1)`) give retail's zero-register stores and
  1.0f immediates. The timer is the landed CTimerHost spelling (0x00C25B10) with SThreadParams_00c355e0.
- **CAIStateGroup_JackOfBladesBase** -- 0x009718F0 `?GetIsEnemyInJacksWay@CAIStateGroup_JackOfBladesBase@@IAE_NABVC3DVector@@0@Z`
  (RELOCATION_MATCH 510v510). Returns true when the live target is nearer in XY to the destination than the start is, and
  lies within 15 degrees of the path direction. Uses engine/CAIStateGroup_JackOfBladesBase.h through the define trick.
  Shape: retail CSEs the XY squares of both GetSquaredMagnitude2D calls into the two inline Normalise sums. Two compound
  forms give retail's register/spill order: `squared = Y * Y; squared += X * X;` in GetSquaredMagnitude2D, and
  `lengthSquared = z * z; += x * x; += y * y;` in Normalise. Each form alone fixed one half.

## Parked for Fable 5.1 (8)
- 00910df0 CAIStateGroup_MinionGuardHeardYellForHelp::Update DIFFER(500v500): inlined GetPOriginator type test is `xor eax,0Fh ; test al,0Fh` in retail, ours `not eax`; the brain load before GetFOV goes through eax (ours ecx) | tried: about 38 grades this chunk: 30 xor forms in scratch TUs (bitfield, enum mask, char/short/long, ^ then test, /G5-/G7, /Os, /Oa, QFE 4035) and 8 brain-load combinations; an exe scan finds the xor form only at the 7 inline sites inside Update bodies (filters and the out-of-line copy 0x00910CC0 use not) | next: lane gap: link-time (LTCG) inline codegen choice
- 0077c960 CTCHighlightItem::DoMovement DIFFER(502v502): bounding-box centre sum `fld [eax+24h] ; fadd [eax+18h]` (ours), retail loads MinCoord.Z first | tried: the best attempt graded no symbol (public DoMovement mangled QAE). The PDB-protected leaf ?DoMovement@CTCHighlightItem@@IAEXXZ restores the grade; the two earlier sweeps covered every sum form | next: lane gap: x87 operand order of a commutative add over one base register
- 0070d580 CAnimateThing::SetMainAnim DIFFER(504v504): the combination-anim vector end is loaded into eax and copied to ebp (retail loads ebp directly) | tried: STLport 4.6 _Vector_base vector with begin()-based operator[] and member-difference size() (23 -> 19 lines, fixed IsValid's Anims[1] register); loop forms c-j (end re-read, begin-then-end, comma for, while, do-while, reference, &v[0] range); /Oa (worse) | next: an iterator-typed loop over a real STLport vector<CAnimCombination*>::iterator
- 007d5070 CTCTavernGameCardSorting::CardsMainGame DIFFER(539v539): retail emits `fstp [esp+2Ch]` (Z) before the X=0 store `[esp+24h]` of the Move target | tried: full Ego_r respelling (virtual UAE leaf, GetDef / protected PlaySoundW / SetStateAfterOK / SetState with ETavernGameStates); named float z, ctor-body assignments, copy ctor, a named C3DVector local, Z/X/Y member order (=); PlaySoundW with a temporary CCharString (564) | next: lane gap: store scheduling of a three-float temporary
- 007f5ae0 CTCTargetingBase::UpdateNearestInterestingThing DIFFER(544v544): the search result goes into eax and is copied to ebp (retail straight into esi); the &PNearestInterestingThing CSE is in esi (retail ebp); the first layer is in a slot and the map in esi (retail: layer in ebp, map spilled to [esp+1Ch]) | tried: PDB respelling (protected virtual MAE leaf, CTCBase::GetWorld / GetThingSearchTools, CWorld::PeekWorldMap slot 13, static CWorld::GetFrame, const references) = 48 lines as before; GetWorld().GetFrame() inside the modulo (529, the dead call is dropped); inline layer compare (554); map pointer; result used directly (554); declaration order (=) | next: lane gap: allocator priority between best and the intelligent-pointer CSE
- 00871a80 NCreatureAction::CActionEventTriggerParticle::Dispatch DIFFER(547v547): scratch-register rotation (eax/ecx/edx) shifted by one from the first HasHelperPoint argument on | tried: PDB respelling (namespace, CActionEventBase base, virtual UAE leaf, non-const CCreatureActionBase&), smart-pointer effects def, inline IsAttachmentValid helper, inline wrappers, an effect copy (=); `bool found = GetSubDef && GetEffectFromString` puts the rotation exactly right (13 lines) but VC7.1 then tail-merges the two release epilogues (520v547); two early returns (582); single exit (510) | next: a form that adds the bool temporary without merging the failure release into the end release
- 00589a30 CTCInventory::OpenPCMapMenu DIFFER(549v549): after the inline strcmp's `repe cmpsb`, retail reloads esi (this) before edi (the index) | tried: the earlier sweeps' ~20 operator== / strcmp forms and the real names; this chunk the index declared at three positions (=) | next: lane gap: reload order after repe cmpsb
- 004a34f0 CWorld::AutoLoadRegionLeaving DIFFER(564v564): instructions identical; frame slots swap (retail heroState/fullPath share esp+8 and path takes +0Ch) | tried: path as a const reference, an inline name helper, direct-init heroState, the dead loadNow branches removed, LoadGameState defined after the caller (all =) | next: lane gap: frame-slot packing of the CWideString temporaries

## Skipped
None.

## Label corrections
- 0x00C11D00: the chunk/oracle label `CTCHero::CTCHero::Initialize` is wrong. It is the libpk CSoundEventManager constructor:
  it stores vtable 0x012B2748, the landed ~CSoundEventManager 0x00C13470 restores that vtable, and the four parts it news are
  the four that destructor deletes. The caller 0x00C094A0's binding `??0CSteeringSolver@@...` is also stale (NAME_FIXES.tsv).
- 0x00910DF0: the oracle name column holds a mangled name (`CAIStateGroup_MinionGuardHeardYellForHelp::?Update@...`).
- 0x00871A80: the module is `NCreatureAction::CActionEventTriggerParticle` (PDB namespace), not `CActionEventTriggerParticle`.
- Catalog labels at callees this chunk used, all propagated across byte-identical (ICF-folded) bodies and wrong for these call sites:
  0x00686A70 "GetDisplayEngine" (is CTCBase::GetWorld), 0x0049D870 "GFGetPreMainMemoryUsed" (is CWorld::GetFrame),
  0x00686D90 "_Cons_val<...>" (is CTCBase::GetThingSearchTools / PeekThingSearchTools: identical bodies, folded),
  0x004C73D0 "DoSizeof" (CThing::GetPos), 0x007F5810 "AssignCreatureToTask" (CTCTargetingBase::SetPNearestInterestingThing),
  0x004AE8C0 is CWorld::PeekWorldMap (CWorld vtable 0x012390F0 slot 13).
- 0x00C222D0: catalog label 'NScript_CEvent_Ctor' (vland RELOC_NAME warning) is the libpk CSoundEventSet(float, float, float)
  constructor; 0x00C11D00 builds its +0x2C member with it, and the landed ~CSoundEventManager destroys that member (0x00C22520).

## Header debt
- 007f5ae0: local polymorphic CTCBase / CTCTargetingBase. engine/CTCBase.h stores the vptr as a data member, so it cannot be a
  base for a class with virtual functions, and the leaf is virtual (MAE). CThing / C2DBoxI / filters are local too.
- 00871a80: everything local (CThing view, CDefPointer-like effects pointer though engine/CDefPointer.h exists, CActionEventBase).
- 007d5070: local CTCTavernGame / CTCTavernGameCardBase / CTCTavernGameCardSorting (no headers); it uses the shared engine/CDefPointer.h.
- 0077c960 (CTCHighlightItem.h), 00589a30 (CTCInventory.h), 004a34f0 (CWorld.h): the parked sources still use local models next
  to an existing engine header.
- 00c11d00: CSoundEventManager and its parts are local (libpk, no header). 009718f0 uses engine/CAIStateGroup_JackOfBladesBase.h
  and engine/CBaseObject.h (FABLE_RETAIL_BASE_OBJECT, which clears vland's SHARED_HEADER_AVAILABLE warning); its CThing /
  intelligent-pointer view is local.

## Names
- Proven (two sources: PDB spelling plus retail evidence such as vtable slot, ret size or call site):
  - 009718f0 leaf (FableWin + Ego_r)
  - 007d5070 leaf and callees (GetDef, PlaySoundW, SetStateAfterOK, SetState, GetCardDef)
  - 007f5ae0 leaf and callees (GetWorld, GetFrame, PeekWorldMap, IsThingInFOV, GetNavigationLayerAt, ContainsPoint,
    SetPNearestInterestingThing, filter ctors)
  - 00871a80 leaf
  - 0077c960 leaf
  - the earlier sweeps' leaves for 00910df0 / 0070d580 / 00589a30 / 004a34f0
- Probable: 00c11d00 `CSoundEventManager` (landed libpk class name; no PDB covers libpk) and its parameter types.
- Descriptive:
  - 00c11d00's CVec3, CVtable_012b4030, CVtable_012b401c and member names
  - 007f5ae0's g_NearestThingUpdatePeriod and m_type / m_flags91
  - 00871a80's CreateEffectThing and CTCDParticleEmitter methods
- 007f5ae0: `CThing::UniqueID` is Ego_r `ulong64`, but retail divides it signed (__allrem), so the model keeps a signed __int64 with a note.
