#ifndef FABLE_ENGINE_CLIGHTINGSIMDVARBLOCK_H
#define FABLE_ENGINE_CLIGHTINGSIMDVARBLOCK_H
#include "rebuild_abi.h"
// Both original PDB struct records:272bytes,17 arrays of4 floats.
struct CLightingSIMDVarBlock {
    float Zero[4];
    float One[4];
    float DiffuseVectorX[4];
    float DiffuseVectorY[4];
    float DiffuseVectorZ[4];
    float DiffuseColourR[4];
    float DiffuseColourG[4];
    float DiffuseColourB[4];
    float BackColourR[4];
    float BackColourG[4];
    float BackColourB[4];
    float AmbientColourR[4];
    float AmbientColourG[4];
    float AmbientColourB[4];
    float ObjectPosX[4];
    float ObjectPosY[4];
    float ObjectPosZ[4];
};
FABLE_STATIC_ASSERT(sizeof(CLightingSIMDVarBlock)==272);
FABLE_STATIC_ASSERT(offsetof(CLightingSIMDVarBlock,DiffuseColourR)==80);
#endif
