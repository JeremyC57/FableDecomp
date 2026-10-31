# Chunk 647 -- 0 of 10 landed (2026-10-31)

The last chunk cut before the re-rank (all ten are same-length compiler ties). Every best attempt was regraded first, then at least two
new one-lever probes per row; all ten stay DIFFER in `pending/2026-10-27_linux_hand647/differ/` (residue on line 2; every differ now
carries the exact PDB spelling and a full mangled `// Leaf:` line, objdump-confirmed): 005a887d CTCInventoryTrade::RefreshBuyPossibilityPC
(catalog "PC_PopulateBuyList"; the `deletion.Method = 3` store is placed where the source puts it -- drop the inherited same-file
list::insert stub and the invented AllocateListNode before any landing), 0047d296 / 0047dd9d CTCInGameMenu PCSetQuitToFrontEnd / PCSetToLoad
(float-slot `push ecx` before `fsubp`, and the third observer block's vtable/global order mirrored between the twins), 007e8a40
CTCFlourishTarget::ProcessActionMovementDelta (frame fixed 0x3C by a C3DVector local holding the three projections; x87 order of the two dot
products left -- 18 diff lines from 128, retry first), 0062bef0 CWorld::EAUseProjectileWeapon (camera out-local in the `this` spill slot;
the nested positive-if form moves it to retail's slot but loads too early), 00687540 CMessageEventManager::AddMessage esi/edi swap,
006d68d0 / 006d6cb0 CTCHeroExperience IsBlockRecoilAnimToConserveAnimationHandedness / IsToAutoBlock (catalogued as "...ComboChain"; the
real ComboChain body 0x006D6AC0 is not in the catalog) clamp store / `fmul` swap, 00a90260 C3DPrimitive2::CreateDegenerateTriangleIndexBuffer
whole-body allocation, 00b396e0 CEngineInternalAttachedPrimitive::Update transform address in ebp not ebx. Lessons: three float projections
retail keeps in adjacent slots are one C3DVector local; the x87 order of a dot product follows where the result is stored; a store to a
local struct member is scheduled where the source puts it; twins can mirror the same scheduling choice (record both swaps); the nested
positive-if form moves an address-taken out-local to a different dead spill slot; PDB respelling changed no bytes in 5 of 5 rows (do it
for the names, not as a codegen lever). 10 NAME_FIXES + 4 review rows queued (prefix c647): CWorld::GetFrame 0x0049D870 static fastcall
(landed 0x006D6770 declares it on a CWorld_Calls struct), CEnginePrimitiveManager::AddPrimitive, the CBaseIntelligentPointer ctor / GetPItem
(CBaseObject*) / SetPItem / dtor, CBaseObject protected ctor/dtor, C3DVector::AccurateNormalise, CThing::GetPos const&; labels 0x006D68D0 /
0x006D6CB0 / 0x005A887D; catalog gap 0x006D6AC0; GOTCHAS chunk-181 "GetWorldFrame" label; header debt (CWorld.h / CTCHeroExperience.h /
CTCInGameMenu.h use PDB container sizes; no CEngineInternalAttachedPrimitive / C3DPrimitive2 header; CWorld+0x34 is PCurrentGameCamera).
Labels: ghidra_out/labels_chunk647_2026-10-27.tsv.
