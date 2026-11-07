# Chunk 843 header overlay

Every edit is an opt-in branch or a new header: the default path of each existing header is textually unchanged (only lines were
added; `diff rebuild/include/engine/X.h hdr/engine/X.h` shows no removed or changed line). Includers of the three edited headers see
only shifted line numbers (harmless for `__LINE__` static asserts, GOTCHAS chunk 829). Copied fresh from rebuild/include on 2026-11-07
and re-diffed before hand-back.

| header | change | evidence | path |
|---|---|---|---|
| engine/CEngineCamera.h | new nested opt-in `FABLE_RETAIL_CENGINECAMERA_GAMUT` (an `#elif` inside the existing FABLE_RETAIL_CENGINECAMERA branch, plus an `#ifdef` include block): the own members 0x0C..0x228 with their Ego_r.pdb names and offsets (CameraDesc, ViewTransform, window floats, Inverse / NoScale matrices, LookVector, FrustrumPlanes[4], debug-script members), the private `SetupGamut()` and the forceinline FableWin helpers `TransformScreenXYLinearZToView(C3DVector&) const` / `TransformViewToWorld(C3DVector&) const` (bodies from 0x00B2FD60's bytes) | Ego_r struct_layouts_egor.tsv offsets (same as the generated default); retail SetupGamut reads / writes exactly those offsets; landed ClipSphere 0x00B30480 reads WorldPosition +0x54 and FrustrumPlanes +0x1C0 | opt-in |
| engine/CMatrix3x4.h | new opt-in `FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR_SUMS` block (separate `#ifdef`, before the untouched chunk 756 TRANSFORMVECTOR block; define one of the two): TransformVector rows in the parenthesised `x*E1k+(y*E2k+z*E3k)` form | 0x00B3015A..0x00B301B1 sums E3k*z, E2k*y, E1k*x in all three rows | opt-in |
| engine/C3DVector.h | new opt-in `FABLE_RETAIL_C3DVECTOR_SUBASSIGN` (inside the FABLE_RETAIL_C3DVECTOR branch): `operator-=(const C3DVector&)` | FableWin.pdb ??ZC3DVector@@QAEABV0@ABV0@@Z (debug copy 0x01D899E0; no Ego_r / retail out-of-line copy); 0x00A86110 copies each vertex and subtracts the centre in place | opt-in |
| engine/CPlane.h | NEW header (no includer before chunk 843): Ego_r CPlane layout (Normal +0, Distance +0xC, sizeof 0x10), the three Ego_r Initialise overloads (declarations; 0x00A42140 is the (normal, distance) one) and the FableWin inline constructors (default, (normal, distance), user copy ctor ??0CPlane@@QAE@ABV0@@Z) | Ego_r / FableWin name sets; 0x00A86110 builds the by-value result in place | new |
| engine/CEngineCameraDesc.h | NEW header: Ego_r CEngineCameraDesc (struct key U) data members, sizeof 0x6C | Ego_r layout; retail reads Window +0x14 / FarZPlane +0x70 off the camera (desc at +0x14); landed RefreshD3DTransforms reads +0x6C..+0x78 | new |

Includers rechecked: none needed (no default-path change). Users of the new opt-ins: differ/00b2fd60.cpp (GAMUT, TRANSFORMVECTOR_SUMS),
differ/00a86110.cpp (SUBASSIGN). Both rows are parked, so the coordinator may hold the overlay until they land.
