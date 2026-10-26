// BEGIN exact source snapshot SHA256 00bc20dbc3950dc6bc79330322f3a39c785f7677478d06979d1601dda385f22f
// 00761620 NShape::CTrack::EditorDrawPreparePrimitivesForRendering,288bytes.
// Name/signature recovered from Ego_r PDB; RTTI CTrack slot3.
// For each segment draw previous->next and previous->ground-projected previous.
// Layout: no CTrack or CShape generated header. Retail Points vector at+8 is12bytes.
// CEnginePrimitiveLine is0x3c (constructor004c3090); info.Engine at+0x34.
#include "engine/CWorldMap.h"
struct Vec3 {float X,Y,Z;Vec3(){}Vec3(float x,float y,float z):X(x),Y(y),Z(z){} Vec3(const Vec3& v):X(v.X),Y(v.Y),Z(v.Z){} };
struct Colour {unsigned char B,G,R,A;};
class CEnginePrimitiveLine { public:
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
class CTrack { public:void* Primary;void* Owner;PointVector Points;
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

// END exact source snapshot
#include <stdio.h>
CEnginePrimitiveLine::CEnginePrimitiveLine(unsigned __int64,const Vec3& s,const Vec3& e,const Colour& c,float w,long b,unsigned long f,bool z,long i,long op):Type(6),InterpolationSet(i),Start(s),End(e),Color(c),Width(w),BankIndex(f),BlendMode(b),BlendOp(op),UseZBuffer(z){}
float WorldMapCalls::GetGroundSizeZAt(const Vec3* p)const{return p->X+p->Y;}
struct Engine:EngineCalls {Vec3 starts[4],ends[4];int count;Engine():count(0){}
 void SetPrimitive(const CEnginePrimitiveLine& l){if(count<4){starts[count]=l.Start;ends[count]=l.End;}++count;}
 void Slot00(){}
 void Slot01(){}
 void Slot02(){}
 void Slot03(){}
 void Slot04(){}
 void Slot05(){}
 void Slot06(){}
 void Slot07(){}
 void Slot08(){}
 void Slot09(){}
 void Slot10(){}
 void Slot11(){}
 void Slot12(){}
 void Slot13(){}
 void Slot14(){}
 void Slot15(){}
 void Slot16(){}
 void Slot17(){}
 void Slot18(){}
 void Slot19(){}
};
int main(){
 NShape::CTrack t;Vec3 points[3]={Vec3(1,2,8),Vec3(3,4,9),Vec3(5,6,10)};Engine e;PrepareInfo info;info.Engine=&e;WorldMapCalls world;Colour c={1,2,3,4};Vec3 cursor;
 t.Points.Start=points;t.Points.Finish=points+1;t.EditorDrawPreparePrimitivesForRendering(info,world,cursor,c);if(e.count)return 1;
 t.Points.Finish=points+3;t.EditorDrawPreparePrimitivesForRendering(info,world,cursor,c);if(e.count!=4)return 2;
 if(e.starts[0].X!=1||e.starts[0].Z!=8||e.ends[0].X!=3||e.ends[0].Z!=9)return 3;
 if(e.ends[1].X!=1||e.ends[1].Y!=2||e.ends[1].Z!=3)return 4;
 if(e.starts[2].X!=3||e.ends[2].X!=5||e.ends[3].Z!=7)return 5;
 puts("00761620_TEST PASS");return 0;
}