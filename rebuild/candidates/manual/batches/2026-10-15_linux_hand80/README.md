# Hand chunk 80 (CTCActionUseBuyHouse .. CTCAssassinRush): 40 of 40 byte-exact

Authored in the Linux Wine lane (DDK 13.10.4035 cl.exe), graded with `q.py` (all RELOCATION_MATCH, default flags or
an in-source `#pragma optimize("s", on)`), behaviour-tested with g++ and with the lane's VC7.1 cl.exe (40/40 PASS each).
Class names, members and signatures come from Ego_r.pdb (`ghidra_out/egor_pdb_names.tsv`, `struct_layouts_egor.tsv`) and
rtti-vtables.tsv. Where they disagree with the retail bytes, the retail offsets win: 12-byte vectors and CArrays, and a
CThing that is 0x10 bytes narrower before +0x91.

## What matched (40)
- **Lock-guarded flag setters** (00c11180 / 00c111c0, label "CTCActionUseBuyHouse::OnCreate"): `CScopedLock lock(&g_0143bb14); m_bitN = 1;`. The load / `or eax,N` / store split around the destructor's `lea ecx` is a one-bit BITFIELD assignment; `m_flags |= N` gives a read-modify-write `or [mem],N`.
- **CTCActionUseBuyOrSteal**
  - ctor 0072a630: the CTCActionUseBase constructor is inlined. Its own vtable store is dead and only its `PActionUseDef = 0` / `Usable = true` stores remain ahead of the derived vtable. Then the CIntelligentPointer(0) member and the bool, with `AddToGlobalTCs()` as the body.
  - dtor 0072a680: `RemoveFromGlobalTCs()` is the body. CIntelligentPointer and CTCActionUseBase have IMPLICIT destructors, which store no vtable. The def release reads the pointer into a local.
  - ??_G 0072a730: the same body inlined into `??_G`. The implicit base destructor is called out of line from inside a `__forceinline` destructor. The fix: an explicit `__forceinline ~CTCActionUseBase() {}`, with `__declspec(novtable)` on that base so it stores no vtable. A `new` factory plus an inline constructor emit the vtable and with it `??_G`.
- **CTCSummonSpell / CTCWillEnflame ??_G** (0079bdf0 / 007a50f0, label "CTCActionUseBuyOrSteal"): an empty `__forceinline virtual` destructor. The members die in reverse order. The 8-byte CIntelligentPointer is unaligned (+0x11, +0x21), reproduced with `#pragma pack(1)` on the IP classes.
- **CTCActionUseKickChicken::OnActionUse** (007ece20): `creature.SetCurrentAction(&CCombatAction_KickThingOnGround(&creature, GetThing()))`. The temporary is built in the call. The owner must come through an inline `GetThing()` accessor to land in ecx (a direct member read uses eax).
- **CTCActionUsePutInInventory::OnActionUse** (007eb390): the base call first. `&user` is null-tested, then the `Dead` bit 0 at +0x91 and the thing type 5. The 0xb0-byte action temporary has an implicit destructor (IP member, then the base destructor).
- **CTCActionUseReadable**
  - ctor 004da9d7 (/Os): both members follow the vtable store.
  - OnSerialise 007e8160: `Transfer("GameTextDefName", TextTag, EmptyWideString())`. The two frame strings come from an inline helper that returns a NAMED local by value (no NRVO): it is constructed, copied, and destroyed before the call.
- **CTCActionUseScriptedHook::GetRegionEntranceName** (00729c50): the first world-map fetch, and its vtable, precede the argument computation, so it is a named local. The two outer fetches are inline in the expression.
- **CThingComponentSet::Add** (00870750, label "SetEntranceAsLocked"): registry lookup (a `__fastcall` static), then `Entries.push_back(CEntry(info->m_name, info->m_type, flag))`. CEntry is a packed 9-byte record with an inline constructor and sits at frame offset 3.
- **Stream callback** (00c497e0, label "SetEntranceAsLocked"): a free `__stdcall (void*, void*, CFrameStreamer*)` registered by `push offset` at 0x00C49A2F. `SFrame* frame = Pop()` is address-taken, so it is homed in the dead third-argument slot.
- **CTCActionUseTorch::OnCreate** (007eaf20): `TorchLit = !((flags >> 8) & 1)` on an `unsigned long` virtual return. A struct-of-bitfields return gives the same bit code, but through a hidden pointer.
- **CTCActivationReceptorBase**
  - ctor 007f7710: local retail model, because engine/CTCActivationReceptorBase.h has the 16-byte PDB vector. The plain bool init comes before the vtable store; the vector member's inline constructor and the later inits come after it.
  - OnKill 007f7850 (label "CreatureGenerator vfunc_14"): `for_each(begin, end, CInformTriggerOfReceptorDeath(this))` through the `__fastcall` `_For_each`, which returns the functor by hidden pointer in ecx.
- **Mailbox receiver** (00c38ac0, label "AddTrigger"): `SMailMessage m; m_queue.DispatchMessage(&m); m_174 = m.p1; m_178 = m.p2; return m.id;`.
- **CTCActivationReceptorChest::Activate / Deactivate** (007ffc70 / 007ffcd0) and **CTCActivationReceptorCreatureGenerator::Activate** (007f7650): the landed out-parameter `GetInterface(&component, type)` template. The pointer is left uninitialised, so retail's garbage read on the absent path is reproduced. 0x3c is the chest, 0x3d the creature generator (`ExternalTrigger` at +0x4f, or `SetAsActive(true)`).
- **CTCActivationReceptorCreatureGenerator**
  - Validate 007f75c0: `(A && !T) || (!A && T)` gives retail's redundant `test al,al ; jne` and the re-read of T.
  - OnSerialise 007f7600: base call, then two `Transfer(name, bool&, false)` calls. The literal `const bool&` default is homed in the dead `loading` slot.
- **CTCPhysicsStandard ctor** (00723fd0, label "CTCActivationTrigger"): all sixteen `bool:1` flags are initialised in the init list. Each byte's eight constant bit stores fold into one byte store (`mov al,4`, shared by both bytes).
- **CTCContainerRewardHero ctor** (0074e6b0, label "CTCActivationTrigger"): the CTCContainer constructor is inlined (dead vtable store, the vector zeroed), then the two bools, then the vtable.
- **CCombatAbilityMeleeAttackData ctor** (0044c47b, /Os, label "CTCAmbientBird") and **CTCAmbientBird ctor** (004d5efb, /Os): a float zero `fldz ; fstp` comes before the vtable store, the bytes after it.
- **CTCAmbientBird::OnInitialActivate** (007e1f70): `if (!GetSubsystem()->IsSuppressed()) if (GFFloatRandom(1.0f, GetWorld()->GetPRandomSeed()) > 0.5f) ParentThing->Kill(false);`.
- **CTCAnimatingObject**: the GetInterface(0x5a) template on the CTCAnimationComplex.
  - SetCurrentAnimAsManualUpdate 007f8d00: `UpdateMode = 0`.
  - SetFractionComplete 007f8d60: the old fraction goes through the x87. It is a NAMED float local, and the new value is stored first in the source.
  - PlayAnimation 007f8dc0: the animator is a named local, so `GetAnimation()` precedes the SetMainAnim pushes.
- **CThingCreatureBase::GetAnimMovementDistance / GetAnimDuration** (006627a0 / 006627f0, label "CTCAnimationComplex::GetAnimDuration") and **CTCAnimationComplex::GetAnimDuration** (0070b4a0):
  - The bank pointer is a named local.
  - The magnitude is an inline `Magnitude(float x, float y, float z)` taking the components BY VALUE, called from `C3DVector::Length()`. This reproduces the Z, Y, X x87 loads and the register squaring.
- **CTCAnimationComplex ctor** (0070b3f0): `PAnimation(new CAnimateThing(owner))`, with two tails for the `new` null test.
- **CTCAppearanceMorphBase::UploadTextureMorphsToEngine** (006ca7f0): `const CEngineTextureMorphSet& morphs = GetMeshTextureMorphs()->OriginalMeshTextureMorphs;` (attempt 3). A pointer local gives `add eax,0Ch` one push later; an inline call pushes everything first.
- **Destructors labelled "CTCAssassinRush::FinishMove"**
  - 00443220: a `novtable` derived class with an empty destructor. An inline list destructor (`clear(); if (head) free(head);`), an out-of-line list destructor, and a base `jmp`.
  - 0047747c (/Os): the same pattern, with a `__forceinline` vector destructor.
  - 004ad650: a NON-polymorphic `??_G` emitted through a `delete` helper.
- **CTCAssassinRush::SetThingCollisionPtr** (004cbfb0): `if (!StartTrailDelay) CollidedThing = thing;`. This runs through the converting CIntelligentPointer constructor and an inline `operator=`; CollidedThing is unaligned at +0x39 (`pack(1)`).

## Parked for Fable 5.1 (0)
(none)

## Skipped
(none)

## Label corrections
Each chunk label below is propagated; the identity on the right comes from the bytes, Ego_r.pdb and RTTI.

| Address | Chunk label | Identity |
|---|---|---|
| 00c11180 / 00c111c0 | CTCActionUseBuyHouse::OnCreate | lock-guarded bitfield setters on the +0x7c8 word (same family as 0x00C10DD0) |
| 0079bdf0 | CTCActionUseBuyOrSteal::`vector_deleting_destructor' | CTCSummonSpell scalar deleting destructor (vtable 0x0126C2B4) |
| 007a50f0 | CTCActionUseBuyOrSteal::`vector_deleting_destructor' | CTCWillEnflame scalar deleting destructor (vtable 0x0126C824) |
| 0072a730 | `vector_deleting_destructor' | the scalar form |
| 00870750 | CTCActionUseScriptedHook::SetEntranceAsLocked | CThingComponentSet::Add(const CCharString&, bool) |
| 00c497e0 | CTCActionUseScriptedHook::SetEntranceAsLocked | `__stdcall` audio stream callback |
| 00c38ac0 | CTCActivationReceptorBase::AddTrigger | mailbox message receiver |
| 007f7850 | CTCActivationReceptorCreatureGenerator::vfunc_14 | CTCActivationReceptorBase::OnKill |
| 00723fd0 | CTCActivationTrigger::CTCActivationTrigger | CTCPhysicsStandard::CTCPhysicsStandard |
| 0074e6b0 | CTCActivationTrigger::CTCActivationTrigger | CTCContainerRewardHero::CTCContainerRewardHero |
| 0044c47b | CTCAmbientBird::CTCAmbientBird | CCombatAbilityMeleeAttackData::CCombatAbilityMeleeAttackData |
| 006627a0 | CTCAnimationComplex::GetAnimDuration | CThingCreatureBase::GetAnimMovementDistance(long) const |
| 006627f0 | CTCAnimationComplex::GetAnimDuration | CThingCreatureBase::GetAnimDuration(long) const |
| 00443220 / 0047747c / 004ad650 | CTCAssassinRush::FinishMove | unidentified destructors, named by address |

The `vfunc_N` labels resolve, via the Ego_r.pdb slot names, to OnActionUse (28), Activate (28), Deactivate (29), Validate (17) and OnSerialise (1).

Callee labels also corrected along the way:

| Address | Manifest label | Identity |
|---|---|---|
| 0x00686ED0 | std::list::push_back | CTCBase::AddToGlobalTCs |
| 0x00686830 | ~CChunkedFileChunk | ~CTCBase |
| 0x0062E900 | ~CTCLightningOrb | the kick action's destructor |
| 0x00693EF0 | CPhysicsMeshInfo::~CPhysicsMeshInfo | ~CCreatureActionBase |
| 0x007EAB90 | $E2 | the empty CTCActionUseBase::OnActionUse |
| 0x009D5060 | CRegion::SetMiniMapRegionExitTextOffsetX | CDefString::CDefString(const CCharString&) |
| 0x00870480 | CThingComponentSet::Add | CSmallVector<CEntry,8>::push_back |
| 0x0070B460 | CWorld::DrawGetEnvironment | CTCAnimationComplex::GetAnimation |
| 0x004C73D0 | DoSizeof | CThing::GetPosition |
| 0x004C7AB0 / 0x00686A80 | CViewBase::GetDisplayEngine | CThing / CTCBase world accessors |

## Shared-header note
Several sources declare local `struct CTCBase {`, `CIEngine`, `CRegion`, `CThingCreatureBase`, etc. There are two reasons. The engine headers are flattened PDB PODs (`void* __vftable`, no virtuals, no member functions), and the CTCActivationReceptorBase, CRegion and CTCCreatureGenerator headers carry the PDB's wider vectors. Each source says which applies. Expect SHARED_HEADER_AVAILABLE warnings from `verify_and_land.py`; under LANDVERIFY_STRICT_HEADERS=1 they are rejections. The landed CTCBase-derived constructors (e.g. 0x004D61CA) use the same local CTCBase model.
