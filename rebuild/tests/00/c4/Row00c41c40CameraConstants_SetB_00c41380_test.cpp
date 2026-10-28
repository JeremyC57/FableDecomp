#include <stdio.h>
// Row00c41c40::CameraConstants::SetB(const Vec& value) -- Update a vector and mark its dirty bit when any component changes.
// Leaf: ?SetB@CameraConstants@Row00c41c40@@QAEXABUVec@2@@Z
// Identity: own wrapper00c34010 calls setter00c41380; field0x164 and mask0x10000. Existing namespace/callee spelling from00c41c40 retained as descriptive binding only; no original camera identity claimed.
#ifndef _MSC_VER
#define __stdcall
#endif
// Layout: no active shared header. Own wrapper calls virtual slot1; real vptr0, vectors344/356/368 and dirty word472. Original libpk class/vector names unknown. Retain Row00c41c40::CameraConstants/Vec spellings only to preserve existing callee bindings; the camera label is unsupported.
namespace Row00c41c40{
struct Vec{float x,y,z;Vec(){}Vec(const Vec& v):x(v.x),y(v.y),z(v.z){} Vec& operator=(const Vec& v){x=v.x;y=v.y;z=v.z;return *this;}};
struct CameraConstants{virtual void UnusedSlot0();virtual bool Accepts();unsigned char Unmodeled04[340];Vec a,b,c;unsigned char Unmodeled17C[92];unsigned long dirty;void SetA(const Vec&);void SetB(const Vec&);void SetC(const Vec&);};
}
void Row00c41c40::CameraConstants::SetB(const Vec& v){if(b.x!=v.x||b.y!=v.y||b.z!=v.z){b=v;dirty|=0x10000;}}
void Row00c41c40::CameraConstants::UnusedSlot0(){}bool Row00c41c40::CameraConstants::Accepts(){return true;}
int main(){Row00c41c40::CameraConstants p;Row00c41c40::Vec v;v.x=1.5f;v.y=-2;v.z=0.25f;p.b=v;p.dirty=0x200;p.SetB(v);bool ok=p.dirty==0x200;for(int i=0;i<3;++i){if(i==0)v.x=4;else if(i==1)v.y=8;else v.z=16;p.dirty=0x200;p.SetB(v);ok=ok&&p.dirty==(0x200|0x10000)&&p.b.x==v.x&&p.b.y==v.y&&p.b.z==v.z;}p.dirty=0;p.SetB(p.b);ok=ok&&!p.dirty;puts(ok?"00c41380_TEST PASS":"00c41380_TEST FAIL");return ok?0:1;}