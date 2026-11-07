# hdr/ (chunk 830)

Four headers, each copied fresh from rebuild/include and changed only by one opt-in `#ifdef` block (all new declarations inside it;
no existing line reflowed). Diffed against rebuild/include at hand-back: the repo copies are unchanged since the copy. No repo file
defines any of the four macros, so the default path is textually unchanged; g++ -E -P with and without this overlay was compared for
all 70 direct rebuild/src includers (SIBLINGS.tsv): identical apart from the __LINE__-numbered static-assert typedef name after the
CMainGameComponent / CThingManager blocks. No includer recheck by grade was needed (default path untouched).

| Header | Macro | Change | Evidence | Used by |
|---|---|---|---|---|
| engine/CMainGameComponent.h | FABLE_RETAIL_CMAINGAMECOMPONENT_GETWORLD (with FABLE_RETAIL_CMAINGAMECOMPONENT) | inline GetPlayerManager() / GetWorld() reading the scoped_ptr words +0x1C / +0x24; forward declarations of CPlayerManager / CWorld inside the same condition | Ego_r.pdb / FableWin.pdb ?GetPlayerManager@CMainGameComponent@@QAEAAVCPlayerManager@@XZ, ?GetWorld@CMainGameComponent@@QAEAAVCWorld@@XZ; retail 0x0083E220 / 0x0083E274 read them in place off 0x013B86A0 | differ/0083e1f0.cpp |
| engine/CThingManager.h | FABLE_RETAIL_CTHINGMANAGER_ALLOCATEUID | declaration `unsigned __int64 AllocateNewThingUID(long, bool, bool)` 0x0051E100 | Ego_r.pdb / FableWin.pdb ?AllocateNewThingUID@CThingManager@@QAE_KJ_N0@Z; landed body 0x0051E100 | differ/0083e1f0.cpp |
| engine/CMatrix3x4.h | FABLE_RETAIL_CMATRIX3X4_SETFROMVECTORANGLE | declaration `void SetFromVectorAndAngle(const C3DVector&, float)` 0x00A55F90 | Ego_r.pdb / FableWin.pdb ?SetFromVectorAndAngle@CMatrix3x4@@QAEXABVC3DVector@@M@Z; bound spelling, landed caller 0x00A13C60 | differ/007542b0.cpp |
| engine/CRightHandedSet.h | FABLE_RETAIL_CRIGHTHANDEDSET_INIT | declaration `void Init(const C3DVector& up, const C3DVector& forward)` 0x004BBE10 | Ego_r.pdb / FableWin.pdb ?Init@CRightHandedSet@@QAEXABVC3DVector@@0@Z (the only two-vector void member); the landed body at 0x004BBE10 (catalog name Set, descriptive) is the two 12-byte copies with `ret 8` | differ/007542b0.cpp |

An earlier CRightHandedSet opt-in (an inline Rotate written from FableWin's ?Rotate@CRightHandedSet@@QAEXABVC3DVector@@M@Z) was
dropped: the best 0x007542B0 source writes the rotations with named matrices in the caller (an inline Rotate packs both matrices
into one frame slot where retail keeps two), so no kept row needs it.
