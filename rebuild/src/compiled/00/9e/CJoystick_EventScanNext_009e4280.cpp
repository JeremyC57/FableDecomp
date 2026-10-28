// CJoystick::EventScanNext(...) -- Advance a scan and copy its next stored event.
// Leaf: ?EventScanNext@CJoystick@@MBE_NPAVCInputEventScanInfo@@PAVCInputEvent@@@Z
// Identity: Own device callers and paired scan/add helpers, original PDB signatures; Joystick RTTI129c0ec slot1 and count+0xd10; capacity64, event52.
#define CJoystick CJoystick_HeaderLayout
#include "engine/CJoystick.h"
#undef CJoystick
// Layout: CInputEvent52, opaque original Data union32; DeviceType+32, DeviceNumber+36, Type+40, CurrentTime+44, StartTime+48. ScanInfo Started+0, Index+4. Only whole-event copies touch Data.
enum EInputDeviceType {InputDeviceDescriptiveZero=0};enum EInputEventType{InputEventDescriptiveZero=0};
class CInputEvent {public:unsigned char Data[32];EInputDeviceType DeviceType;long DeviceNumber;EInputEventType Type;float CurrentTime,StartTime;};
class CInputEventScanInfo {public:bool Started;long Index;};
struct TestAccess;
// Layout: shared CJoystick flat vptr replaced by its genuine virtual interface; other fields retain retail offsets. STLport list has one sentinel pointer.
namespace std{template<class T>class allocator{};template<class T>struct _List_node{_List_node* _M_next;_List_node* _M_prev;T _M_data;};template<class T,class A=allocator<T> >class list{public:_List_node<T>* _M_node;};}
class CMaintainedEvent {public:virtual ~CMaintainedEvent();CInputEvent Event;};
class CJoystick {public:virtual bool EventScanStart(CInputEventScanInfo*,CInputEvent*)const;protected:virtual bool EventScanNext(CInputEventScanInfo*,CInputEvent*)const;public:CInputEvent EventStore[64];CInputManager* InputManager;bool ExclusiveMode;std::list<CMaintainedEvent> MaintainedEventsList;long NoStoredEvents,NoEventScansRunning;friend struct TestAccess;};
bool CJoystick::EventScanNext(CInputEventScanInfo* scan,CInputEvent* event)const{long next=scan->Index+1;if(next<NoStoredEvents){scan->Index=next;*event=reinterpret_cast<const CInputEvent*>(EventStore)[next];return true;}return false;}