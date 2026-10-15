# Hand chunk 107 (CThing .. CThingBaseDef): 38 of 40 byte-exact

Retail CThing layout used throughout (no engine header exists; PDB offsets are 0x10 higher from the CVectorMap on):
vptr +0, class byte +0x10, UniqueID +0x18 (__int64), UsingComponents bitset<274> +0x20, Components {type, TC*}
vector +0x44/+0x48, UpdateComponentsList (4-byte STLport list) +0x54, PTCDriver +0x5c, PhysicsTC +0x60,
PTCGraphicAppearance +0x64, PBaseDef +0x70, DefGlobalIndex +0x8c, CurrentMapNumber +0x8e, PlayerNumber +0x90,
flag bytes +0x91/+0x92/+0x93 (PDB bit order), InitialisationStage:3 +0x94. CTCBase::ToBeRemoved is +8.

## What matched (38)
- CThing component loops (004c7470 ValidateThing, 004c7500 IsFreeForDeletion, 004c77d0 OnLevelUnload, 004c7820 SendMessageW):
  plain `for (pair* it = first; it != last; ++it) { CTCBase* tc = it->second; if (!tc->ToBeRemoved ...) }` with
  the last pointer read as a member each iteration; AddingTCsPermitted / LevelIsUnloading bitfield stores merge into
  one load/and/or/store before the loop and an `or byte ptr` after it.
- CThing::InitTC (004c73e0): if/else chain on the 3-bit InitialisationStage read once into a local (a switch would `dec`).
- CThing::DrawIsDrawable / vfunc_29 (004c7790, 004c77b0): `return Drawable && Activated && !Dead;` (full-register 0/1).
- CThing::vfunc_17 (004c7b80): early `return false` (no def) + one `&&` chain; `!Dead` must go through an inline
  `bool IsAlive() const { return !Dead; }` to get retail's `mov cl,al ; not cl ; test cl,1` (the direct bit test
  merges Dead|InLimbo into `test cl,21h`).
- CThing::IsRenderedThisFrame / IsFrameUpdated (004c78f0, 004c7bf0): the appearance query is an inline returning
  LONG (`if (!reasons && resendBit) return 1; return reasons;` -> `movzx eax,cl ; test eax,eax`); IsFrameUpdated is one
  `||` chain with IsRenderedThisFrame inlined.
- CThing::DrawPreparePrimitivesForRendering (004c7540): `if (InLimbo || !DrawIsDrawable()) return false;` and the driver
  fetched through an inline accessor into a condition local so it lands straight in ecx.
- CThing::DrawGetEditDrawInfo (004c74b0): the CRGBColour written by an inline `Set(r, g, b, a)` storing R, G, B, A over the
  B,G,R,A layout (a constructed temporary goes through a frame dword); `Label = ""` is CCharString::operator=(const char*).
- CThing::SetCutscenePauseBehaviour (004c78a0): three-case switch on a long (`sub eax,0 ; je ; dec ; je ; dec ; jne`).
- CThing::SetForceFrameUpdate / SetToKillOnLevelUnload (004c7c60, 004c9c40): bitfield assignment then the bitfield
  re-read in the `if` (retail re-tests the stored byte); `KillOnLevelUnload != kill` compares bitfield with bool.
- CThing::GetDefName (004c7cc0): CDefString with user copy ctor/dtor passes the hidden result straight through.
- CThing::OnKill (004c7cf0): the landed OnDie (0x004C7D50) for_each/functor model plus the quest-manager call taking the
  LevelIsUnloading bit as a bool argument (`xor eax,eax ; mov al ; shr al,4 ; and eax,0FFFFFF01h` reproduced).
- CThing::CreateUID (004c7ff0): both bool arguments come from frame bytes: a conditionally-set `levelLess` local AND a
  NAMED `classFlag` local (the inline expression loads it with `movzx` into a register instead).
- CThing::GetPhysPos (004c7950): the C2DVector call result is consumed by an inline `operator+(const C2DVector&, const
  C3DVector&)` so it is read through the returned eax; a named or reference local re-reads it from the frame.
- CThing::FrameUpdate (004c9d20): `if (bt->IsActive() && !bt->IsTimeForFullUpdate() && IsAffectedByBulletTime())
  WorldUpdateBulletTime(); else WorldUpdate();` -- both virtual calls become tail jumps.
- CThing::WorldUpdateBulletTime (004c8ba0): the landed GetInterface out-parameter template (interface 0x46) followed by the
  STLport list walk `for (node = head->next; node != head; node = node->next)` with the head re-read as a member.
- CThing::GetTCDriver<...> (004acb50, 006ae5c0): raw bitset word test in the `if`; (007890ea): out-of-line
  `bitset::test(0xed)` with `#pragma optimize("s", on)` (shared `pop esi ; ret 4` tail).
- CThing::GetPBaseDef (007334b0, 0083af60): `return m_def;` through a CDefPointer with an inline add-ref copy constructor.
- CThing::SetAsGameStatePersisted (0078ddb0): one-bit unsigned char bitfield store (bit 5 of +0x15c).
- CThing::AddToRenderList (008e5fb0): `Velocity *= 1.0f - GetAirResistance();` with an inline C3DVector `*=` float.
- CThing::OnDie (00991750): flag cleared then virtual, then a 36-byte-stride vector loop calling slot 2.
- CThing::AddToRenderList (00c597e0, 00c59820): `if (IsValid()) return g->Forward(a, b, c, m_handle); return -9;`.
- CThingAICreature::GetTargetPosition (006a9520, 0082fe60): the wave-24o named-local C3DVector filled field by field
  (`fxch st(2)` + three fstp) with a memberwise user copy constructor.
- CThingAICreature::IsAvailableForInteractionType (00830320): one `&&` expression (full-register bool).
- CThingAICreature::vfunc_14 (008337b0): GetInterface template had to be `__forceinline` (VC7.1 emitted it out of line
  inside the if-arm).
- CThingAICreatureInit::CThingAICreatureInit (004facd0): plain init list in declaration order with the out-of-line
  CCharString copy constructor (0x0099EC30) modelled locally.
- CThingAnimatable::SetSuspendAnimation (00833a10): `if (s) set.InsertUnique(p); else set.Erase(p); AISuspended = !set.empty();`
  with the key bound to the argument slot by const reference.
- CThingBaseDef::operator= (00464536): explicit memberwise operator= (base operator= first) under
  `#pragma optimize("s", on)`; the 14-byte two-byte-aligned PODs then copy as `movsd x3 ; movsw` blocks.

## Parked for Fable 5.1 (2)
- 004c8c90 CThing::IsUnderScriptControl DIFFER(83v81): `mov eax,[eax+4] ; mov cl,[eax+18h] ; ... mov ecx,eax` vs retail `mov ecx,[eax+4] ; mov al,[ecx+18h]` (component pointer register) | tried: GetInterface `!`-form, HasTC + pointer-returning GetTC, inline component member `IsUnderControl()`, `if (GetInterface) return ...; return false;` (fixed block order) | next: a const-qualified GetInterface / a different CVectorMap instance (retail calls LowerBound 0x004365B0, not 0x0040F020), or `CTCScriptControl* c = 0;` initialised before the lookup
- 00830260 CThingAICreature::RandomiseMovementSpeed DIFFER(84v84): `mov [esp+4],eax` (seed store) scheduled before `fld [ecx+0B0h]`, retail stores it after `push ecx ; lea ecx,[esp+8]` | tried: seed local then call, range local first, seed wrapped in an inline CRandomSeed object | next: lane gap: scheduling of a local's initial store relative to the outgoing-argument push

## Skipped
- none (no row spans two functions).

## Label corrections
- 006ae5c0 "CThing::GetTCDriver<CTCDExperienceOrb>": propagated from 0x004ACB50; tests interface bit 105 (word 3 mask 0x200), so a different driver type.
- 007334b0 / 0083af60 "CThing::GetPBaseDef": def pointer at +0x48 / +0x4c, the retail CThing's PBaseDef is +0x70: other def-owning classes.
- 0078ddb0 "CThing::SetAsGameStatePersisted": writes bit 5 of +0x15c (CThing's is +0x92 bit 5): a derived class's bitfield.
- 008e5fb0 "CThing::AddToRenderList": calls CTCPhysicsBase::GetAirResistance on `this` and scales the velocity at +0x28: a physics component method.
- 00991750 "CThing::OnDie": vector of 36-byte entries at +0x30 and a flag at +0x24: not CThing.
- 00c597e0 / 00c59820 "CThing::AddToRenderList": 0x00C5xxxx no-RTTI module, referenced from five vtables each (0x012B5C4C.. / 0x012B5C50..): wrapper methods forwarding to the service at 0x0143BB70.
- 00833a10 "CThingAnimatable::SetSuspendAnimation": operates on CThingAICreature's Suspenders set / AISuspended bit (+0x1cc / +0x1c8, cf. landed 0x00833BA0).
- 00464536 "CThingBaseDef::operator=": the PDB CThingBaseDef is 0x74 bytes and its operator= is the callee 0x00463950; this 0x94-byte body is a derived def's assignment.
- Callee labels: 0x004CB9F0 "_Copy_backward<CPixelFormat*>" is for_each<..., CCallOnKill>; 0x006A4D00 "CVertexBufferWin32::DoSizeof" is a CThing position accessor returning const C3DVector&.
