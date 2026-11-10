#ifndef FABLE_ENGINE_C3DMESHSTATS_H
#define FABLE_ENGINE_C3DMESHSTATS_H
#include "rebuild_abi.h"
#include "engine/CMovableResource.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/C3DBoundingBox.h"
#include "fable_string.h"
class CMovableResourceMemoryPool;
// Full original Ego_r0x3580 data range, native STLport maps/arrays. External declaration only.
class C3DMeshStats: public CMovableResource {
public:
 bool HasDummyObject(unsigned long)const;
private:
 CMovableResourceMemoryPool* MemoryPool;
 long StatsMemoryHandle;
 CCharString Name;
 C3DBoundingSphere BoundingSphere;
 C3DBoundingBox BoundingBox;
 unsigned char HelperPoints[16]; // CPreAllocVectorMap<unsigned long,C3DHelperPoint,...>
 unsigned char DummyObjects[16]; // CPreAllocVectorMap<unsigned long,C3DDummyObject,...>
 unsigned char MeshVolumes[16]; // CVectorMap<unsigned long,C3DMeshVolume,...>
 unsigned char MeshGenerators[12]; // CArray<C3DGeneratorObject>
 char* PackedNames;
 unsigned short PackedNamesSize;
 bool AnimatedFlag;
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshStats)==124);
#endif
