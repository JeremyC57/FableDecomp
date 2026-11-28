// 0074E080 CTCContainerRewardHero::AdjustTrajectoryVector
// Leaf: ?AdjustTrajectoryVector@CTCContainerRewardHero@@IBEXJAAVC3DVector@@@Z
// Flags:
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_LOCALS_XYZ
#define FABLE_RETAIL_GFFASTONEOVERSQRT_HOST_BITS_PASS23
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#include "engine/CTCContainerRewardHero.h"

void CTCContainerRewardHero::AdjustTrajectoryVector(long up_count, C3DVector& trajectory_vector) const
{
    trajectory_vector.Normalise();
    trajectory_vector *= 0.3f;
    trajectory_vector.Z += (up_count + 1.0f) * 0.2f;
}
