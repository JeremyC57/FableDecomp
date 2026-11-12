# Chunk 2033 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x0069AE80) A temporary passed by `const T&` to an out-of-line STLport tree insert escapes unless the whole insert chain is visible:
  FABLE_RETAIL_STLP_MAP alone (insert_unique body) changed nothing because the value still reaches the declaration-only `_M_insert`;
  adding FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY made the map pair's counted-pointer release `dec [reg] ; jne` (no Info re-read),
  dropped its dead `Data = 0 ; Info = 0` stores and gave retail's register split (map pointer edi, count block ebp). Free check: a
  two-insert fragment probe shows it before grading.
- (0x0069AE80) `map.insert(std::make_pair(k, CCountedPointer<T>(p)))` is three counted pointers in retail: the temporary, make_pair's
  copy (its `inc` threaded, unconditional) and the value_type conversion (`inc` behind a null test), released pair first then twice
  through the shared Info register.
- (0x0069AE80) A local whose address goes to two callees (GetDef<T>(name, &local) and SetCameraManagerSetDef(local)) shares the dead
  parameter word only when BOTH bodies are visible (noinline): the GetDef by-name body (FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_NAME_BODY)
  and a same-TU body of the landed SetCameraManagerSetDef 0x0069AC90; the release then is `mov ecx,[slot] ; dec [ecx+4] ; jne`.
- (0x0069AE80) `x / 3` constants: 0x3EAAAAAB is `1.0f / 3.0f`; the literal 0.333333f is 0x3EAAAA9F.
- (0x0069AE80) A chained `A = B = list.end();` stores B first (retail 0x0069B585 / 0x0069B58B).
- (0x0069AE80) Inline factory statics (`static CBaseInit* StatAlloc() { return new XInit; }`, FableWin-only names) expand to the bare
  `new` + vptr store; the abstract base's vptr store is dead and gone.
- (0x00A4C5E0) A whole stream reader over the shared CDataInputStream retail Read / ReadType<T> and CSimpleMemoryPool::AllocateType<T>
  matched at the first grade: the typed reads' values live in the dead stream-argument word, the variable-size Read keeps its
  `size <= 0` test, and a trailing SetupIdentityFlag if / else duplicates the CCharString destructor and epilogue per arm.
- (0x00A4C5E0) Catalog reservation lengths can include the next, uncatalogued function: 0x00A4CD20 (71 bytes, no xrefs) sits in the
  gap after 0x00A4C5E0.

Review at landing:
- NAME_FIXES.tsv: seven retail addresses whose landed / bound spellings are wrong (StatRegisterMode, DeleteData<CBaseInit>,
  insert_unique, the two GetDef<T> by-name instances, SetCameraManagerSetDef's const T, CTools ctor). Respell the landed callers
  (0x00636E90, 0x0069AD10, 0x0069ADC0, 0x0069D520, the 0x00878A50 and 0x0069BBA0 rows themselves).
- New overlay headers engine/NCameraModes_CBaseInit.h and engine/CCameraManagerSetDef.h (replaces the quarantined generated copy).
- engine/C3DAnimationSequenceData.h overlay is based on the pending chunk 3001 copy (INTERPOLATE opt-in): merge order.
- 0x0069AE80 declares five camera-mode classes row-locally with only their nested init class and statics (no header exists).
- Cut a row for the uncatalogued 0x00A4CD20 (candidate C3DAnimationSequenceData::IsScalingNonUniform(const C3DVector&)).
- 0x0069AE80's same-TU SetCameraManagerSetDef body (visible, noinline) is written from retail 0x0069AC90 but is not byte-exact with it
  (its own GetDef<CCameraManagerDef>(long) / SetCameraManagerDef callees are declarations there); only the constructor is graded.
