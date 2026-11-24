// CEngineLandscapeMemoryManager::DeleteTexture(texture) -- queue a nonnull texture on the current delayed-disposal list.
// Leaf: ?DeleteTexture@CEngineLandscapeMemoryManager@@QAEXPAVCTexture@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BDA110, 54 bytes: a null test, `mov eax,[ecx+34h]` (CurrentDisposalListIndex) selecting the
//   list sentinel at +0x10 + index*4, the STLport node built in place (`push 0Ch ; call malloc 0x00BFEA0E`, placement-new null test) and
//   linked before the sentinel (push_back); `ret 4`.
// Name (NF265, 2026-11-22; was ?DeleteTextureBlockWhileBusy@CEngineLandscapeMemoryManager@@AAEXPAVCTexture@@@Z): the public
//   ?DeleteTexture@CEngineLandscapeMemoryManager@@QAEXPAVCTexture@@@Z, Ego_r.pdb 009c26a0 / FableWin.pdb 02cb7490. The destroy-now
//   bodies are the private ...BlockWhileBusy trio: retail FreeVirtual 0x00BD9B90 / 0x00BD9C00 / 0x00BD9C20 (virtual-dtor + FreeVirtual) /
//   AllocVirtualAligned 0x00BD9C40 / FreeVirtualAligned 0x00BD9CA0 / 0x00BD9CC0 (Uninitialise + FreeVirtual) is Ego_r's six-function run
//   FreeVirtual 009c2300 / DeleteVertexBufferBlockWhileBusy 009c2370 / DeleteIndexBufferBlockWhileBusy 009c23a0 / AllocVirtualAligned
//   009c23d0 / FreeVirtualAligned 009c2430 / DeleteTextureBlockWhileBusy 009c2450 in order. The three queue bodies 0x00BDA090 / 0x00BDA0D0 /
//   0x00BDA110 (index / vertex / texture lists) are then the public trio (Ego_r 009c25e0 / 009c2640 / 009c26a0, same IB / VB / Tex
//   order); every caller is another class (0x00BE5C30 / 0x00BE6090), and neither name set has another texture- or buffer-taking member.
//   Second angle (NF267): Ego_d.pdb's field list has DeleteTexture public and DeleteTextureBlockWhileBusy private (same CTexture*
//   parameter); none of the 22 retail E8 sites of the queue trio is inside the manager, and the destroy-now copies 0x00BD9C00 /
//   0x00BD9C20 / 0x00BD9CC0 have no call or address reference (inlined into ProcessDisposalList / FreeDisposalList).
// Layout: engine/CEngineLandscapeMemoryManager.h retail branch (RTTI bases CFailedAllocationHandler @0 / CMovableResource @4, the
//   three STLport list<T*>[3] arrays +0x10 / +0x1C / +0x28, CurrentDisposalListIndex +0x34) with the add-only DeleteTexture declaration
//   opt-in; push_back from stlport/fable_containers.h. Was a row-local model of the manager, its bases and std::list (renamed include).
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETETEXTURE   // DeleteTexture 0x00BDA110 (Ego_r + FableWin)
#include "engine/CEngineLandscapeMemoryManager.h"

void CEngineLandscapeMemoryManager::DeleteTexture(CTexture* texture)
{
    if (texture)
        TextureDisposalList[CurrentDisposalListIndex].push_back(texture);
}
