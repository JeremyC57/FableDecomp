#ifndef FABLE_ENGINE_QUADRICOPTIMISERTYPES_H
#define FABLE_ENGINE_QUADRICOPTIMISERTYPES_H
#include "rebuild_abi.h"
#include "engine/C3DVector.h"
// Original Ego_r class records; retail three container reductions give native vertex strideB8.
namespace QuadricOptimiserInternals {
class CQuadric {public:double AA,AB,AC,AD,BB,BC,BD,CC,CD,DD,Weight;void operator+=(const CQuadric&);double EvaluateError(const C3DVector&)const;};
class COptimisedVertex {public:
 C3DVector Position,Normal,TextureCoord;CQuadric PositionError;float Cost;
 unsigned char FaceList[12];bool Exists;unsigned long UniqueIndex,OutputIndex;unsigned char Groups[12],PairContractions[12];
 float GetGroupDifference(const COptimisedVertex&)const;
};
class CSubContraction {public:unsigned long Index1,Index2;};
FABLE_STATIC_ASSERT(sizeof(CQuadric)==0x58);
FABLE_STATIC_ASSERT(sizeof(COptimisedVertex)==0xb8);
FABLE_STATIC_ASSERT(sizeof(CSubContraction)==8);
}
#endif
