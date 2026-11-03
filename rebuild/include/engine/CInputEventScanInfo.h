#ifndef FABLE_ENGINE_CINPUTEVENTSCANINFO_H
#define FABLE_ENGINE_CINPUTEVENTSCANINFO_H
// CInputEventScanInfo (Ego_r.pdb, class V, 8 bytes): bool Started +0x00, long Index +0x04. FableWin.pdb
// ??0CInputEventScanInfo@@QAE@XZ (debug only: inline in release): 0x006286F0 constructs one on the stack with a single
// `mov byte ptr [esp+48h],0` before CInputManager::EventScanStart (which sets Index).
#include "rebuild_abi.h"
class CInputEventScanInfo {
public:
    bool Started;                                                        // +0x00
    long Index;                                                          // +0x04
    CInputEventScanInfo():Started(false){}
};
FABLE_STATIC_ASSERT(sizeof(CInputEventScanInfo) == 0x8);
#endif // FABLE_ENGINE_CINPUTEVENTSCANINFO_H
