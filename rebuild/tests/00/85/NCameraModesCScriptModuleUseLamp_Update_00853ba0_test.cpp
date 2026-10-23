// 00853ba0 NCameraModes::CScriptModuleUseLamp::Update
// Flags: /O2 /Oy /W3
// Leaf: Update
// Shape: Height is copied into a z-only offset vector before the angle (retail loads +0x78 first and keeps it on the x87 stack
//   across the sin/cos block); the focal query takes GetPItem() into a local CThing* so the appearance load goes straight to ecx.
#include <math.h>
#pragma intrinsic(sin,cos)
namespace NCameraModes {
// Layout: camera pointer+38, intelligent handles8 at54/5c; Origin64, scalar phases70/74/78 and deltas7c/80/84, AxisX88/AxisY94; pointer vptr0/link4.
struct C3DVector{float x,y,z;C3DVector(){}__forceinline C3DVector(float a,float b,float c):x(a),y(b),z(c){}__forceinline C3DVector(const C3DVector&p):x(p.x),y(p.y),z(p.z){}__forceinline C3DVector&operator+=(const C3DVector&b){x+=b.x;y+=b.y;z+=b.z;return *this;}__forceinline C3DVector operator*(float a)const{C3DVector r;r.x=x*a;r.y=y*a;r.z=z*a;return r;}__forceinline C3DVector operator+(const C3DVector&b)const{C3DVector r;r.x=x+b.x;r.y=y+b.y;r.z=z+b.z;return r;}};
struct CCharString{void*PStringData;};
extern const CCharString g_helperCameraFocal_013bac4c,g_helperFocal_013bac48;
struct CTCGraphicAppearance{bool GetHelperPoint(const CCharString&,int,unsigned,C3DVector&,float)const;C3DVector GetFocalPos(float)const;C3DVector GetCameraFocalPos(float)const;};
__declspec(noinline) C3DVector CTCGraphicAppearance::GetCameraFocalPos(float time)const{C3DVector pos;if(GetHelperPoint(g_helperCameraFocal_013bac4c,0,4,pos,time))return pos;if(GetHelperPoint(g_helperFocal_013bac48,0,4,pos,time))return pos;return GetFocalPos(time);}

struct CThing{char pad[0x64];CTCGraphicAppearance*appearance;char gap[0x29];unsigned char dead;};
struct LinkNode{CThing*object;unsigned refs;};
struct CBaseIntelligentPointer{LinkNode*link;CBaseIntelligentPointer():link(0){}virtual ~CBaseIntelligentPointer(){if(link&&!--link->refs&&!link->object)delete link;}__declspec(noinline) CThing*GetPItem(){if(!link)return 0;if(link->object)return link->object;LinkNode*p=link;link=0;if(!--p->refs)delete p;return 0;}};
struct Camera{char pad[0x10];C3DVector focal;char gap[0x38];C3DVector position;};extern const double Scale;
// Layout: RTTI verifies NCameraModes::CScriptModuleUseLamp; scalar Theta70/Rad74/Height78 and delta7c/80/84, AxisX88/AxisY94 per Ego_r160-byte layout.
struct CScriptModuleUseLamp{char pad[0x38];Camera*camera;char gap[0x18];CBaseIntelligentPointer target,owner;C3DVector Origin;float Theta,Rad,Height,DeltaTheta,DeltaRad,DeltaHeight;C3DVector AxisX,AxisY;void Update();};
void CScriptModuleUseLamp::Update(){if(!owner.GetPItem()||(owner.GetPItem()->dead&1))return;if(!target.GetPItem()||(target.GetPItem()->dead&1))return;{C3DVector heightOffset(0,0,Height);C3DVector*dest=&camera->position;double scale=Scale;double angle=Theta*scale;C3DVector pos=(AxisX*(float)sin(angle)+AxisY*(float)cos(angle))*Rad+Origin;pos.z+=heightOffset.z;Theta=DeltaTheta+Theta;Rad=DeltaRad+Rad;Height=DeltaHeight+Height;*dest=pos;}{CThing* thing=target.GetPItem();C3DVector focal=thing->appearance->GetCameraFocalPos(1.0f);camera->focal=focal;}}
}


#include <stdio.h>
#include <assert.h>
#include <string.h>
namespace NCameraModes {const double Scale=6.2831854820251465;static int focalCalls;static C3DVector expectedFocus(4,-5,6);const CCharString g_helperCameraFocal_013bac4c={0},g_helperFocal_013bac48={0};bool CTCGraphicAppearance::GetHelperPoint(const CCharString&name,int index,unsigned flags,C3DVector&out,float time)const{assert(&name==&g_helperCameraFocal_013bac4c&&index==0&&flags==4&&time==1);++focalCalls;out=expectedFocus;return true;}C3DVector CTCGraphicAppearance::GetFocalPos(float time)const{assert(time==1);return expectedFocus;}}
static bool Near(float a,float b){return fabs((double)a-b)<0.00002;}
int main(){using namespace NCameraModes;assert(sizeof(CBaseIntelligentPointer)==8&&sizeof(C3DVector)==12);CScriptModuleUseLamp s;Camera c;CThing owner,target;CTCGraphicAppearance appearance;memset(&c,0,sizeof c);memset(&owner,0,sizeof owner);memset(&target,0,sizeof target);s.camera=&c;s.Origin=C3DVector(1,-2,3);s.Theta=0.4f;s.Rad=2;s.Height=5;s.DeltaTheta=0.125f;s.DeltaRad=-0.25f;s.DeltaHeight=0.5f;s.AxisX=C3DVector(2,3,-4);s.AxisY=C3DVector(-1,2,0.5f);s.Update();assert(!focalCalls&&c.position.x==0);LinkNode*o=new LinkNode;o->object=&owner;o->refs=1;s.owner.link=o;owner.dead=1;s.Update();assert(!focalCalls);owner.dead=0;s.Update();assert(!focalCalls);LinkNode*t=new LinkNode;t->object=&target;t->refs=1;s.target.link=t;target.dead=1;s.Update();assert(!focalCalls);target.dead=0;target.appearance=&appearance;for(int i=0;i<3;++i){double angle=s.Theta*Scale;float amplitude=s.Rad,height=s.Height;float expected[3];float AxisX[3]={s.AxisX.x,s.AxisX.y,s.AxisX.z},AxisY[3]={s.AxisY.x,s.AxisY.y,s.AxisY.z},Origin[3]={s.Origin.x,s.Origin.y,s.Origin.z};for(int j=0;j<3;++j)expected[j]=(float)((float)(AxisX[j]*(float)sin(angle))+(float)(AxisY[j]*(float)cos(angle)))*amplitude+Origin[j];expected[2]+=height;float oldAngle=s.Theta;s.Update();assert(Near(c.position.x,expected[0])&&Near(c.position.y,expected[1])&&Near(c.position.z,expected[2])&&Near(s.Theta,oldAngle+0.125f)&&c.focal.x==4&&c.focal.y==-5&&c.focal.z==6&&focalCalls==i+1);}s.Theta=-0.125f;s.Rad=0;s.Height=-3;s.DeltaTheta=0;s.DeltaRad=0;s.DeltaHeight=0;s.Update();assert(Near(c.position.x,1)&&Near(c.position.y,-2)&&Near(c.position.z,0)&&focalCalls==4);o->object=0;t->object=0;s.Update();assert(!s.owner.link&&s.target.link==t&&focalCalls==4);s.Update();assert(s.target.link==t);s.target.GetPItem();assert(!s.target.link);puts("00853ba0_TEST PASS");return 0;}