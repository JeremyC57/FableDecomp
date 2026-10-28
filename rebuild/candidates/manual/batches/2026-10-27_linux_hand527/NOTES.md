# Chunk 527 notes (0x009C2800 .. 0x00A2D960, 20 rows)

Result: 12 RELOCATION_MATCH in src/ (all tests PASS under g++ and VC7.1), 1 retail_asm RELOCATION_MATCH
(0x00A145C0), 7 DIFFER parked in differ/. No SKIP, no SRC_FAIL. Graded under RTM cl 13.10.3077.
No prior parks existed for these rows in rebuild/candidates/manual; only 0x009CC8B0 and 0x00A1AD40 had pending notes.

## Proven (Ego_r.pdb spelling + a second source: donor order, caller, or vtable)
| Address | Symbol | Second source |
|---|---|---|
| 0x009C2800 | `_Rb_tree<CDisplayModeDesc,...>::insert_unique` (full leaf with `_Nonconst_traits`) | STLport 4.6 set<CDisplayModeDesc>; body byte-matches |
| 0x009C5170 | `??1CFileInstaller@@UAE@XZ` | vtables 0x0129B43C / 0x0129B414 (CTBaseSingleton<CFileInstaller>) stored on exit |
| 0x009CC8B0 | `?UpdateEntryInfoOnly@CBankFile@@QAEXKAAVCDataInputStream@@K@Z` | unique signature; caller WriteTextureToBank 0x00B2BD80 (site 0x00B2BFDA) |
| 0x009CE050 | `?InitEntry@CBankFile@@AAEXKABVCCharString@@0@Z` | signature + the PDB members it fills |
| 0x009EB760 | `?ConsoleCommandListContainingString@CConsole@@SIX...` | donor order; registration stores it under "CommandListContaining" (0x009ED565) |
| 0x009EB9B0 | `?ConsoleVariableListContainingString@CConsole@@SIX...` | donor order; compares type with "variable" |
| 0x009EC890 | `?RunScript@CConsole@@QAEXABVCWideString@@@Z` | donor order after RunTextCommand 0x009EC710; ConsoleRunScript 0x009ECB70 calls it (0x009ECC53) |
| 0x009F39A0 | `?CopyToMemory@CSurface@@QAEXPBVC2DBoxI@@PAX0J@Z` | donor order after CopyFromSurface 0x009F37E0 |
| 0x00A12D20 | `?GetNavigationLineList@CPhysicsMeshObject@@QBEXAAV?$vector@VC2DLineF@@...@std@@J@Z` | body push_backs C2DLineF from mesh line start/end |
| 0x00A14040 | `?RotateXY@C3DVector@@QAEXM@Z` | donor order; one float arg |
| 0x00A14260 | `?Rotate@C3DVector@@QAEXMM@Z` | donor order; two float args (ret 8) |
| 0x00A145C0 | `?GetRandomUnitVector@C3DVector@@SI?AV1@PAK@Z` | Ego_r + FableWin.pdb; donor order after C2DVector::Normalise |
| 0x00A146C0 | `?GetRandomPointWithinUnitSphere@C3DVector@@SI?AV1@PAK@Z` | donor order after GetRandomUnitVector |
| 0x00A1AD40 | `?GetPosInDirection@CNavigatorAStar@@ABE?AVC3DVector@@ABV2@ABVC2DVector@@M@Z` | donor order between GetNearestNavigablePosTo 0x00A1AB40 and GetMyNodeAt 0x00A1AEC0; callers in 0x00A1BA10 |
| 0x00A22CA0 | `?GetAngleXZ@CRightHandedSet@@QBEMXZ` | donor order; reads Forward/Up |
| 0x00A2C7F0 / 0x00A2C960 / 0x00A2CAC0 | `vector<C3DMeshInfo / CPhysicsMeshInfo / C3DAnimationInfo>::_M_insert_overflow` (IAE, protected) | STLport source; element vtables (RTTI) 0x0129CD28/0x0129CD50 etc. |
| 0x00A2D960 | `vector<CCountedPointer<CMeshDataBankEntry> >::_M_fill_insert` | STLport source; element stride/add-ref |

Callees proven along the way (all in NAME_FIXES.tsv): 0x009C1F70 iterator operator--, 0x0099ED10 GetAsLowerCase,
0x0099EC30 CCharString copy ctor, 0x0099E4B0 CCharString default ctor, 0x004115A0 CBasicString<char>::operator==,
0x00999230 CAFile::PathExists, 0x00414E30 CStringParser ctor, 0x0099A300 ~CBaseClassNonCopyable (MAE), 0x009CC6F0
SetInfo (private), 0x00A62960 CCriticalSectionLock::Release, 0x009CBE10 ConvertPathToFilename (ABE), 0x004014A0
CCRC::Calc, 0x00A1AEC0 GetMyNodeAt, 0x00993CA0 `?ReadWithSrcChunkOverflow@CDataInputStream@@AAEXPAEJ@Z`,
0x00A629F0 `??ACPackedUIntArray@@QBEKK@Z`, the STLport `__uninitialized_copy` instances 0x00A2AE20 / 0x00A2AE80 /
0x00A2AF20 / 0x00A2A610 and `__uninitialized_fill_n` 0x00A2AED0.

## Probable
- 0x00A169B0 `std::__find<C2DVector*,C2DVector>`: STLport `__find` is certain; the element C2DVector rests on the
  8-byte two-float stride and the FableWin.pdb `C2DVector::operator==` epsilon shape (0.0001f), plus FableWin.pdb's
  `std::find<vector<C2DVector>::iterator,C2DVector>` instance. No caller names the element type directly.
- 0x009C5110 `?ClearBuffers@CFileInstaller@@QAEXXZ`: Ego_r.pdb names ClearBuffers; donor order and the lock-guarded
  buffer-release body fit. Only one independent source beyond the PDB list, so probable.
- 0x00411910: the out-of-line `vector<char>(unsigned int)` ctor; whether the landed symbol should be spelt on
  `vector<char>` or `CArray<char>` needs the landing coordinator to check (CArray<T> derives from vector<T>).

## Descriptive
- 0x013CD550 `g_CosTable` (1025 floats, cos(2*pi*k/1024)), used by inlined GFFastCos/GFFastSin.
- 0x013D27D0 nav height-test offset (BSS float) in GetPosInDirection.
- 0x0129C5F0 is the `"\0"` string literal passed to the third GFRemoveComments in RunScript.

## Label corrections (manifest label -> real name)
BinarySearchTree_Insert_4int -> insert_unique; CPatchTesselationEdgeStrip_DestructorHelper -> ~CFileInstaller;
CBankFile_LoadEntryData -> UpdateEntryInfoOnly; BankEntry_ProcessPath -> InitEntry; CConsole_ExecuteConfigFile ->
RunScript; CSurface_CopySurfaceData -> CopyToMemory; CPhysicsMesh_BuildCollisionLines -> GetNavigationLineList;
Math_CartesianToSpherical -> RotateXY; C3DVector_FromSpherical -> Rotate; Vector2_Find -> __find;
CNavigatorAStar_GetRandomNavigablePos -> GetPosInDirection; VectorAngle_ComputeRotation -> GetAngleXZ;
std_vector_expand_96bytes -> vector<C3DMeshInfo>::_M_insert_overflow; Vector_ReallocInsert_CScriptThing ->
vector<CPhysicsMeshInfo>::_M_insert_overflow; vector_insert_fill_generic_0x28 -> vector<C3DAnimationInfo>::_M_insert_overflow;
Vector_Insert_Pair_RefCount -> vector<CCountedPointer<CMeshDataBankEntry> >::_M_fill_insert.

## Header debt
- engine/CPhysicsMeshInfo.h, CMeshDataBankEntry.h, C3DMeshInfo.h, C3DAnimationInfo.h are flat (no vptr, no base);
  the placement copy in the _M_insert_overflow rows needs the polymorphic chain, so those rows use local models.
- engine/CDataInputStream.h, CSurface.h, CCriticalSectionLock.h carry no methods; CCriticalSectionLock needs
  `__declspec(align(8))` (gives the `and esp,-8` frame in ~CFileInstaller / InitEntry).
- CBankFile / CFileInstaller PDB layouts use Dinkumware container sizes; retail is STLport (vector 12 bytes, map
  header layout), so offsets drift past the first container member.
- vland warns SHARED_HEADER_AVAILABLE for CBankFile, CMeshDataBankEntry, CIVCountedPointeeBase (local models kept for
  the reasons above).
