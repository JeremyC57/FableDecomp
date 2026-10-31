# Hand chunk 625 (CDisplayManager .. CV_RandomPopulationSimScript): 3 of 10 byte-exact

All 10 rows were parked same-length retries (101-123 bytes). Each best attempt was regraded first, then changed one lever per grade.
Probe sources for every grade are in `probe/` (`.out` = q.py output). `bin/md.py <q.py output>` prints a diff with relocations
masked and jump targets as offsets from the function start (udiff.py does not mask relocations, so it overcounts).

## What matched (3)
- CAIStateGroup_NonVillageGuardPatrol::FindNextGuardPatrolPosition 0x0093CAB0 (RELOCATION_MATCH). It moves
  CurrentGuardpostThing to the node's successor (+0xD0), or wraps to PeekTrackStartNode(), or stays put. Shape: one whole
  `CurrentGuardpostThing = CIntelligentPointer<const CThingTrackNode>(next);` statement in each arm. VC7.1 merges the two
  identical tails. The merged block forms ecx before pushing the argument and keeps the member address in esi and the
  successor in edi. The landed sibling Update 0x0093CB20 inlines this exact per-arm body, which is the second source.
  The parked single assignment after an early return gave the esi/edi swap and the push-first order.
- CTCBuyableHouse::CreateMainSignPost 0x006C0F00 (RELOCATION_MATCH). It picks the rent sign when the house is owned and
  rented, the owned sign when it is owned, and otherwise the main for-sale sign. It creates the sign through
  CreateSignPost(name, true) and stores it in MainSignPost. Shape: the same lever with three arms, one whole
  `MainSignPost = CIntelligentPointer<CThing>(CreateSignPost(name, true));` statement per arm. The common `push 1` is
  hoisted, the three `push offset` arms meet at one call, and the merged tail runs in this-before-argument order.
  `this` comes from engine/CTCBuyableHouse.h through the `#define` include pattern. MainSignPost (opaque bytes in the
  header) is viewed with a reinterpret_cast in each arm. A reference local to it instead makes VC7.1 keep &MainSignPost
  in esi (10 lines off).
- CTCInventoryMap::GetGuildSealRecallRegion 0x005FA990 (RELOCATION_MATCH). It returns the region of the world's guild-seal
  recall position, or 0 while that position is the zero vector. Shape: take the position by const reference
  (`PeekWorld().GetGuildSealRecallPos()`, which the PDB declares as returning const C3DVector&) and use the inline
  GetSquaredMagnitude(). VC7.1 then reads Z, Y, X straight into x87 registers with X on top and squares X first. Every
  parked form copied the vector to a local (implicit copy, 3-float constructor, or a Z/Y/X body copy constructor) and
  either spilled it or squared Y first.

## Parked for Fable 5.1 (7)
- 009bee40 CDisplayManager::SetupDepthAndBackBufferTextures DIFFER(101v101): else arm `mov edx,[esi+198h]` v retail `mov ecx,...`; join `mov [esp+18h],eax ; lea eax,[esp+14h] ; push eax ... lea ecx ; push ecx` v retail `lea edx ; push edx ; mov [esp+1Ch],eax ... lea eax ; push eax` (14 masked lines) | tried: named X local (best), Y-first/X-first, class-level ternary with a C2DExtentsI(x,y) ctor, assignment arms `size = C2DExtentsI(..)` / `size = RenderTargetDimensions`, whole call per arm (133 bytes), `bool ok` per arm, GetPixelFormat() inline as the argument (96 / 88 bytes); C2DExtentsI constructors stop the X sinking (107) | next: lane gap: register choice (the sec5 "7-line" result did not reproduce with this header; the old struct/public head gives the same 14 lines here)
- 0051d870 std::vector<CNavigationAreaUpdate::CNavigationLayerUpdate>::resize(unsigned int) DIFFER(104v104): `mov ecx,[esp+14h] ... mov edi,ecx` v retail `mov edi,[esp+14h] ... mov ecx,edi` (4 masked lines) | tried: named `last` local, tag local declared first, the real STLport model (allocator, _STLP_alloc_proxy, _Vector_base/vector split, same-TU noinline `__fastcall __destroy_aux`; probe wv_1), which reloads _M_start after the __destroy_aux call (22 lines) | next: lane gap: register choice; the identity is proven, but the differ source still uses the 2026-10-15 stand-in names (CRecord24Vector/SRecord24)
- 00c0a790 CUnprovenStruct_00c0a790 ctor DIFFER(106v106): `mov edx,3F800000h` hoisted to the top v retail right before the first 1.0f store (2 masked lines) | tried: a base class holding +0/+4/+0x10 in its init list (probe wa_1, same residue) plus all sec6 levers | next: lane gap: constant-register placement (scheduling)
- 00c720c0 CAudioSource::StartExponentialFade DIFFER(111v111): `fld qword ptr [0.0]` v retail `fld dword ptr [0.0f]` (2 masked lines; compare order and after-merge pop now match) | tried: `double length; if (length != 0.0f)` (best: right order and pop, qword zero), `length != 0.0` (same), float local (right compare, but dies at the division: `fdivrp` + pop in the arm, 9 lines), `(float)ticks` twice, `==` with swapped arms, a ternary store, a `rate` local | next: a compare that keeps a double value live but loads a float zero, e.g. a float `length` that stays live past the merge, or an extern float zero (relocation identity would be wrong)
- 005c8101 CTCInventoryWeapons::PutWeaponsInSheathes DIFFER(117v117): `push ebx` at entry with the melee weapon in ebx v retail `push ebx`/`pop ebx` inside the melee block with both weapon addresses in edi; `mov ecx,[esi+4] ; add ecx,20h` v `mov eax,[esi+4] ; lea ecx,[eax+20h]` (32 masked lines) | tried: one reassigned weapon pointer local, block-scoped weapon references, plus the sec1b levers | next: lane gap: callee-saved push placement (shrink-wrapped ebx)
- 00bddf30 CLocalDetailCacheMap::CQuadTreeElement ctor DIFFER(119v119): flag-byte load `mov dl,[eax+48h]` after the children v retail before them; +0x2c store before the and/store v after (4 masked lines) | tried: bitfields in the mem-initialiser list (the load lands in retail's slot but as `and byte ptr [eax+48h],0F0h`, 114 bytes), init list + body clear (130), body order children/+4/flags | next: lane gap: scheduling (the init-list RMW form shows where retail's load comes from)
- 00ec3270 CV_RandomPopulationSimScript::GetCurrentPopulationLevel DIFFER(123v123): switch default `jne +6b` v retail `jne +56` (to the `> 1.0f` test) (2 masked lines) | tried: reference Clamp(float&, lo, hi) helper (same), GFClamp01 by value (155), a default arm that returns (131), plus the sec6 levers | next: lane gap: jump threading. Respell at landing to NScript::CV_RandomPopulationSimScript::GetCurrentPopulationLevel(POPULATION_TYPES)

## Skipped
- None.

## Label corrections
- 0x00C0A790: the catalog label CEngineWaterBackgroundSubPatch::CEngineWaterBackgroundSubPatch is propagated. The body stores
  no vtable, while the PDB class is 0x60 bytes with a vptr. The real class was not found in the PDB (searched size 92 and the
  +0x40 float / +0x44 long pair), so it is CUnprovenStruct_00c0a790.
- 0x0051D870: not ~CLODDynamicBuildInfo. It is std::vector<CNavigationAreaUpdate::CNavigationLayerUpdate>::resize(unsigned int)
  (sec1 identity; the element destructor matches the landed ??_G 0x0051C760).
- 0x00C720C0: the catalog label Math_ConvertToPowerOfTwo_or_ComputeScale is wrong. The descriptive CAudioSource::StartExponentialFade
  is kept from the landed sibling StartLinearFade 0x00C72130, which shares the same layout, clock global and __ftol2
  shape. No PDB name exists for this libpk/OpenAL class.
- 0x00686A80 is CTCBase::PeekWorld and 0x00686A70 is CTCBase::GetWorld (manifest: CViewBase::GetDisplayEngine, propagated).

## Names
- Proven (PDB name sets): FindNextGuardPatrolPosition (AAE), CurrentGuardpostThing, PeekTrackStartNode (QBE, returns const),
  CreateMainSignPost / CreateSignPost (AAE), GetGuildSealRecallRegion (ABE), PeekWorld, GetGuildSealRecallPos,
  GetSquaredMagnitude, GetRegionNumberMapIsIn (QBE), PeekWorldMap, GetMapNumberAt, and the CBaseIntelligentPointer members.
- Descriptive:
  - m_pNextNode (+0xD0): the landed sibling's spelling; the PDB's GetNextNode() is inlined here.
  - g_MainSignPostDefName / g_OwnedSignPostDefName / g_RentSignPostDefName: named from their initialiser strings.
  - g_positionEpsilon 0x0129BA3C: one of ten bound spellings; the coordinator should pick one.
  - The CWorld slots v00..v12: placeholders, as in landed sources.
- NAME_FIXES.tsv has 12 rows of landed or bound spellings that are wrong or placeholders.

## Header debt
- engine/CWorld.h flattens the vptr and GuildSealRecallPos into _X/_Y/_Z floats, so it cannot carry the PeekWorldMap virtual call.
  005fa990 keeps a local CWorld view.
- engine/CTCBuyableHouse.h declares MainSignPost / DressingSignPost / WifeLivingHere as opaque `unsigned char[8]`. Their real
  type is CIntelligentPointer<CThing>.
- engine/CWorldMap.h also flattens its vptr (`void* __vftable`), so it cannot carry the GetMapNumberAt virtual call.
  vland warns SHARED_HEADER_AVAILABLE CWorldMap on 005fa990; the local view is deliberate.
