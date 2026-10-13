// CLocalDetailCacheMap::Render(CEngineCamera*, float)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDDD70, oracle length 29 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  test byte ptr [ecx+54h],2 ; je R ; mov eax,[esp+4] ; mov edx,[esp+8] ; push 0 ; push eax ; push edx ; add ecx,0Ch ; call CQuadTreeElement::RenderUpdate ; R: ret 8
// engine/CLocalDetailCacheMap.h (PDB): +0x0C CQuadTreeElement QuadTree (0x4C bytes). Bit 1 of the
// quad-tree element's flag byte (+0x48 inside it, i.e. +0x54) gates rendering; when set the element
// is render-updated at 0x00BDDA80 with (distance, camera, 0). Argument order follows the pushes.

struct CEngineCamera;

struct CQuadTreeElementCalls {
    unsigned char _pad_0x00[0x48];
    unsigned char Flags;                    // +0x48 ; bit 1 => has renderable content
    void RenderUpdate(float distance, CEngineCamera* camera, long flags);   // 0x00BDDA80
};

#include "engine/CLocalDetailCacheMap.h"

struct CLocalDetailCacheMap_Methods : CLocalDetailCacheMap {
    void Render(CEngineCamera* camera, float distance);
};

void CLocalDetailCacheMap_Methods::Render(CEngineCamera* camera, float distance)
{
    CQuadTreeElementCalls* tree = reinterpret_cast<CQuadTreeElementCalls*>(&this->QuadTree[0]);
    if (tree->Flags & 2)
        tree->RenderUpdate(distance, camera, 0);
}
