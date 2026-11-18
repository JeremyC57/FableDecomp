// CTCPhysicsStandard::UpdateVelocityForImpassability(const C3DVector& current_pos, C3DVector& velocity) -- near the ground, push the velocity away from the impassable neighbouring blocks and refit its height to the ground
// Leaf: ?UpdateVelocityForImpassability@CTCPhysicsStandard@@IAEXABVC3DVector@@AAV2@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007243C0, 617 bytes, thiscall `ret 8`.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_GETMAGNITUDEANDNORMALISE
#define FABLE_RETAIL_C3DVECTOR_CLEAR
#define FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_XYZ
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD
#define FABLE_RETAIL_CTCPHYSICSBASE_WORLDMAP
#define FABLE_RETAIL_CTCPHYSICSBASE_GROUNDHEIGHT
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_IMPULSE
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_OVERRIDES
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_IMPASSABILITYAROUND
#include <math.h>
#include "engine/C3DVector.h"
#include "engine/CWorldMap.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/CTCPhysicsStandard.h"

void CTCPhysicsStandard::UpdateVelocityForImpassability(const C3DVector& current_pos, C3DVector& velocity)
{
    float map_z = GetGroundHeight(current_pos);
    if (current_pos.Z - map_z < 0.7f) {
        float map_gap = current_pos.Z - map_z;
        std::vector<float> impassability_factor_around;
        impassability_factor_around.reserve(9);
        GetWorldMap().GetImpassabilityFactorAround(current_pos, impassability_factor_around, IsOnMeshSurface());
        C3DVector cell_offset;
        cell_offset.X = 0.5f - (float)fmod((double)current_pos.X, 1.0);
        cell_offset.Y = 0.5f - (float)fmod((double)current_pos.Y, 1.0);
        cell_offset.Z = 0.0f;
        C3DVector final_force;
        final_force.Clear();
        for (long c1 = 0; c1 < 9; c1++) {
            C3DVector to_impass = C3DVector((float)g_BlockNeighbourOffsets[c1].X, (float)g_BlockNeighbourOffsets[c1].Y, 0.0f) + cell_offset;
            float mag = to_impass.GetMagnitudeAndNormalise();
            if (mag != 0.0f && mag < 1.4f) {
                float fade_out_factor = (mag > 1.0f) ? (1.4f - mag) * 2.5f : 1.0f;
                C3DVector force_component = to_impass * (fade_out_factor * impassability_factor_around[c1] / (mag * mag));
                final_force += force_component;
            }
        }
        float magnitude = final_force.GetMagnitudeAndNormalise();
        float dot = final_force.Dot(velocity);
        final_force *= -(dot * magnitude);
        if (magnitude > 0.0f && dot > 0.0f) {
            velocity += final_force;
            if (!IsOnMeshSurface()) {
                float new_z = GetGroundHeight(current_pos + velocity) + map_gap - current_pos.Z;
                velocity.Z = new_z;
            }
        }
    }
}