#include <stdio.h>
// global::SetVectorC(object, vector) -- Copy the supplied vector and dispatch to the accepting object.
// Leaf: ?SetVectorC@@YGXPAUCameraConstants@Row00c41c40@@PBUVec@2@@Z
// Identity: own wrapper00c34140 calls setter00c413e0; field0x170 and mask0x80000. Existing namespace/callee spelling from00c41c40 retained as descriptive binding only; no original camera identity claimed.
#ifndef _MSC_VER
#define __stdcall
#endif
// Layout: no active shared header. Own wrapper calls virtual slot1; real vptr0, vectors344/356/368 and dirty word472. Original libpk class/vector names unknown. Retain Row00c41c40::CameraConstants/Vec spellings only to preserve existing callee bindings; the camera label is unsupported.
namespace Row00c41c40{
struct Vec{float x,y,z;Vec(){}Vec(const Vec& v):x(v.x),y(v.y),z(v.z){} Vec& operator=(const Vec& v){x=v.x;y=v.y;z=v.z;return *this;}};
struct CameraConstants{virtual void UnusedSlot0();virtual bool Accepts();unsigned char Unmodeled04[340];Vec a,b,c;unsigned char Unmodeled17C[92];unsigned long dirty;void SetA(const Vec&);void SetB(const Vec&);void SetC(const Vec&);};
}
void __stdcall SetVectorC(Row00c41c40::CameraConstants* object,const Row00c41c40::Vec* vector){Row00c41c40::CameraConstants* accepting=object->Accepts()?object:0;Row00c41c40::Vec value=*vector;accepting->SetC(value);}
static int accepts,calls;static const Row00c41c40::CameraConstants* expected;static Row00c41c40::Vec captured;void Row00c41c40::CameraConstants::UnusedSlot0(){}bool Row00c41c40::CameraConstants::Accepts(){++accepts;return true;}void Row00c41c40::CameraConstants::SetC(const Vec& v){if(this==expected)++calls;captured=v;}
int main(){Row00c41c40::CameraConstants object;expected=&object;Row00c41c40::Vec v;v.x=1.5f;v.y=-2.0f;v.z=0.25f;SetVectorC(&object,&v);bool ok=accepts==1&&calls==1&&captured.x==1.5f&&captured.y==-2.0f&&captured.z==0.25f;v.x=0;v.y=16;v.z=-4;SetVectorC(&object,&v);ok=ok&&accepts==2&&calls==2&&captured.x==0&&captured.y==16&&captured.z==-4;puts(ok?"00c34140_TEST PASS":"00c34140_TEST FAIL");return ok?0:1;}