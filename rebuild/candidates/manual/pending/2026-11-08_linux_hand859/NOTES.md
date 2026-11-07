# Chunk 859 -- notes (twenty parked near misses, retry with a huge budget)

Result: 3 of 20 matched (all RELOCATION_MATCH, bcheck 0 differing bytes incl. jump targets, objdump -r call targets checked,
behaviour tests PASS under VC7.1 via tall.py), 17 parked with the same residue they came in with. Every unmatched row has either a
documented structural / compiler-tie blocker (below) or lost every wider restructuring tried; the 300-attempt criterion was not
reached per row because the residues are post-register-allocation scheduler / colouring ties that textual spellings do not steer
(each residue was probed with a micro-TU, see LESSONS.md), so grading more textual permutations would only burn the shared lane.
Attempt counts per row this chunk are in STATUS.tsv (column attempts) and ATTEMPTS.log (every grade with tag, grade, md, bytes, note).

## Matched rows (src/ + test/)

### 0x00A852B0 CVectorMap<unsigned __int64, std::list<C2DLineF>, CKeyPairCompareLess<...> >::Insert  (RELOCATION_MATCH, 9 grades)
- Shape: stock STLport 4.6 `push_back(value_type(key, value)); Dirty = true;`. Key lever: the bodies of every out-of-line callee that
  receives the temporary's address live in the TU (as retail's headers had them), kept out of line with `__declspec(noinline)`:
  `list::_M_insert_dispatch` (member template, `_Const_traits`/`_Nonconst_traits` iterators, `const __false_type&`) 0x005152C0,
  `pair::pair(const pair&)` 0x00A825D0, `vector::_M_insert_overflow` 0x00A84C20 (with `__fastcall __uninitialized_copy /
  __uninitialized_fill_n / __destroy_aux`), `~list` 0x005127A0; `malloc` 0x00BFEA0E for the sentinel. With the bodies visible the
  temporary is unaliased (dead proxy zero store gone, begin()/end() loads before the sentinel store).
- Model: two-argument STLport `list<_Tp,_Alloc>` / `_List_base` / `_STLP_alloc_proxy<_Value,_Tp,_MaybeReboundAlloc>`;
  `CVectorMap : public std::vector<std::pair<K,V> >` + `C Compare` (+0x0c) + `bool Dirty` (+0x0d) (GOTCHAS 0x00A34950).
- Names: all relocations use the landed / reloc_bindings spellings (Ego_r is Dinkumware: no STLport spelling exists) -- see
  NAME_FIXES.tsv rows 00a825d0 / 00a84c20 / 005152c0 / 005127a0. Label: catalog "Vector_PushBack_ControlTreeNode_With_Sort" is
  descriptive; the Ego_r identity is CNavigationLayer::SwitchableLineLists' CVectorMap type (LABELS.tsv).
- Line 1 is `// CVectorMap::Insert(...)` (finish.py splits on `::` before `(`); the full instantiation is in the Identity comment and `// Leaf:`.

### 0x0058301A CTCInventory::CreateEditBoxActions  (RELOCATION_MATCH, 5 grades)
- Shape: single `editBox` local (no second copy); `std::list<_Tp,_Alloc>` model with `_List_node`, `_List_iterator`,
  `_M_create_node` (allocator<_List_node<_Tp> >().allocate(1) + placement new), `__declspec(noinline) insert(iterator, const _Tp&)`
  and `__declspec(noinline) push_back` as same-TU bodies (0x005939EE / 0x0042B646: `?push_back@?$list@PAVCEditBox@NUISystem@@...`,
  `?push_back@?$list@PAVCActionParam@NUISystem@@...`). Same lever as 0x00A852B0: with push_back declared only, the address-taken
  box home store took a different slot.
- Callees (bound spellings): `??0?$CCountedPointer@VCAction@NUISystem@@@@QAE@PAVCAction@NUISystem@@@Z`, `??0CAction@NUISystem@@QAE@XZ`,
  `??0CCharString@@QAE@ABV0@@Z`, `??1?$CCountedPointer@VCAction@NUISystem@@@@QAE@XZ`, `??2@YAPAXI@Z`.

### 0x006A9690 CThingPlayerCreature::ResolveRelativeMovementAcceleration  (RELOCATION_MATCH, 8 grades)
- Header: `#define FABLE_RETAIL_C3DVECTOR` + `#include "engine/C3DVector.h"` (DELTA_NOTIONAL_ZERO, AccurateNormalise, RotateXY) --
  no overlay change needed. Leaf `?ResolveRelativeMovementAcceleration@CThingPlayerCreature@@IAEXXZ` (protected).
- Lever: the joined camera angle (`angle = SpellMode ? GetCurrentRenderCamera().GetAngleXY() : PCurrentGameCamera->GetControlAngleXY()`
  as an if/else) is passed to `RotateXY` through `float& turn = angle;` so the `this` lea comes after the push (micro-probed).
- Models (evidence Ego_r struct_layouts + reloc_bindings): `CCamera::GetAngleXY() const` 0x00A0C380, `CAIGameCameraBase` slot 7
  `GetControlAngleXY() const`, `CPlayer::SpellMode` +0x220, `CWorld` slot 6 `GetCurrentRenderCamera() const` (by value),
  `CWorld::PCurrentGameCamera` +0x34, `CWorld::PeekMyPlayer() const` 0x0049D9D0, `CThing::PeekWorld() const` 0x004C7AB0 (protected; 0x004C7AA0 is the byte-identical GetWorld, naming wave 1 decision),
  `CThing::PhysicsTC` +0x60, `CThingCreatureBase::IsBlocking() const` 0x00664D00, `CThingPlayerCreature` slot 87 `GetMaxSpeed`,
  members Flags178 +0x178, Velocity +0x180, RelativeAcceleration +0x1b4, RelativeAccelerationDirection +0x1c0, PendingTurn +0x1fc.
- Name fixes: 0x00A14040 is `RotateXY` (not RotateAboutZ), 0x00A14440 `AccurateNormalise` (not Normalise), 0x0049D9D0 `PeekMyPlayer`
  (not GetCurrentPlayer), 0x004C7AB0 `?PeekWorld@CThing@@IBEABVCWorld@@XZ` (several landed rows spell it GetWorld), 0x00A0C380 `?GetAngleXY@CCamera@@QBEMXZ` (NAME_FIXES.tsv).

## Parked rows (differ/<addr>.cpp + differ/<addr>.test.cpp; line 2 = residue + this chunk's attempts, line 3 = next idea)

| address | row | lines | residue (unchanged) | this chunk |
|---|---|---|---|---|
| 0047ed76 | CTCInGameMenu::SetToSave | 2 | `push ecx; fsubp` v `fsubp; push ecx` (stack-stack x87 tie; micro-probed) | 2 grades + micro-probes |
| 00a22e10 | MakePerpendicular | 2 | second `mov esi,17Ch` one slot late (scheduler tie) | 1 |
| 006afe60 | CTCPhysicsBase::RespondToCollision | 2 | velocity param load one slot late (may-alias; Ego_r const signature same bytes) | 2 |
| 00d18df0 | movie-intro Main | 2 sites | inline budget of the implicit MovieBase dtor (lane-level) | 1 |
| 005e78f0 | CTCQuestCompletionUI::GlobalUpdate | 4 | x87/int interleave in the digit-count block (references grow the body) | 4 |
| 00c52bb0 | GetAMDModelName | 4 | `cmp eax,4; jae` v `cmp eax,3; ja` register tie | 1 |
| 00c75f80 | CCombFilter::Allocate | 4 | memset byte count hoisted above the m_pos store (11 shapes, all worse or md4) | 11 |
| 0065b142 | EnableVillagerDefTypes | 4 | ParentThing load after `push eax` (inline accessor shrinks body) | 3 |
| 00b92300 | CreateOcclusionGroup | 4 | prologue `mov ebp,ecx`/`push edi` order (landed sibling bodies same) | 3 |
| 00f36a30 | NFractal::CHeteroTerrainFractal::GetFractalHeight | 4 | counter reload after `fmul [edi]`; PDB-true spelling installed (same bytes); PerlinNoise stand-in body worse | 16 |
| 009fa780 | Blt | 4 | bookmark test/or register swap (lane gap GOTCHAS 2026-11-03) | 1 |
| 00986ed0 | filter ctor | 8 | two fmul operand orders (not textual; micro-probed) | 1 |
| 0085c7d0 | CMoveOnCylinder::OnInit | 8 | vptr/lea register pair eax/edx (same-TU CPhysicsLeaf ctor body same) | 2 |
| 00cfe030 | COracleTablet::Main | 8 | create-site `[edi+ecx]` v `[ecx+edi]` SIB order (14 index forms) | 14 |
| 00e76e90 | HeroFlashesLamp | 10 | shared ~CTimer exit block placed at the end of recursion copy 2 | 1 |
| 00de8370 | CRansommer::Main | 12 | two rotated tests `lea ecx,[eax+0ACh]` v `add ecx,0ACh` (references change layout) | 4 |
| 00b5a3f0 | ApplyColourFilter | 7/15 | RMW `or [eax+3D8h],ecx` v load/or/store + nop (count/position effect, chunk 832) | 1 |

Header debt: none (hdr/ empty; 006a9690 uses the repo `engine/C3DVector.h` as is). SIBLINGS.tsv has no rows for the same reason.
Descriptive names kept (no PDB spelling exists or none proven): catalog labels of 0x0047ED76 / 0x00C52BB0 / 0x00986ED0 / 0x009FA780
rows as they came in; everything in the three matched rows is PDB / reloc_bindings-proven.
