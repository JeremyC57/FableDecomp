// CInputEventPackage::CInputEventPackage(CAFile& file) -- load a recorded input package: event count, time stamp, checksum, mouse and joystick positions, then each 0x34-byte event, restamped against the current time, appended to Events
// Leaf: ??0CInputEventPackage@@QAE@AAVCAFile@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009F5690, 262 bytes (+ int3 padding), `ret 4`.
// Identity: the catalog/manifest label "Save" is wrong -- the body fills `this` from the file; Ego_d / FableWin.pdb
//   ??0CInputEventPackage@@QAE@AAVCAFile@@@Z (FableWin 0x030D08D0; Save is the neighbour ?Save@CInputEventPackage@@QAEXAAVCAFile@@@Z).
// Callees: CAFile slot 3 Read(void*, long, CASuspendableProcess*), CInputEvent() 0x00A04410, ~CInputEvent 0x00A03B30,
//   CInputEvent(const CInputEvent&) 0x009E4850 (FableWin ??0CInputEvent@@QAE@ABV0@@Z, out of line), GFGetTime() 0x009E1BC0
//   (?GFGetTime@@YINXZ), vector<CInputEvent>::_M_insert_overflow 0x009F6AC0.
// Layout (Ego_r.pdb CInputEventPackage with STLport's 12-byte vector): Events +0x00, TimeStamp +0x0c, CChecksum Checksum +0x10
//   (four longs, zeroed by its inline ctor), C2DVector MousePos +0x20, JoystickPos +0x28.
#include "engine/CAFile.h"
#define FABLE_RETAIL_CINPUTEVENT_COPY
#include "engine/CInputEvent.h"
#define FABLE_RETAIL_CINPUTEVENTPACKAGE
#include "engine/CInputEventPackage.h"                   // typed Ego_r.pdb layout + CChecksum's inline zeroing ctor

double __fastcall GFGetTime();                          // 0x009E1BC0 ?GFGetTime@@YINXZ

CInputEventPackage::CInputEventPackage(CAFile& file)
{
    long count;
    file.Read(&count, 4, 0);
    file.Read(&TimeStamp, 4, 0);
    file.Read(&Checksum, 16, 0);
    file.Read(&MousePos, 8, 0);
    file.Read(&JoystickPos, 8, 0);
    for (long i = 0; i < count; ++i) {
        CInputEvent event;
        file.Read(&event, sizeof(CInputEvent), 0);
        event.CurrentTime = (float)GFGetTime();
        double age = event.StartTime;
        event.StartTime = (float)((GFGetTime() - age));   // redundant parentheses (PAREN tuple): retail's memory-form `cmp ecx,[esi+8]` (0x009F5746) before the fstp stays one memory-form compare
        Events.push_back(event);
    }
}