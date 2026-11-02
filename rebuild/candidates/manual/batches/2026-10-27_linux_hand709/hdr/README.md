# chunk 709 header overlay (`FABLE_HDR_OVERLAY=<workspace>/hdr`)

Both corrections are opt-in. With the macro undefined, each header's default text is byte-for-byte the repo copy, so landed
includers compile unchanged. No `#error`, no new `#pragma pack`, and neither `engine/CBankFile.h` nor `rebuild_abi.h` is touched.

## engine/CWorld.h: new `FABLE_RETAIL_CWORLD` branch
- **The problem.** The generated default flattens CWorld into a non-polymorphic struct with no vtable and no bases. Retail CWorld is
  `CBaseClassNonCopyable@0, CIDrawWorld@4` (RTTI .?AVCWorld@@, `ghidra_out/rtti_bases.tsv`). Its primary vtable is 0x012390F0 with
  14 slots, and its CIDrawWorld vtable is 0x012390DC with 4 slots.
  - CWorld::Unload 0x0049E270 calls `GetThingManager()` through slot 3 (`call [eax+0Ch]`).
  - CWorld::EventIsWorldEvent 0x0049D8C0 needs the protected non-virtual `GetEventDispatchTableEntry(long) const`.
- **The branch.** It declares the retail class: every slot name and order comes from the vtables, and each name is a CWorld
  member in Ego_r.pdb and/or FableWin.pdb. The slot-by-slot evidence is in the header comment.
  - VC7.1 lays out the two `GetCurrentRenderCamera` overloads in reverse declaration order.
  - The non-virtual members are `GetFrame` (static), `GetWorldSeed`, `EventIsWorldEvent`, `Unload` and
    `GetEventDispatchTableEntry`.
  - The data members from +0x08 are the default branch's members at the same retail STLport sizes, unpacked. Every member sits at
    its natural alignment, and PlayerManager at +0x0C is read by Unload.
  - `FABLE_STATIC_ASSERT(sizeof(CWorld) == 0x198)`.
- **The default.** The `#else` branch keeps the original lines unchanged (checked with diff). The shared offset asserts follow
  both branches.
- **Users.** src/0049d8c0.cpp and src/0049e270.cpp. No landed source defines `FABLE_RETAIL_CWORLD`.
- **Proof the overlay is used.** probe/0049d8c0.log p9: with the overlay off, the source fails to compile (SRC_FAIL).

## engine/CPlayer.h: two members added inside the existing `FABLE_RETAIL_PLAYER` branch
- `void ResetModes(); // 0x00634870`: `?ResetModes@CPlayer@@QAEXXZ` (Ego_r 0x005579A0, FableWin 0x01F5AA20). It is the only
  binding in reloc_bindings.
- `CThingPlayerCreature* GetPControlledCreature() const; // 0x00487DC0`:
  `?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ` (Ego_r 0x00493650, FableWin 0x01ACDE00). 65 landed
  binders use this spelling.
- A forward declaration `class CThingPlayerCreature;` is guarded by the same macro.
- **Users.** src/0049e270.cpp (CWorld::Unload). The one landed source that defines `FABLE_RETAIL_PLAYER`,
  CTCTargetingPlayer_GetPTargetedThing_00802610.cpp, was rechecked read-only against the overlay (vland `--recheck-landed`).
