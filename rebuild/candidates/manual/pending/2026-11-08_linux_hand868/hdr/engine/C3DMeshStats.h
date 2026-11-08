#ifndef FABLE_ENGINE_C3DMESHSTATS_H
#define FABLE_ENGINE_C3DMESHSTATS_H
// C3DMeshStats (chunk 868, row 0x00A2F740): the per-mesh helper-point / dummy-object / volume / generator statistics block, a movable
// resource. Base: CMovableResource (RTTI .?AVC3DMeshStats@@ : CMovableResource, ghidra_out/rtti_bases.tsv; RelocateData is its
// override, Ego_r.pdb ?RelocateData@C3DMeshStats@@UAEXJPAX0J@Z, retail 0x00A2E820 landed). Needs FABLE_RETAIL_CMOVABLERESOURCE.
// Members in Ego_r.pdb order (struct_layouts_egor.tsv; the retail offsets differ from the +0x58 CVectorMap on because retail's
// STLport vector is 12 bytes where Ego_r's Dinkumware one is 16 -- the landed 0x00A2E760 / 0x00A2E900 / 0x00A2EEF0 rows read the retail
// offsets): CMovableResourceMemoryPool* MemoryPool +0x4, long StatsMemoryHandle +0x8, CCharString Name +0xC, C3DBoundingSphere
// BoundingSphere +0x10, C3DBoundingBox BoundingBox +0x20, CPreAllocVectorMap<unsigned long,C3DHelperPoint> HelperPoints +0x38,
// CPreAllocVectorMap<unsigned long,C3DDummyObject> DummyObjects +0x48, CVectorMap<unsigned long,C3DMeshVolume> MeshVolumes +0x58,
// CArray<C3DGeneratorObject> MeshGenerators +0x68, char* PackedNames +0x74, unsigned short PackedNamesSize +0x78, bool AnimatedFlag
// +0x7A; retail sizeof 0x7C (Ego_r 0x84).
// Methods: Ego_r.pdb / FableWin.pdb (the retail addresses are landed rows): the three constructors 0x00A2F190 / 0x00A2F210 / 0x00A2F270,
// InitialiseFromFileData 0x00A2F740 (this chunk), InitialiseMeshGenerators 0x00A2EC40, AllocateBuffers 0x00A2E760, PackNames
// 0x00A2EEF0, UnpackNames 0x00A2ED10, LoadBinary 0x00A2F2D0, SaveBinary 0x00A2EAA0, RelocateData 0x00A2E820, HasHelperPoint 0x00A2EA20,
// GetHelperPoint 0x00A2E8C0, FindHelperPointsBegin / End 0x00A2E900 / 0x00A2E910, HasDummyObject 0x00A2EA60, GetDummyObject 0x00A2E920,
// FindDummyObjectsBegin / End 0x00A2E960 / 0x00A2E970, FindMeshVolumesBegin / End, FindMeshVolumesContainingPoint, GetAllHelperPointNames
// 0x00A2F120, GetAllDummyObjectNames 0x00A2F0F0, GetHelperPointNamesStartingWith 0x00A2F170, GetDummyObjectNamesStartingWith 0x00A2F150;
// the inline accessors GetName, IsAnimated, GetBoundingSphere, GetMeshGeneratorsBegin / End (FableWin only).
#include "rebuild_abi.h"
#include "engine/CMovableResource.h"
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/C3DVector.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/C3DBoundingBox.h"
#include "engine/C3DHelperPoint.h"
#include "engine/C3DDummyObject.h"
#include "engine/C3DGeneratorObject.h"
#include "engine/C3DMeshVolume.h"
#include "engine/CPreAllocVectorMap.h"
#include "engine/CVectorMap.h"
#include "engine/CArray.h"
class CMovableResourceMemoryPool;
class C3DMeshFile;
class CMeshDataBank;
class CMemoryDataInputStream;
class CDataOutputStream;
class C3DMeshStats : public CMovableResource {
public:
    C3DMeshStats();                                                                                  // 0x00A2F190
    C3DMeshStats(const CCharString& name, const C3DMeshFile& file, bool animated, CMovableResourceMemoryPool* pool);   // 0x00A2F210
    C3DMeshStats(CMemoryDataInputStream& stream, CMovableResourceMemoryPool* pool);                  // 0x00A2F270
    bool InitialiseFromFileData(const CCharString& name, const C3DMeshFile& file, bool animated, CMovableResourceMemoryPool* pool);   // 0x00A2F740
    void InitialiseMeshGenerators(const CMeshDataBank& bank, const CCharString& meshName);          // 0x00A2EC40
    bool LoadBinary(CMemoryDataInputStream& stream);                                                 // 0x00A2F2D0
    void SaveBinary(CDataOutputStream& stream);                                                      // 0x00A2EAA0
    virtual void RelocateData(long handle, void* oldBase, void* newBase, long size);                 // 0x00A2E820
    const CCharString& GetName() const { return Name; }
    bool IsAnimated() const { return AnimatedFlag; }
    C3DBoundingSphere GetBoundingSphere() const { return BoundingSphere; }
    bool HasHelperPoint(unsigned long id) const;                                                     // 0x00A2EA20
    const C3DHelperPoint* GetHelperPoint(unsigned long id, unsigned long index) const;              // 0x00A2E8C0
    const std::pair<unsigned long, C3DHelperPoint>* FindHelperPointsBegin(unsigned long id) const;  // 0x00A2E900
    const std::pair<unsigned long, C3DHelperPoint>* FindHelperPointsEnd(unsigned long id) const;    // 0x00A2E910
    bool HasDummyObject(unsigned long id) const;                                                     // 0x00A2EA60
    const C3DDummyObject* GetDummyObject(unsigned long id, unsigned long index) const;              // 0x00A2E920
    const std::pair<unsigned long, C3DDummyObject>* FindDummyObjectsBegin(unsigned long id) const;  // 0x00A2E960
    const std::pair<unsigned long, C3DDummyObject>* FindDummyObjectsEnd(unsigned long id) const;    // 0x00A2E970
    void FindMeshVolumesContainingPoint(const C3DVector& point, std::set<CCharString>& names) const;
    void GetAllHelperPointNames(std::vector<CCharString>& names, bool) const;                        // 0x00A2F120
    void GetAllDummyObjectNames(std::vector<CCharString>& names, bool) const;                        // 0x00A2F0F0
    void GetHelperPointNamesStartingWith(std::vector<CCharString>& names, const CCharString& prefix, bool) const;   // 0x00A2F170
    void GetDummyObjectNamesStartingWith(std::vector<CCharString>& names, const CCharString& prefix, bool) const;   // 0x00A2F150
    CArray<C3DGeneratorObject>::const_iterator GetMeshGeneratorsBegin() const { return MeshGenerators.begin(); }
    CArray<C3DGeneratorObject>::const_iterator GetMeshGeneratorsEnd() const { return MeshGenerators.end(); }
private:
    bool AllocateBuffers(unsigned long helperPoints, unsigned long dummyObjects, unsigned long packedNamesSize);   // 0x00A2E760
    void PackNames(CArray<char>& packed, const std::set<CCharString>& helperPointNames, const std::set<CCharString>& dummyObjectNames);   // 0x00A2EEF0
    void UnpackNames(std::vector<CCharString>& names, bool, const CCharString&, const CCharString&) const;         // 0x00A2ED10
    CMovableResourceMemoryPool* MemoryPool;                              // +0x04
    long StatsMemoryHandle;                                              // +0x08
    CCharString Name;                                                    // +0x0C
    C3DBoundingSphere BoundingSphere;                                    // +0x10
    C3DBoundingBox BoundingBox;                                          // +0x20
    CPreAllocVectorMap<unsigned long, C3DHelperPoint> HelperPoints;      // +0x38
    CPreAllocVectorMap<unsigned long, C3DDummyObject> DummyObjects;      // +0x48
    CVectorMap<unsigned long, C3DMeshVolume> MeshVolumes;                // +0x58
    CArray<C3DGeneratorObject> MeshGenerators;                           // +0x68
    char* PackedNames;                                                   // +0x74
    unsigned short PackedNamesSize;                                      // +0x78
    bool AnimatedFlag;                                                   // +0x7A
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshStats) == 0x7C);
#endif // FABLE_ENGINE_C3DMESHSTATS_H
