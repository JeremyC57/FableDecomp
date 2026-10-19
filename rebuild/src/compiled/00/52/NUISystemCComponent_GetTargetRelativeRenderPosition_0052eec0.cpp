// NUISystem::CComponent::GetTargetRelativeRenderPosition()const -- relative-coordinate conversion and optional parent zoom/position transform.
// Retail0052EEC0 length285. Actual component table124608C slots102=52F1A0 IsPositionIndependent,
//117=52F3B0 IsUsingRelativePosition,122=self,123=52ECC0 GetTargetRelativeRenderZoom.
// Virtual vector results use explicit ABI result buffer: ECX=this, stack=result, EAX=result, ret4.
// Donor targetposition44->native3c, parentD8->C8, positionparent13C->118 due native STL sizes.
#define CComponent CComponent_PDB
#include "engine/_quarantine/CComponent.h"
#undef CComponent
struct C2DVector {
 float X,Y;C2DVector(){}C2DVector(float x,float y):X(x),Y(y){}
 C2DVector(const C2DVector& v):X(v.X),Y(v.Y){}
};
namespace NUISystem {
struct CUIScreenUnitConverter {static C2DVector FABLE_FASTCALL ConvertVector(C2DVector);}; //0052E580 actual donor55ED1A; ECXhiddenbuffer,stack8,ret8
struct CComponent;
struct CComponent {
 void* __vftable;unsigned char _beforeTargetPosition[0x38];C2DVector TargetPosition;
 unsigned char _beforeParent[0x84];CComponent* PParent;unsigned char _beforePositionParent[0x4c];CComponent* PPositionParent;
 bool IsPositionIndependent()const;
 bool IsUsingRelativePosition()const;
 const C2DVector& TargetRelativeZoomInto(C2DVector& out)const;
 const C2DVector& TargetRelativePositionInto(C2DVector& out)const;
 __forceinline C2DVector TransformPosition(C2DVector& position,CComponent* parent)const{

  if(!IsPositionIndependent()){
   {C2DVector result;position.X*=parent->TargetRelativeZoomInto(result).X;}
   {C2DVector result;position.Y*=parent->TargetRelativeZoomInto(result).Y;}
   {C2DVector result;const C2DVector& base=parent->TargetRelativePositionInto(result);position.X+=base.X;position.Y+=base.Y;}
  }
  return C2DVector(position.X,position.Y);
 }
 C2DVector GetTargetRelativeRenderPosition()const;
};
union CComponentSlot {
 void* Address;
 bool(CComponent::*IsPositionIndependent)()const;
 bool(CComponent::*IsUsingRelativePosition)()const;
 const C2DVector&(CComponent::*GetTargetRelativeRenderPositionInto)(C2DVector&)const;
 const C2DVector&(CComponent::*GetTargetRelativeRenderZoomInto)(C2DVector&)const;
};
__forceinline bool CComponent::IsPositionIndependent()const{return (this->*reinterpret_cast<CComponentSlot*>(__vftable)[102].IsPositionIndependent)();}
__forceinline bool CComponent::IsUsingRelativePosition()const{return (this->*reinterpret_cast<CComponentSlot*>(__vftable)[117].IsUsingRelativePosition)();}
__forceinline const C2DVector& CComponent::TargetRelativeZoomInto(C2DVector& out)const{return (this->*reinterpret_cast<CComponentSlot*>(__vftable)[123].GetTargetRelativeRenderZoomInto)(out);}
__forceinline const C2DVector& CComponent::TargetRelativePositionInto(C2DVector& out)const{return (this->*reinterpret_cast<CComponentSlot*>(__vftable)[122].GetTargetRelativeRenderPositionInto)(out);}
FABLE_STATIC_ASSERT(sizeof(CComponentSlot)==4);
FABLE_STATIC_ASSERT(offsetof(CComponent,TargetPosition)==0x3c);
FABLE_STATIC_ASSERT(offsetof(CComponent,PParent)==0xc8);
FABLE_STATIC_ASSERT(offsetof(CComponent,PPositionParent)==0x118);
C2DVector CComponent::GetTargetRelativeRenderPosition()const{
 C2DVector position=TargetPosition;
 if(IsUsingRelativePosition())position=CUIScreenUnitConverter::ConvertVector(position);
 CComponent* parent=PPositionParent;if(!parent)parent=PParent;
 if(parent&&!IsPositionIndependent())return TransformPosition(position,parent);
 return position;
}
}