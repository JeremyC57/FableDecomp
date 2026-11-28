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

#ifndef FABLE_TEST_LINK_TRAJECTORY_OWNER
void CTCContainerRewardHero::AdjustTrajectoryVector(long up_count, C3DVector& trajectory_vector) const
{
    trajectory_vector.Normalise();
    trajectory_vector *= 0.3f;
    trajectory_vector.Z += (up_count + 1.0f) * 0.2f;
}
#endif

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// The inherited container is empty throughout this fixture. Detect any unexpected string destruction.
CCharString::~CCharString() { abort(); }

unsigned long GFastOneOverSqrtTable[128];
class RewardHeroTest : public CTCContainerRewardHero {
public:
    using CTCContainerRewardHero::AdjustTrajectoryVector;
};

static void check_coordinate(float observed, double expected) {
    const double error = fabs(observed - expected);
    if (!(error <= 0.000002 * (1.0 + fabs(expected)))) {
        fprintf(stderr, "expected %.12g, got %.12g\n", expected, (double)observed);
        exit(1);
    }
}

// Decode the seed as an exponent and fraction using arithmetic. This oracle
// is independent of the game's bit-field and x87 expression implementations.
static double inverse_length(float x, float y, float z) {
    const double norm = x*x + y*y + z*z;
    int exponent = 0;
    const double mantissa = frexp(norm, &exponent) * 2.0;
    const int biased = norm == 0.0 ? 0 : exponent + 126;
    const int column = norm == 0.0 ? 0 : static_cast<int>((mantissa - 1.0) * 64.0);
    const unsigned long entry = GFastOneOverSqrtTable[64*(biased%2) + column];
    const int seed_exponent = ((380 - biased)/2) | static_cast<int>(entry/256);
    const double seed = ldexp(1.0 + (entry%256)/256.0, seed_exponent - 127);
    return seed * (1.5 - 0.5*seed*seed*norm);
}

int main() {
    const float coordinates[][3] = {
        {0,0,0}, {1,0,0}, {-1,0,0}, {0,2,0}, {0,0,-0.5f}, {3,4,0},
        {-3,4,0}, {1,2,2}, {-1,-2,-2}, {4,-1,2}, {0.25f,-0.5f,0.125f},
        {1.5f,0.25f,-2.5f}, {4,4,4}
    };
    const long counts[] = {-9,-2,-1,0,1,7,131};
    RewardHeroTest hero;
    for (unsigned int pattern=0; pattern<4; ++pattern) {
        for (unsigned int i=0; i<128; ++i)
            GFastOneOverSqrtTable[i] = (i*(17+pattern*6) + pattern*127) % 512;
        for (unsigned int i=0; i<sizeof(coordinates)/sizeof(coordinates[0]); ++i) {
            const float* v = coordinates[i];
            const double factor = inverse_length(v[0],v[1],v[2]);
            for (unsigned int k=0; k<sizeof(counts)/sizeof(counts[0]); ++k) {
                C3DVector direction;
                direction.X=v[0]; direction.Y=v[1]; direction.Z=v[2];
                hero.AdjustTrajectoryVector(counts[k],direction);
                check_coordinate(direction.X,v[0]*factor*0.3);
                check_coordinate(direction.Y,v[1]*factor*0.3);
                check_coordinate(direction.Z,v[2]*factor*0.3+(counts[k]+1)*0.2);
            }
        }
    }
    puts("0074e080_TEST PASS");
    return 0;
}
