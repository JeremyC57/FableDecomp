# Hand chunk 129 (CCreatureAction_HeroLoadRangedWeapon .. CCreatureAction_ScorpionTailPlunge): 38 of 40 byte-exact

All 38 matched rows grade RELOCATION_MATCH with default flags (no pragma, no /EHsc) and pass their behaviour test under g++ and
the lane's VC7.1 cl.exe. No source includes an engine header: every PDB header for these classes is the flattened layout that sits
0xc (CCreatureActionBase, 0xb4 PDB vs 0xa8 retail) or 0x18 (CCombatActionBase, 0x12c vs 0x114) high, so each source models `this`
locally with the retail offsets and says so; member names are the PDB names where the PDB has them.

## What matched (38)
- **GetInterface FinishAction/Initialise family** (CCreatureAction_JackFloatUp 008c1180, JackGetUp 008c1260, JackFall 008c11f0,
  ScorpionRecover 008c4b00, PickUpJug 0084a180, PlayerInteractionViewQuests 00845a70, PickUpGenericBox 00848ae0): the landed
  out-parameter `GetInterface(T**, type)` template. Retail very often IGNORES its bool result: the absent arm then reads the
  uninitialised out pointer from the lookup key's frame word (`mov esi,[esp+8]`), and VC tail-duplicates the following call into
  both arms -- write `GetInterface(&p, T); p->Call();` with no test. PickUpGenericBox uses a second LowerBound instantiation
  (0x004365B0) and the bit-7 presence test comes out as `mov cl,[eax+28h] ; test cl,cl ; jns` from the same template.
- **Checked GetInterface** (ReleaseStrengthBerserk::Initialise 008e6490, PutDownJug::PerformAction 0084a200,
  HeroLoadRangedWeapon::FinishAction 0078b640): the `if (GetInterface(...))` form; a `const&` key argument bound to `this`
  (`Erase(this)`) is built in the lookup-key frame word after the push.
- **InterruptableMidAttackAutoTurn::PerformAction 0087cd50**: an out-parameter getter on the animation entry whose out pointer must
  live in an INLINE ACCESSOR (`bool GetAutoTurn() const { P* p; Get(&p); return p->flag; }`) to share the single `push ecx` frame
  word with the later GetInterface key; a plain local takes its own word (frame 8 instead of 4).
- **JackFall::FinishAction 008c2490**: the creature for the action constructor is a named local fetched before the base call, the
  SetCurrentAction object is a chained GetPItem, the action is `&CCreatureAction_GoUnderground(creature)` built in the call and
  destroyed through the base destructor (frame 0x118 = key + 0x114).
- **HobbeSpellcasterAim::FinishAction 008c99c0**: IS_THING_ALIVE inline, `Kill(true)` chained, the clear is the landed
  stack-CIntelligentPointer assign shape (0x0084B1C0).
- **JackFloatUp::FrameUpdate 008c0790**: the per-frame rise is `C3DVector rise(0, 0, speed / (float)fps)` -- only Z is used, the two
  zero stores are dead but the vector keeps three frame words (retail frame 0x1c); a float local shrinks it to 0x10. The new position
  is an inline 3-float constructor (Z computed first, Y via the FPU, X as a dword move); the clamp is `if (Z > limit) Z = limit`.
  The physics component at thing+0x60 must come through an inline accessor to load straight into ecx for the virtual call.
- **NymphGetHitDie::FrameUpdate 008c9f40**: position copied by value (three integer moves), `HeightOnInit * (1.0f - progress)` as a
  named float, ground height + height into Z, physics via the inline accessor.
- **ScorpionTailPlunge::PerformAction 008c4810**: the def-name CCharString in its own block (its frame word is reused for the
  unchecked GetTCDriver out pointer, the landed 0x00750DD0 template); `Get().GetDefGlobalIndexFromName(name)` chained.
- **Combat-action constructors** (RiseUp 008bd880/0074dac0, JackShotFloat 00976f30, JumpOutFromCover 0097b3f0, JackScream 00977100,
  ScorpionChargeInto 008c4e20, KrakenHigh/LowAttackLoop 00925780/009255e0, KrakenLowChargeLoop 009254c0, PlayCombatAnimation
  008b7f80): the landed family -- `GetSoundPair(owner, "LIT")` implicit conversion (or `GetSoundPair(owner, 0)` for a null name:
  `push -1 ; push 0`), `CSoundPair()` zeroed by its inline constructor, scoped `CCharString name("..", -1); SetAnimationName(name);`.
  Member placement: init-list scalars may be scheduled before or after the vtable store (Kraken: +0x118 before, +0x114 after,
  same init list); a CCountedPointer member zeroed after the vtable is a member object with an inline constructor; a
  CIntelligentPointer member is a member with an inline constructor and a scalar stored after it is a body assignment; three dwords
  copied from a `const C3DVector&` through edx/ecx straight off the argument are three scalar init-list members (a struct member
  copies through `lea`).
- **CCreatureActionBase constructors** (PickUpThingAtTable 008440c0, PlayAnimation 008424f0/00842570/008425e0,
  PlayIntoLoopOutOfAnimation 00842740/00842960, Read 007e7fd0): the landed 0x00842A20 shape; 0x006924B0 is the creature's animation
  lookup by NAME (`__fastcall`, edx = const CCharString&). Init-list members in declaration order reproduce the interleaved byte
  stores. Read: the "STANDARD_CAST" name must be an IMPLICIT conversion temporary (defaulted length) -- VC builds it first and keeps
  the sound pair's immediate zero stores beside the lookup call; the inline by-value helper of GOTCHAS 24a hoists those stores
  above the string constructor (DIFFER 117v117). 00842960 clears the base AnimDelayTime (+0x18) in the body.
- **Copy constructors** (PlayAnimation 0084c340, PlayIntoLoopOutOfAnimation 0084c3e0, PerformExpressionExtended 0084be30,
  InterruptableMidAttackAutoTurn 0087e690): explicit memberwise copy constructors over the out-of-line base copy constructor
  (0x0062EBC0 / 0x0062F3B0); CCharString modelled locally so 0x0099EC30 stays a call; CDefPointer copy = copy + `if (p) ++p->count`;
  CIntelligentPointer copy = base ctor + SetPItem(other.GetPItem()); RequiredFacing = a vector with a memberwise dword copy constructor.
- **~CombatDialogueManager 006f2f60** (catalogued as ~CCreatureAction_PerformExpressionExtended): non-polymorphic class, members
  destroyed in reverse (CIntelligentPointer, CDefPointer released through a pointer local with the clear inside the null test,
  STLport `_Rb_tree` destructor = `clear()` with header re-reads then `if (header) free(header)`).

## Parked for Fable 5.1 (2)
- 008c4a80 CCreatureAction_ScorpionSummon::Initialise DIFFER(119v117): retail merges the two GetInterface arms straight into ecx (`mov ecx,[eax+4]` / `mov ecx,[esp+8]`), ours into eax plus `mov ecx,eax` before AddMode | tried: out-parameter GetInterface, inline GetModeManager accessor, value-returning inline lookup, local declared before the base call | next: the twin ScorpionRecover::FinishAction (008c4b00, same source shape) matches with eax -- look for a different AddMode declaration (e.g. a base-class member reached through a cast) or a register-allocation lane gap
- 005e3620 CKeyedCountedList::Add (catalogued CCreatureAction_InterruptableMidAttackAutoTurn ctor) DIFFER(132v114): retail keeps the pair's key/pointer in ecx/esi for the in-place copy and the final release (`test esi,esi ; dec [esi+4]` after the InsertOverflow call), ours re-reads the address-taken pair from its frame slot | tried: member-pointer ctor/dtor, local-pointer ctor/dtor (best), /Oa | next: two objects (a register-only counted copy that is released + a bitwise frame pair for the InsertOverflow argument), or STLport `make_pair`/converting-pair forms

## Skipped
- none (no row covers two functions).

## Label corrections
- 009255e0: catalogued CCreatureAction_KrakenHighAttackLoop; stores vtable 0x01290C64 (`?A0x29922355::CCreatureAction_KrakenLowAttackLoop`) and the "ATTACK_LOW_LOOP" name -> CCreatureAction_KrakenLowAttackLoop ctor (source carries `// Leaf: CCreatureAction_KrakenLowAttackLoop`).
- 006f2f60: catalogued ~CCreatureAction_PerformExpressionExtended; sole caller is ~CCombatManager (0x006ED480) on its member at +0x38 = PDB CombatDialogueManager DialogueManager -> CombatDialogueManager::~CombatDialogueManager (`// Leaf: ??1CombatDialogueManager`).
- 005e3620: catalogued CCreatureAction_InterruptableMidAttackAutoTurn ctor; no vtable store, `ret 8`, pushes a pair onto a vector -> a vector-of-pairs Add (named CKeyedCountedList::Add in differ/).
- 0x006924B0 (callee): the landed "table animation by kind" lookup takes a `const CCharString&` name (007e7fd0 passes the address of a CCharString).
- 0x0058119B "CBankFile::CompareDataOffset::operator()" is a tree erase(const key&) on a component map; 0x00800020 "CAnimation_RecalculateScale" is that component's recalculation; 0x006925E0 "CInputTypeMouseMovementEvent::operator==" is an action progress getter (float); 0x00738A00 is the animation entry's out-parameter parameter getter; 0x0080CBF0 "CCreatureActionBase::AddDependentThing" is the explosion driver's exclusion add.
