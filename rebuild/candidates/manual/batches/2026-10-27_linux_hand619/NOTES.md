# Hand chunk 619 (CCreatureAction_ScorpionSummon .. CPhysicsMesh): 1 of 10 byte-exact

Parked-row retry chunk (10 rows, every best attempt already the retail length; CHUNK_BRIEF.md). Each row was re-graded from its
best repo attempt (probe/*_0.cpp), respelled toward the PDB names, then given up to four more grades. The work was split
over two agent runs (the first was cut off before writing these files); every differ/ source was re-graded at the end of the
second run (tmp/dg/*.out) and its grade line matches the result.

## What matched (1)
- CPlane (1): 0x00A42470 `CPlane::GetIntersectionWithTriangle(const C3DVector&, const C3DVector&, const C3DVector&, C3DVector&, C3DVector&) const`
  RELOCATION_MATCH (750 bytes), test PASS under g++ and VC7.1, vland RELOCATION_MATCH/PASS. Finds the segment where the plane
  cuts a triangle (the lone vertex on its side, intersected along its two edges via GetIntersectionWithLine 0x00A422C0 x2).
  Shape lesson: the third plane distance dc is a stepwise Z-first sum through a reference alias `const C3DVector& n = Normal;` --
  the alias makes VC7.1 load c.Z before Normal.Z (`fld [edi+8] ; fmul [esi+8]`); every direct `Normal.Z` spelling loaded the
  member first (the 14-line residue of the retry sweep). Class keys V (`ABVC3DVector`) make the PDB leaf bind.

## Parked for Fable 5.1 (9)
- 008c4fd0 CCreatureAction_ScorpionSummon::CreateBabies DIFFER(723v722): frame words rotated (retail family def pointer 0x10 shared with the name string, tools/boss 0x14, familyIndex 0x18; ours tools 0x10, familyIndex 0x14, family 0x18) and the CThingAICreatureInit default-argument temporaries in other slots, +1 byte | tried: private AAE leaf (binds), positions declared first (worse), tools declared before the name block (identical), engine/CDefPointer.h (732 bytes) | next: respell the callees to their PDB symbols first (CCharString is still `struct`, several callees descriptive) -- the real CDefPointer/CThingAICreatureInit types may move the frame; else lane gap: frame-slot assignment
- 0066a1e0 CThingSearchTools::ForEachInRoughArea<CThingPhysical, CFilter_Bind3<...>, CThingFindHeightZBelowAt> DIFFER(725v725): loop-exit spill reloads (node loop: retail edx,esi,edi,ebp, ours esi,edi,ebp,edx; map loop: retail esi,edi, ours edi,esi) | tried: it before maps, named box longs, named width, `long x = box.TLX` hoisted above the cell (identical), inlined protected GetMapwhoArea (718v725), landed-twin 0x0075BC40 offset/cellsOffset row form (SIB order regresses) | next: lane gap: spill reload order (the original non-PDB model probe/0066a1e0_0.cpp with dead offset locals gets ND 4 with the edx/esi order right -- variable numbering is the lever, no PDB-shaped form found)
- 005542f0 NUISystem::CMorphingSprite::GetDataFromDef DIFFER(727v727): pair temporary first/second stores scheduled around the insert pushes, eax/edx v ecx/edx colouring in the two inlined CResource::Touch copies and the frame-data release | tried: PDB respell (CUIDef::States, CUIStateDef::GraphicIndex, CManager::GetGraphicsBank, PeekGraphic/PeekGraphicInfoFrame), operator[] via begin(), named Touch head, make_pair/value_type/by-value pair; this pass 2 more probes (worse, ND 50) | next: model CResource::Touch's list insert through the real CResourceList helper if the PDB has one, else lane gap: register colouring
- 009ce5e0 CBankFile::AddEntryMultipleFiles DIFFER(741v741): symbol in ebp / index in ebx (retail ebx / ebp) and the first-branch filename / "" temporary / resize zero temporaries in rotated frame words | tried: class keys (full leaf binds), OldSymbolIndices as the real STLport map with inline _M_find, CArray<T> : vector<T>, nested CRuntimeData, per-branch index locals (identical) | next: lane gap: register/frame assignment
- 0047e651 CTCInGameMenu::VideoOptionsChanged DIFFER(752v752): `push ecx` (ChangePosition float slot) before the X `fsubp`, retail after | tried: all chunk-537 forms (PDB names, same-TU noinline getters, named temporary C2DVector, dialog local, named x/y, `/ 2.0f`, negation, `-=`), const parameters | next: lane gap: scheduling (shared with twins 0x0047E9AE / 0x0047F30F)
- 005d8978 CPersistContext::TransferMap<long, unsigned long> DIFFER(754v754): frame slots of the discarded insert result / value / ";" temporary / start, and the value traits slot (retail homes both traits objects in the dead map argument slot [ebp+0Ch]) | tried: case orders 4-3-2-1 / 2-3-1-4, traits temporary, the real insert_unique pair<iterator,bool> result (+4 frame), user CPersistTraits ctor/dtor (+0xC) | next: lane gap: several short-lived locals homed in one dead argument slot (chunk 589 family). Fixed this pass: mode 2 = GFSerialiseMapBinaryIn(PLoadStream) 0x0045C95A, mode 3 = GFSerialiseMapBinaryOut(PSaveStream) 0x0045C91D (comment had them swapped)
- 0055d180 CTCPhotojournalCapture::TakeQuickScreenshot DIFFER(772v772, /GS): retail gives the PlaySound CCharString temporary its own slot [esp+18h] where ours shares a wide-string temp slot, frame 0x270 v 0x274 (every offset above it 4 lower) | tried: decoded path chain, profile reference, CDiskFileWin32 PDB model + inline dtor, singleton pointer local, named sound local | next: lane gap: temporary slot sharing
- 007b0360 CTCFishingSpot::FrameUpdate DIFFER(773v773): block 2 only colours the particle pointer ebx / Create result esi (retail esi / ebx; blocks 1 and 3 right) | tried: thing-before-emitter, CCharString temporaries, explicit blocks, one emitter for all blocks, pointer parameter | next: lane gap: register colouring
- 00a18b50 CPhysicsMesh::HasCollidedWithTriangleAt DIFFER(783v783): frame-slot order of step / to / edge1 / edge2 (ours 0xc higher for the edges) | tried: private ABE leaf (binds), step declared first, to from step, reversed accumulate, 5 more forms in this chunk (all longer: 767-815 bytes) | next: lane gap: frame-slot order

## Skipped
None (no row holds two functions).

## Names and evidence
- 00a42470: Ego_r.pdb `?GetIntersectionWithTriangle@CPlane@@QBE_NABVC3DVector@@00AAV2@1@Z` (leaf confirmed with objdump), callee
  GetIntersectionWithLine 0x00A422C0.
- 005542f0, 007b0360, 009ce5e0, 0066a1e0, 008c4fd0 (own symbol), 00a18b50: own symbols from Ego_r/FableWin name sets as recorded
  in each differ header's Identity/Leaf line; all leaves bind.
- Name debt in parked sources (fix before they can land even if the bytes come): 008c4fd0 still declares `struct CCharString`
  and descriptive callee spellings; 005d8978 models 0x0045CA09 as `map::insert` returning an iterator (the real symbol is the
  `_Rb_tree::insert_unique` in NAME_FIXES.tsv; spelling it right costs +4 frame bytes); 005542f0 keeps a polymorphic local model
  (header debt noted in its comment).
- NAME_FIXES.tsv: six landed spellings proven wrong (0x0049C6F0, 0x0049C700, 0x0045C95A, 0x0045C91D, 0x0045CA09, 0x009D2DD0).

## Label corrections
- 0x005D8978 catalog `TransferMap<long,long>` -> `CPersistContext::TransferMap<long, unsigned long>` (Ego_r.pdb spelling).
- 0x00A18B50 catalog `global Collision_TestTriangleSphere` -> `CPhysicsMesh::HasCollidedWithTriangleAt` (private const, FableWin.pdb name set; 7 arguments, `ret 0x1c`).
- 0x005542F0 catalog `CMorphingSprite` -> `NUISystem::CMorphingSprite`.
- 0x0066A1E0 catalog module `ForEachInRoughArea<...>` -> class `CThingSearchTools` (const member template, Ego_r.pdb).

## Header debt
- 005542f0: CMorphingSprite / CUIDef modelled locally (polymorphic local model), see the differ comment.
- 0066a1e0: CThingSearchTools modelled locally like the landed 0x0075BC40 (generated header carries no member template).
