// global::SetVectorB(object, vector) -- Copy the supplied vector and dispatch to the accepting object.
// Leaf: ?SetVectorB@@YGXPAUCameraConstants@Row00c41c40@@PBUVec@2@@Z
// Identity: own wrapper00c34010 calls setter00c41380; field0x164 and mask0x10000. Existing namespace/callee spelling from00c41c40 retained as descriptive binding only; no original camera identity claimed.
#ifndef _MSC_VER
#define __stdcall
#endif
// Layout: no active shared header. Own wrapper calls virtual slot1; real vptr0, vectors344/356/368 and dirty word472. Original libpk class/vector names unknown. Retain Row00c41c40::CameraConstants/Vec spellings only to preserve existing callee bindings; the camera label is unsupported.
namespace Row00c41c40{
struct Vec{float x,y,z;Vec(){}Vec(const Vec& v):x(v.x),y(v.y),z(v.z){} Vec& operator=(const Vec& v){x=v.x;y=v.y;z=v.z;return *this;}};
struct CameraConstants{virtual void UnusedSlot0();virtual bool Accepts();unsigned char Unmodeled04[340];Vec a,b,c;unsigned char Unmodeled17C[92];unsigned long dirty;void SetA(const Vec&);void SetB(const Vec&);void SetC(const Vec&);};
}
void __stdcall SetVectorB(Row00c41c40::CameraConstants* object,const Row00c41c40::Vec* vector){Row00c41c40::CameraConstants* accepting=object->Accepts()?object:0;Row00c41c40::Vec value=*vector;accepting->SetB(value);}