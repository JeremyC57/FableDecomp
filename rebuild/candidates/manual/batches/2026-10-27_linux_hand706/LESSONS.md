# Chunk 706 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A downcast pointer passed to a call that comes after a second smart-pointer fetch must sit in a named block local:
  `CThingAICreature* creature = static_cast<CThingAICreature*>(PCreature.Get()); creature->SuspendAI(false, 0);` puts GetPItem
  before the `push 0 ; push 0`; the inline forms `static_cast<..>(PCreature.Get())->SuspendAI(..)` / C-style cast / `*PCreature`
  push the arguments first (0x008B6A20 GoUnderground::FinishAction, DIFFER 63v63 x3 -> RELOCATION_MATCH).
- Def-TU `Transfer` rows at odd / unaligned addresses are real starts when the previous function ends `ret 4` right before them; they
  need `#pragma optimize("s", on)` (0x004DE253: DIFFER 57v57 without it).
- A dense `switch` over 0..3 that returns `&Array[n]` per case and `return 0` after compiles to VC7.1's `cmp eax,3 ; ja ; jmp
  [eax*4+table]` with the table right after the code, in case order (0x00BE5C60). The table bytes are relocations; check their order
  by hand (objdump -r) because the grader does not compare jump tables.
- `IS_THING_ALIVE` is the PDB template `template <class T> inline bool __fastcall IS_THING_ALIVE(const T& thing) { return &*thing &&
  !thing->IsDead(); }`; it works over both a CIntelligentPointer member (0x008547D0) and a raw pointer local (0x007CA630) and emits
  the exact PDB manglings.
- A CTCPhysics* `UpdateInterpolation` at slot 94 is a tail jump to the protected base 0x006AF780; the slot-94 entries of
  CTCPhysicsBase / CTCPhysicsStandard / CTCPhysicsRigidBody are three different PDB functions (two were mislabelled).
- hdr/ overlay proof without `#error`: insert one member before the field the row reads (the row turns DIFFER), then after it (it
  stays MATCH). This shows both that the overlay is used and which offsets matter; cmp the restored copy against the backup.

- The retail sizeof of a component class is the `push imm32 ; call operator new` before its ctor call (the ctor stores the class's
  vtable); the ctor's member stores give the retail offsets. Use them to place opaque members and assert the size (0x00759970: 0x258,
  0x007CA630: 0xAC, 0x00802610: ctor 0x004E2250).

Review at landing:
- 0x00723730 is landed as CCreatureAction_TrollWhackGroundBase::Initialise but is CTCPhysicsStandard::UpdateInterpolation (vtable
  0x01265F84 slot 94); its callee binding 0x006AF780 is CTCPhysicsBase::UpdateInterpolation. Respell both (NAME_FIXES.tsv).
- 0x00C77650 is landed as `CLandscapeBackgroundPatch_vectordeletingdestructor_00c77650.cpp` (Leaf `vector_deleting_destructor`, local
  struct T); it is slot 0 of the libpk vtable 0x012BF0C8: the scalar deleting dtor of CVtable_012bf0c8 (`??_G`). Wrong class and kind.
- 0x006925A0 carries eight spellings. The CCreatureActionBase::HasPerformedAction PDB spelling is proven for the GoUnderground call;
  the OpenChest / SummonerLightningOrb / ForcePush / UnsheatheWeapons callers may be identical-code-folded readers of other +0x62
  bools. Check each caller before respelling it.
- Opaque storage still stands in for two unmodelled member types: CRigidBody (0x00759970) and CEnginePrimitiveHandle (0x007CA630).
  Each sits at its retail offset with a size assert, per the coordinator decision.
- Local base models next to existing engine headers (flattened): CTCPhysicsBase, CCreatureActionBase, CTCTargetingPlayer,
  CAnimComponentCombatFlourishTargetOffset, CThingAICreature, CThingCreatureBase (NOTES.md "Header debt").
- No row needed more than 4 grades. No catalog row holds several functions.
