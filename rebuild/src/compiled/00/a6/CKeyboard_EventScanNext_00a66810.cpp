// CKeyboard::EventScanNext(...) -- Advance a scan and copy its next stored event.
// Leaf: ?EventScanNext@CKeyboard@@QBE_NAAVCInputEventScanInfo@@AAVCInputEvent@@@Z
// Identity: Own device callers and paired scan/add helpers, original PDB signatures; CKeyboardDX caller and count+0x3404; capacity256, event52.
#define CKeyboard CKeyboard_HeaderLayout
#include "engine/CKeyboard.h"
#undef CKeyboard
// Layout: CInputEvent52, opaque original Data union32; DeviceType+32, DeviceNumber+36, Type+40, CurrentTime+44, StartTime+48. ScanInfo Started+0, Index+4. Only whole-event copies touch Data.
enum EInputDeviceType {InputDeviceDescriptiveZero=0};enum EInputEventType{InputEventDescriptiveZero=0};
class CInputEvent {public:unsigned char Data[32];EInputDeviceType DeviceType;long DeviceNumber;EInputEventType Type;float CurrentTime,StartTime;};
class CInputEventScanInfo {public:bool Started;long Index;};
struct TestAccess;
class CKeyboard:public CKeyboard_HeaderLayout {public:bool EventScanNext(CInputEventScanInfo& scan,CInputEvent& event)const;};
bool CKeyboard::EventScanNext(CInputEventScanInfo& scan,CInputEvent& event)const{long next=scan.Index+1;if(next<NoStoredEvents){scan.Index=next;event=reinterpret_cast<const CInputEvent*>(EventStore)[next];return true;}return false;}