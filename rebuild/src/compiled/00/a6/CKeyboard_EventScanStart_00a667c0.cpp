// CKeyboard::EventScanStart(CInputEventScanInfo& scan, CInputEvent& event) const -- mark the scan started and count a running scan (+0x3408); when events are stored (+0x3404 > 0) the scan index is 0 and the first stored event (+4) is copied out (13 dwords), returning true
// Leaf: ?EventScanStart@CKeyboard@@QBE_NAAVCInputEventScanInfo@@AAVCInputEvent@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A667C0, length 80 bytes.
// The chunk label "CJoystick::EventScanStart" is propagated: the +0x3404/+0x3408 counters are CKeyboard's (engine header). Shape: as the joystick body (0x009E4230), `ret 8`.
// Body port (NF321, 2026-11-23): defined on the real class of engine/CKeyboard.h (FABLE_RETAIL_CKEYBOARD), whose PDB-const declaration
//   (both PDB name sets: QBE) and `mutable long NoEventScansRunning` it now uses (was ?EventScanStart@CKeyboard@@QAE... on a local CKeyboard
//   model with local CInputEvent / CInputEventScanInfo / CMaintainedEvent models); CInputEvent, CInputEventScanInfo and CMaintainedEvent
//   are the shared engine/ headers (preamble of the landed CKeyboard::GetKeyName 0x00A66AD0).
#define FABLE_RETAIL_CKEYBOARD
#include "stlport/fable_containers.h"
#include "engine/CBaseClass.h"
#include "engine/CInputEvent.h"
#include "engine/CInputEventScanInfo.h"
#include "engine/CMaintainedEvent.h"
#include "engine/CKeyboard.h"

bool CKeyboard::EventScanStart(CInputEventScanInfo& scan, CInputEvent& event) const
{
    scan.Started = true;
    ++NoEventScansRunning;
    if (NoStoredEvents > 0) {
        scan.Index = 0;
        event = EventStore[0];
        return true;
    }
    return false;
}
