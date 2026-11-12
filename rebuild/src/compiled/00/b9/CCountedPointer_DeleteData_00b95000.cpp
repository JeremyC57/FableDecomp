// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CEngineSceneMap>) the counted pointer's per-type deleter: `delete (T*)data`, null-guarded, through the object's virtual destructor (vtable slot 0, flag 1)
// Leaf: ?DeleteData@?$CCountedPointer@VCEngineSceneMap@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B95000, 11 bytes: `test ecx,ecx; je; mov eax,[ecx]; push 1; call [eax]; ret` -- a static __fastcall member (data in ecx);
//   only its address is used: it is stored as the count block's DeleteFunc (`mov [eax+4], 0x00B95000`).
// Names (agentFAM10, 2026-11-12; was _gapscan::sub_00b95000, the behavioural extern "C" DeleteIfSet): FableWin.pdb ?DeleteData@?$CCountedPointer@VCEngineSceneMap@@@@SIXPAX@Z; stored by Reset 0x00B95AA0, called at 0x00B94D02 / 0x00B94D17 on the object built by the CEngineSceneMap ctor 0x00B949A0 (RTTI vtable .?AVCEngineSceneMap@@); Ego_r.pdb map<long, CCountedPointer<CEngineSceneMap> > (pair second).
// Shape: engine/CCountedPointer.h DeleteData (`delete static_cast<T*>(data)`) on a polymorphic T; the explicit instantiation emits the COMDAT.
class CEngineSceneMap {
public:
    virtual ~CEngineSceneMap();                                   // slot 0 (scalar deleting destructor)
};

#include "engine/CCountedPointer.h"

template void FABLE_FASTCALL CCountedPointer<CEngineSceneMap>::DeleteData(void*);
