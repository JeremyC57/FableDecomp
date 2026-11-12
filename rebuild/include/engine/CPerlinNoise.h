#ifndef FABLE_ENGINE_CPERLINNOISE_H
#define FABLE_ENGINE_CPERLINNOISE_H
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
// Layout: both original PDB type records and donor Update02F3F4C0;
// native B0DC20 member accesses use the original retail container sizes.
namespace NFractal {
class CPerlinNoise {
public:float GetGaussian2DNoise(const C2DVector&)const;
private:unsigned long IndexTable[256];C3DVector VectorTable[256];
};
}
FABLE_STATIC_ASSERT(sizeof(NFractal::CPerlinNoise)==4096);
#endif
