// CLocalDetailCacheMap::Render(CEngineCamera*, float)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDDD70, oracle length 29 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  test byte ptr [ecx+54h],2 ; je R ; mov eax,[esp+4] ; mov edx,[esp+8] ; push 0 ; push eax ; push edx ; add ecx,0Ch ; call CQuadTreeElement::RenderUpdate ; R: ret 8
// engine/CLocalDetailCacheMap.h (PDB): +0x0C CQuadTreeElement QuadTree (0x4C bytes). Bit 1 of the
// quad-tree element's flag byte (+0x48 inside it, i.e. +0x54) gates rendering; when set the element
// is render-updated at 0x00BDDA80 with (distance, camera, 0). Argument order follows the pushes.
//
// Lane feedback: a named local for the embedded quad-tree pointer hoists the `add ecx,0Ch` above the flag
// test (30 bytes); re-deriving it at each use matches retail. RELOCATION_MATCH.
struct CEngineCamera;

// Call-surface view of CQuadTreeElement (engine/CLocalDetailCacheMap.h embeds it opaquely at +0x0C).
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
    // Keeping the tree pointer in a local hoisted `add ecx,0Ch` above the flag test; retail
    // tests `[ecx+54h]` through `this` and only forms the sub-object pointer for the call.
    if (reinterpret_cast<CQuadTreeElementCalls*>(&this->QuadTree[0])->Flags & 2)
        reinterpret_cast<CQuadTreeElementCalls*>(&this->QuadTree[0])->RenderUpdate(distance, camera, 0);
}