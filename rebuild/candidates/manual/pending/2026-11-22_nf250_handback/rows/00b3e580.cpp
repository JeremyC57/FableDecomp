// CCountedPointer<CMeshBankStateBlock>::CCountedPointer(CMeshBankStateBlock* data) -- adopt the state block: Data stored, then for a
// non-null pointer a fresh count block (count 1, DeleteData 0x00B3E1A0, Data re-read through `this`), else Info 0
// Leaf: ??0?$CCountedPointer@VCMeshBankStateBlock@@@@QAE@PAVCMeshBankStateBlock@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3E580, 78 bytes, `ret 4`. Callees: operator new 0x00BFEA1A.
// Names (agentNF250, 2026-11-22, CODEXP27 L119): was the address-suffixed stand-in CCountedPointer_00b3e1a0::CCountedPointer_00b3e1a0
//   (a local {object, count block} model storing the deleter as ?Destroy_00b3e1a0@@YIXPAX@Z). The stored deleter 0x00B3E1A0 is
//   ?DeleteData@?$CCountedPointer@VCMeshBankStateBlock@@@@SIXPAX@Z (FableWin.pdb 0x02DD7530 and Ego_r.pdb S_PUB32; retail operand at
//   0x00B3E5A6), and this constructor is the one the landed 0x00B3BC90 (CEngineResourceManager::SetupEditorAnimation) binds as
//   ??0?$CCountedPointer@VCMeshBankStateBlock@@@@QAE@PAVCMeshBankStateBlock@@@Z. Now the shared engine/CCountedPointer.h in-class (T*)
//   constructor, emitted by an explicit instantiation (the 0x007A4210 pattern), over engine/CMeshBankStateBlock.h (polymorphic T: the
//   DeleteData copy is the virtual-delete body).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CMESHBANKSTATEBLOCK
#include "fable_string.h"
#include "engine/CCountedPointer.h"
#include "engine/CMeshBankStateBlock.h"

template CCountedPointer<CMeshBankStateBlock>::CCountedPointer(CMeshBankStateBlock*);
