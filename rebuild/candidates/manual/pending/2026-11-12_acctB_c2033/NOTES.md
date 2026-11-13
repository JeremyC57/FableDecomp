# Hand chunk 2033 (CGameCameraManager .. C3DAnimationSequenceData): 2 of 2 byte-exact

## What matched (2)
- **0x0069AE80 CGameCameraManager::CGameCameraManager(const CWorld&)** (1923 bytes, RELOCATION_MATCH at grade 3 of pass 1). Builds
  the default camera (`CCamera(C3DVector(0,0,0), C3DVector(1,0,0), C3DVector(0,0,1), 0.19444f, 0.0f)`), copies it into OldCamera,
  initialises the members in declaration order (STLport list / vector / map heads, counted / intelligent / def pointers), sets the
  camera-set floats and flags in the body (CamZoomMinHeightFactor `1.0f / 3.0f`), clears two static bytes, adds the six built-in
  modes to RegisteredModes with `RegisteredModes.insert(std::make_pair(T::StatGetModeType(), CCountedPointer<CBaseInit>(T::StatAlloc())))`
  (retail order CStringAndCage 3, CFirstPerson 4, CLookAtFront 0xD, CSACCombat 0xE, CPCMouseLook 0x11, CZTargetNew 0xB), calls
  CProjectileWeapon::StatRegisterMode, looks up CAMERA_MODE_MODE_NULL into DEFAULT_CAMERA_MODE_DEF and CAMERA_MANAGER_SET_DEFAULT into
  a local, sets the three list indices to their lists' end and installs the set with SetCameraManagerSetDef.
  Shape lessons: (1) the value_type pair passed to the out-of-line insert_unique escapes unless STLport's _tree.c insert_unique AND
  _M_insert bodies are visible (repo opt-ins FABLE_RETAIL_STLP_MAP + FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY): with them the pair's
  release is `dec [eax] ; jne` (no re-read of Info), the dead `Data = 0 ; Info = 0` stores vanish and the map pointer / count block
  take retail's edi / ebp (grade 1 -> 2: 0.27 -> 0.81 instruction share). (2) The set-def local and the second CCharString temporary
  get retail's frame words (the local in the dead parameter word, frame 0x24 instead of 0x28) and retail's `mov ecx,[esp+38h] ; dec
  [ecx+4] ; jne` release once the callees that take its address are visible: the by-name GetDef<T> body
  (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY, noinline) and a same-TU noinline CGameCameraManager::SetCameraManagerSetDef
  written from the retail body 0x0069AC90 (grade 2 -> 3: match).
- **0x00A4C5E0 C3DAnimationSequenceData::ReadCompressedFromFile(CDataInputStream&, CSimpleMemoryPool&)** (1844 bytes,
  RELOCATION_MATCH at grade 1 of pass 1). Reads the bone name (virtual ReadNullTerminatedString into a local CCharString), allocates
  BoneName from the pool (AllocateType<char>(GetLength() + 1)), reads the bone-type byte, strcpy's the name, then the sample rate,
  frame count (4-byte read stored as a word), animated flag, the three 2-bit track modes, the two factors and the four tracks (size
  word, AllocateType<T>(size), Read(track, size * sizeof(T))), and finally expands SetupIdentityFlag (identity iff all three track
  modes are 0: `test byte ptr [ebp+0Bh],3Fh`, if / else of two bit-field stores, the CCharString dtor and epilogue duplicated).
  Natural C++ over the shared headers (engine/CDataInputStream.h retail Read / ReadType<T>, engine/CSimpleMemoryPool.h AllocateType<T>,
  fable_string.h GetLength / GetAsCharArray) matched at once.

## Parked for Fable 5.1 (0)
(none)

## Skipped (0)
(none). Length check: 0x00A4C5E0's reservation estimate (1927) includes int3 padding and an uncatalogued 71-byte function at
0x00A4CD20 (probably Ego_r.pdb C3DAnimationSequenceData::IsScalingNonUniform(const C3DVector&), one source only); the oracle's 1844 is
the true length (SPLITS.tsv). 0x0069AE80's 1923 is exact.

## Names and evidence
- Row names: both from Ego_r.pdb + FableWin.pdb name sets plus structural evidence (vtable 0x0125C754 store; the landed caller
  0x00A4CDD0 passes stream and pool, `ret 8`).
- Callees proven and respelled (NAME_FIXES.tsv): 0x00878A50 NCameraModes::CProjectileWeapon::StatRegisterMode (was
  CameraState::ResetCameraInputFlag), 0x0069BBA0 CCountedPointer<NCameraModes::CBaseInit>::DeleteData (was OperatorDelete_thunk_0069bba0),
  0x0069D900 _Rb_tree<EMode, pair<const EMode, CCountedPointer<CBaseInit> >, ...>::insert_unique (was CRetailTree_0069d900::Insert),
  0x00637650 / 0x004AB990 CDefinitionManager::GetDef<CCameraModeDef / CCameraManagerSetDef>(const CCharString&, CDefPointer<const T>*) const
  (were non-template / CGameDefinitionManager spellings), 0x0069AC90 SetCameraManagerSetDef with the const-T parameter, 0x00877160
  NCameraModes::CTools::CTools(const CGameCameraManager&) (catalog: CGameflowAssistanceScript ctor).
- vland (read-only) reports RELOCATION_MATCH PASS for both rows; its three RELOC_NAME warnings are exactly the catalog labels above.

## Descriptive names
- `g_PreloadScriptedCameras` (0x013BA8D4) and `g_CameraManagerDebugFlag` (0x013BA8D5): two bytes the constructor clears; no data-symbol
  name source exists (the PDB name sets are functions only; no strings, FSE or sibling-repo reference). The first gates the camera
  preload in SetScriptedCameraMode (0x00699F0F), the second is read nowhere in retail.
- Constructor / CCamera parameter names (`world_position`, `forward`, `up`, `horizontal_fov`, `vertical_fov`; `camera_manager`;
  `definition_manager`, `camera_manager_set`, `bone_name`): descriptive.
- NCameraModes::EMode enumerators: in no PDB name set, string or reference; StatGetModeType returns `static_cast<EMode>(N)`.

## Header debt
- The five registered modes without a shared header (CFirstPerson, CLookAtFront, CSACCombat, CPCMouseLook, CZTargetNew) are declared in
  the 0x0069AE80 row with ONLY their nested `<Mode>Init : CBaseInit` class and the two inline statics (no data members, no base):
  they need real engine/NCameraModes_<Mode>.h headers (Ego_r layouts) when a row needs their bodies.
- engine/CCameraManagerSetDef.h is a new overlay header replacing the quarantined generated one (same offsets).
- engine/C3DAnimationSequenceData.h in this overlay is the pending chunk 3001 copy plus the READ opt-in: merge after 3001 lands.
- The landed rows of this TU (0x00699BF0 ~CGameCameraManager, 0x0069ADC0, 0x0069AC90, ...) still use local stand-in models; the
  overlay's FABLE_RETAIL_CGAMECAMERAMANAGER_FULL class is the shared model they could move to.
