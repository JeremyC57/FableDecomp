// NUISystem::CList::ComputeScrollBarZoom -- set scrollbar zoom from visible/total scrolling children
// Retail0053664c length179. Actual donor CList retained; native STL containers use 12-byte vectors,
// and native base is15c rather than donor18c. Native visibility is a byte vector, not donor vector<bool>.
// Virtual ChangeZoom slot35 at+8c; unsigned conversion constant122dcb4=4294967296.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define CList DonorCList
#include "engine/CList.h"
#undef CList
// Actual CComponent donor retained; native interface below projects only its genuine vptr and typed member ABI.
#define CComponent DonorCComponent
#include "engine/_quarantine/CComponent.h"
#undef CComponent
struct C2DVector {float X,Y; C2DVector(float x,float y):X(x),Y(y){} };
namespace NUISystem {
struct CComponent;
#pragma pointers_to_members(full_generality,single_inheritance)
union ComponentMethod {
 void* Address;
 void (CComponent::*ChangeVector)(const C2DVector&,float,bool);
 void (CComponent::*Refresh)();
};
struct CComponent {void* __vftable;};
template<class T> struct Vector {T* First;T* Last;T* End; unsigned int size()const{return (unsigned int)(Last-First);} bool empty()const{return First==Last;} };
struct CList {
    void*           __vftable;                               // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char   _pad_0x04[0x4];                          // +0x04
    unsigned char   _pad_0x08[0x4];                          // +0x08
    unsigned char   _pad_0x0c[0x4];                          // +0x0c
    unsigned char   _pad_0x10[0x4];                          // +0x10
    unsigned char   _pad_0x14[0x4];                          // +0x14
    unsigned char   _pad_0x18[0x4];                          // +0x18
    unsigned char   _pad_0x1c[0x4];                          // +0x1c
    unsigned char   _pad_0x20[0x4];                          // +0x20
    unsigned char   _pad_0x24[0x4];                          // +0x24
    unsigned char   _pad_0x28[0x4];                          // +0x28
    unsigned char   _pad_0x2c[0x4];                          // +0x2c
    unsigned char   _pad_0x30[0x4];                          // +0x30
    unsigned char   _pad_0x34[0x4];                          // +0x34
    unsigned char   _pad_0x38[0x4];                          // +0x38
    unsigned char   _pad_0x3c[0x4];                          // +0x3c
    unsigned char   _pad_0x40[0x4];                          // +0x40
    unsigned char   _pad_0x44[0x4];                          // +0x44
    unsigned char   _pad_0x48[0x4];                          // +0x48
    unsigned char   _pad_0x4c[0x4];                          // +0x4c
    unsigned char   _pad_0x50[0x4];                          // +0x50
    unsigned char   _pad_0x54[0x4];                          // +0x54
    unsigned char   _pad_0x58[0x4];                          // +0x58
    unsigned char   _pad_0x5c[0x4];                          // +0x5c
    unsigned char   _pad_0x60[0x4];                          // +0x60
    unsigned char   _pad_0x64[0x4];                          // +0x64
    unsigned char   _pad_0x68[0x4];                          // +0x68
    unsigned char   _pad_0x6c[0x4];                          // +0x6c
    unsigned char   _pad_0x70[0x4];                          // +0x70
    unsigned char   _pad_0x74[0x4];                          // +0x74
    unsigned char   _pad_0x78[0x4];                          // +0x78
    unsigned char   _pad_0x7c[0x4];                          // +0x7c
    unsigned char   _pad_0x80[0x4];                          // +0x80
    unsigned char   _pad_0x84[0x4];                          // +0x84
    unsigned char   _pad_0x88[0x4];                          // +0x88
    unsigned char   _pad_0x8c[0x4];                          // +0x8c
    unsigned char   _pad_0x90[0x4];                          // +0x90
    unsigned char   _pad_0x94[0x4];                          // +0x94
    unsigned char   _pad_0x98[0x4];                          // +0x98
    unsigned char   _pad_0x9c[0x4];                          // +0x9c
    unsigned char   _pad_0xa0[0x4];                          // +0xa0
    unsigned char   _pad_0xa4[0x4];                          // +0xa4
    unsigned char   _pad_0xa8[0x4];                          // +0xa8
    unsigned char   _pad_0xac[0x4];                          // +0xac
    unsigned char   _pad_0xb0[0x4];                          // +0xb0
    unsigned char   _pad_0xb4[0x4];                          // +0xb4
    unsigned char   _pad_0xb8[0x4];                          // +0xb8
    unsigned char   _pad_0xbc[0x4];                          // +0xbc
    unsigned char   _pad_0xc0[0x4];                          // +0xc0
    unsigned char   _pad_0xc4[0x4];                          // +0xc4
    unsigned char   _pad_0xc8[0x4];                          // +0xc8
    unsigned char   _pad_0xcc[0x4];                          // +0xcc
    unsigned char   _pad_0xd0[0x4];                          // +0xd0
    unsigned char   _pad_0xd4[0x4];                          // +0xd4
    unsigned char   _pad_0xd8[0x4];                          // +0xd8
    unsigned char   _pad_0xdc[0x4];                          // +0xdc
    unsigned char   _pad_0xe0[0x4];                          // +0xe0
    unsigned char   _pad_0xe4[0x4];                          // +0xe4
    unsigned char   _pad_0xe8[0x4];                          // +0xe8
    unsigned char   _pad_0xec[0x4];                          // +0xec
    unsigned char   _pad_0xf0[0x4];                          // +0xf0
    unsigned char   _pad_0xf4[0x4];                          // +0xf4
    unsigned char   _pad_0xf8[0x4];                          // +0xf8
    unsigned char   _pad_0xfc[0x4];                          // +0xfc
    unsigned char   _pad_0x100[0x4];                         // +0x100
    unsigned char   _pad_0x104[0x4];                         // +0x104
    unsigned char   _pad_0x108[0x4];                         // +0x108
    unsigned char   _pad_0x10c[0x4];                         // +0x10c
    unsigned char   _pad_0x110[0x4];                         // +0x110
    unsigned char   _pad_0x114[0x4];                         // +0x114
    unsigned char   _pad_0x118[0x4];                         // +0x118
    unsigned char   _pad_0x11c[0x4];                         // +0x11c
    unsigned char   _pad_0x120[0x4];                         // +0x120
    unsigned char   _pad_0x124[0x4];                         // +0x124
    unsigned char   _pad_0x128[0x4];                         // +0x128
    unsigned char   _pad_0x12c[0x4];                         // +0x12c
    unsigned char   _pad_0x130[0x4];                         // +0x130
    unsigned char   _pad_0x134[0x4];                         // +0x134
    unsigned char   _pad_0x138[0x4];                         // +0x138
    unsigned char   _pad_0x13c[0x4];                         // +0x13c
    unsigned char   _pad_0x140[0x4];                         // +0x140
    unsigned char   _pad_0x144[0x4];                         // +0x144
    unsigned char   _pad_0x148[0x4];                         // +0x148
    unsigned char   _pad_0x14c[0x4];                         // +0x14c
    unsigned char   _pad_0x150[0x4];                         // +0x150
    unsigned char   _pad_0x154[0x4];                         // +0x154
    unsigned char   _pad_0x158[0x4];                         // +0x158
 int SelectedChild; float LastInputTime;
 Vector<CComponent*> ScrollingChildren,NonScrollingChildren;
 Vector<void*> ScrollingChildrenStates;
 C2DVector PositionOffset,PositionOffsetToUse;
 float AlphaOffset,AlphaOffsetToUse,NextScrollingChildAlpha;
 C2DVector NextScrollingChildPosition;
 void* PActionsOnInactive; void* PActionsOnSelected;
 void* CPActionOnScrollUp_Data;void* CPActionOnScrollUp_Info;
 void* CPActionOnScrollDown_Data;void* CPActionOnScrollDown_Info;
 int DefaultChild;Vector<unsigned char> ScrollingChildrenVisibility;
 CComponent* PInitialPositionDummy;CComponent* PScrollBar;CComponent* PScrollBarOutside;CComponent* PListUp;CComponent* PListDown;
 bool RecomputeOffsets;float InputDelay;C2DVector PDefPositionOffset;unsigned char PDefAlphaOffset;
 void ComputeScrollBarZoom();void ScrollToLastChild();
};
}
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(NUISystem::ComponentMethod)==4);
FABLE_STATIC_ASSERT(offsetof(NUISystem::CList,ScrollingChildren)==0x164);
FABLE_STATIC_ASSERT(offsetof(NUISystem::CList,ScrollingChildrenVisibility)==0x1c8);
FABLE_STATIC_ASSERT(offsetof(NUISystem::CList,PScrollBar)==0x1d8);
#endif
void NUISystem::CList::ComputeScrollBarZoom(){
 if(PScrollBar && !ScrollingChildren.empty()){
  C2DVector zoom(1.0f,1.0f);
  unsigned int visible=0;
  for(unsigned int i=0;i<ScrollingChildrenVisibility.size();++i)
   if(ScrollingChildrenVisibility.First[i]) ++visible;
  zoom.Y=(float)visible/(float)ScrollingChildren.size();
  (PScrollBar->*static_cast<const ComponentMethod*>(PScrollBar->__vftable)[35].ChangeVector)(zoom,0.0f,false);
 }
}