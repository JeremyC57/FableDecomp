// CJoystick::ProcessMaintainedEvents(const CInputEvent*). Leaf: ProcessMaintainedEvents
#include "engine/CJoystick.h"
#include <stdlib.h>
class C2DVector{ public:float X,Y;C2DVector(){}C2DVector(const C2DVector& b):X(b.X),Y(b.Y){}};
enum EInputEventType { RetailEventNone=0, JoystickButtonPressed=19, JoystickButtonHeld=20, JoystickButtonReleased=21 };
class CInputEvent { public:
 unsigned char Data[32];long DeviceType,DeviceNumber;EInputEventType Type;float CurrentTime,StartTime;
 CInputEvent();~CInputEvent();
 
public:
    long GetJoystickButton() const;   // 0x00A03B90
    float GetJoystickButtonPressureFraction() const;   // 0x00A03BA0
    C2DVector GetJoystickPosition() const;   // 0x00A043A0
    EInputEventType GetType() const;   // 0x00A03B40
    void SetAsJoystickButtonHeld(long, float, const C2DVector&, double);   // 0x00A040D0
};
struct BaseClassNative{BaseClassNative();virtual ~BaseClassNative();};
struct MaintainedEvent:BaseClassNative{CInputEvent Event;MaintainedEvent();virtual ~MaintainedEvent();};
struct EventNode{EventNode* Next;EventNode* Prev;MaintainedEvent Value;};
struct Iterator{EventNode* Node;Iterator(EventNode* p):Node(p){}Iterator(const Iterator& b):Node(b.Node){}};
struct EventList {
 EventNode* Head;
 Iterator InsertDefault(Iterator); //native9E4A20: hidden result + by-value iterator; constructs genuine maintained event.
 void PushBack(){InsertDefault(Iterator(Head));}
 MaintainedEvent& Back(){return Head->Prev->Value;}
 void Erase(EventNode* node){EventNode* next=node->Next;EventNode* prev=node->Prev;prev->Next=next;next->Prev=prev;node->Value.~MaintainedEvent();free(node);}
};
// Retail list is a single sentinel pointer at D0C; donor list has extra size/allocator fields.
struct JoystickNative {
 unsigned char Prefix[0xd0c];EventList MaintainedEventsList;
 void ProcessMaintainedEvents(const CInputEvent*);
};
void JoystickNative::ProcessMaintainedEvents(const CInputEvent* event){
 if(event->GetType()==19){
  MaintainedEventsList.PushBack();
  MaintainedEvent& held=MaintainedEventsList.Back();
  C2DVector position;position=event->GetJoystickPosition();
  held.Event.SetAsJoystickButtonHeld(event->GetJoystickButton(),event->GetJoystickButtonPressureFraction(),position,event->StartTime);
  held.Event.DeviceNumber=event->DeviceNumber;
 }else if(event->GetType()==21){
  for(EventNode* node=MaintainedEventsList.Head->Next;node!=MaintainedEventsList.Head;node=node->Next){
   if(node->Value.Event.GetType()==20&&node->Value.Event.GetJoystickButton()==event->GetJoystickButton()){
    MaintainedEventsList.Erase(node);break;
   }
  }
 }
}
FABLE_STATIC_ASSERT(sizeof(CInputEvent)==52);
FABLE_STATIC_ASSERT(sizeof(MaintainedEvent)==56);
FABLE_STATIC_ASSERT(offsetof(JoystickNative,MaintainedEventsList)==0xd0c);