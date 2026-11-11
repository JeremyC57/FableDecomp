// NCameraModes::CTools::PotentialFieldBufferObjects(const C3DVector& field_is_at, const list<const CThingGameObject*>& objects) const -- the gradient of the camera's object-avoidance potential field: sample the field of every object (strength its mesh extent, radius the extent's root) at the position and one step along X, Y and Z, and return the forward differences divided by the step
// Leaf: ?PotentialFieldBufferObjects@CTools@NCameraModes@@QBE?AVC3DVector@@ABV3@ABV?$list@PBVCThingGameObject@@V?$allocator@PBVCThingGameObject@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00876CA0, length 368 bytes, __fastcall, hidden result pointer, `ret 0Ch`.
// Callees: NCameraModes::CTools::GetMeshExtent(const CThing&) const 0x00875F00, CThing::GetPos() const 0x004C73D0.
// Globals: as the landed twin 0x00876E10 PotentialFieldBufferPoints: the sample step 0x01385D20 and the four sample offsets
//   0x01385D24 (descriptive names, in neither PDB name set).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS
#define FABLE_RETAIL_NCAMERAMODES_CTOOLS_POTENTIALFIELD
#include <math.h>
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CThingGameObject.h"
#include "engine/NCameraModes_CTools.h"
#pragma intrinsic(sqrt)

extern float g_PotentialFieldSampleStep;                    // 0x01385D20 (0.1f)
extern C3DVector g_PotentialFieldSampleOffsets[4];          // 0x01385D24

namespace NCameraModes {

C3DVector CTools::PotentialFieldBufferObjects(const C3DVector& field_is_at, const std::list<const CThingGameObject*>& objects) const
{
    float field_at_pos[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    for (std::list<const CThingGameObject*>::const_iterator it = objects.begin(); it != objects.end(); ++it) {
        for (long counter = 0; counter < 4; ++counter) {
            C3DVector pos_to_check = field_is_at + g_PotentialFieldSampleOffsets[counter] * g_PotentialFieldSampleStep;
            float extent = GetMeshExtent(**it);
            C3DVector object_pos = (*it)->GetPos();
            field_at_pos[counter] += PotentialField(pos_to_check, object_pos, extent, (float)sqrt(extent));
        }
    }
    return C3DVector((field_at_pos[1] - field_at_pos[0]) / g_PotentialFieldSampleStep,
                     (field_at_pos[2] - field_at_pos[0]) / g_PotentialFieldSampleStep,
                     (field_at_pos[3] - field_at_pos[0]) / g_PotentialFieldSampleStep);
}

}