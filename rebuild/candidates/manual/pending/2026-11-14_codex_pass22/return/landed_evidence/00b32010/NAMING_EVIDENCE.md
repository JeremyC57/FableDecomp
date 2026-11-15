# Original identity and full extent

`?SetDrawDistanceQuality@CEnginePrimitiveRenderer@@QAEXW4EEngineVideoOptionQuality@@ABVCEngineVideoOptionsDef@@@Z` is in both PDBs, Ego_r009D2DF0 and FableWin02DA7E60. The original donor body performs the same seven field copies, invalid flag, scene loop and repeated-mesh enable operation. Native00B32010 is a complete134-byte member, ret8 at00B32093, not a catch. Its caller is CEngine::SetVideoOptions00B24690. The old relocation spelling called it SetMeshDetail on a nonexistent CEngineMeshDetailManager receiver; it must be corrected.

Original type records for all involved classes and the enum are retained in evidence/{Ego_r,FableWin}_TYPES.json. EEngineVideoOptionQuality is exactly the int-based enum LOW0,NORMAL1,HIGH2,HIGHEST3,MAX_COUNT4 in both PDBs. The parameter is unused in this native implementation, so no guessed switch is added. Parameter/local names are descriptive where not supplied by the PDB name lists.

# Fields and header repairs

The existing real renderer/shared-data inheritance is reused. Both PDBs name DrawDistanceFactor+7C, DrawDistanceMinimum+80, DrawDistanceMaximum+84, RepeatedMeshDrawDistanceFactor+88, ZSpriteDrawDistanceFactor+8C, MinimumZSpriteAsMeshDistance+90, MaximumZSpriteAsMeshDistance+94 and DrawDistanceInvalid+98. Its original private RepeatedStaticMeshManager is a CCountedPointer<CEnginePrimitiveManagerRepeatedStaticMeshes> at native+290 and SceneList is CArray<CEngineScene*> at+338, proved by the two direct loads and the adjacent landed SceneList loops. Earlier opaque storage remains at every other member position.

The existing video-def header incorrectly inherited directly from CDefClassBase. Native RTTI proves CParentDefClassBase; both original PDB type records agree. A new option uses the existing real parent header, with all original own field names. Its packed parent ends at+39 and naturally aligned HiresTextureMemory begins at+3C. The seven input float fields are+48..60 and EnableRepeatedMeshes is+73, exactly as the native row reads; total size78 is asserted. All older header branches remain unchanged.

CEngineScene had no shared definition. Its new header uses the native RTTI CBaseClassNonCopyable base, both-PDB private members, and original nested-map storage at+4 / render-stat map at+2C. Native constructor00B94A80 confirms the12-byte map layout and Name+28; remaining members extend through PreloadFramesMoving+58, total5C. The unused composite map/stat contents remain named opaque storage, not replacement classes.

The original SetEnable member is plain inline on the real CEnginePrimitiveManager. FableWin02DA8010 is precisely Enable=parameter at+8, matching the expanded final native store. No separate retail body is claimed for this fully expanded accessor. No inline-budget device is used.

# Relocation and tests

The sole relocation at offset99 calls00B93500, `?InvalidateBoundingBoxesAndDrawDistances@CEngineScene@@QAEXXZ`. Both PDBs name it (Ego_r00A46C40/FableWin02E21B80); its donor and native bodies walk the scene's nested map collection. No helper stand-in or user-defined extra callee is carried. All134 bytes match after masking that call's displacement; objdump confirms the exact member leaf.

Forty standalone fixtures compile and pass VC7.1 /Od /W3 /GR- without /EHsc and g++ C++03. They copy exactly representable distinct float values, cover all four quality values and0..4 initial scenes, verify every invalidation observes the updated fields before manager enable changes, and exercise a callee appending a scene within reserved capacity. This checks the native loop's re-read of end(). The callee also changes the input enable flag, verifying its final late read. The full target body is copied verbatim into the fixture.
