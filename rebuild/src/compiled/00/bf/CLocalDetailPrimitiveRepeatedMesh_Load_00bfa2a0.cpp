// CLocalDetailPrimitiveRepeatedMesh::Load, retail 0x00BFA2A0, 1533 bytes.
// Leaf: ?Load@CLocalDetailPrimitiveRepeatedMesh@@UAE_NAAVCDataInputStream@@@Z
// Flags:
#define FABLE_RETAIL_LOCAL_DETAIL_RENDER_PASS29
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_ORIGINAL_READ_SCOPE
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER
#define FABLE_RETAIL_PASS22_WATER_PATCH_LOAD
#define FABLE_RETAIL_PASS22_REPEATED_MESH_CLIP_SIMD
#define FABLE_RETAIL_REPEATED_MESH_ALLOCATIONS_PASS30
#define FABLE_RETAIL_REPEATED_MESH_LOAD_PASS30
#include "engine/CLocalDetailPrimitiveRepeatedMesh.h"
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/CEngineLandscapeRenderer.h"
#include "engine/CDataInputStream.h"
#include "fable_assert.h"

bool CLocalDetailPrimitiveRepeatedMesh::Load(CDataInputStream& stream)
{
    FABLE_RETAIL_ASSERT(!ObjectOffsets && !ObjectMatricies && !LandscapeNormalArray && !SubSections && !WindDelayArray);
    stream.Read(&BoundingBox, sizeof(BoundingBox));
    stream.Read(&BoundingSphere, sizeof(BoundingSphere));
    ObjectCount = stream.ReadSLONG();
    FABLE_RETAIL_ASSERT(ObjectCount > 0);
    MaxObjectScale = stream.ReadFloat();
    long aligned_object_count = (ObjectCount + 3) & ~3L;
    CEngineLandscapeMemoryManager* manager = NEngineInternal::GPLandscapeRenderer->GetMemoryManager();
    ObjectMatricies = manager->AllocArrayOfObjectsNoConstuctors<C4DVector>(ObjectCount, this);
    if (!ObjectMatricies)
        return false;
    ObjectOffsets = manager->AllocAlignedArrayOfObjectsNoConstuctors<C4DVector>(aligned_object_count, 16, this);
    if (!ObjectOffsets)
        return false;
    stream.Read(ObjectMatricies, ObjectCount * sizeof(C4DVector));
    stream.Read(ObjectOffsets, ObjectCount * sizeof(C4DVector));
    if (stream.ReadEBOOL()) {
        long long overflow_check = static_cast<long long>(aligned_object_count) * 3;
        if (overflow_check < 0 || overflow_check > 0x7fffffff) {
            FABLE_RETAIL_ASSERT(false);
            return false;
        }
        LandscapeNormalArray = manager->AllocAlignedArrayOfObjectsNoConstuctors<float>(aligned_object_count * 3, 16, this);
        if (!LandscapeNormalArray)
            return false;
        stream.Read(LandscapeNormalArray, aligned_object_count * 3 * sizeof(float));
    }
    if (stream.ReadEBOOL()) {
        WindDelayArray = manager->AllocArrayOfObjectsNoConstuctors<unsigned char>(ObjectCount, this);
        if (!WindDelayArray)
            return false;
        stream.Read(WindDelayArray, ObjectCount);
    }
    if (stream.ReadEBOOL()) {
        SubSectionsCount = stream.ReadSLONG();
        FABLE_RETAIL_ASSERT(SubSectionsCount > 0);
        long subsection_size = SubSectionsCount * sizeof(NEnginePrimitiveManagerRepeatedStaticMeshes::CSubsectionElement);
        SubSections = manager->AllocAlignedArrayOfObjectsNoConstuctors<NEnginePrimitiveManagerRepeatedStaticMeshes::CSubsectionElement>(SubSectionsCount, 16, this);
        if (!SubSections)
            return false;
        stream.Read(SubSections, subsection_size);
    }
    return true;
}