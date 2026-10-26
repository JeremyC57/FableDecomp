// BEGIN exact source snapshot SHA256 0c02fed38d7b0c3adf162eb673e3d5d1ca6718ec43b67e22b819a6cc48f19a46
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

// END exact source snapshot
#include <stdio.h>
struct Fixture:NUISystem::CZoomingComponent {
 Vec position,size,result; int sets;
 Fixture():position(0,0),size(1,1),result(0,0),sets(0) { ZoomCenter=Vec(0,0); Panning=false; Zooming=false; }
 Vec GetSize(){return size;} Vec GetPosition(){return position;}
 void SetPosition(const Vec& p,float,int){result=p;++sets;}
 void Slot000() {}
 void Slot001() {}
 void Slot002() {}
 void Slot003() {}
 void Slot004() {}
 void Slot005() {}
 void Slot006() {}
 void Slot007() {}
 void Slot008() {}
 void Slot009() {}
 void Slot010() {}
 void Slot011() {}
 void Slot013() {}
 void Slot014() {}
 void Slot015() {}
 void Slot017() {}
 void Slot018() {}
 void Slot019() {}
 void Slot020() {}
 void Slot021() {}
 void Slot022() {}
 void Slot023() {}
 void Slot024() {}
 void Slot025() {}
 void Slot026() {}
 void Slot027() {}
 void Slot028() {}
 void Slot029() {}
 void Slot030() {}
};
int main() {
 Fixture f;f.Pan(Vec(0,0));if(f.Panning || f.sets) return 1;
 f.Pan(Vec(0.125f,0.25f));if(!f.Panning||f.sets!=1||f.result.X!=-3.75f||f.result.Y!=-7.5f)return 2;
 f.Zooming=true;f.Pan(Vec(1,1));if(f.Zooming||f.sets!=1)return 3;
 f.Pan(Vec(-1,-1));if(f.result.X!=0||f.result.Y!=0)return 4;
 f.Pan(Vec(128,128));if(f.result.X!=-1380||f.result.Y!=-1892)return 5;
 f.Pan(Vec(0,0));if(f.Panning)return 6;
 puts("005466b0_TEST PASS");return 0;
}