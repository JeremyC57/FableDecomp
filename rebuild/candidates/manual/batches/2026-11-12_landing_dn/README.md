# 2026-11-12 landing dn -- chunk Q22: 2 rows landed, 1 held back

Source: `pending/2026-11-12_linux_handQ22/`. Landed (RELOCATION_MATCH, test PASS under VC7.1): 0x0096CD70
CAIStateGroup_EatAtTable::Update, 0x00A78B20 CNavQuadTree::SaveToFile. Held back: 0x006C1250 (a match on local models of header
classes; chunk Q23 ports it).

Headers (8, three-way merged against 381bf2557, all unchanged upstream, additions only; hdrcheck2 default=same, every DIFF macro new):
CAIStateGroupBase PERFORMACTIONONCE, CTCTavernTable SEATS, NCreatureMode_CModeSitFloor CMODESITTAVERNSTOOL, CNavQuadTree and
CANavQuadTreeNode SAVETOFILE, CAFile WRITEVAR, NPlayerGui_CDrawQuestInfo UPDATEMINIGAME, NPlayerGui_CDrawBase SETASDRAWING.

Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
**RELOC_CONFLICT (queued land1011dn):** 0x0096CD70 binds `??0?$CCountedPointer@VCModeSitBase@NCreatureMode@@@@QAE@PAV12@@Z` to
0x0094B3A0 and the dtor to 0x008AB320, while the landed 0x0093B1D0 binds the same symbols to 0x008AB380 / 0x008AB3D0. One symbol
has one address, so one of the two rows names the wrong template argument.
Also queued: 0x0063F150 / 0x0063F1E0 (out-of-line CFilter_Bind2 / Bind3 operator()<CThingPhysical>, landed under made-up names),
0x00A12890 CPhysicsMeshObject::IsPointContainedInBoundingBox (a propagated manifest label).
