// std::fill(CLipSyncDataBankEntry* first, CLipSyncDataBankEntry* last, const CLipSyncDataBankEntry& val) -- STLport 4.6 _algobase.h: assign val to every lip-sync bank entry of [first, last)
// Leaf: ??$fill@PAVCLipSyncDataBankEntry@@V1@@std@@YIXPAVCLipSyncDataBankEntry@@0ABV1@@Z
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A43890, 72 bytes, /Gr `__fastcall` (first ecx, last edx, the value on the stack), `ret 4`.
// Identity (agentFAM06): the only caller is 0x00A43CD0 (call 0x00A43DB2), bound in rebuild/oracles/reloc_bindings.tsv as
//   vector<CLipSyncDataBankEntry>::_M_fill_insert; it and this row sit in the CLipSyncDataBank TU (0x00A42830 PeekLipSyncInfo,
//   0x00A42980 CLipSyncDataBankEntry::BeginLoading, 0x00A42FC0 GetLipSyncData); FableWin.pdb's CLipSyncDataBank TU holds
//   CArray / vector<CLipSyncDataBankEntry> and CIVCountedPointer<CLipSyncDataBankEntry::CData> as its only intrusive pointer.
//   Ego_r.pdb CLipSyncDataBankEntry is 8 bytes: Data (CIVCountedPointer<CLipSyncDataBankEntry::CData>) +0, Duration +4, the
//   8-byte stride and the counted assignment here. The landed source's NGameText::CDataBankTextEntry (also 8 bytes, the
//   CDataBank TU at 0x009C8xxx) is a propagated guess; the catalog label CIVCountedPointer_Attach is a placeholder.
// Body: the STLport 4.6 fill of the shared rebuild/include/stlport/fable_containers.h; the entry's implicit copy assignment is the
//   shared CIVCountedPointer<T>::operator= (FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN) then the Duration word. No shared header exists
//   for CLipSyncDataBankEntry: declared here with Ego_r.pdb's members (NOTES.md "Header debt").
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/CIVCountedPointeeBase.h"
#include "engine/CIVCountedPointer.h"

class CLipSyncDataBankEntry {                                // Ego_r.pdb, 8 bytes
public:
    class CData;
    CIVCountedPointer<CData> Data;                           // +0x00
    float                    Duration;                       // +0x04
};
FABLE_STATIC_ASSERT(sizeof(CLipSyncDataBankEntry) == 8);

template void std::fill<CLipSyncDataBankEntry*, CLipSyncDataBankEntry>(CLipSyncDataBankEntry*, CLipSyncDataBankEntry*, const CLipSyncDataBankEntry&);
