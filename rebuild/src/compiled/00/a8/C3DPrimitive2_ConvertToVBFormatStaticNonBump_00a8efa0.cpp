// C3DPrimitive2::ConvertToVBFormatStaticNonBump(CTVertexNormTex1* vertices) -- copy every device-independent C3DVertex2 (position, normal, U, V) into the static non-bump vertex-buffer format
// Leaf: ?ConvertToVBFormatStaticNonBump@C3DPrimitive2@@AAEXPAVCTVertexNormTex1@@@Z
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A8EFA0, 520 bytes, thiscall, `ret 4`.
// Identity (proven): the one caller is 0x00A9282B inside the landed C3DPrimitive2::ConvertToVBFormatStatic 0x00A92740
//   (`flags & 2` picks the bump sibling 0x00A8F1B0, else this one); Ego_r.pdb C3DPrimitive2 declares the private pair
//   ConvertToVBFormatStaticNonBump(CTVertexNormTex1*) / ConvertToVBFormatStaticBump(CTVertexNormTex1Bump*), and both the
//   source (C3DVertex2: Position, Normal, U, V = 32 bytes; CDeviceIndependent::Vertices at +4 behind the scoped_ptr Data
//   at +0x2C) and the destination (CTVertexNormTex1: X Y Z NormalX NormalY NormalZ U1 V1 = 32 bytes, the `sar ebx,5` count and
//   the 0x20 strides) are the PDB layouts. The manifest "TransformVertexBuffer_LoopUnrolled_x4" is descriptive (the x4 is
//   VC7.1's own unroll of a plain loop).
// Shape: the landed ConvertToVBFormatRepeatingMesh 0x00A8F770 model (same TU): GetVertexCount once, PeekVertex per element,
//   the by-value `const C3DVector` getters built through the memberwise copy ctor (X, Y forwarded on the x87 stack, Z through a
//   register with its dead frame store) into the PDB CGenericVertex SetPosition / SetNormal / SetU1 / SetV1 inlines; VC7.1
//   unrolls the loop four times and finishes the remainder, re-reading Data->Vertices through `this` at every element.
#include "engine/CMovableResource.h"
#include "engine/CBaseClassNonCopyable.h"
#include "engine/C3DVector.h"
class C3DVertex2{public:C3DVector Position,Normal;float U,V;const C3DVector GetPosition()const{return Position;}const C3DVector GetNormal()const{return Normal;}float GetU()const{return U;}float GetV()const{return V;}};
class CTVertexNormTex1Base{public:float X,Y,Z,NormalX,NormalY,NormalZ,U1,V1;};     // Ego_r.pdb, 32 bytes
template<class T>class CGenericVertex:public T{public:
 void SetPosition(const C3DVector&p){this->X=p.X;this->Y=p.Y;this->Z=p.Z;}
 void SetNormal(const C3DVector&p){this->NormalX=p.X;this->NormalY=p.Y;this->NormalZ=p.Z;}
 void SetU1(float u){this->U1=u;}void SetV1(float v){this->V1=v;}
};
class CTVertexNormTex1:public CGenericVertex<CTVertexNormTex1Base>{};
#include "engine/CArray.h"
namespace boost{template<class T>class scoped_ptr{T*ptr;public:T*operator->()const{return ptr;}};}
namespace N3DPrimitive{class CDeviceIndependent:public CBaseClassNonCopyable{private:CArray<C3DVertex2>Vertices;public:unsigned long GetVertexCount()const{return Vertices.size();}const C3DVertex2&PeekVertex(unsigned long i)const{return Vertices[i];}};}
class C3DPrimitive2:public CMovableResource{long MaterialIndex,RepeatingMeshReps;unsigned char _pad_0x0c[0x20];boost::scoped_ptr<N3DPrimitive::CDeviceIndependent>Data;void ConvertToVBFormatStaticNonBump(CTVertexNormTex1*);};
void C3DPrimitive2::ConvertToVBFormatStaticNonBump(CTVertexNormTex1*out){
 unsigned long count=Data->GetVertexCount();
 for(unsigned long i=0;i<count;++i){
  const C3DVertex2&src=Data->PeekVertex(i);
  CTVertexNormTex1&vertex=out[i];
  vertex.SetPosition(src.GetPosition());vertex.SetNormal(src.GetNormal());vertex.SetU1(src.GetU());vertex.SetV1(src.GetV());
 }
}