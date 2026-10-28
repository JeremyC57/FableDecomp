// Row00c41c40::CameraConstants::SetC(const Vec& value) -- Update a vector and mark its dirty bit when any component changes.
// Leaf: ?SetC@CameraConstants@Row00c41c40@@QAEXABUVec@2@@Z
// Identity: own wrapper00c34140 calls setter00c413e0; field0x170 and mask0x80000. Existing namespace/callee spelling from00c41c40 retained as descriptive binding only; no original camera identity claimed.
#ifndef _MSC_VER
#define __stdcall
#endif
// Layout: no active shared header. Own wrapper calls virtual slot1; real vptr0, vectors344/356/368 and dirty word472. Original libpk class/vector names unknown. Retain Row00c41c40::CameraConstants/Vec spellings only to preserve existing callee bindings; the camera label is unsupported.
namespace Row00c41c40{
struct Vec{float x,y,z;Vec(){}Vec(const Vec& v):x(v.x),y(v.y),z(v.z){} Vec& operator=(const Vec& v){x=v.x;y=v.y;z=v.z;return *this;}};
struct CameraConstants{virtual void UnusedSlot0();virtual bool Accepts();unsigned char Unmodeled04[340];Vec a,b,c;unsigned char Unmodeled17C[92];unsigned long dirty;void SetA(const Vec&);void SetB(const Vec&);void SetC(const Vec&);};
}
void Row00c41c40::CameraConstants::SetC(const Vec& v){if(c.x!=v.x||c.y!=v.y||c.z!=v.z){c=v;dirty|=0x80000;}}