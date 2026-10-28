// CJoystick::AddEventToStore(...) -- Append a bounded device event.
// Leaf: ?AddEventToStore@CJoystick@@IAE_NPBVCInputEvent@@@Z
// Identity: Own device callers and paired scan/add helpers, original PDB signatures; Joystick RTTI129c0ec slot1 and count+0xd10; capacity64, event52.
#define CJoystick CJoystick_HeaderLayout
#include "engine/CJoystick.h"
#undef CJoystick
// Layout: CInputEvent52, opaque original Data union32; DeviceType+32, DeviceNumber+36, Type+40, CurrentTime+44, StartTime+48. ScanInfo Started+0, Index+4. Only whole-event copies touch Data.
enum EInputDeviceType {InputDeviceDescriptiveZero=0};enum EInputEventType{InputEventDescriptiveZero=0};
class CInputEvent {public:unsigned char Data[32];EInputDeviceType DeviceType;long DeviceNumber;EInputEventType Type;float CurrentTime,StartTime;};
class CInputEventScanInfo {public:bool Started;long Index;};
struct TestAccess;
class CJoystick:public CJoystick_HeaderLayout {protected:bool AddEventToStore(const CInputEvent*);friend struct TestAccess;};
bool CJoystick::AddEventToStore(const CInputEvent* event){if(NoStoredEvents>=0 && NoStoredEvents<64){reinterpret_cast<CInputEvent*>(EventStore)[NoStoredEvents]=*event;++NoStoredEvents;return true;}return false;}