# Chunk 108 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Def `operator=` rows labelled "CThingBuildingDef::operator=" are the CDefClassBase (0x00431F10) assignments of MANY def
  classes: identify them by (a) the incremental-link `Copy` thunk in front (`e9 00000000` falling into the body; its manifest
  name is the class's real `Copy` vtable slot) and (b) matching the copied member signature against Ego_r.pdb with retail
  vectors and maps 4 bytes narrower (16 -> 12) (004e12b8 CFishDef, 004e5839 CFlammableDef, 004e4d88 CExperienceDef).
- A member record whose base has its own out-of-line `operator=` needs a written-out `__forceinline operator=` inside the
  record (`CCombatAbilityStrafingData`, `CFireEffectCreationSet`): the implicit one is out-lined by the DDK; inlined, it keeps
  the record's this/other in ebx/edi (`add edi,28h ; lea ebx,[esi+28h]`) like retail (00453f63, 00453f93, 004e5839). An uncopied
  dword in the middle of the copy is the inlined record's vptr.
- A member object modelled as `unsigned char _body[N]` aligns to 1 and shifts the members after it (00463ba2: CSoundMap
  at +0x28 pushed SoundIdentifier to +0x45): give opaque member records a pointer/long member so they align to 4.
- `GetWorld()->GetWorldFrame()` with a STATIC GetWorldFrame keeps the `call GetWorld` (object expression evaluated, result
  unused) before the static call (006619f0, landed 006929e0); a stored frame loaded BETWEEN the two calls is the argument of
  an inline CWorld helper on a NAMED world local (`CWorld* world = GetWorld(); world->GetFramesSince(WF)`), the chained form
  loads it before GetWorld (006624a0).
- Two float fields copied through edx/eax into an 8-byte frame record before any call = a by-value `CFloatRange` local
  (`CFloatRange r = def->Range; return GFFloatRandom(r.Max - r.Min, seed) + r.Min;`) (00662040, 006620a0).
- A member pointer null-tested and then used as `this` of a virtual call loads straight into ecx only as a NAMED local
  (00661ef0, 0082d000); a later re-read of the member after the call is written through the member.
- `a != end()` after an out-of-line find re-computes end() in retail: in one expression the lane CSEs the argument's end;
  split it (`T* found = find_if(...); return found != end();`) and call through an inline SGI `find_if(first, last, pred)`
  front end so the iterators are evaluated before the stack arguments (00870980). The empty predicate is value-initialised
  (`mov byte ptr [..],0` then its dword pushed), the const-ref category tag has a user constructor (frame byte untouched).
- A `push imm32` whose value lies outside the image (0x01AB6CCE > 0x0146B47C) is a constant (stream end marker), not a
  relocated address (008708f0); `0x00993EB0` writes 4 bytes (WriteLong), its "WriteCBYTE" label is wrong.
- An out-of-line copy constructor copying every word through ONE register (edx) in member order = an explicit memberwise
  copy constructor; a member stored first in retail-order terms belongs in the body if it comes LAST (the initialiser list
  stores it first) (007f9730).
- `fcomp ; test ah,5 ; jp` with `mov al,1`/`xor al,al` exits = `if (GetValue(a) > GetValue(b)) return true; return false;`
  (left operand evaluated first and parked in the dead argument slot) (007f9f00, scheduling of `pop ebx` aside).
- CDefPointer getters: a raw-pointer inline `GetDef()` reproduces retail's "release BEFORE the field read" order, but the lane
  loads the def pointer into eax and turns `--m_refCount` into load/dec/store; with an extra dead `def.Object = 0` store the
  lane loads straight into esi and emits retail's `dec dword ptr [esi+4] ; jne` (0082d870: 74v66, the store is the only diff).

Review at landing:
- Lane gap rows (8 parked): the CDefPointer getter family 0082d6b0 / 0082d7d0 / 0082d870 / 0082d8c0 and the by-value def
  returns 0083c1e0 / 006c5630 should be re-graded on the RTM toolset (wave-24k "CDefPointer local cached in esi" gap);
  007f9f00 is epilogue scheduling only; 006621c0 used all 4 attempts (68v68, block layout) -- one more form is listed in NOTES.
- Local models next to existing engine headers: CThingCreatureBase.h (PDB offsets, retail is 0x1c lower around +0xe0..+0x100
  and the handedness pair sits at +0x138/+0x13c), CCombatAbility*Data/*Def.h and CSmashableDef-style def headers (flattened
  bases, no base operator= to call) -- all 15 def assignments and 7 creature rows use local models for that reason.
- Already-landed: rebuild/src/compiled/00/43/CThingBuildingDef_operator_00431f10.cpp is an `_emit` bake of what is really
  CDefClassBase::operator= (bitfield copy of +0x24); rebuild/src/compiled/00/47/CTCInGameMenu_PeekGuiDef_0047d5a3.cpp is a naked
  asm bake; rebuild/src/compiled/00/65/CTCVillage_IsCookTime_00658a35.cpp models a counted pointer with raw vtable casts.
- 006c5630's owner class is unresolved (component reached through interface type 0x51, ushort def-index table at +0x2e,
  called by CTCHeroStats / CTCVillage / CTCCreatureOpinionOfHero / CTCHeroOpinionDeedLog deed code).
