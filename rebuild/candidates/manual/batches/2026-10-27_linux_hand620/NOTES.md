# Hand chunk 620 (CTexture .. CTCInventory): 0 of 10 byte-exact

Resume of an interrupted instance. Every row already had a seeded best source from earlier passes (sec/retry/chunk 537);
the first instance re-graded all ten and probed 0x00A177A0 (6 variants) and 0x00A17F80 (2). This instance added targeted
grades on 0x00A17F80 (2), 0x0085C470 (2), 0x005726A0 (2), 0x00622A00 (1), 0x0047E9AE (1), 0x009FB750 (1). No row contains a
jump table (no `jmp dword ptr [..*4]` in dis_all.txt), so q.py grades are not masking a switch. All ten rows are the same length
as retail; every residue is scheduling, register choice or frame-slot order. Residue counts below use a normaliser that masks
`push imm32` relocations against the object's `push 0` (the instance's ud.py did not, which inflated e.g. 0x005726A0 to 108 lines).

## What matched (0)
(none)

## Parked for Fable 5.1 (10)
- 009fb750 CTexture::LoadFromDataStreamToPreallocatedSurface DIFFER(789v789): the CDataInputStream& lives in ebx and the row count / total in ebp (retail: ebp / ebx), so every stream access and the inline Read's memcpy register pair differ | tried: row loop from the height local (worse), `total` sharing the height variable (`total = height; total *= rowBytes`, inert), leaf binding fixes | next: lane gap: register allocation
- 00489d40 CPlayer::CreateCharacter DIFFER(815v815): frame slots only -- pos at [esp+1Ch] / forward at [esp+28h] (retail forward 1Ch / pos 28h); the holy-site out-local and the offset temporary share the wrong partners | tried: ~30 probes (declaration orders, scope blocks, named locals, debug-branch and offset-vector forms) in earlier passes; re-graded unchanged here | next: lane gap: frame-slot
- 00a17f80 CPhysicsMesh::GetPointIntersectionFromToUsingFullCollision DIFFER(818v818): `add esi,edx` (the /6 face count) one slot early, the plane-copy `mov ecx,[esp+28h]` a slot late, one fld/fmul pair order in normal*v0, `fst [esp+1Ch]`/`fstp [eax+18h]` where retail stores data first | tried: PDB-named model (same), `nearest = data.DistanceSquared = d` (824 bytes), data-first order with found before the position copy, `data.DistanceSquared = nearest` (all inert) | next: lane gap: scheduling (twin of 0x00A177A0)
- 005726a0 CTCInventoryExperience::RecheckAvailabilities DIFFER(824v824): in both erase loops `mov [ebp-10h],1` (deletion.Method) sits above the it->second / vtable loads; retail writes it just before the CDeletion copy-ctor call | tried: 13 earlier grades (real STLport map, SetDeletion on it->second directly, component first, SetMethod in the argument); here a component local read after the ctor (store sinks past the node load and the first push, not past the vtable load: 12 lines) and the comma argument `SetDeletion((d.Method = 1, d))` (same) | next: lane gap: store scheduling past the vtable load
- 00a177a0 CPhysicsMesh::GetPointIntersectionFromToUsingSegmentation DIFFER(840v840): one `mov ecx,[esp+28h]` (normal.X copy into the plane) scheduled after the Y-scale `fld/fmul` instead of before | tried: CPlane ctor(normal, d) / ctor(normal, point) / Initialise(normal, point) / Initialise(normal, d), `v0 * normal`, no user C3DVector copy ctor, PDB-named model (all the same 4 lines or worse) | next: lane gap: scheduling
- 0085c470 NCameraModes::CMoveOnCylinder::Update DIFFER(857v857): tail eax/ecx/edx rotation -- `mov edx,[esi+44h]; fadd [edx+0D4h]` (retail eax), `lea eax/push eax` for SetGoToPosition (retail ecx), LookAt copy through ecx,edx,eax (retail edx,eax,ecx) | tried: earlier sweep (ternary GetConstTC, implicit operator=), here a named map local with `ground += MinHeightAboveGround` and a member-wise LookAt copy (inert) | next: lane gap: register rotation
- 0047e9ae CTCInGameMenu::GlobalUpdate DIFFER(860v860): `push ecx` (ChangePosition float slot) before the X `fsubp` where retail pushes it after | tried: C2DVector ctor/copy forms, temp orders, named x/y, named temporary, RTM+QFE (earlier), unnamed argument temporary here (inert) | next: lane gap: scheduling (shared with 0x0047E651 / 0x0047F30F)
- 0057df85 CTCHeroStats::KickHeroToMostRecentSafeRegion DIFFER(876v876): currentRegion / the "REGION_KICK_TO_POINT" literal / the HasDefName filter temporary at [ebp+40h]/[ebp+38h]/[ebp+3Ch] (retail 38h/3Ch/40h), the loop iterator following | tried: 11 earlier grades (declaration orders, filter forms, same-TU bodies); re-graded unchanged here | next: lane gap: frame-slot order
- 00622a00 CTCInventoryQuests::OpenInventory DIFFER(877v877): ours caches 0 in ebx for the whole body (`push ebx`, `cmp x,ebx`, `mov [..],bl`) and homes `component` in a frame slot; retail uses immediates, zeroes edi/ebp locally, keeps component in ebx and things._M_finish in ebp across the push_back loop | tried: real STLport vector templates (_Vector_base/_STLP_alloc_proxy ctor, ~vector -> _Destroy(first, last)): the destroy loop now caches its end like retail but the zero cache stays (183 lines, worse; probe/622a_r1.cpp) | next: find which zero uses retail lacks (start: the sound-group / SetEnabled `push 0`s and the CTCQuestCards null default), or lane gap: zero-register allocation
- 0058f649 CTCInventory::OpenPCInventory DIFFER(883v883): the front-end manager in [ebp-8] and the CCharString temporaries in [ebp-4] (retail swapped), PTop_Level_List address formed before the second name ctor, and the photojournal action.first = 1 store above the vtable load | tried: scope blocks, named name locals, manager declaration placements (earlier); re-graded unchanged here | next: the 0x005726A0 component-local-after-ctor lever for the action store, then lane gap: frame-slot order

## Skipped
(none) -- no row holds padding plus a second function.

## Label corrections
- 0x005726A0: catalog "CTCInventoryExperience::UpdateAbilityDisplay" is CTCInventoryExperience::RecheckAvailabilities (PDB, private void()); already a proven decision in labels_naming_wave1_2026-10-28.tsv.
- 0x0085C470: catalog "CMoveOnCylinder::Update" is NCameraModes::CMoveOnCylinder::Update (PDB namespace).
- 0x00A6CBB0 (callee): manifest "CEngineInternalPrimitiveBase::AddChildPrimitive" / landed global `int* AddChildPrimitive()` is CPhysicsMesh::GetSingleInstanceCollisionSet (private const, returns the static vector<long>&).
- 0x00A14480 (callee): manifest "C3DVector::GetScaled" is C3DVector::GetMagnitudeAndNormalise (already landed under that name).
All in NAME_FIXES.tsv except 0x00A14480 (landed spelling already right).
