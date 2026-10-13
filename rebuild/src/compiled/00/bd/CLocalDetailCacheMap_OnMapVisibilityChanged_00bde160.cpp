// CLocalDetailCacheMap::OnMapVisibilityChanged(bool)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BDE160, oracle length 39 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov al,[esp+4] ; test al,al ; push esi ; mov esi,ecx ; jne R ; mov eax,[esi+4] ; push eax ; lea ecx,[esi+0Ch] ; call CQuadTreeElement::UnloadStaticContents ; mov ecx,[esi+4] ; test ecx,ecx ; je R ; call <file block manager flush> ; R: pop esi ; ret 4
// engine/CLocalDetailCacheMap.h (PDB): +0x04 FileBlockManager, +0x0C CQuadTreeElement QuadTree.
// When the map goes invisible the quad tree drops its static contents back to the file-block
// manager (0x00BDE020, argument = manager) and the manager, if present, releases finished blocks
// (0x00BE04F0, __fastcall). Becoming visible needs no work here.
#include "engine/CLocalDetailCacheMap.h"

struct CQuadTreeElementCalls {
    void UnloadStaticContents(CEngineStaticMapFileBlockManager* manager);   // 0x00BDE020
};

struct CFileBlockManagerCalls {
    void RemoveFinishedBlocks();                                           // 0x00BE04F0
};

struct CLocalDetailCacheMap_Methods : CLocalDetailCacheMap {
    void OnMapVisibilityChanged(bool visible);
};

void CLocalDetailCacheMap_Methods::OnMapVisibilityChanged(bool visible)
{
    if (!visible) {
        reinterpret_cast<CQuadTreeElementCalls*>(&this->QuadTree[0])->UnloadStaticContents(this->FileBlockManager);
        CFileBlockManagerCalls* manager = reinterpret_cast<CFileBlockManagerCalls*>(this->FileBlockManager);
        if (manager)
            manager->RemoveFinishedBlocks();
    }
}