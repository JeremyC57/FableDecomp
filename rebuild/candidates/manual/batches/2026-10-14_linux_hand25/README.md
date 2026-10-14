# Batch `2026-10-14_linux_hand25` — coordinator hand-authoring chunk 25 (CCreatureAction_LeapLong .. CCreatureAction_SheatheItemToInventory_ForEnflame): 35 landed here, 1 in `_hand25_b` (36 of 40)

Chunk 25 of the class-labelled 17–96-byte pool, authored by the coordinator (not an agent) with the Wine lane:
creature-action constructors over CCombatActionBase (0x00858030 / 0x00857F30 / 0x00857E40) and
CCreatureActionBase (0x00693B30, seven arguments with a table-animation lookup in register arguments), copy
constructors and clones with the family-44 CIntelligentPointer copy inlined, Initialise / PerformAction /
FinishAction bodies (scoped CCharString names, the inlined `GetInterface` lookup of 0x00849320 for the nymph /
villager / mode-manager / spell components, world messages, the scripted camera manager), two abstract
thing-component destructors (counted pointer release + intelligent pointers, one byte-packed at +0x1d), and
seven Dinkumware tree-node factories mislabelled "CCreatureAction_*::Clone" (malloc + placement copy of the
value at +0x10; the value types reconstructed from the copy: a pair's explicit member-wise copy with plain
records copied as blocks through re-based pointers, a CCharString copied out of line, a vector copy
constructor, an intelligent pointer).

Label corrections (vtable stores in rtti-vtables.tsv): 008c95b0 "LeapLong" -> CCreatureAction_BanditKingUnstuck
ctor; 007ed700 / 0084c9a0 / 008d63f0 "PickUpJug" copy ctors -> GiveMoneyToThing / PickUpThingAtTable /
FireMissileTest; 00947b70 "RemoveFoodFromTable" -> CCreatureAction_PickUpTankard; 009680d0 PickUpJug::Clone
inlines the PickUpThingAtTable copy constructor @ 0x0084C9A0; 006ca760 / 007b5130 are CTCBase-derived
components, not creature actions; the "NavToPos::Clone" / "PlayCombatAnimation::Clone" / "ScorpionChargeInto::Clone"
rows are STL node factories (`CRetailTree_<addr>::BuyNode`).

Lessons (also in GOTCHAS): a CCharString temporary handed to a base constructor must come from an inline helper
returning it by value (`static inline CCharString AnimationName()`); a bare temporary pushes the trailing constant
first and reuses the constructor's eax. A member pointer used as `this` of a virtual call is read through an inline
accessor (`GetBody()`, `GetCamera()`) to land in ecx directly. A `GetWorld()` call whose result feeds a message call
is a named local (evaluated before the argument pushes). CIntelligentPointer models need the `m_pItem` word when a
member follows them. Value types of tree nodes: explicit pair copy + plain POD records (block copies through
re-based pointers), not member-wise record copies.

Not landed (best sources in `$S/w22/agent25/differ/`, notes in NOTES.md): 008c6160 RockTrollFallToDeath::Initialise
(retail fetches the creature before pushing SetDyingMode's constants; five shapes), 007a9b10 scripted camera
FinishAction (register allocation), 00528930 tree CopyNode (zero-register reuse), 004c41d0 tree BuyNode (byte-copy
scheduling). 009680d0's behaviour test used `dynamic_cast` (no RTTI in the lane) and re-landed in `_hand25_b`.
