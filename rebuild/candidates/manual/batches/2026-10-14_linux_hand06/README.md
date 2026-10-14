# Batch `2026-10-14_linux_hand06` — hand-authored CAIStateGroupBehaviourBase .. CAIStateGroup_EnemyCreatureBase functions: 40 bodies (35 landed)

Delegated hand-authoring pass (compiler in the loop, Wine lane) over the seventh chunk of
class-labelled open functions of 17–96 bytes: `IsRunnable` / `Update` / `Initialise` / `OnActivate`
bodies of the behaviour state groups (BuyFromSeller, CleanHouse, CleanWork, CreatureGoUnderground,
CreatureIdleUnderground, CreatureRangedCombat2, EatAtTable, EnemyCreatureBase, BeetleToggleHover,
AvoidRegionExit), `GetMarkerType` constants, the `CarryCrateAndFollow`, `CreatureUseCoverBase` and
`MinionBehaviourCrowdBase` constructors, `CAnimFilterOf2Groups`, `CMessageEvent`, `CStringParser`
and `CCreatureAction_HobbeSpellcasterAim` constructors, a cover-list removal (inlined `vector::erase`
through the retail `_Copy_impl` helper) and several descriptive `_<addr>` classes whose manifest
labels were propagated. Three rows were already landed (two EH-framed, one duplicate).

Lessons recorded by the pass: `PerformActionAndWait(&CCreatureAction_X(creature), false)` builds
the action as a temporary inside the call expression (`push 0` first, base destructor after the
call, frame of the temporary's size); `PBrain->Creature` must be read into a named local, one per
branch; identical `if/else` tails are tail-merged; an inlined interface lookup is
`__forceinline bool GetInterface(long, T**)` with a branch bitset test and the key copied to a
separate local for `LowerBound`; the retail copy helper is `__fastcall(first, last, dest, const Tag&, long 0)`
with the empty tag homed in the dead argument slot; `static bool __stdcall` gives a `ret 4` static
member; `(long)(g * 3.0f)` is `fild ; fmul [pool] ; __ftol2`; the VC6-era `std::list` constructor
is zero store, `malloc(16)`, self-links, head store. Two bodies differ by scheduling only
(`BirdStartled::Update`, `ApprenticeHero::IsRunnable`).

All landed bodies graded `MATCH` / `RELOCATION_MATCH` + `PASS` on 2026-10-14.
