// NUISystem::CManager::ScrollMenu(long) -- scroll active viewport through events, or move front list by25 units per tick.
// Retail0041DA2D length240. Native typed single-inheritance PMF projection, no fabricated virtual declarations.
#pragma optimize("s", on)
#pragma optimize("y", off)
#define CManager CManagerDonor
#include "engine/CManager.h"
#undef CManager
#define CComponent CComponentDonor
#include "engine/_quarantine/CComponent.h"
#undef CComponent
#define CList CListDonor
#include "engine/CList.h"
#undef CList
#include "engine/CScrollingComponent.h"
#ifndef _MSC_VER
#define __forceinline inline
#endif
struct C2DVector {float X,Y;C2DVector(float x,float y):X(x),Y(y){}};
namespace NUISystem {
enum EEvent {ScrollForward=6,ScrollBackward=7}; // Native numeric values proven; event spelling descriptive.
struct CComponent {
 void* VTable;
 typedef bool(CComponent::*DraggedMethod)();
 typedef CComponent*(CComponent::*ScrollBarMethod)();
 typedef void(CComponent::*ChangeVectorMethod)(const C2DVector&,float,bool);
 __forceinline bool IsDragged(){return (this->*static_cast<DraggedMethod*>(VTable)[150])();}
 __forceinline CComponent* GetScrollBar(){return (this->*static_cast<ScrollBarMethod*>(VTable)[175])();}
 __forceinline void ChangePositionDelta(const C2DVector& d,float time,bool linear){(this->*static_cast<ChangeVectorMethod*>(VTable)[33])(d,time,linear);}
};
struct CScrollingViewport:CComponent {
 unsigned char NativeViewportState[0x188-sizeof(void*)];CComponent* PScrollBar;
 typedef float(CScrollingViewport::*SpeedMethod)();
 __forceinline float GetScrollingSpeed(){return (this->*static_cast<SpeedMethod*>(VTable)[144])();}
};
struct ComponentNode {ComponentNode* Next;ComponentNode* Previous;CComponent* Data;};
extern CScrollingViewport* g_pActiveScrollingViewport; //013B8AAC
extern ComponentNode* g_ActiveListHead; //013B8A98
struct CManager {
 void* VTable;unsigned char NativeManagerState[0xc4-sizeof(void*)];float DescriptionScrollingOffset;
 typedef void(CManager::*EventMethod)(EEvent);
 __forceinline void SetEvent(EEvent e){(this->*static_cast<EventMethod*>(VTable)[0])(e);}
 __forceinline void ScrollViewport(CScrollingViewport* viewport,long count){
  if(count>0){DescriptionScrollingOffset=count*viewport->GetScrollingSpeed();SetEvent(ScrollForward);DescriptionScrollingOffset=-1.0f;}
  else{count=-count;DescriptionScrollingOffset=count*viewport->GetScrollingSpeed();SetEvent(ScrollBackward);}
 }
 void ScrollMenu(long);
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(CComponent::DraggedMethod)==4);
FABLE_STATIC_ASSERT(sizeof(CComponent::ScrollBarMethod)==4);
FABLE_STATIC_ASSERT(sizeof(CComponent::ChangeVectorMethod)==4);
FABLE_STATIC_ASSERT(sizeof(CScrollingViewport::SpeedMethod)==4);
FABLE_STATIC_ASSERT(sizeof(CManager::EventMethod)==4);
FABLE_STATIC_ASSERT(offsetof(CScrollingViewport,PScrollBar)==0x188);
FABLE_STATIC_ASSERT(offsetof(CManager,DescriptionScrollingOffset)==0xc4);
#endif
void CManager::ScrollMenu(long count){
 CScrollingViewport* viewport=g_pActiveScrollingViewport;
 if(viewport){
  if(viewport->PScrollBar){
   if(viewport->PScrollBar->IsDragged())return;
   ScrollViewport(viewport,count);
  }else ScrollViewport(viewport,count);
 }else {ComponentNode* first=g_ActiveListHead->Next;if(first!=g_ActiveListHead){
  CComponent* bar=first->Data->GetScrollBar();
  if(bar&&bar->IsDragged())return;
  C2DVector delta(0.0f,count*25.0f);
  g_ActiveListHead->Next->Data->ChangePositionDelta(delta,0.0f,false);
 }}
}
}