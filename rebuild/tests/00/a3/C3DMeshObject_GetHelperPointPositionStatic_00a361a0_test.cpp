// C3DMeshObject::GetHelperPointPositionStatic(const CCharString&,unsigned long) -- transform a named helper's position by the object's matrix.
// Retail 0x00A361A0, 193 bytes. CRC(seed=0) 0x004014A0; mesh-stat GetHelperPoint 0x00A2E8C0.
// PDB PositionMatrix starts +0x10; mesh stats counted pointer Data is +8. Empty string oracle 0x0129AAF4.
#ifdef _MSC_VER
#include "fable_string.h"
#else
#define __fastcall
class CCharStringData { public: char* text; unsigned int unknown04,unknown08; unsigned char flags0C; int owners; };
class CCharString { public: CCharString(); ~CCharString(); CCharStringData* storage_; };
#endif
// Header exposes the real data record; the one-pointer object view supplies native inline accessors.
struct CCharStringNativeView {
    CCharStringData* PStringData;
    const char* c_str() const { return PStringData == 0 ? "" : PStringData->text; }
    unsigned long length() const { return PStringData == 0 ? 0 : PStringData->unknown04; }
};
class C3DVector { public:
    float X,Y,Z;
    C3DVector() {}
    C3DVector(float x,float y,float z):X(x),Y(y),Z(z) {}
    void Set(float x,float y,float z) { X=x;Y=y;Z=z; }
    C3DVector(const C3DVector& p):X(p.X),Y(p.Y),Z(p.Z) {}
};
class C3DHelperPoint { public: C3DVector Position; long BoneIndex; };
class C3DMeshStats { public: C3DHelperPoint* GetHelperPoint(unsigned long,unsigned long); };
unsigned long __fastcall ComputeCRC(unsigned long seed,const char* text,unsigned long length); // 0x004014A0
struct CMeshPointer { C3DMeshStats* Data; void* Info; C3DMeshStats* operator->() const { return Data; } };
class C3DMeshObject { public:
    void* __vftable;
    void* MeshInfo;
    CMeshPointer Mesh;
    float PositionMatrix[12];
    C3DVector GetHelperPointPositionStatic(const CCharString&,unsigned long);
};
C3DVector C3DMeshObject::GetHelperPointPositionStatic(const CCharString& name,unsigned long index)
{
    const CCharStringNativeView& text=*reinterpret_cast<const CCharStringNativeView*>(&name);
    const C3DVector& position = Mesh->GetHelperPoint(ComputeCRC(0,text.c_str(),text.length()),index)->Position;
    C3DVector point(position);
    // Transform the copied point in place, preserving the input coordinates needed later.
    const float x = point.X;
    const float y = point.Y;
    point.X = x*PositionMatrix[0] + y*PositionMatrix[3] + point.Z*PositionMatrix[6] + PositionMatrix[9];
    point.Y = x*PositionMatrix[1] + y*PositionMatrix[4] + point.Z*PositionMatrix[7] + PositionMatrix[10];
    point.Z = x*PositionMatrix[2] + y*PositionMatrix[5] + point.Z*PositionMatrix[8] + PositionMatrix[11];
    return point;
}

#include <cstdio>
#include <cstring>
static int errors,stage,scenario;
static CCharStringData textData;
static C3DHelperPoint helper;
static C3DMeshStats originalStats, replacementStats;
static C3DMeshObject* object;
static unsigned long expectedIndex;
static const char* expectedText;
static unsigned long expectedLength;
CCharString::CCharString(){storage_=scenario==0?0:&textData;}
CCharString::~CCharString(){}
unsigned long __fastcall ComputeCRC(unsigned long seed,const char* text,unsigned long length){
 if(stage!=0||seed!=0||length!=expectedLength||std::strcmp(text,expectedText))++errors;
 if(scenario!=0&&text!=textData.text)++errors;
 stage=1; // Leave the receiver unchanged: C++ argument/receiver evaluation order differs at /Od.
 return 0xa1234567UL;
}
C3DHelperPoint* C3DMeshStats::GetHelperPoint(unsigned long crc,unsigned long index){
 if(stage!=1||this!=&originalStats||crc!=0xa1234567UL||index!=expectedIndex)++errors;
 stage=2;
 if(scenario==3){object->PositionMatrix[9]=13;object->PositionMatrix[10]=-17;object->PositionMatrix[11]=19;}
 return &helper;
}
int main(){
 C3DMeshObject mesh;object=&mesh;
 char name[]="HELPER_TEST";textData.text=name;textData.unknown04=11;textData.unknown08=77;textData.flags0C=3;textData.owners=8;
 const float matrices[4][12]={
  {1,0,0,0,1,0,0,0,1,0,0,0},
  {1,0,0,0,1,0,0,0,1,7,-11,13},
  {2,3,5,7,11,13,17,19,23,29,31,37},
  {-2,3,-5,7,-11,13,-17,19,-23,0,0,0}
 };
 for(scenario=0;scenario<4;++scenario){
  stage=0;mesh.Mesh.Data=&originalStats;mesh.Mesh.Info=(void*)&textData;
  for(int i=0;i<12;++i)mesh.PositionMatrix[i]=matrices[scenario][i];
  helper.Position.Set(2,-3,5);helper.BoneIndex=41;
  expectedIndex=scenario==3?0xfedcba98UL:(unsigned long)scenario;
  expectedText=scenario==0?"":name;expectedLength=scenario==0?0:11;
  CCharString input;
  C3DVector got=mesh.GetHelperPointPositionStatic(input,expectedIndex);
  float expected[3];
  for(int axis=0;axis<3;++axis)expected[axis]=2*mesh.PositionMatrix[axis]-3*mesh.PositionMatrix[axis+3]+5*mesh.PositionMatrix[axis+6]+mesh.PositionMatrix[axis+9];
  if(got.X!=expected[0]||got.Y!=expected[1]||got.Z!=expected[2]||stage!=2)++errors;
  if(helper.Position.X!=2||helper.Position.Y!=-3||helper.Position.Z!=5||helper.BoneIndex!=41)++errors;
  if(mesh.Mesh.Data!=&originalStats||mesh.Mesh.Info!=&textData||textData.owners!=8||textData.unknown04!=11||textData.unknown08!=77||textData.flags0C!=3)++errors;
 }
 if(errors){std::printf("00a361a0_TEST FAIL %d\n",errors);return 1;}
 std::printf("00a361a0_TEST PASS\n");return 0;
}