// CEngineSubPrimitiveRenderer::SetMaxSortDistance @ 0x00b84290
#include "engine/CEngineSubPrimitiveRenderer.h"  // retyped onto the PDB layout; byte parity re-verified
extern float g_sortDistanceNumerator; // [0x12a3a20]


void __fastcall SetMaxSortDistance(CEngineSubPrimitiveRenderer* renderer, float dist)
{
    renderer->MaxSortDistance = dist;
    renderer->SortDistanceMultiplier = g_sortDistanceNumerator / dist;
}