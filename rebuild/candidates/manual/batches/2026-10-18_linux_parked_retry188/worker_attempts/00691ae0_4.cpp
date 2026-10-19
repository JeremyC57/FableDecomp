// CInputProcessPhotojournalCapture::ProcessInput(const CInputEvent&,CProcessedInput&) -- translate character and photojournal actions into GUI events.
// Leaf: ProcessInput
#include "engine/CInputProcessGameBase.h"
#include "engine/CProcessedInput.h"
#include "engine/CManager.h"
namespace PhotoInput {
struct Vector2 {float X,Y;Vector2(){} Vector2(const Vector2& v):X(v.X),Y(v.Y){}}; // PDB 01898E60 genuine C2DVector copy constructor
enum EInputEventType { Character=1,RepeatedCharacter=15 }; // PDB original enum type; descriptive enumerator names for native observed values
enum EGameAction { Capture=71,Cancel=25,Back=76 };
enum EEvent { UIBack=5,UICapture=25,UICancel=26,UICharacter=33,UIRepeatedCharacter=34 };
struct InputEvent {unsigned char DataPrefix[12];Vector2 MouseMovementXY;float MouseMovementZ;Vector2 MousePos;long DeviceType,DeviceNumber;EInputEventType Type;float CurrentTime,StartTime;EInputEventType GetType()const;wchar_t GetChar()const;};
#pragma pointers_to_members(full_generality,single_inheritance)
struct Manager;
union UISlot {void* Native;void(Manager::*Event)(EEvent);void(Manager::*Char)(wchar_t);};
struct Manager {void* VTable;unsigned char BeforeMouseMovement[0xac];Vector2 MouseMovement;static Manager* __fastcall GetInstance();void SetMouseMovement(Vector2& v){MouseMovement=v;}}; //native mouse+B0 vs donor E0, actual GUI CManager header above
#ifdef _MSC_VER
typedef char native_ui_slot[sizeof(UISlot)==4?1:-1];
#endif
// Field-free native method view: helper owners are CInputProcessGameBase in PDB.
struct PhotoCapture:CInputProcessGameBase {
 protected:bool IsEventButtonHeld(const InputEvent&)const; // PDB6B2C40 IBE, protected const
 public:bool IsEventGameAction(EGameAction,const InputEvent&)const;
 Vector2 GetMouseMovement(const InputEvent& e)const{return e.MouseMovementXY;} // PDB 006B2E30
 void ProcessInput(const InputEvent&,CProcessedInput&);
};
inline void Send(EEvent event){Manager* m=Manager::GetInstance();(m->*static_cast<UISlot*>(m->VTable)[0].Event)(event);}
void PhotoCapture::ProcessInput(const InputEvent& event,CProcessedInput&){
 if(event.GetType()==Character){Manager* m=Manager::GetInstance();(m->*static_cast<UISlot*>(m->VTable)[12].Char)(event.GetChar());Send(UICharacter);}
 if(event.GetType()==RepeatedCharacter){Manager* m=Manager::GetInstance();(m->*static_cast<UISlot*>(m->VTable)[12].Char)(event.GetChar());Send(UIRepeatedCharacter);}
 if(IsEventButtonHeld(event)){
  if(IsEventGameAction(Capture,event)){Vector2 movement=GetMouseMovement(event);Manager::GetInstance()->SetMouseMovement(movement);Send(UICapture);}
  else if(IsEventGameAction(Cancel,event))Send(UICancel);
  else if(IsEventGameAction(Back,event))Send(UIBack);
 }else if(IsEventGameAction(Capture,event)){Vector2 movement=GetMouseMovement(event);Manager::GetInstance()->SetMouseMovement(movement);Send(UICapture);}
}
}
