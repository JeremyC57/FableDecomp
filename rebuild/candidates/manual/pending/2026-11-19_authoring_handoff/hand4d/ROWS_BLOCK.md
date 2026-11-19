1. 0x00540320 CEditBox::KeyPressed (755 bytes; extent scan 755; best DIFFER(755v755); residue: DIFFER(755v755) -- 14 lines, all register/encoding: the three vtable loads take eax where retail takes edx (and back), and the home-key scroll `i - len - 5` is `sub edi,5` where retail has `add edi,-5)
   ref/00540320/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
2. 0x0063F7C0 CThingSearchTools::ForEachInRoughArea<CThingPhysical,CFilter_Bind3<CThingFilter_IsNot,CThingFilter_HasNavigat (769 bytes; extent scan 785; best DIFFER(769v769); residue: DIFFER(785v785) -- residue: node-loop exit reload order only (retail reloads edx first); ForEachInRoughArea with CFilter_Bind3 reference members and the inline OverlappingBoundingBox CAABox Grade: DIF)
   ref/0063f7c0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
3. 0x00727220 CApplyAdditionalImpulseIfInCombatAndTooClose::operator() (732 bytes; extent scan 732; best DIFFER(732v732); residue: DIFFER(732v732) -- structure and frame right; left: x87 operand order on the XY offset's Y (VC7.1 keeps the dy CSE copy at +0x10 and reads it first in the length and the two scalings) and the owner-cr)
   ref/00727220/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt, egor_object.txt
4. 0x00774D20 _global::CTCShop_SellHighlightedItem (1149 bytes; extent scan 1149; best DIFFER(1149v1149); residue: )
   ref/00774d20/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt
5. 0x008185B0 NMaths::GetRootsNumerically<CVeryRandomFunctionToFindThePointOnAnElipseThatIsClosestToAPoint> (1360 bytes; extent scan 1360; best DIFFER(1360v1360); residue: )
   ref/008185b0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
6. 0x0085B2A0 CThingSearchTools::GetPBestThingInRoughArea<CFilter_Bind3<CThingFilter_IsAICreatureAwareOf,CThingFilter_IsUnc (775 bytes; extent scan 775; best DIFFER(775v775); residue: DIFFER(775v775) -- residue: node-loop exit reload order -- retail reloads the map iterator after `this` (or the spill VC reloads first differs); the SIB order is fixed by the 007FCEF0 recipe; also the)
   ref/0085b2a0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
7. 0x008DD7E0 CModeThunderLightningStorm::UpdateDamage (744 bytes; extent scan 744; best DIFFER(744v744); residue: DIFFER(744v744) -- one store slot: the box MinX store (mov [esp+34h],ecx) lands after add ebp,eax; retail has it right after mov edx,esi. Corner objects + inline GetParentThing (advisor C1) closed the)
   ref/008dd7e0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
8. 0x00A58130 _global::CPSCSplineBase_GenerateCurvePoints (1272 bytes; extent scan 1272; best DIFFER(1272v1272); residue: )
   ref/00a58130/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt
9. 0x00A8F1B0 _global::TransformVertexBufferWithNormals_LoopUnrolled (1469 bytes; extent scan 1469; best DIFFER(1469v1469); residue: )
   ref/00a8f1b0/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt
10. 0x00AEDE60 CParticleEmitter::SetOrientation (796 bytes; extent scan 796; best DIFFER(796v796); residue: DIFFER(796v796) -- x87 commutative operand order only: VC7.1 canonicalises every fmul pair in the inlined SetInverseNoTranslate (and the Right.Z cross term) to the mirror of retail regardless of sourc)
   ref/00aede60/: retail_dis.txt, draft, prior_*.cpp, prior_notes.txt, parked_index.txt, egod_locals.txt
