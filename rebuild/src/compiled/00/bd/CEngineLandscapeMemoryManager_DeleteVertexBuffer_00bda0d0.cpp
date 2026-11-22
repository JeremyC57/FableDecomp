// CEngineLandscapeMemoryManager::DeleteVertexBuffer(buffer) -- queue a nonnull vertex-buffer on the current delayed-disposal list.
// Leaf: ?DeleteVertexBuffer@CEngineLandscapeMemoryManager@@QAEXPAVCVertexBufferWin32@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BDA0D0, 54 bytes: a null test, `mov eax,[ecx+34h]` (CurrentDisposalListIndex) selecting the
//   list sentinel at +0x1C + index*4, the STLport node built in place (`push 0Ch ; call malloc 0x00BFEA0E`, placement-new null test) and
//   linked before the sentinel (push_back); `ret 4`.
// Name (NF267, 2026-11-22; was ?DeleteVertexBufferBlockWhileBusy@CEngineLandscapeMemoryManager@@AAEXPAVCVertexBufferWin32@@@Z): the public
//   ?DeleteVertexBuffer@CEngineLandscapeMemoryManager@@QAEXPAVCVertexBufferWin32@@@Z, Ego_r.pdb 009c2640 / FableWin.pdb 02cb7450; Ego_d.pdb's CEngineLandscapeMemoryManager field list
//   has DeleteVertexBuffer public and DeleteVertexBufferBlockWhileBusy private, with the same CVertexBufferWin32* parameter. Every retail caller of this
//   address is a member of another class (the destructors of CLandscapeBackgroundPatch 0x00BE6090, CEngineWaterBackgroundSubPatch
//   0x00BFD480, CLandscapeLayerMesh 0x00BFDE20 ...; 22 E8 sites over the queue trio, none inside the manager), so it is the public
//   member; the body queues on VertexBufferDisposalList (+0x1C), not the destroy-now path. The private DeleteVertexBufferBlockWhileBusy
//   (Ego_r 009c2370) is the destroy-now body retail 0x00BD9C00 (virtual destructor + FreeVirtual; no caller: inlined into
//   ProcessDisposalList 0x00BDA150 / FreeDisposalList 0x00BD9CE0).
// Layout: engine/CEngineLandscapeMemoryManager.h retail branch (RTTI bases CFailedAllocationHandler @0 / CMovableResource @4, the
//   three STLport list<T*>[3] arrays +0x10 / +0x1C / +0x28, CurrentDisposalListIndex +0x34) with the add-only DeleteVertexBuffer declaration
//   opt-in; push_back from stlport/fable_containers.h. Was a row-local model of the manager, its bases and std::list (renamed include).
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETEVERTEXBUFFER   // DeleteVertexBuffer 0x00BDA0D0 (Ego_r + FableWin)
#include "engine/CEngineLandscapeMemoryManager.h"

void CEngineLandscapeMemoryManager::DeleteVertexBuffer(CVertexBufferWin32* buffer)
{
    if (buffer)
        VertexBufferDisposalList[CurrentDisposalListIndex].push_back(buffer);
}
