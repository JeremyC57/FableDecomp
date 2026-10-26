// 005466b0 NUISystem::CZoomingComponent::Pan, 314 bytes.
// Pan name/signature recovered from Ego_r PDB; vtable142 and actual body agree.
// Layout: generated header absent. PDB ZoomCenter +0x190, Panning+0x1a8,
// Zooming+0x1a9 are retail +0x160,+0x178,+0x179 (base shrink 0x30).
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
struct Vec { float X,Y; Vec() {} Vec(float x,float y):X(x),Y(y) {} Vec(const Vec& v):X(v.X),Y(v.Y) {} Vec operator*(float f) const { return Vec(X*f,Y*f); } Vec& operator-=(const Vec& v) { X-=v.X; Y-=v.Y; return *this; } };
namespace NUISystem {
class CZoomingComponent { public:
 virtual void Slot000() = 0;
 virtual void Slot001() = 0;
 virtual void Slot002() = 0;
 virtual void Slot003() = 0;
 virtual void Slot004() = 0;
 virtual void Slot005() = 0;
 virtual void Slot006() = 0;
 virtual void Slot007() = 0;
 virtual void Slot008() = 0;
 virtual void Slot009() = 0;
 virtual void Slot010() = 0;
 virtual void Slot011() = 0;
 virtual Vec GetSize() = 0;
 virtual void Slot013() = 0;
 virtual void Slot014() = 0;
 virtual void Slot015() = 0;
 virtual Vec GetPosition() = 0;
 virtual void Slot017() = 0;
 virtual void Slot018() = 0;
 virtual void Slot019() = 0;
 virtual void Slot020() = 0;
 virtual void Slot021() = 0;
 virtual void Slot022() = 0;
 virtual void Slot023() = 0;
 virtual void Slot024() = 0;
 virtual void Slot025() = 0;
 virtual void Slot026() = 0;
 virtual void Slot027() = 0;
 virtual void Slot028() = 0;
 virtual void Slot029() = 0;
 virtual void Slot030() = 0;
 virtual void SetPosition(const Vec& p,float time,int mode) = 0;
 unsigned char InheritedState[0x160-4];
 Vec ZoomCenter,MinZoom,MaxZoom;
 bool Panning,Zooming;
 void Pan(Vec movement);
};
void CZoomingComponent::Pan(Vec movement) {
 if(sqrt(movement.X*movement.X+movement.Y*movement.Y)>0.01) {
  if(!Zooming) {
   Vec position=GetPosition();
   Vec size=GetSize();
   position-=movement*30.0f;
   if(position.X>ZoomCenter.X) position.X=ZoomCenter.X;
   float minX=ZoomCenter.X-size.X*1380.0f;
   if(position.X<minX) position.X=minX;
   if(position.Y>ZoomCenter.Y) position.Y=ZoomCenter.Y;
   float minY=ZoomCenter.Y-size.Y*1892.0f;
   if(position.Y<minY) position.Y=minY;
   SetPosition(position,0.2f,0);
   Panning=true;
  } else Zooming=false;
 } else Panning=false;
}
}