#include <stdio.h>
// Row00c41c40::CameraConstants::SetA(const Vec& value) -- Update a vector and mark its dirty bit when any component changes.
// Leaf: ?SetA@CameraConstants@Row00c41c40@@QAEXABUVec@2@@Z
// Identity: own wrapper00c33fd0 calls setter00c41320; field0x158 and mask0x8000. Existing namespace/callee spelling from00c41c40 retained as descriptive binding only; no original camera identity claimed.
#ifndef _MSC_VER
#define __stdcall
#endif
// Layout: no active shared header. Own wrapper calls virtual slot1; real vptr0, vectors344/356/368 and dirty word472. Original libpk class/vector names unknown. Retain Row00c41c40::CameraConstants/Vec spellings only to preserve existing callee bindings; the camera label is unsupported.
namespace Row00c41c40{
struct Vec{float x,y,z;Vec(){}Vec(const Vec& v):x(v.x),y(v.y),z(v.z){} Vec& operator=(const Vec& v){x=v.x;y=v.y;z=v.z;return *this;}};
struct CameraConstants{virtual void UnusedSlot0();virtual bool Accepts();unsigned char Unmodeled04[340];Vec a,b,c;unsigned char Unmodeled17C[92];unsigned long dirty;void SetA(const Vec&);void SetB(const Vec&);void SetC(const Vec&);};
}
void Row00c41c40::CameraConstants::SetA(const Vec& v){if(a.x!=v.x||a.y!=v.y||a.z!=v.z){a=v;dirty|=0x8000;}}
void Row00c41c40::CameraConstants::UnusedSlot0(){}bool Row00c41c40::CameraConstants::Accepts(){return true;}
int main(){Row00c41c40::CameraConstants p;Row00c41c40::Vec v;v.x=1.5f;v.y=-2;v.z=0.25f;p.a=v;p.dirty=0x200;p.SetA(v);bool ok=p.dirty==0x200;for(int i=0;i<3;++i){if(i==0)v.x=4;else if(i==1)v.y=8;else v.z=16;p.dirty=0x200;p.SetA(v);ok=ok&&p.dirty==(0x200|0x8000)&&p.a.x==v.x&&p.a.y==v.y&&p.a.z==v.z;}p.dirty=0;p.SetA(p.a);ok=ok&&!p.dirty;puts(ok?"00c41320_TEST PASS":"00c41320_TEST FAIL");return ok?0:1;}