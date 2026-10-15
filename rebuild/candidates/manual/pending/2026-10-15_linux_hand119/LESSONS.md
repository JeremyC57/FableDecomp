# Chunk 119 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- The landed GetInterface out-parameter template must be `__forceinline` when the function also makes other calls or
  uses it twice: plain `inline` emits it out of line (0x0097E200 next to two IsStopping calls, 0x00929220/0x009292B0
  with two lookups).
- An inline accessor that RETURNS `m_member.GetPItem()` gives retail's `lea ecx,[eax+N]` for the member's `this`;
  `p->m_member.GetPItem()` written directly, or a pointer-returning accessor, gives `mov ecx,.. ; add ecx,N`
  (0x00917DB0).
- `C3DVector d = a - b;` through an inline value-returning operator- keeps the 12-byte frame slot while the unused Z
  subtraction is dropped (`sub esp,0Ch` where a C2DVector local gives 8) (0x00979260 / 0x0097AAE0).
- A by-value comparator is constructed in place in the argument area (`sub esp,10h ; mov ecx,esp`) only when it has a
  user copy constructor; members built from a C3DVector through `C2DVector(const C3DVector&) : X(v.X), Y(v.Y)` are
  dword moves, a `(float, float)` constructor goes through the x87 (0x00979260).
- A POD C3DVector member assigned with the IMPLICIT assignment is `lea ecx,[esi+N]` then dword stores through ecx (edx
  reused for X and Y); a user memberwise operator= (inline or `__forceinline`) or a destination pointer local folds
  the stores into `[esi+N]` (0x00920990).
- A CIntelligentPointer member reset/assigned from a raw pointer (`m_p = CIntelligentPointer<T>(x)`) is a frame
  temporary through an inline converting constructor (base ctor, vtable, SetPItem(x)), the inline operator=
  (SetPItem(other.GetPItem())) and the base destructor (0x00920990, 0x0095FE60, 0x0091D850, 0x0091DD30, 0x0094EF90,
  0x0094C7E0).
- `if (alive) { while (Action(..) == 2) Idle(); }` gives the non-rotated loop that jumps back to the argument pushes
  without re-testing `alive`; `while (alive) { if (Action() != 2) break; Idle(); }` is rotated (0x0094F000).
- A named CCharString in an inner scope `{ CCharString s(""); Call(&CAction(.., s, ..), false); } Terminate();` is built
  before any argument push and destroyed before Terminate (0x0091F0D0); a creature named local AFTER it gives the
  `mov eax,[esi+4] ; mov eax,[eax+18h]` chain where the inline expression uses ecx/edx.
- `GetPWorld()->Mgr->Call(creature)` with the creature already in a local pushes it before the GetPWorld call; take
  the world into a named local too (0x0091D850).
- `GetPWorld()->StaticOrGlobal()` still emits the (discarded) GetPWorld call -- seen twice (0x0093A540, 0x009143E0);
  a frame-rate global read through it goes into a local BEFORE a following seed/frame call.
- Nested ifs with a single `return true` and one shared `return false` reproduce an early `push ebx` and one tail;
  the same predicate as sequential early returns shrink-wraps ebx and duplicates the epilogue (0x009444E0).
- Mangled oracle names: forward-declare parameter classes as `class` (`ABVCWorld@@`, `struct` gives `ABUCWorld@@`) and
  keep `protected` virtuals protected (`MAE`); a mismatching leaf makes q.py silently grade the largest symbol
  (0x0094C7E0, 0x0095FE60). An `operator()` row needs `// Leaf: ??R<Class>` (the symbol never contains "operator()").
- Lane gap (two rows, parked): a map component used as `this` of a virtual call -- retail `mov ecx,[eax+4] ;
  mov eax,[ecx] ; call [eax+N]`, the lane `mov eax,[eax+4] ; mov edx,[eax] ; mov ecx,eax` for every lookup form
  (0x0095EBF0, 0x0091F050).

Review at landing:
- 0x009576C0 is a catalog row holding two functions (13-byte CAIStateGroupBehaviourBase::OnActivate tail-jumping
  vtable slot 16, then an unreferenced 123-byte body at 0x009576D0): split it before authoring.
- Local base models next to existing engine headers: CAIStateGroupBase.h is used only by 0x008FD1B0 (with
  CAIBrain.h; the other rows need virtual calls on `this`/PBrain, which the header's `void* __vftable` cannot express);
  CAIStateGroupBehaviourBase.h and CAIStateGroup_BanditPlayDice.h are used through `_Methods` views (0x009578A0 parked,
  0x009284B0); the headers for BanditPlot, BirdGroundWander, BuyFromSeller, CarryCrateAndFollow, CreatureAdvanceInCover,
  CreatureRangedCombat2, CreatureRangedCombatUseWillShield, CreatureTakeCover, CreatureUseCoverBase, Drunk,
  EnemyCreatureBase, FollowPlayer and Gaze flatten the base into pads, so those rows model locally with the header's
  member names.
- engine/CAIStateGroup_CreatureUseCoverBase.h puts PCoverThing at +0x1f (a pack(1) artefact) and gives CoverList the
  PDB's 16-byte vector; retail's STLport vector at +0x28 is 12 bytes.
- 0x0095E870 returns an int-width value (`mov eax,1`); the source models it as `long` (the catalog name is not
  mangled). If the landed symbol must be `bool`, the `return a && b;` full-register form (as in 0x009226B0) is the
  alternative to check.
- No row needed more than 4 grades.
