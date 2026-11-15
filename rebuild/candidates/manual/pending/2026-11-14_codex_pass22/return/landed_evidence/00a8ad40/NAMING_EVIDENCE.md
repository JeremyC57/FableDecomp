# C3DMesh2::LoadBinary 00A8AD40

Exact leaf: `?LoadBinary@C3DMesh2@@QAE_NAAVCMemoryDataInputStream@@PAVCMovableResourceMemoryPool@@PBVCGraphicDataBank@@_N@Z`. Both original PDB public sets and concrete debug procedure (03366280, module lib_3d_mesh_2.obj,3150bytes) prove the public member and argument identities. Retail caller evidence is XREFS.txt; actual donor locals are ORIGINAL_LOCALS.json. Manifest alone was never used as proof.

Every direct call is proved by both PDB exact symbols, matching donor call role and the retail operand. RELOCATION_AUDIT.json checks all24operands,10distinctcallees, with no correct symbol already bound to another address. InitialisePointers is private AAEXKK and InitialiseBoneNamePointers private AAEXK, not the public/boolean guesses in older neighbouring drafts. Stats MemoryPool is protected. Three old relocation spellings are listed in NAME_FIXES.tsv; no global binding file changed.

Original locals include lock (CScopedLockFlagBool), static_block_count and anim_block_count (unsigned long), the three signed64 overflow checks, three unsigned sizes, current_bone_name (char*), c_bone/c_material/c_prim and primitive destination. Fields and class keys are recorded in both original type JSONs. Native layout has an8byte shorter resource prefix and4byte smaller material than later donor;24compile assertions cover every accessed member and array stride. No invented production classes.

ReadCompressed virtual calls use the existing stream virtual at+0x10, consistent with original shared declarations and all four native sites. The nine unused out-of-line inline COMDAT copies are not target callees: their relevant inline expansions are already inside the1621matchedbytes. No standalone matches are claimed for those unused sections.
