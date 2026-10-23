// 00761620 NShape::CTrack::EditorDrawPreparePrimitivesForRendering,288bytes.
// Name/signature recovered from Ego_r PDB; RTTI CTrack slot3.
// For each segment draw previous->next and previous->ground-projected previous.
// Layout: no CTrack or CShape generated header. Retail Points vector at+8 is12bytes.
// CEnginePrimitiveLine is0x3c (constructor004c3090); info.Engine at+0x34.
#include "engine/CWorldMap.h"
struct Vec3 {float X,Y,Z;Vec3(){}Vec3(float x,float y,float z):X(x),Y(y),Z(z){} Vec3(const Vec3& v):X(v.X),Y(v.Y),Z(v.Z){} };
struct Colour {unsigned char B,G,R,A;};
struct CEnginePrimitiveLine {
 unsigned long Type,InterpolationSet;unsigned char Theme[4];
 Vec3 Start,End;Colour Color;float Width;unsigned long BankIndex;long BlendMode,BlendOp;bool UseZBuffer;unsigned char Align[3];
 CEnginePrimitiveLine(unsigned __int64,const Vec3&,const Vec3&,const Colour&,float,long,unsigned long,bool,long,long); //0x004c3090
};
struct EngineCalls {
 virtual void Slot00()=0;
 virtual void Slot01()=0;
 virtual void Slot02()=0;
 virtual void Slot03()=0;
 virtual void Slot04()=0;
 virtual void Slot05()=0;
 virtual void Slot06()=0;
 virtual void Slot07()=0;
 virtual void Slot08()=0;
 virtual void Slot09()=0;
 virtual void Slot10()=0;
 virtual void Slot11()=0;
 virtual void Slot12()=0;
 virtual void Slot13()=0;
 virtual void Slot14()=0;
 virtual void Slot15()=0;
 virtual void Slot16()=0;
 virtual void Slot17()=0;
 virtual void Slot18()=0;
 virtual void Slot19()=0;
 virtual void SetPrimitive(const CEnginePrimitiveLine&)=0;
};
struct PrepareInfo {unsigned char BeforeEngine[0x34];EngineCalls* Engine;};
struct WorldMapCalls:CWorldMap {float GetGroundSizeZAt(const Vec3*) const;}; //0x0063a300
struct PointVector {Vec3* Start;Vec3* Finish;Vec3* Limit;unsigned size()const{return Finish-Start;}};
namespace NShape {
struct CTrack {void* Primary;void* Owner;PointVector Points;
 void EditorDrawPreparePrimitivesForRendering(const PrepareInfo&,const WorldMapCalls&,const Vec3&,const Colour&);
};
void CTrack::EditorDrawPreparePrimitivesForRendering(const PrepareInfo& info,const WorldMapCalls& world,const Vec3&,const Colour& color) {
 if(Points.size()>1) {
  Vec3* it=Points.Start;
  Vec3 previous=*it;
  ++it;
  for(;it!=Points.Finish;++it) {
   Vec3 ground;ground.X=previous.X;ground.Y=previous.Y;ground.Z=world.GetGroundSizeZAt(&previous);
   info.Engine->SetPrimitive(CEnginePrimitiveLine(0,previous,*it,color,0.05f,0,0,true,0,0));
   info.Engine->SetPrimitive(CEnginePrimitiveLine(0,previous,ground,color,0.05f,0,0,true,0,0));
   previous=*it;
  }
 }
}
}