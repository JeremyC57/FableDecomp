# 2026-11-12 landing ed -- chunk F3: CreatePPhysicsMeshInfo matched (the first Fable rapid-fire landing)

Landed (RELOCATION_MATCH, test PASS under VC7.1, LANDVERIFY_STRICT_HEADERS=1, evidence verifier ok): 0x006B05A0
CTCPhysicsBase::CreatePPhysicsMeshInfo -- open since chunk Q28 (110v110, one push / mov order at two virtual calls).
Header: CTCPhysicsBase.h CREATEPPHYSICSMESHINFO (a protected non-virtual declaration; additions only, default=same).
Held for the owner: 0x008031E0 (matches, but with an inferred dead test; see `pending/2026-11-12_linux_handF3/`).
Checks: reserved_check 0; bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
Verifier observation (queued land1011ed): the landed 5-byte jmp thunk 0x006B08E0 is labelled CreatePPhysicsMeshInfo, but Ego_r names
the matching thunk SetPhysicsMeshInfoFromGraphic (public).
