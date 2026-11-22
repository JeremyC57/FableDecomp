// CLocalDetailPrimitiveMesh::operator delete(void* block, CMovableResource* owner) -- class deallocator: hands the block back to the landscape memory manager (CEngineLandscapeMemoryManager::FreeVirtual); the owner is unused
// Leaf: ??3CLocalDetailPrimitiveMesh@@SIXPAXPAVCMovableResource@@@Z
// Retail: Fable.exe @ 0x00BF9920, 18 bytes, `__fastcall` static (block in ecx, owner in edx, ignored), out of line. Catalog label
// "CEngineSubPrimitiveBase::operator_new" is wrong (propagated across byte-identical class deallocators).
// Name (NF263, two sources): Ego_r.pdb and FableWin.pdb both name the placement form ??3CLocalDetailPrimitiveMesh@@SIXPAXPAVCMovableResource@@@Z; retail's
// allocators sit at 0x00BF9900 (??2, landed) / 0x00BF9920 (??3) / 0x00BF9940 (??_U) / 0x00BF9960 (??_V), the same order as Ego_r's 0x20-spaced quartet; every retail caller passes the owner as `xor edx,edx` (scalar deleting destructor 0x00BDFD60 at 0x00BDFD6F).
// FableWin's one-parameter form ...SIXPAX@Z is a separate function (`xor edx,edx ; jmp` to this one).
// Callee: CEngineLandscapeMemoryManager::FreeVirtual(void*) 0x00BD9B90 on NEngineInternal::GPLandscapeRenderer (0x01436EA8, MemoryManager +0x6B0).
// Shape: the direct expression loads the global into eax and pushes ecx (the sibling allocators name the manager in a local).
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER                // FreeVirtual 0x00BD9B90 (Ego_r + FableWin)
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_GPLANDSCAPERENDERER // NEngineInternal::GPLandscapeRenderer, 0x01436EA8
#include "engine/CEngineLandscapeMemoryManager.h"
#include "engine/CEngineLandscapeRenderer.h"

class CLocalDetailPrimitiveMesh {
public:
    static void __fastcall operator delete(void* block, CMovableResource* owner);
};

void __fastcall CLocalDetailPrimitiveMesh::operator delete(void* block, CMovableResource* owner)
{
    NEngineInternal::GPLandscapeRenderer->MemoryManager->FreeVirtual(block);
}
