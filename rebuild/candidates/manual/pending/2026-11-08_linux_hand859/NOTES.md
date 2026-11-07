# Chunk 859 -- notes (twenty parked near misses, retry with a huge budget)

Result: 4 of 20 matched. Round 1 (first snapshot, landed by the coordinator): 00a852b0, 0058301a, 006a9690 (RELOCATION_MATCH).
Round 2 (coordinator asked for >= 100 grades per parked row): 00c75f80 CCombFilter::Allocate MATCH (plain MATCH, no relocations;
bcheck 111/111/0; g++ + VC7.1 behaviour test PASS) via a forwarding-barrier lever (LESSONS.md). Every other parked row received
>= 100 grades this chunk through cross-product grids of behaviour-identical spellings (bin/grid.py + spec/<addr>_g*.py; each
variant is a distinct source, graded, logged in ATTEMPTS.log with its axes); none moved. 00d18df0 keeps its documented structural
blocker (its two MovieBase dtor sites expand only under the diagnostic /d2inlS15 meter: a caller-size inline-budget gap, not a
spelling) and was not regraded. Attempt counts per row are in STATUS.tsv (column attempts) and ATTEMPTS.log.
payload.json holds only the round-2 match (00c75f80); the three round-1 rows stay in src/ + test/ exactly as landed.

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

### 0x00C75F80 CCombFilter::Allocate  (MATCH, 391 grades this chunk; 158 before)
- Residue was "the Clear() byte count is hoisted above the m_pos store and m_data is re-read in retail". Cause: VC7.1 forwards a member
  store into a later load only when both use the same base expression; with forwarding it also CSEs `m_size * sizeof(float)` with the
  cursor bump's `length * 4` scale. Retail's form: `float* block = *pMemory; *pMemory += length; CRingBuffer* ring = &m_ring;
  ring->m_data = block; ring->m_size = length; ring->m_pos = 0; m_ring.Clear(); m_mode = index;` then `*pBytes += length * sizeof(float);`.
  Everything else (CDspObject base, ring layout +4/+8/+0xC, m_mode +0x10, memset intrinsic, shared tail) as the parked source.
- Found by hypothesis after two failed grids (types/signedness of the two `length * 4`: 225 cells all CSE'd; CRingBuffer as a
  second base class: no), then a 40-cell grid over pointer/reference bases for the stores vs the Clear() call: every `rs*` cell
  (stores through the pointer, Clear() on the member) is a MATCH; the reverse (`rc`), all-through-pointer (`rp`), all-through-reference
  (`rr`) and pointer-before-the-test (`rf`) are not.
- Names: catalog spelling kept (`?Allocate@CCombFilter@@QAEXIPAPAMPAII@Z`, bound); no PDB (libpk reverb module).

## Parked rows (differ/<addr>.cpp + differ/<addr>.test.cpp; line 2 = residue + this chunk's attempts, line 3 = next idea)

| address | row | lines | residue (unchanged) | grades this chunk |
|---|---|---|---|---|
| 0047ed76 | CTCInGameMenu::SetToSave | 2 | `push ecx; fsubp` v `fsubp; push ecx` (stack-stack x87 tie; micro-probed) | 101 |
| 00a22e10 | MakePerpendicular | 2 | second `mov esi,17Ch` one slot late (scheduler tie) | 111 |
| 006afe60 | CTCPhysicsBase::RespondToCollision | 2 | velocity param load one slot late (may-alias; Ego_r const signature same bytes) | 112 |
| 00d18df0 | movie-intro Main | 2 sites | inline budget of the implicit MovieBase dtor (lane-level) | 1 |
| 005e78f0 | CTCQuestCompletionUI::GlobalUpdate | 4 | x87/int interleave in the digit-count block (references grow the body) | 126 |
| 00c52bb0 | GetAMDModelName | 4 | `cmp eax,4; jae` v `cmp eax,3; ja` register tie | 254 |
| 0065b142 | EnableVillagerDefTypes | 4 | ParentThing load after `push eax` (inline accessor shrinks body) | 113 |
| 00b92300 | CreateOcclusionGroup | 4 | prologue `mov ebp,ecx`/`push edi` order (landed sibling bodies same) | 110 |
| 00f36a30 | NFractal::CHeteroTerrainFractal::GetFractalHeight | 4 | counter reload after `fmul [edi]`; PDB-true spelling installed (same bytes); PerlinNoise stand-in body worse | 126 |
| 009fa780 | Blt | 4 | bookmark test/or register swap (lane gap GOTCHAS 2026-11-03) | 111 |
| 00986ed0 | filter ctor | 8 | two fmul operand orders (not textual; micro-probed) | 111 |
| 0085c7d0 | CMoveOnCylinder::OnInit | 8 | vptr/lea register pair eax/edx (same-TU CPhysicsLeaf ctor body same) | 112 |
| 00cfe030 | COracleTablet::Main | 8 | create-site `[edi+ecx]` v `[ecx+edi]` SIB order (14 index forms) | 141 |
| 00e76e90 | HeroFlashesLamp | 10 | shared ~CTimer exit block placed at the end of recursion copy 2 | 111 |
| 00de8370 | CRansommer::Main | 12 | two rotated tests `lea ecx,[eax+0ACh]` v `add ecx,0ACh` (references change layout) | 114 |
| 00b5a3f0 | ApplyColourFilter | 7/15 | RMW `or [eax+3D8h],ecx` v load/or/store + nop (count/position effect, chunk 832) | 111 |

Header debt: none (hdr/ empty; 006a9690 uses the repo `engine/C3DVector.h` as is). SIBLINGS.tsv has no rows for the same reason.
Descriptive names kept (no PDB spelling exists or none proven): catalog labels of 0x0047ED76 / 0x00C52BB0 / 0x00986ED0 / 0x009FA780
rows as they came in; everything in the three matched rows is PDB / reloc_bindings-proven.
