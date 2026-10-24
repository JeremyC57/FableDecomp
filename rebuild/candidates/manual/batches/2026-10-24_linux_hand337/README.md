# Hand chunk 337 (CTCEntityEvents::GetEvent .. N3DPrimitive::CVertexShaderInputAnimated): 36 of 40 byte-exact

All 36 grade MATCH / RELOCATION_MATCH (gall.sh); every test prints `<addr>_TEST PASS` under g++ (tall.sh) and the
lane's VC7.1 (tall.py). The 22 GetEvent rows come from one generator (`gen_ev.py` + `rows_ev.py`, src and test),
the 9 thing-search rows from another (`gen_pb.py` + `rows_pb.py`, tests `gen_pb_test.py`), the 3 HasEvent tests
from `gen_he_test.py`. Pragmas: 0x0065F6DA `optimize("y", off)` + `optimize("s", on)`; 0x0065F152 `optimize("s", on)`;
no row needed a `--flags` switch.

## What matched (36)

- **CTCEntityEvents::GetEvent<TEvent, TFilter> (22 rows: 007de2e0, 008d4840, 0096bac0, 0096bb70, 008d4cd0, 008d48f0,
  008d49b0, 008d4b50, 008d4c10, 008d4a90, 007fce10, 00970190, 007afb90, 0094f890, 0095c680, 0095c720, 008d4ed0,
  005ed5a0, 008ac7a0, 00953000, 00953090, 008d4e30)**: walk Events (+0x0c, list of CCountedPointer<CEventBase>)
  newest first, copy each counted pointer, return the first event whose GetType (slot 1) is TEvent's and that passes
  the filter. Shape lessons:
  - the **STLport reverse_iterator** (`*it` = copy the base list iterator, `--` the copy, deref) is what gives retail's
    register choice in most TUs (node in edi, event in ebx, `this` in ebp, filter re-read from the stack). The landed
    hand-folded form (`current->_M_prev`) matched only 14 of the 22 rows; the STLport form matched 21. It also closes agent 336's
    three parked GetEvent rows: their differ/ sources with only the iterator swapped grade RELOCATION_MATCH
    (0x0095D920, 0x0096BC20, 0x0094E530; probes in probe/o336_<addr>.cpp). 0x007FCE10 alone still needs the
    folded form (it is per-row, set in rows_ev.py);
  - filters take `const CEventBase*` / `const CEventHitBase*` (Ego_r.pdb: `unary_function<const CEventHitBase*, bool>`);
    CFilter_Bind2/3/4 hold their filters by reference; the empty out-of-line filters are called with `ecx = &filter`;
  - inline filters: CIsEventCreatedAfter / BeforeOrOn (WorldFrame +4), CIsHitEventHitBy (GetPThingHitting 0x00810FC0 ==),
    CIsHitEventHitAndKnockedDownBy (+ IsKnockDown 0x00810F00), CEventFilter_None, the anonymous-namespace
    HitEventWithNoWeapon / HitEventWithWeapon (GetPWeapon 0x00810FD0, dead bit +0x91, DefGlobalIndex +0x8c),
    HitWithDecapitate (0x00810F10), HitWithFlourish (virtual IsFlourish, slot 5), HitThingWithFlourish (byte +0x13),
    `std::unary_negate<CIsHitEventBlocked>`; out of line: CIsHitEventHitByShot 0x00810F70, CIsHitEventHitByMelee
    0x00810F80, CIsHitEventBlocked 0x00810F50, CIsHitEventHasHitByThing 0x00810F90,
    CIsHitEventHitByThingWithScriptName 0x00811100.
- **CTCEntityEvents::HasEvent<TFilter> (008d4f80, 008e89d0, 00956650)**: the same newest-first walk reading the event
  straight from the node (`filter((*it).Data)`, no counted-pointer copy); CIsEventOfType, CIsEventOfTypeCreatedAfter
  (one functor: Type +0, WorldFrame +4), and Bind3<OfType, After, BeforeOrOn>. Plain MATCH (no relocations but GetType).
- **CThingSearchTools::GetPBestLeafThing / GetPBestThing (9 rows: 007f7040, 00936070, 00982450, 0048cb50, 0048cbd0,
  006cd3b0, 004ac2c0, 009650c0, 0065f6da)**: the landed 0x00936950 template bodies; filters take the thing by
  reference; `std::binder2nd<const_mem_fun1_ref_t>` / `const_mem_fun_ref_t` call through the member pointer; the
  inline `binary_compose<logical_or>` evaluates `_M_fn3` first (right-to-left arguments). Lessons: the CFilter_Bind2 /
  Bind4 `operator()` and the `binary_compose` `operator()` need `__forceinline` (004ac2c0 / 009650c0 otherwise call
  them out of line); in the size-optimised 0x0065F6DA (`IsIncludedInThingScans` called) `*it` goes in a named local
  for the scan call and the candidate, but the type test must read a fresh `*it`, else VC7.1 drops retail's
  `test ebx,ebx`.
- **IS_THING_ALIVE<CIntelligentPointer<NParticleEngine::CParticleEmitter> > (0079dc20)**: GetPItem twice, nested ifs;
  the emitter's Dead flag is an `unsigned short` bit field (bit 5 of +0x40) read through an inline `bool IsDead()`
  (`mov al ; shr al,5 ; test al,1`); the plain field read folds to `test byte ptr [eax+40h],20h`.
- **HasGuardProperty::operator() (0065f152)**: by-value CIntelligentPointer<CThingAICreature> (callee destroys it via
  0x00A01C10), `GetPItem()->GetAIProperty() == 3` (virtual slot 48, the name of the landed 0x006A75D0 filter),
  `sete bl` across the argument destructor; `optimize("s", on)`.

## Parked for Fable 5.1 (4)
- 00738a80 CTCEntityEvents::GetEvent<const CEventHitBy, CFilter_Bind3<CIsHitEventHitByMelee, unary_negate<CIsHitEventBlocked>, CIsHitEventHitBy> > DIFFER(177v177): register swap only -- retail keeps the count block in edi and the event in esi, ours esi/edi (every other instruction identical) | tried: STLport reverse_iterator (matches 21 siblings), hand-folded iterator (matches 0x007FCE10), copy ctor assigning Info before Data, `operator->()` for the filter argument | next: Bind3 members as pointers F1/F2/F3 (the PDB names), or a named `const CEventHitBase*` local for the filter argument
- 00a98760 N3DPrimitive::CVertexShaderInputStatic::`scalar deleting destructor' DIFFER(30v80): VC7.1 inlines the class's own (implicit) destructor into the `??_G` but leaves `??1CVertexShaderInput` as a call (retail inlines the whole chain: free +0x10, the range destroy 0x00A5E900, free +4, CBaseClass::~CBaseClass) | tried: novtable class with user dtor (no `??_G` emitted at all), implicit derived dtor, `__forceinline` on ~CVertexShaderInput and the two array dtors, user derived dtor (adds a vtable store, still calls the base dtor) | next: a non-polymorphic CVertexShaderInput body owner (members in a first-member struct, as the wave-24m "polymorphic base as first member" model) under the virtual CBaseClass, or the release written in the derived dtor's own body
- 00a987b0 N3DPrimitive::CVertexShaderInputAnimated::`scalar deleting destructor' DIFFER(30v80): same as 0x00A98760 (twin body, vtable 0x0129DFB0) | tried: as 0x00A98760 plus CVertexShaderInput without novtable and without a user dtor | next: whatever fixes 0x00A98760
- 0065fbe1 HasBuildingAsHomeOrWork::operator() DIFFER(76v74): the creature's building pointer is formed as `add eax,168h ; mov ecx,eax` (also `mov ecx,eax ; add ecx,168h`), retail `lea ecx,[eax+168h]` (same for +0x170); call order, IS_THING_ALIVE 0x004AB8F0 and the `mov al,1 ; jmp` tail already match | tried: typed CIntelligentPointer<T>::GetPItem forwarder, a CBuildingPointer deriving the base GetPItem, with and without `optimize("s")`, with `optimize("y", off)` (adds a frame) | next: give the building pointer its own declared `GetPItem()` member (as the landed 0x007BB660 filter model) and return `CThing*` from the creature accessor so no derived-pointer arithmetic is involved

## Skipped
None. All 40 row bounds are right (the catalog lengths include trailing int3 padding; every body ends at its `ret`).

## Label corrections
- 0x0096BAC0, 0x0095C680, 0x00953000 (catalog "CIsHitEventHitByMelee") call 0x00810F70, the **shot** test
  (CIsHitEventHitByShot, proven by 0x007DE2E0's Bind2<CIsHitEventHitBy, CIsHitEventHitByShot>); their twins
  0x0096BB70 / 0x0095C720 / 0x00953090 call 0x00810F80, the melee test.
- 0x008D4C10 (catalog TEvent CEventHitBy) tests event type 0x1B = CEventHitBySpecialAbility (GetType 0x00812300).
- 0x008AC7A0 (catalog TEvent CEventHitBy) tests 0x27 = CEventShotStruck (GetType 0x007BEB10).
- 0x00936070 (catalog: Bind2<In2DRange, IsValidAttackTargetForCreature>, BestCreatureToAttack, CThingAICreature) is
  GetPBestLeafThing<Bind2<CThingFilter_OnFire 0x006A8A80, CThingFilter_In2DRange>, CThingCompare_Nearest,
  CThingBuilding> (type list 3; the landed caller 0x00936210 already says so).
- 0x0048CBD0 is a second instantiation next to 0x0048CB50 (different comparator instance 0x0048B220, called by the
  second CPlayer::GetPNearestTCVillage copy 0x004886F0); its thing type is unproven (modelled `const CThingVillage`).
- 0x00A98760 / 0x00A987B0 "Image8::ReleaseAll" are the scalar deleting destructors of
  N3DPrimitive::CVertexShaderInputStatic / CVertexShaderInputAnimated (slot 0 of vtables 0x0129DFB8 / 0x0129DFB0).
- Callees: 0x00810F70 / 0x00810F80 / 0x00810FC0 / 0x00810FD0 ("_Cons_val<...CParticleMorphs...>") are
  CIsHitEventHitByShot::operator(), CIsHitEventHitByMelee::operator(), CEventHitBase::GetPThingHitting
  (PHitParams->PThingHitting, retail +0x50) and GetPWeapon (+0x58); 0x00810F00 / 0x00810F10
  ("CTCShotDefinitionBase::CanBeHitByMeleeStrike") are CEventHitBase KnockDown (+0x1a) / Decapitate (+0x1b) accessors;
  0x0049C770 "CVertexBufferWin32::DoSizeof" is CThingSearchTools::PeekAllThings; 0x006A80A0 "Bitset_Test" is
  CThingFilter_HasTC::operator().
