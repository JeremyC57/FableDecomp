// NUISystem::CDraggable::Update(float) -- finish dragging, update clickable, track snap-back completion and cursor
// Native298bytes. Actual donor headers retained; retail component differs: Time30 vs38,Position34 vs3C,Layer12F vs157;
// retail drag extension194 vs1C4. No CDraggable header exists. Native projections use named fields proven by donor+landed ctor55A9C0.
// CManager native PDropped+BC vs donorEC. Native table124BA94 slot31=55A630 ChangePosition;slot153=55A7C0 UpdateMouseCursor.
// 41E5F2 manager singleton;55ACB0 CClickable::Update. Float129BA3C=0.0001,122DEDC=0.
// Leaf: ?Update@CDraggable@NUISystem@@UAEXM@Z
// Naming wave 28 (family clk): the base update 0x0055ACB0 is ?Update@CClickable@NUISystem@@UAEXM@Z (chunk 696), called
//   qualified on the polymorphic CClickable base; the own Leaf is the virtual (Ego_r / FableWin UAE). Was the non-virtual
//   CDraggable::UpdateClickable (QAE) over a flattened VTable member.
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)
#include <stddef.h>
#include "engine/CManager.h"
#include "engine/CHoverable.h"
struct NativeUIManager {unsigned char Prefix[0xbc];void* PDropped;};
NativeUIManager* __cdecl GetUIManager(); //0041E5F2
extern const float DELTA_NOTIONAL_ZERO;   // 0x0129BA3C (established spelling, naming wave 13)
namespace NUISystem {
struct C2DVector {float X,Y;bool operator==(const C2DVector& p)const{return (float)fabs(p.X-X)<=DELTA_NOTIONAL_ZERO&&(float)fabs(p.Y-Y)<=DELTA_NOTIONAL_ZERO;}};
class CClickable { public:
 virtual void v000();virtual void Update(float delta); //slot 1: 0055ACB0
 unsigned char BeforeTime[0x30-4];float Time;C2DVector Position,TargetPosition;
 unsigned char BeforeLayer[0x12f-0x44];unsigned char Layer;unsigned char BeforeClick[0x170-0x130];bool WasLeftClicked;
 unsigned char BeforeDrag[0x194-0x171];
};
class CDraggable : public CClickable { public:
 C2DVector PositionBeforeDrag;bool WasDragged;unsigned char Align[3];void* PActionsOnDropped;void* PActionsOnDroppedNowhere;bool SnapBackIfDroppedNowhere,ChangeLayer;
 typedef void(CDraggable::*ChangePositionMethod)(const C2DVector&,float,bool);
 typedef void(CDraggable::*CursorMethod)();
 void ChangePosition(const C2DVector& p,float time,bool relative){(this->*static_cast<ChangePositionMethod*>(*(void**)this)[31])(p,time,relative);}
 void UpdateMouseCursor(){(this->*static_cast<CursorMethod*>(*(void**)this)[153])();}
 bool AtTarget()const{return Position==TargetPosition;}
 virtual void Update(float delta);
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(offsetof(CDraggable,PositionBeforeDrag)==0x194);
FABLE_STATIC_ASSERT(offsetof(CDraggable,ChangeLayer)==0x1a9);
FABLE_STATIC_ASSERT(sizeof(CDraggable::ChangePositionMethod)==4);
FABLE_STATIC_ASSERT(sizeof(CDraggable::CursorMethod)==4);
#endif
void CDraggable::Update(float delta){
 if(WasDragged&&!WasLeftClicked){WasDragged=false;if(GetUIManager()->PDropped){if(PActionsOnDroppedNowhere&&SnapBackIfDroppedNowhere){ChangePosition(PositionBeforeDrag,0.0f,false);++Layer;}else{ChangePosition(PositionBeforeDrag,0.2f,false);ChangeLayer=true;}}else{ChangePosition(PositionBeforeDrag,0.0f,false);++Layer;}}
 CClickable::Update(delta);
 if(!AtTarget()||Time-delta==0.0f)UpdateMouseCursor();else if(AtTarget()&&ChangeLayer){++Layer;ChangeLayer=false;}
}
}