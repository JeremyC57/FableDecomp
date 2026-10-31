# Hand chunk 647 (CTCInventoryTrade .. CEngineInternalAttachedPrimitive): 0 of 10 byte-exact

All ten rows were parked rows from the 2026-10-30/31 retry sweep. Each already had a best attempt at retail length. Every
best attempt reproduced its recorded grade first. Each row then got at least two new one-lever probes; the probes are in
`probe/`. Grades were compared with a relocation-masked diff (`bin/md.py <q.py output>`), because q.py's own diff lists every
relocated operand.

## What matched (0)
- None. Two rows moved a long way and are the best candidates for the next session:
  - 0x007E8A40 (128 -> 18 masked diff lines). The three projections written as one `C3DVector` local fix retail's 0x3C frame.
  - 0x0062BEF0. The positive nested-if form puts the camera out-local in retail's frame slot (probe `probe/p6_nested.cpp`).

## Parked for Fable 5.1 (10)
- 005a887d CTCInventoryTrade::RefreshBuyPossibilityPC DIFFER(480v480): `mov dword ptr [ebp-20h],3` (deletion.Method) sits between `mov eax,esp` and the iterator-slot store in ours; retail has it last, after `lea ecx,[ebp-1Ch]` | tried: comma-this on push_back, named end() iterator before the store, comma-this on a direct insert() call, STLport 4.6 `_List_iterator_base` iterator (all four byte-identical: the scheduler places the store wherever the source puts it); access/class keys respelled to PDB (`AAEXXZ`) | next: lane gap: store scheduling (the store is free to move; no source position changes it)
- 0047d296 CTCInGameMenu::PCSetQuitToFrontEnd DIFFER(482v482): `push ecx` (float slot) before the x `fsubp` (retail after), AND in the third observer block retail loads `mov edi,[eax]` before `mov edx,[g_ActiveMousePointer]` (ours after; the brief did not record this second swap) | tried: named x/y + copy-initialised vector (frame -8), default depth/flag arguments, swapped extern order of the two globals, a same-TU preceding function | next: lane gap: scheduler tie-break (see 0047dd9d: the twin's retail and ours swap the third block the opposite way)
- 007e8a40 CTCFlourishTarget::ProcessActionMovementDelta DIFFER(482v482): frame and all integer code now match. Left: forward dot Z,Y,X (retail Y,Z,X); side dot Z(st),Y,X (retail X,Z(st),Y); the third product of `forward * local.X` loads forward.Z first (retail local.X) | tried: C3DVector local of the three projections (frame fixed), member-wise local, `delta.Dot(v)`, `f * X` operator*, named forward/side floats feeding the local; PDB respell (const, `C3DVector&`, AccurateNormalise, GetPos, CIntelligentPointer<CThing>) | next: build the frame as a CMatrix3x3/3x4 (rows forward/side/up) and transform delta with the PDB `operator*`; else lane gap: x87 operand order
- 0062bef0 CWorld::EAUseProjectileWeapon DIFFER(483v483): camera fetch `mov ecx,[this] ; mov ecx,[ecx+34h] ; mov edx,[ecx] ; lea eax,&camera` (ours edx/ecx/eax/edx), camera out-local shares the `this` spill (+4) where retail shares the ranged spill (+0), `lea eax` v `lea ecx` for the 0x48 key | tried: named CTCWeapon local (same), positive nested-if body (camera slot fixed, but the camera load moves ahead of the CCharString push; 22 lines v 20), __forceinline CWorld camera helper returning a reference / a pointer (the CCharString is built before the fetch: worse); access/class keys respelled to PDB | next: nested form plus the camera source read through `PCurrentGameCamera` typed CAIGameCameraBase* (CWorld.h +0x34) with GetCameraManager declared on it
- 00687540 CMessageEventManager::AddMessage DIFFER(486v486): esi/edi swap (retail: vtable constant 0x01238C6C and later the target in edi, originator in esi); faction `mov eax,[eax+4] ; mov edx,[eax+18h]` (ours edx/eax); list head `mov eax,[ebx+4]` (ours ecx) | tried: PDB parameter kinds and class keys (`const C3DVector&`, protected `CBaseObject` ctor/dtor, `CWorld::GetFrame`), named faction-member local, the if/else-if `sel` interface lookup (all identical) | next: lane gap: callee-saved colouring of three overlapping webs (the originator must be coloured before the constant)
- 0047dd9d CTCInGameMenu::PCSetToLoad DIFFER(493v493): the same `push ecx`/`fsubp` swap as 0047d296, and the third observer block ours `mov edi,[eax]` first (retail global first: the mirror image of 0047d296) | tried: named x/y + copy-initialised vector, default depth/flag arguments | next: lane gap: scheduler tie-break (family 0047d296 0047e651 0047e9ae 0047f30f 00576c4f 00576ed1)
- 006d68d0 CTCHeroExperience::IsBlockRecoilAnimToConserveAnimationHandedness DIFFER(495v495): `mov [esp+0Ch],eax` (clamped stat-5 max) after `fmul [0.3f]` in ours, before in retail | tried: GetFraction without __forceinline (not inlined, 187 bytes), static Fraction(level, maxLevel) helper (450 bytes) | next: lane gap: scheduling (the last two waves tried 9 forms)
- 006d6cb0 CTCHeroExperience::IsToAutoBlock DIFFER(495v495): the same swap as 006d68d0 | tried: `*(TrainableStatLevels.begin() + stat)`, `abilities.size() - 1` (both byte-identical) | next: lane gap: scheduling
- 00a90260 C3DPrimitive2::CreateDegenerateTriangleIndexBuffer DIFFER(496v496): colouring across the body: totalIndices edi v ebx, the loop-1 block pointer `lea esi,[ebx+edx]` v `add edx,edi`, the flag `cmp byte ptr [reg+9],0` v `mov bl,[..] ; test bl,bl` in the copy and clear loops, the loop-2 alignment `jmp` missing, the reset storing edi directly where retail copies through eax | tried: boost scoped_ptr reset (swap with a temporary: identical), per-loop index variables + `StaticBlocks[i + 1].StartIndex` (120 lines, worse); access respelled (private `AAE_NXZ`) | next: CStaticBlock flag as the real PDB type (bitfield/enum?) and C3DPrimitive2's real CArray<CStaticBlock> for StaticBlocks (begin/size accessors)
- 00b396e0 CEngineInternalAttachedPrimitive::Update DIFFER(497v497): `lea ebx,[esi+48h] ; push ebx ... push ebx` (retail) v ebp | tried: PDB spellings (AddPrimitive by reference, const RedoMatrixConversion(CMatrix3x4&), g_pEnginePrimitiveTypeInfo->PrimitiveManagers, class keys): identical; no `output` local: 501 bytes | next: lane gap: callee-saved colour choice between two free registers

## Skipped
- None (no row holds two functions).

## Names and their evidence
- 0x006D68D0 = `?IsBlockRecoilAnimToConserveAnimationHandedness@CTCHeroExperience@@QBE_NXZ` and 0x006D6CB0 = `?IsToAutoBlock@CTCHeroExperience@@QBE_NXZ`.
  Evidence: the Ego_r.pdb order (Handedness, ComboChain, IsToAutoBlock), the retail order (0x6D68D0, 0x6D6AC0, 0x6D6CB0), and the
  thresholds each body compares (+0x10C/+0x110 MaintainHandedness, +0x104/+0x108 AutoBlock).
- 0x005A887D = `?RefreshBuyPossibilityPC@CTCInventoryTrade@@AAEXXZ` (private). 0x00A90260 = `?CreateDegenerateTriangleIndexBuffer@C3DPrimitive2@@AAE_NXZ` (private).
  0x0062BEF0 = `?EAUseProjectileWeapon@CWorld@@QAEXPBVCGameEvent@@@Z` (CGameEvent is a class).
  0x007E8A40 = `?ProcessActionMovementDelta@CTCFlourishTarget@@QBEXAAVC3DVector@@@Z` (const, by reference).
  0x00687540 = `?AddMessage@CMessageEventManager@@QAEPAVCMessageEvent@@W4EMessageEventType@@KABVC3DVector@@PAVCThing@@2MMJ@Z`.
  0x00B396E0 = `?Update@CEngineInternalAttachedPrimitive@@QAE_NABVCEnginePrimitive@@ABVCEnginePrimitiveAttachmentInfo@@ABVCEnginePrimitiveHandle@@JK@Z`.
  Every differ/ source now carries the full mangled Leaf. objdump confirmed each one.
- Descriptive names that remain in the parked sources: the CTCFlourishTarget flags (+0x14/+0x15), the 0x49 interface `CTCFactionMember`,
  the `v00`-style vtable fillers, and the AddMessage list model `CMessageList`/`SMessageNode`. The 005a887d model carries a same-TU
  stand-in `list::insert` body with an invented allocator `AllocateListNode` (inherited). The earlier sweep found that
  declaring insert without a body grades the same.

## Label corrections
- 0x006D68D0 catalog "IsBlockRecoilAnimToConserveAnimationComboChain" -> IsBlockRecoilAnimToConserveAnimationHandedness.
- 0x006D6CB0 catalog "IsBlockRecoilAnimToConserveAnimationComboChain" -> IsToAutoBlock.
- The real ComboChain body is the uncatalogued 0x006D6AC0, which compares +0x114/+0x118.
- 0x005A887D catalog "global::PC_PopulateBuyList" -> CTCInventoryTrade::RefreshBuyPossibilityPC.
- 0x0049D870 is CWorld::GetFrame (see NAME_FIXES.tsv). It is not GetQuakeFrame, and not "CWorld::GetWorldFrame" as the chunk-181 GOTCHAS line says.

## Header debt
- 0x00B396E0, 0x007E8A40, 0x00687540, 0x0062BEF0 and 0x00A90260 use local models.
  - CWorld.h, CTCHeroExperience.h and CTCInGameMenu.h exist but carry Ego_r container sizes (inherited reasoning; see each file's Layout line).
  - No engine header exists for CEngineInternalAttachedPrimitive or C3DPrimitive2.
- CWorld +0x34 is `PCurrentGameCamera` (CAIGameCameraBase*) in engine/CWorld.h. The 0x0062BEF0 model still calls it `m_pCameraSource` with a
  local `CCameraManagerSource` class (inherited from the landed 0x0075C5F0); its slot-9 GetCameraManager is CAIGameCameraBase::GetCameraManager (0x006970C0).
