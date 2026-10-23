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