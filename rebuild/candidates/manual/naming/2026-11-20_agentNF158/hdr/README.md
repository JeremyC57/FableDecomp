# NF158 header overlay (2026-11-20)

Base copies of every overlaid header are in `../orig/` (copied from the repo working tree at the start of NF158). Every change is an
`#ifdef` / `#ifndef` opt-in with a member-specific macro name that no landed row defines; without the macro each header is
token-identical to its base. `pin_asserts.py` was run on each header after editing: it pinned 0 asserts (all asserts in these headers
are already `FABLE_STATIC_ASSERT_AT(<line>, ...)`, and none moved).

## engine/CPSCBase.h
- `FABLE_RETAIL_CPARTICLEUPDATEPARAMCALLBACK_IMPLICIT_DTOR`: omits the declared `virtual ~CParticleUpdateParamCallback();` so the
  implicit destructor is used (no vptr store of its own; retail ~CPSCUpdateNormal 0x00B0CA90 / ??_G 0x00B0F990 chain straight to
  ~CBaseClassNonCopyable 0x0099A300; the landed CPSCAttractor 0x00B18780 chain is the same).
- `FABLE_RETAIL_CPSCBASE_IMPLICIT_DTOR`: the same for `virtual ~CPSCBase();`.
- Users: rows/00b0f990.cpp, rows/00b0ca90.cpp.

## engine/CPSCUpdateNormal.h
- `FABLE_RETAIL_CPSCUPDATENORMAL_DTOR_INLINE`: adds the inline `virtual ~CPSCUpdateNormal() { KillAllDecalEmitters(); }` (Ego_r.pdb
  ??1CPSCUpdateNormal@NParticleEngine@@UAE@XZ, retail body 0x00B0CA90) and the protected `KillAllDecalEmitters()` declaration
  (Ego_r + FableWin `?KillAllDecalEmitters@CPSCUpdateNormal@NParticleEngine@@IAEXXZ`, 0x00B0C730). Plain `inline` (no FABLE_FORCEINLINE).
- Users: rows/00b0f990.cpp, rows/00b0ca90.cpp.

## engine/CTCInventoryWeapons.h
- `FABLE_RETAIL_CTCINVENTORYWEAPONS_REMOVEALLCONFISCATEDITEMS`: declares the slot-74 override `virtual void RemoveAllConfiscatedItems();`
  (vtable 0x01253FF4, 0x005C2E68; Ego_r / FableWin `?RemoveAllConfiscatedItems@CTCInventoryWeapons@@UAEXXZ`). Declaration only, no
  layout change. Placed before the VSLOTS block, with its own `public:` labels.
- Users: rows/005c2e68.cpp. Landed includers: 49 rows; none defines the macro, so none changes.

## engine/CParticleEmitterManager.h
- `FABLE_RETAIL_CPARTICLEEMITTERMANAGER_SETENABLEDECALS`: inline `void SetEnableDecals(bool)` (FableWin
  `?SetEnableDecals@CParticleEmitterManager@NParticleEngine@@QAEX_N@Z`; expanded in retail 0x0088F4A0). Plain inline.
- (Resume session) The first session's `FABLE_RETAIL_CPARTICLEEMITTERMANAGER_GETMANAGER` opt-in is withdrawn: it repeated the existing
  `FABLE_RETAIL_CPARTICLEEMITTERMANAGER_BULLETTIME_PASS23` declaration of `NParticleEngine::GetParticleEmitterManager()`, which
  0x0088F4A0 and 0x00B09360 now define instead (as the landed 0x00B012E0 does).
- `FABLE_RETAIL_CPARTICLEEMITTERMANAGER_INTERPOLATED_ONLY` (resume session): a declaration-only partial mode placed BEFORE the include
  guard (`#if defined(..._INTERPOLATED_ONLY) && !defined(FABLE_ENGINE_CPARTICLEEMITTERMANAGER_H)`): it defines the guard, includes only
  rebuild_abi.h, forward-declares C3DVector / CRightHandedSet / NParticleEngine::CParticleEmitter, and declares the class with the
  one member CreateInterpolatedEmitter(long, long, float, const C3DVector&, const CRightHandedSet&, bool) (0x00AEB8C0, Ego_r + FableWin)
  plus the free accessor GetParticleEmitterManager() (0x00AEAAF0). For row 0x006E11F0, which models its vectors and counted pointer in
  the stand-in namespace X_006e11f0 and so cannot take the full header (precedent: FABLE_RETAIL_CTCDPARTICLEEMITTER_METHODS_ONLY,
  FABLE_RETAIL_NTHINGMAP_LAYERINFO_ONLY). Without the macro the block is skipped: `cpp` of a file including the header gives
  byte-identical token streams for orig/ and hdr/ (checked). pin_asserts.py: pinned 0 (the size assert is already
  FABLE_STATIC_ASSERT_AT(33, ...)).
- Users: rows/0088f4a0.cpp (SETENABLEDECALS + existing PASS23); rows/006e11f0.cpp (INTERPOLATED_ONLY); rows/00b09360.cpp uses only existing macros (PASS23, DEFAULTBLEND,
  DEFAULTALIGNMENT). Landed includers: 19 rows; none defines SETENABLEDECALS or INTERPOLATED_ONLY, so none changes.

## Landed includers
- engine/CPSCBase.h: 12 landed rows include it directly (more through other headers); engine/CPSCUpdateNormal.h: 2
  (0x00B0CAD0, 0x00B0D360). None defines any NF158 macro, so the opt-ins change none of them (default path token-identical).
