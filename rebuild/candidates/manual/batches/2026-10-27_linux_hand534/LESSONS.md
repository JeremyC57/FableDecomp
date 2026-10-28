# Chunk 534 lessons

## GOTCHAS one-liners
- Wave 25 / chunk 534: a non-POD return type (CPlane with a user `CPlane(){}`) makes VC7.1 pass the hidden result
  pointer in ecx for a `__fastcall` static callee, as retail does. With a POD CPlane the pointer goes on the stack
  (0x00A868C0 -> CalculateDividingPlane 0x00A86110).
- Wave 25 / chunk 534: when a local's address goes to an out-of-line STLport helper (`_M_insert_overflow`), declaring the
  helper extern makes VC assume the address escapes. That forces reloads and a bigger frame. A same-TU
  `__declspec(noinline)` body of the real retail callee under its real name fixes it (0x00AB1CC0 / 0x00AB3300).
- Wave 25 / chunk 534: `CCountedPointer(T* d) : Data(d), Info(d ? new CCPPointerInfo(1, DeleteData, d) : 0)` (ternary
  initialiser) gives retail's null test. An `if` in the body does not.
- Wave 25 / chunk 534: assigning `const char*` to a CCharString member through the implicit conversion
  (`bone->Name = base + offset;`) matches. An explicit `CCharString(...)` temporary reuses the ctor's eax and shifts the
  temp slot (0x00AB1CC0).
- Wave 25 / chunk 534: a by-value GetDefName()/GetName() temporary inside one `||` chain becomes a conditional
  temporary with a frame flag byte. Give the first test its own `if` / `else if`, and keep only the GetName() pair in the
  `||` (0x00DF9B80).
- Wave 25 / chunk 534: CScriptThing `operator==` written as `PImp->IsEqualTo(*other.PImp)` through CCountedPointer
  `operator->` / `operator*` gives retail's `mov ebx,4` induction offset in the follower loop (0x00DF9B80).
- Wave 25 / chunk 534: STLport `__copy` and `__destroy_aux` for CCountedPointer ranges are out-of-line `__fastcall` in
  this build, and their tag structs (random_access_iterator_tag, __false_type) need user ctors to be homed as retail does.
- Wave 25 / chunk 534: CWideString `operator const wchar_t*` / GetAsCharArray rows need `// Flags: /Zc:wchar_t`
  (PDB `_W`); without it the leaf mangles `G` (0x00A3B9D0).
- Wave 25 / chunk 534: CreateTexture 0x00A68250 is auto-inlined (/Ob2) into CVolumeTexture::Initialise from the same TU.
  Define the protected body in the TU rather than declaring it (0x00A68370 matched on grade 2).
- Wave 25 / chunk 534: VC7.1 tests: `std::string::clear()` does not exist in the VC7.1 library; use `erase()`.
- Wave 25 / chunk 534: a 64-way `jmp [eax*4+table]` row whose catalog length runs past the jump table and index bytes
  into int3 padding is an over-spanned row: SKIP it so the catalog re-splits it (0x00C400D0).

## Review at landing
- vland dry run: WINS 4 (0x00A3B9D0, 0x00A68370, 0x00DF9B80, 0x00E12330), no RELOC_CONFLICT / RELOC_DUPLICATE /
  RELOC_IDENTITY. Three RELOC_NAME warnings are catalog labels, all in NAME_FIXES.tsv: 0x0099AF00
  `??BCWideString@@QBEPB_WXZ` (labelled CWideString::operator_const_wchar_ptr), 0x006E7AB0
  `??1?$CCountedPointer@VCScriptGameResourceObjectMovieBase@@@@QAE@XZ` (labelled CCountedPointer_Release_006e7ab0), and
  0x00CDBFB0 map dtor (labelled CRetailTree_erase00cd344a::~CRetailTree_erase00cd344a).
- SHARED_HEADER_AVAILABLE warnings for CPixelFormat (0x00A68370), CGameScriptInterface (0x00DF9B80, 0x00E12330) and
  CBaseObject (0x00E12330): the generated headers have no virtuals or methods, so local models remain (see NOTES.md,
  header debt). Strict mode would reject these rows until the generator emits slots and methods.
- 0x00A868C0: the landed CPhysicsFacesSegmentBranch ctor 0x00A86B30 calls an invented
  `?Calculate@CPhysicsFacesSegmentBranch@@SI_N...PAVCPlane@@PAV4@4@Z`. Respell it to the CIPhysicsFacesSegmentBase
  protected-static reference form (NAME_FIXES).
- 0x00E0F1D0 is landed / catalogued as CV_ChapelOfEvilScript::EndMission, but it is bound and spawned as
  `?WatchForTermination@CQ_WaspBossScript@NScript@@QAEXXZ`. One address with two names: the coordinator should pick
  the WaspBoss name (the Main spawn string is the second source).
- 0x0129BA3C (1e-4f) has about 25 bound spellings across landed files. This chunk uses `g_epsilon_0129ba3c` (const, `3MB`);
  both `3MA` and `3MB` spellings of it are bound.
- 0x0143782C keeps its bound name `g_CUnprovenCosThreshold_0143782c`, but it is the cloud-dome u/v extent
  (cos(0.15*pi)*13000), not a threshold. Rename it with all its users.
- 0x0099AF00 / 0x0099AF10: about 15 landed spellings (QAE/QBE, G/_W, invented classes) for the two CWideString
  accessors. The coordinator should respell every caller to the `_W` PDB form.
- Parked rows, next idea for each:
  - 0x00A614B0 (one register): try `_M_rightmost()` through a `_Base_ptr` local in the end() arm.
  - 0x00A68690 (frame 0x60 vs 0x58): scope the C3DExtentsI in a block that closes before the scratch CTexture is
    built, so VC can share the slot.
  - 0x00A868C0 (+67 bytes): give the second and third push_back a noinline same-TU CArray<unsigned short>::push_back
    0x009B4A20 body while the first stays inline. Retail has exactly that mix, so it may come from /Ob2 inline budget
    exhaustion.
  - 0x00AB1CC0 (tag temps on the wrong dead-argument slot): reorder the parameter uses so `mesh` dies before `bones`.
  - 0x00B61DD0 (11 bytes, counter placement + u/v order): keep the vertex counter as a member-like long stored back
    each iteration (`vertex = vertex + 1` after the theta load), and write `inv_extent * x` into a named float before
    the scale multiply.

## Why the earlier attempts stalled (chunk_history 312 / 484 / 964-970)
- Every row except CVideoSys/CVolumeTexture carried a placeholder or wrong label: BinaryTree_Insert_RBTree,
  BuildEntityScriptBindings_FromVertexIndices, BuildBuffers_BuildGlowParticles, CScriptBase_FilterCombatantsByType,
  HandleQueenHornetCutscene, a `_global` Calculate, and CreateWorldLevelWad (a CWorldMap function with the wrong arity).
  Agents modelled the wrong class or signature and never got a leaf to compare. Identifying the TU from neighbouring
  retail functions and Ego_r donor order unlocked 0x00AB1CC0, 0x00B61DD0, 0x00DF9B80 and 0x00E12330.
- The STLport rows need the real templates (std::pair as struct `U`, `_Nonconst_traits` iterators, fastcall `__copy` /
  `__destroy_aux`, same-TU overflow bodies); stand-in vectors and maps cannot match.
- The quest scripts need the retail CScriptBase size (0x40, not Ego_r's 0x54) and per-statement
  IsActiveThreadTerminating checks.
- 0x00A3A410, 0x00A3EF00 and 0x00C400D0 remain blocked by type surface, an unnamed class and an over-spanned row
  respectively.
