#ifndef FABLE_ENGINE_CBANKFILEASYNCENTRY_H
#define FABLE_ENGINE_CBANKFILEASYNCENTRY_H
// New header (agentC27, row 0x009FDE80 CGraphicInfoFrame::CFrameData::BeginLoading): only the member rows call. Ego_r.pdb / FableWin.pdb
// ?BeginReadDataAutoAlloc@CBankFileAsyncEntry@@QAE?AV?$CCountedPointer@VCBankFileAsyncData@@@@KKW4EThreadedFilePriority@@@Z, retail
// 0x009D6100 (landed row rebuild/src/compiled/00/9d/CBankFileAsyncEntry_BeginReadDataAutoAlloc_009d6100.cpp). Layout not modelled
// (rows only hold it through CCountedPointer<CBankFileAsyncEntry>).
#include "engine/CCountedPointer.h"
#include "engine/EThreadedFilePriority.h"
class CBankFileAsyncData;
class CBankFileAsyncEntry {
public:
    CCountedPointer<CBankFileAsyncData> BeginReadDataAutoAlloc(unsigned long offset, unsigned long size, EThreadedFilePriority priority);   // 0x009D6100
};
#endif
