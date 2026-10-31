# Hand chunk 626 (CTCTrapBase .. CInputProcessControlCreatureActivateZTarget): 3 of 10 byte-exact (+2 extras)

Chunk 626 is a retry chunk: 10 parked same-length rows. 3 now RELOCATION_MATCH, 7 parked again with
same-length residue. The two extra rows (already byte-exact) got name checks and behaviour tests, so 5 sources sit in src/.
finish.py reads `$S/w22/chunk626.tsv`, which has no rows for the extras. The local finish.py now reads `chunk626_plus.tsv`
(a copy of that file with the two extra rows appended; their bytes come from `$S/batch/chunk622_oracle.tsv` /
`retry1_oracle.tsv`; the original script is kept as finish.py.orig). oracle.tsv/payload.json cover all 5 sources.
vland (read-only) gave all 5 RELOCATION_MATCH + behaviour PASS. It printed one RELOC_CONFLICT, now fixed (see 0x00686D90 below).

## What matched (5)
- **CTCTrapBase::UpdateTriggerThings 0x007881AD** (catalog "CTCCombatBase_UpdateEngageable"; Ego_r `IAE`, protected). The
  owner position goes through a `C2DCoordI((long)pos.X, (long)pos.Y)` TEMPORARY written inside the GetAllThingsInRoughArea
  call. VC7.1 builds it before the outer argument pushes, X first, and moves the X store past the Y `fld`, the same shape as the
  landed IsThemeHidable 0x00749D20. Any named C2DCoordI stores X right after `__ftol2`. The (long,long) named form even
  truncates Y first. Real STLport `std::vector` template (erase/clear). The search template symbol equals the Ego_r donor
  spelling.
- **MicroThread::SwitchToMicroThread 0x009D87F0.** The active-thread global is a TU-STATIC whose address is never taken.
  VC7.1 then knows that its store cannot alias the vector reads, so `ActiveThread = info;` written first keeps start/finish in
  registers and moves the store below the memmove pushes. The saved-stack-pointer global must stay extern, because retail
  re-reads after it. RestoreStack/PreserveStack are the PDB's unmangled TU-statics (inline here). Retail also keeps an
  out-of-line PreserveStack at 0x009D87B0, with the vector in edi.
- **CCreatureActionBase::UpdateActionMovement 0x00692AA0** (Ego_r `IAE`). The virtual slots come from the retail vtable
  0x01259D24 (read from the image) plus the Ego_r donor order: slots 57/58/59 = IsUsingAnimationMovement/IsUsingForcedMovement/
  IsMovementUsingPhysics (all bool const), slot 60 = UpdateActionMoveToPos, and physics slot 34 = SetForcedToMovePos(const
  C3DVector&, bool). Passing the BOOL slot-59 result fixed the old residue (`add esi,68h ; mov ecx,edi` before both
  pushes). The inline FableWin accessor CThing::GetPhysicsTC() puts the object before SetAsMoving's `push 1`. The creature
  flag is the PDB bitfield UseMovementInActions (+0x15d bit 0; PDB +0x185).
- **std::__uninitialized_fill_n<CIntelligentPointer<CThingAICreature>*> 0x007AF940** (extra): the chunk 622/624 source as is;
  callee spellings checked against Ego_r (GetPItem QBEPAVCBaseObject, SetPItem); test added.
- **CGraphicDataBank::UpdateAvailableMemory 0x009FD3A0** (extra). It was already private, but its `// Leaf:` named
  `?FrameEnd@CGraphicDataBank@@QAEXXZ`, a symbol the TU does not have. It now names
  `?UpdateAvailableMemory@CGraphicDataBank@@AAEXXZ` (Ego_r donor 0x00B56B40), and line 1 / the header were rewritten.
  Still RELOCATION_MATCH; test added.

## Parked for Fable 5.1 (7)
- 008b9dd0 CCreatureAction_CastPhysicalShieldSpell::Initialise DIFFER(125v125): retail this=edi / &PCreature=esi, ours this=esi / &PCreature=edi (5 register operands) | tried: CIntelligentPointer<CThingCreatureBase> operator-> + PDB HasTC/AddTC/CClass::Type model (the old named-PCreature-ref, HasTC member, this copy, scope, early-return forms) | next: lane gap: register tie-break (same family as 00954740); maybe a different inline route for the 3 GetPItem uses
- 009fd700 CGraphicInfo::CGraphicInfo DIFFER(126v126): `push ebp ; mov ebp,[esp+10h]` and `push edi` scheduled earlier than retail (after mov ebx,ecx / before the bit-field store) | tried: all-body header ctor (fixes push ebp, breaks the header store order), init-list shorts + body, landed 0x009FD6C0 init list, member-based call args | next: header ctor with only the shorts + PixelFormat in the list and the bit-field first in the body; else lane gap: prologue push scheduling
- 0095d4c0 CAIStateGroup_RangedCombatUsePreferredFirePoint::GetFirePointList DIFFER(130v130): `lea ecx,[esp+34h]` (filter operand homed in the dead `fire_points` slot) vs retail `lea ecx,[esp+6]` | tried: all-in-call (filter stored before GetPos), named C2DBoxF (this file), named preferred + named unary_negate (frame 0x30), named preferred only | next: three named one-byte locals `CThingFilter_IsOccupiableOccupied occupied; std::unary_negate<CThingFilter_IsOccupiableOccupied> unoccupied(occupied); CThingFilter_IsPreferredFirePoint preferred;` then `GFFilter_Bind2(preferred, unoccupied)` (retail frame has four byte slots +4..+7, +6/+7 bound). Not graded: 4-attempt cap.
- 00c2cbd0 CEventScript::CEventScript (libpk; catalog CQ_GuildTrainingPreMeleeScript_Initialize) DIFFER(132v132): owner load `mov eax,[esp+14h]` before `push esi`; retail loads it after the EH `this` home store | tried: owner by const reference to the root ctor (+ earlier base body-assign, optimize(s)) | next: lane gap: argument-load scheduling
- 008e8ff0 NCreatureMode::GetStrafingAnimationName DIFFER(135v135): `mov eax,[ecx]` (physics vtable) placed before the call, where retail puts it right after the first `fld` | tried: in-call C3DVector temp + in-call facing (131), facing declared before the delta (155), implicit copy ctor, PDB accessors GetPhysicsTC()/PeekPlayer (this file) | next: lane gap: x87/integer interleave scheduling
- 00954740 CAIStateGroup_MazeShieldedChase::IsStillRunnable DIFFER(136v136): this/&PTargetThing esi-edi swap (retail shares esi between &PTargetThing and the interface map) | tried: one-ternary GetInterface (that part is now identical), landed 0x0095A420 IS_THING_ALIVE helper + split ifs, unnamed map (134) | next: lane gap: register tie-break
- 00688260 CInputProcessControlCreatureActivateZTarget::ProcessInput DIFFER(137v137): `mov byte [esp+31h],0 ; mov byte [esp+33h],0` after the push, where retail does `xor al,al ; mov [esp+2dh],al ; mov [esp+2fh],al` before it | tried: landed 006882F0 forceinline AddGameEvent(long), early return + PDB names | next: the init-list ctor MATCHES the retail out-of-line copy 0x004159B0 (shared xor cl,cl), so only the inlined copy differs: try the event built through a non-forceinline member of CInputProcessGameBase, else lane gap: zero-register reuse after inlining

## Skipped
None.

## Label corrections
- 0x007881AD "CTCCombatBase_UpdateEngageable" -> CTCTrapBase::UpdateTriggerThings (Ego_r IAE; landed caller FrameUpdate 0x007888B4).
- 0x009D87F0 "global::SwitchToMicroThread" -> MicroThread::SwitchToMicroThread (Ego_r/FableWin YI).
- 0x009D87B0 (no row) = MicroThread::PreserveStack, a TU-static (PDB unmangled name; same bytes as the second half of 0x009D87F0).
- 0x008E8FF0 "global::GetStrafingAnimationName" -> NCreatureMode::GetStrafingAnimationName(const CThing&, EMovementType).
- 0x00C2CBD0 "CQ_GuildTrainingPreMeleeScript_Initialize" -> the libpk CEventScript constructor. It is not a quest script: its
  members match the landed destructor 0x00C2CC60 and the CFlagOverride ctor 0x00C2C6B0. No Aeon port or NScript sibling
  applies. The names are descriptive.
- 0x0095D4C0 takes `std::vector<CThing*>&`, not a list (Ego_r/FableWin).
- 0x00686D90 = CTCBase::PeekThingSearchTools() const; 0x00686D80 = GetThingSearchTools().
- 0x00445B80 = CGamePlayerInterface::IsToggleZTarget() const. 0x009FD6C0 = CGraphicHeader() and 0x009FD780 = CGraphicInfo(),
  both in the same TU as 0x009FD700.
- More callee respellings are in NAME_FIXES.tsv.

## Names: proven vs descriptive
- Proven (PDB plus a second source): every row function name above; the callees listed in NAME_FIXES.tsv; C2DBoxI(const C2DCoordI&, long)
  (FableWin); CGraphicDataBank::GetTextureType / CBankFile::GetEntryDataType (Ego_r); MicroThreadSleep (Ego_r).
- Descriptive: the three MicroThread globals (ActiveThread, SavedStackPointer, StackTop: the name sets carry no data symbols);
  the whole libpk 0x00C2CBD0 model (CEventScript, CScriptRoot, CEventContainer, CFlagOverride: no PDB covers libpk); the
  30.0f fire-point half-size.

## Header debt
- 0x007881AD: engine/CTCTrapBase.h has Ego_r (16-byte vector) offsets, so this uses a local model (the landed 0x007887BE does
  the same). vland warns SHARED_HEADER_AVAILABLE for CTCBase/CThingSearchTools (method-only stand-ins).
- 0x00692AA0: vland warns SHARED_HEADER_AVAILABLE CTCPhysicsBase (local class carrying the slot-34 virtual).
- 0x009FD3A0 (extra, unchanged model): warns for CTextureManager / CCountedPointer.
