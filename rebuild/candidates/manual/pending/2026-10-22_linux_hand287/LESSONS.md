# Chunk 287 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Same-TU noinline stand-in bodies (`#pragma auto_inline(off)` + `__declspec(noinline)`) do more than fix frames. Callees covered: STLport `_M_insert_overflow`/InsertGrow/reserve, SetAsWeightedBlendOfThemes, IsFileCopyNecessary, the CActiveFile ctor, a read-only map operator[]. Read members into locals first, never touch the tag param, write only your own object or out-param. VC7.1 then knows the callee leaves the caller's memory alone:
  - Cached registers survive the call (weights._M_finish in ebp; counted-pointer Data/Info in edi/ebx).
  - The dead tag/def argument slot is shared, so the frame shrinks.
  - Spill-slot choices follow retail.
  - This landed 0x006B4900 and 0x009C53A0 (advisor C1); it is a 3-row confirmation.
- The lever can also hurt. A stand-in for a callee whose retail caller RELOADS a member after the call (0x009FF220 GetEntryDataType: retail reloads [esi+0x40]) makes VC7.1 CSE the member across the call, so the result moves further from retail. Add a stand-in only where retail keeps values live across the call.
- A class with a vptr followed by a double member gets 4 bytes of padding after the vptr under the default pack(8). Retail offsets that sit 4 lower need `#pragma pack(push,4)` (0x00599E3F CFrontEndManager).
- A tick-state struct whose vector sits at an odd offset (+0xc0d) needs `#pragma pack(1)` (CEnvironmentGameTickState).
- A `throw()` member dtor skips the EH state store in a compiler-generated EH teardown (0x00C22520). A non-polymorphic aggregate dtor stores no vptr.
- Inline `~X(){ T* first = First; DestroyRange(first, Last, tag); if (first) free(first); }`: naming First before the call stops the reload after it.
- scoped_ptr-style members with inline `: px(0)` ctors reproduce retail's member-init store order in big ctors (0x006B4900).
- Releasing a CRefBase temporary through the base subobject gives `add eax,4; dec [eax]`.
- A named `double now = GFGetTime();` gives retail's `fld st(0)` ... `fstp st(0)` pair.
- A loop-invariant cos written inside the loop is hoisted inside the loop guard, as retail does.
- `push_back(C3DVector(position))` with a user copy ctor gives the frame copy temporary. So does `X = C3DVector(...)`.
- An inline 64-bit-range ReadLong needs __forceinline. Sharing one long for the count and each index restores frame-slot sharing (0x00B420F0).
- `make_pair(...)` plus the pair<const K,V> conversion gives retail's two CCharString copies on map insert.
- The calling convention matters for these helpers:
  - CCharString operator+ 0x0099F570 is __fastcall.
  - GFFloatRandom and GFROR13 are __fastcall.
  - 0x00404440 is the folded CTBaseSingleton::Get.
- Struct sizes:
  - CMemoryDataInputStream is 0x1c bytes.
  - CBlendedEnvironmentTheme is 0x1f4 bytes.
- Residues no source spelling reached:
  - 0x006B3030: the epilogue `pop edi` hoist (both toolsets).
  - 0x008884D0/0x008886E0: the esi/ecx order of the First load before a __fastcall callee.

Review at landing:
- 0x00B7F0F0, 0x00B7FA20, 0x00B7C510, 0x00B7EEC0, 0x00B7F3B0 are landed under corrected Setup*ShaderConstantsAndFog names. Check the catalogue labels.
- 0x00C22520 is landed as ~CEngineVSConstantBlockSet (an EH member teardown, /EHsc), not the labelled CEngineVSConstantLayoutLights ctor. Confirm the owning class.
- 0x006B4900 and 0x009C53A0 carry same-TU stand-in bodies for callees that exist in retail. The landed callee symbols must not collide with them.
- /GS: 0x0089B780. /EHsc: 0x00C22520.
