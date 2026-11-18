// CInputManager::SetEventsFromEventPackage(const CInputEventPackage& package) -- replay a recorded event package: clear the processed events, take the package's events as the event store (and their count), and hand the recorded mouse and joystick positions to the mouse and the first joystick
// Leaf: ?SetEventsFromEventPackage@CInputManager@@IAEXABVCInputEventPackage@@@Z
// NF75 (round 7, land1117fu q10928): protected in Ego_r.pdb 0x00B49150 / FableWin.pdb 0x030D07F0 (IAE); the row's pre-existing local
//   declaration is now under protected: (was public, QAE).
// NF72 (round 7, q10873): the local CInputEvent / CRetailVector / CInputEventPackage / C2DVector models are replaced by engine/CInputEvent.h,
//   stlport/fable_containers.h (STLport 4.6 vector: operator= declared out of line by FABLE_RETAIL_STLP_VECTOR_ASSIGN, erase(first, last)
//   by FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE) and engine/CInputEventPackage.h (FABLE_RETAIL_CINPUTEVENTPACKAGE). The calls now bind
//   the STLport instances: 0x009F6890 ??4?$vector@VCInputEvent@@V?$allocator@VCInputEvent@@@std@@@std@@QAEAAV01@ABV01@@Z (the NF68 self row)
//   and 0x009F67E0 vector<CInputEvent>::erase. CInputManager / CMouse / CJoystick stay the row's earlier local models (full port open).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_block->Info are the header CCountedPointer's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F54D0, length 103 bytes.
// Callees: vector<CInputEvent>::erase(first, last) @ 0x009F67E0, vector<CInputEvent>::operator= @ 0x009F6890 (manifest
// "CCombatWheel::operator=" propagated), CMouse virtual slot 4 (+0x10) and CJoystick virtual slot 7 (+0x1c), each taking
// a C2DVector by address.
// Layout: retail CInputEventPackage (PDB with the 12-byte vector): Events +0x00, TimeStamp +0x0c, Checksum +0x10,
// MousePos +0x20, JoystickPos +0x28.
// Shape: the count is the pointer difference over the 0x34-byte element (`imul 4EC4EC4Fh ; sar edx,4`).
// Layout: retail CInputManager (0x80 bytes, 12-byte vectors; GOTCHAS wave 24m): ProcessedEvents +0x04, EventStore +0x10,
// NoStoredEvents +0x1c, NoEventScansRunning +0x20, PMouse +0x24, PKeyboard +0x28, PJoysticks +0x2c (8-byte
// CCountedPointer<CJoystick> elements), Loading +0x38, Saving +0x39, Checksum +0x3c, LoadedEventPackages +0x4c.
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_STLP_VECTOR_ERASE_RANGE_OUTOFLINE
#define FABLE_RETAIL_CINPUTEVENTPACKAGE
#include "stlport/fable_containers.h"
#include "engine/CInputEvent.h"
#include "engine/CInputEventPackage.h"
class CJoystick {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void SetRecordedPosition(const C2DVector& position);   // slot 7 (+0x1c)
};
#include "engine/CCountedPointer.h"
class CMouse {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void SetRecordedPosition(const C2DVector& position);   // slot 4 (+0x10)
};
class CKeyboard;
class CInputManager {
public:
    virtual void v00();
    std::vector<CInputEvent> ProcessedEvents;    // +0x04
    std::vector<CInputEvent> EventStore;         // +0x10
    long                       NoStoredEvents;     // +0x1c
    long                       NoEventScansRunning;   // +0x20
    CMouse*                    PMouse;             // +0x24 scoped_ptr<CMouse>
    CKeyboard*                 PKeyboard;          // +0x28 scoped_ptr<CKeyboard>
    std::vector<CCountedPointer<CJoystick> > PJoysticks;   // +0x2c
protected:
    void SetEventsFromEventPackage(const CInputEventPackage& package);   // ?SetEventsFromEventPackage@CInputManager@@IAEXABVCInputEventPackage@@@Z
public:
};

void CInputManager::SetEventsFromEventPackage(const CInputEventPackage& package)
{
    ProcessedEvents.clear();
    EventStore = package.Events;
    NoStoredEvents = EventStore.size();
    if (PMouse)
        PMouse->SetRecordedPosition(package.MousePos);
    if (PJoysticks[0].Data)
        PJoysticks[0].Data->SetRecordedPosition(package.JoystickPos);
}