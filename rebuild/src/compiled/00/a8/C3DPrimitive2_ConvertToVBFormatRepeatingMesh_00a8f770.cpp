// 00a8f770 C3DPrimitive2::ConvertToVBFormatRepeatingMesh
// Leaf: ?ConvertToVBFormatRepeatingMesh@C3DPrimitive2@@AAEXPAVCTVertexMatrixIndexedNormTex1@@@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Flags:
#include "engine/CMovableResource.h"
#include "engine/CBaseClassNonCopyable.h"
class C3DVector{public:float X,Y,Z;C3DVector(const C3DVector&o):X(o.X),Y(o.Y),Z(o.Z){}};
class C3DVertex2{public:C3DVector Position,Normal;float U,V;const C3DVector GetPosition()const{return Position;}const C3DVector GetNormal()const{return Normal;}float GetU()const{return U;}float GetV()const{return V;}};
class CTVertexMatrixIndexedNormTex1Base{public:float X,Y,Z,NormalX,NormalY,NormalZ,U1,V1;short MatrixIndex,Pad;};
template<class T>class CGenericVertex:public T{public:
 void SetPosition(const C3DVector&p){this->X=p.X;this->Y=p.Y;this->Z=p.Z;}
 void SetNormal(const C3DVector&p){this->NormalX=p.X;this->NormalY=p.Y;this->NormalZ=p.Z;}
 void SetU1(float u){this->U1=u;}void SetV1(float v){this->V1=v;}
};
class CTVertexMatrixIndexedNormTex1:public CGenericVertex<CTVertexMatrixIndexedNormTex1Base>{};
#include "engine/CArray.h"
namespace boost{template<class T>class scoped_ptr{T*ptr;public:T*operator->()const{return ptr;}};}
namespace N3DPrimitive{class CDeviceIndependent:public CBaseClassNonCopyable{private:CArray<C3DVertex2>Vertices;public:unsigned long GetVertexCount()const{return Vertices.size();}const C3DVertex2&PeekVertex(unsigned long i)const{return Vertices[i];}};}
// Layout: original shared movable-resource base; original native RepeatingMeshReps8 and Data2c.
class C3DPrimitive2:public CMovableResource{long MaterialIndex,RepeatingMeshReps;unsigned char _pad_0x0c[0x20];boost::scoped_ptr<N3DPrimitive::CDeviceIndependent>Data;void ConvertToVBFormatRepeatingMesh(CTVertexMatrixIndexedNormTex1*);};
void C3DPrimitive2::ConvertToVBFormatRepeatingMesh(CTVertexMatrixIndexedNormTex1*out){
 long count=Data->GetVertexCount();
 for(long rep=0;rep<RepeatingMeshReps;++rep){
  for(unsigned long i=0;i<count;++i){
   const C3DVertex2&src=Data->PeekVertex(i);
   CTVertexMatrixIndexedNormTex1&vertex=out[rep*count+i];
   vertex.SetPosition(src.GetPosition());vertex.SetNormal(src.GetNormal());vertex.SetU1(src.GetU());vertex.SetV1(src.GetV());vertex.MatrixIndex=(short)rep;
  }
 }
}