# Chunk 525 -- lessons

## GOTCHAS one-liners
- Explicit `CCharString("X")` temporary used as an argument: VC7.1 pushes the ctor's `eax`. An implicit `const char*` -> `const CCharString&` conversion recomputes `lea reg,[esp+N] ; push reg`. Retail CCombatActionBase callers (GetSoundPair / SetCombatAnimation) use the implicit form (0x008B8260/8310).
- C3DVector copy with an explicit user copy ctor `X(v.X), Y(v.Y), Z(v.Z)` stores to `[esi+off]` directly. The implicit copy ctor / operator= goes through `lea reg,[esi+off]` first. Match whichever retail shows (0x0087CDF0 init list = user copy ctor; NCameraModes reset copies = implicit operator=).
- C3DVector is declared with alignment 1 (`#pragma pack(1)`). Ego_r puts CCreatureAction_ProjectileCrouch::FacePos at +301 after a bool at +300 and keeps size 316, so FacePos is +0x115 in retail.
- Reading `PCreature->PhysicsTC` (+0x60) directly and calling a by-value-returning virtual gives `mov eax,[eax+60h] ; ... mov ecx,eax`. Going through the inline `CThing::GetPhysicsTC()` gives retail's `mov ecx,[eax+60h] ; mov eax,[ecx]` (0x0087CA30).
- Overloaded virtuals take their slots in reverse declaration order: CTCPhysicsBase::GetFacingVector() is slot 70 and GetFacingVector(float) is slot 69. Ego_r emits `()` first.
- An inline-function order run in FableWin.pdb (same class, adjacent addresses) maps one to one onto retail stub order. Two classes (player / AI creature) proved CThingCreatureBase::IsPlayerCharacter = slot 77.
- STLport `std::__find<T*, T>` random-access instances are `__fastcall` in this /Gr build (ecx first, edx last, `ret 8` = value ref + tag ref). The text of the landed `__find_if` 0x008C2F70 with `*__first == __val` gives a full MATCH (0x008AB5D0).
- Inlining CPhysicsState's ctor and Reset() on a member subobject makes VC7.1 keep `&LookAt` / `&GoTo` in ebp/edi. Retail addresses those members as `[esi+disp]`. A flattened ctor body matches everything but the final `mov bl,1` (0x0084E420, parked).

## Review at landing
- vland dry run: 19 wins, no RELOC_CONFLICT, no RELOC_DUPLICATE. Two RELOC_NAME warnings, both NAME_FIXES rows:
  - 0x006925C0 `?GetFramesRunning@CCreatureActionBase@@QBEJXZ`. Currently labelled `_global::GetMember_dword0x1c_006925c0`; landed as `__fastcall DrawGetEnvironment` in rebuild/src/compiled/00/69/global_DrawGetEnvironment_006925c0.cpp.
  - 0x00692910 `?PeekDefinitionManager@CCreatureActionBase@@IBEABVCGameDefinitionManager@@XZ`. Currently labelled `CCreatureActionBase::GetPDefinitionManager`; respell the 11 landed callers.
- 0x00692C10 GetWorldMap is a folded const/non-const pair: one address, two PDB spellings. This chunk binds the non-const `IAEAAV`. Landed rows spell it `CWorldMap* GetWorldMap() [const]`. The coordinator should pick one spelling for the address; the fold means both are honest.
- 0x00849E80 / 0x00849D90 are byte twins: PutItemIntoPocket / GetItemOutOfPocket, two names for two addresses. The catalog row for 0x00849E80 still says GetItemOutOfPocket.
- 0x008AB5D0: the landed caller 0x008A80F0 declares `__find` in the global namespace with a same-TU stand-in body. Respell it as the `std::` template instance.
- 0x0084E420 (DIFFER) is one register choice away. A Fable 5.1 retry should try a CBase-level inline helper that writes the two Reset blocks, or the CullPhysics flags from a shared bool local.
