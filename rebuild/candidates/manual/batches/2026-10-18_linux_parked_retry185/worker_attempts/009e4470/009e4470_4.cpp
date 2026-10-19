// CJoystick::ProcessMaintainedEvents(const CInputEvent*). Leaf: ProcessMaintainedEvents
#include "engine/CJoystick.h"
#include <stdlib.h>
struct Vector2{float X,Y;Vector2(){}Vector2(const Vector2& b):X(b.X),Y(b.Y){}};
struct InputEventNative {
 unsigned char Data[32];long DeviceType,DeviceNumber,Type;float CurrentTime,StartTime;
 InputEventNative();~InputEventNative();
 long GetType()const;long GetJoystickButton()const;float GetJoystickButtonPressureFraction()const;Vector2 GetJoystickPosition()const;
 void SetAsJoystickButtonHeld(long,float,const Vector2&,double);
};
struct BaseClassNative{BaseClassNative();virtual ~BaseClassNative();};
struct MaintainedEvent:BaseClassNative{InputEventNative Event;MaintainedEvent();virtual ~MaintainedEvent();};
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
 void ProcessMaintainedEvents(const InputEventNative*);
};
void JoystickNative::ProcessMaintainedEvents(const InputEventNative* event){
 if(event->GetType()==19){
  MaintainedEventsList.PushBack();
  MaintainedEvent& held=MaintainedEventsList.Back();
  Vector2 position;position=event->GetJoystickPosition();
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
FABLE_STATIC_ASSERT(sizeof(InputEventNative)==52);
FABLE_STATIC_ASSERT(sizeof(MaintainedEvent)==56);
FABLE_STATIC_ASSERT(offsetof(JoystickNative,MaintainedEventsList)==0xd0c);
