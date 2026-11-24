// CEngineLandscapeMemoryManager::DeleteIndexBuffer(buffer) -- queue a nonnull index-buffer on the current delayed-disposal list.
// Leaf: ?DeleteIndexBuffer@CEngineLandscapeMemoryManager@@QAEXPAVCIndexBuffer@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BDA090, 54 bytes: a null test, `mov eax,[ecx+34h]` (CurrentDisposalListIndex) selecting the
//   list sentinel at +0x28 + index*4, the STLport node built in place (`push 0Ch ; call malloc 0x00BFEA0E`, placement-new null test) and
//   linked before the sentinel (push_back); `ret 4`.
// Name (NF267, 2026-11-22; was ?DeleteIndexBufferBlockWhileBusy@CEngineLandscapeMemoryManager@@AAEXPAVCIndexBuffer@@@Z): the public
//   ?DeleteIndexBuffer@CEngineLandscapeMemoryManager@@QAEXPAVCIndexBuffer@@@Z, Ego_r.pdb 009c25e0 / FableWin.pdb 02cb7350; Ego_d.pdb's CEngineLandscapeMemoryManager field list
//   has DeleteIndexBuffer public and DeleteIndexBufferBlockWhileBusy private, with the same CIndexBuffer* parameter. Every retail caller of this
//   address is a member of another class (the destructors of CLandscapeBackgroundPatch 0x00BE6090, CEngineWaterBackgroundSubPatch
//   0x00BFD480, CLandscapeLayerMesh 0x00BFDE20 ...; 22 E8 sites over the queue trio, none inside the manager), so it is the public
//   member; the body queues on IndexBufferDisposalList (+0x28), not the destroy-now path. The private DeleteIndexBufferBlockWhileBusy
//   (Ego_r 009c23a0) is the destroy-now body retail 0x00BD9C20 (virtual destructor + FreeVirtual; no caller: inlined into
//   ProcessDisposalList 0x00BDA150 / FreeDisposalList 0x00BD9CE0).
// Layout: engine/CEngineLandscapeMemoryManager.h retail branch (RTTI bases CFailedAllocationHandler @0 / CMovableResource @4, the
//   three STLport list<T*>[3] arrays +0x10 / +0x1C / +0x28, CurrentDisposalListIndex +0x34) with the add-only DeleteIndexBuffer declaration
//   opt-in; push_back from stlport/fable_containers.h. Was a row-local model of the manager, its bases and std::list (renamed include).
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER
#define FABLE_RETAIL_CENGINELANDSCAPEMEMORYMANAGER_DELETEINDEXBUFFER   // DeleteIndexBuffer 0x00BDA090 (Ego_r + FableWin)
#include "engine/CEngineLandscapeMemoryManager.h"

void CEngineLandscapeMemoryManager::DeleteIndexBuffer(CIndexBuffer* buffer)
{
    if (buffer)
        IndexBufferDisposalList[CurrentDisposalListIndex].push_back(buffer);
}
