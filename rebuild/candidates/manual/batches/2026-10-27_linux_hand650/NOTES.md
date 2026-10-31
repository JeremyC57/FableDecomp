# Hand chunk 650 (CParticleSystem .. CCreatureAction_UseObject): 2 of 10 byte-exact

Stopped early on owner instruction (coordinator message): no further probes after the 00a64910 match.

## What matched (2)
- std::__adjust_heap<CThing**, int, CThing*, CThingCompare_InDirectionOfAndNearestToHeuristicBitch> 0x0097AF70 (RELOCATION_MATCH, 1 grade):
  STLport sift-down then __push_heap. Lever: the landed sibling 0x0097B0E0 (__make_heap) model, i.e. C2DVector with the
  memberwise user copy ctor plus operator() as a same-TU stand-in under `#pragma auto_inline(off)` + `__declspec(noinline)`.
  The comparator copy into __push_heap's argument area then loads words two ahead (retail). Declaration-only operator() with an
  Initialise-based copy ctor gave fld/fstp copies (the parked residue).
- CMemoryAllocatorVariableSize::RemoveFromTreeFixup 0x00A64910 (RELOCATION_MATCH, 3 grades): red-black delete fixup. Lever:
  RotateLeft/RotateRight as ordinary non-inline members defined in the TU before the caller (their out-of-line copies are retail
  0x00A646E0 / 0x00A64680). VC7.1 /Ob2 auto-inlines five of the six calls and keeps the inner-left RotateLeft(sibling) call, which
  gives retail's this=ebp / info=edx / 4 pushes. The parked `#pragma inline_depth(0)` around inline members gave 756v757.
  Second lever: the black test stays one inline helper (open-coded `a == 0 || a->Colour == 1` gives 761 bytes). Names respelled to
  the PDB: nested `CMemoryAllocatorVariableSize::CMemoryArea`, protected (IAE) members, `this` over engine/CMemoryAllocatorBase.h.

## Parked for Fable 5.1 (8)
- 00af56e0 NParticleEngine::CParticleSystem::DeleteAllParticles DIFFER(138v138): bottom `mov [esp+0xc],edi` (retail, after mov edi,ebx) vs `mov [esp+0xc],ebx` | tried (this session, 11 probes): `while ((current = it) != end())`, current=it before --N, chained `current = it = erase(it)`, named `next` from erase, `for(;;){current=it; if(!(it!=end()))break; ...}`, comma condition, role swap (address-taken `it` + register copy; 139), same-TU ClearParticle stand-in under auto_inline(off) (reads *it only), `erase(it++)` (register rotation) | next: STLport 4.6 real `_List_iterator`/`operator!=` free templates and the real `list::clear` body; otherwise treat as a copy-propagation tie.
- 007e4530 CTCWillResponse::IsThingLightningable DIFFER(159v159): `mov edx,[eax+4] ; test byte [edx+0xc],18h` vs retail eax | tried (6 probes): unnamed static_cast, named `flags` local, inline IsZTargetable() (Ego_r.pdb has ?IsZTargetable@CTCTargeted@@QBE_NXZ) as `!= 0` and as if/return, single `return a || b || c` (162), `&&` in one if (160) | next: PeekTC<const CTCTargeted> out-param only on the 9 path with the real CThing header; check whether the 0x4E path's partial `mov al,1` keeps eax live. The copied file still carries `#pragma pack(push,1)` (owner rule: strip before landing; grade not re-checked).
- 00973dc0 CAIStateGroup_JackOfBladesBase::SummonPillars: untouched (stopped); brief residue: Pillars[i] add vs lea, squared-length order, dead Z scale, SetPosition vcall register.
- 00f38ff0 CSurfaceCompositor::SetBaseBlendModes DIFFER(1841v1842): untouched; capture-block esi/edx register tie.
- 008de080 CModeDragonStrafeBurninate::FrameUpdate DIFFER(1438v1437): untouched; see brief (fxch normalise, SetAngleXY physics load).
- 008e03b0 CModeGuildmasterPowerUpSpeed::OnActivate: untouched; guard hoisted into esi vs retail *it reloads.
- 00e83230 CCreatureAction_TrollWhackGroundBase::HandleVillageSelection DIFFER(818v820): untouched; Game vtable reload in branch 1.
- 006a03a0 CCreatureAction_UseObject::Initialise DIFFER(827v825): untouched; frame 0x44 v 0x48, zero in edi vs ebp.
  Next idea for the untouched rows: the auto-inline lever from 00a64910 (callee defined as a plain function in the TU, not
  `inline`/pragma-forced), and same-TU noinline stand-ins for opaque callees that take pointers to locals.

## Skipped
None.

## Label corrections
- 0x0097AF70 catalog `_Adjust_heap<...>` (Dinkumware) -> STLport `std::__adjust_heap`.
- 0x00AF56E0 is bound as `?Stop@CParticleSystem@@QAEXXZ` at one caller (0x00AF5870); the PDB name is DeleteAllParticles.
- 0x00AF5090 ClearParticle bindings use an invented `CRetailList` and drop the NParticleEngine namespace (NAME_FIXES.tsv).
- 0x0097ADF0 landed as global `_Push_heap`; it is STLport `std::__push_heap` (NAME_FIXES.tsv).

## Header debt
- 00a64910 models `this` as `CMemoryAllocatorVariableSize : CMemoryAllocatorBase` (header flattens the vptr into `void* __vftable`).
  There is no CMemoryAllocatorVariableSize.h; CMemoryArea and CDeletedNodeChildInfo are local, with PDB layouts.
