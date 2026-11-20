#ifndef FABLE_ENGINE_CLIPSYNCDATABANKENTRY_H
#define FABLE_ENGINE_CLIPSYNCDATABANKENTRY_H
// CLipSyncDataBankEntry (hand3c3, row 0x006E5A00 CScriptConversation::FrameUpdate): one record of CLipSyncDataBank::Entries
// (CArray<CLipSyncDataBankEntry>, +0x164). Ego_r.pdb struct layout (ghidra_out/struct_layouts_egor.tsv): 8 bytes, Data +0
// (CIVCountedPointer<CLipSyncDataBankEntry::CData>), Duration +4 (float); the landed row 0x00A42980 BeginLoading models it the same way.
// FableWin.pdb ?GetDuration@CLipSyncDataBankEntry@@QBEMXZ (inline in retail: 0x006E5D6A `mov eax,[eax+4]`),
// ?IsLoaded@CLipSyncDataBankEntry@@QBE_NXZ, ?BeginLoading@CLipSyncDataBankEntry@@AAEXKAAVCLipSyncDataBank@@@Z (private; the bank calls it).
// The nested CData (a CResource, 0x34 bytes) is only declared here: rows that build or destroy one define it.
#include "rebuild_abi.h"
#include "engine/CIVCountedPointer.h"
class CLipSyncDataBank;
class CLipSyncDataBankEntry {
public:
    class CData;
    float GetDuration() const { return Duration; }
    bool IsLoaded() const;
private:
    friend class CLipSyncDataBank;
    void BeginLoading(unsigned long index, CLipSyncDataBank& bank);          // 0x00A42980
    CIVCountedPointer<CData> Data;                                           // +0x00
    float                    Duration;                                       // +0x04
};
#endif
