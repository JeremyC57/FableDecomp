// N3DPrimitive::CAnimatedBlock::ConvertToVBFormatNonBump(CTVertexNormBlend4Tex1* out, CDeviceIndependent& mesh) -- fill the blended vertex buffer: for every block vertex copy position/normal/uv from the mesh vertex it indexes, then its bone indices (x3, palette slots) and weights, zero-padding to four
// Leaf: ?ConvertToVBFormatNonBump@CAnimatedBlock@N3DPrimitive@@AAEXPAVCTVertexNormBlend4Tex1@@AAVCDeviceIndependent@2@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A8FF50, length 231 bytes (+9 int3), `ret 8`. No callees.
// Layout: Ego_r.pdb CAnimatedBlock (BonesPerVertex +0x14, Data +0x2c CIVCountedPointer<CDeviceIndependent>: VertexList +0x08,
//   BoneWeights +0x14 (C3DIndexedBoneWeight {BoneIndex, Weight})); CTVertexNormBlend4Tex1Base (X Y Z, BlendingIndices +0x0c,
//   BlendingWeights +0x10, Normal +0x14, U1 V1 +0x20); the mesh's N3DPrimitive::CDeviceIndependent Vertices +0x04 (C3DVertex2).
// Shape: the landed same-TU C3DPrimitive2::ConvertToVBFormatStaticNonBump 0x00A8EFA0 model: PeekVertex per element, the
//   by-value `const C3DVector` getters built through the memberwise copy ctor (X, Y on the x87 stack, Z through a register with
//   its dead frame store) into the CGenericVertex SetPosition / SetNormal / SetU1 / SetV1 inlines; then the bone loop (index x3
//   as a byte, re-reading BonesPerVertex) and the zero padding to four from the same counter.
#include "engine/CBaseClassNonCopyable.h"
class C3DVector{public:float X,Y,Z;C3DVector(const C3DVector&o):X(o.X),Y(o.Y),Z(o.Z){}};
class C3DVertex2{public:C3DVector Position,Normal;float U,V;const C3DVector GetPosition()const{return Position;}const C3DVector GetNormal()const{return Normal;}float GetU()const{return U;}float GetV()const{return V;}};
class CTVertexNormBlend4Tex1Base{public:float X,Y,Z;unsigned char BlendingIndices[4];unsigned char BlendingWeights[4];float NormalX,NormalY,NormalZ,U1,V1;};     // Ego_r.pdb, 40 bytes
template<class T>class CGenericVertex:public T{public:
 void SetPosition(const C3DVector&p){this->X=p.X;this->Y=p.Y;this->Z=p.Z;}
 void SetNormal(const C3DVector&p){this->NormalX=p.X;this->NormalY=p.Y;this->NormalZ=p.Z;}
 void SetU1(float u){this->U1=u;}void SetV1(float v){this->V1=v;}
};
class CTVertexNormBlend4Tex1:public CGenericVertex<CTVertexNormBlend4Tex1Base>{};
class C3DIndexedBoneWeight{public:unsigned long BoneIndex;unsigned long Weight;};
namespace std{
template<class T>class allocator{};
template<class V,class T,class A>class _STLP_alloc_proxy:public A{public:V _M_data;};
template<class T,class A>class _Vector_base{protected:T*_M_start,*_M_finish;_STLP_alloc_proxy<T*,T,A>_M_end_of_storage;};
template<class T,class A=allocator<T> >class vector:public _Vector_base<T,A>{public:unsigned int size()const{return this->_M_finish-this->_M_start;}T&operator[](unsigned int i){return this->_M_start[i];}const T&operator[](unsigned int i)const{return this->_M_start[i];}};
}
template<class T>class CArray:public std::vector<T>{public:T&operator[](unsigned int i){return this->_M_start[i];}const T&operator[](unsigned int i)const{return this->_M_start[i];}};
template<class T>class CIVCountedPointer{T*Data;public:T*operator->()const{return Data;}};
namespace N3DPrimitive{
class CDeviceIndependent:public CBaseClassNonCopyable{private:CArray<C3DVertex2>Vertices;public:unsigned long GetVertexCount()const{return Vertices.size();}const C3DVertex2&PeekVertex(unsigned long i)const{return Vertices[i];}};
class CAnimatedBlock{
public:
 class CDeviceIndependent{public:long _vfptr_or_base;long IVRefCount;CArray<unsigned long>VertexList;CArray<C3DIndexedBoneWeight>BoneWeights;};
private:
 unsigned char _padding_[0xc];
 unsigned long BoneConstantsFixupOffset;
 unsigned long VertexCount;
 unsigned short BonesPerVertex;
 bool PalettedFlag;
 unsigned char GroupCount;
 unsigned char Groups[0x14];
 CIVCountedPointer<CDeviceIndependent> Data;
 void ConvertToVBFormatNonBump(CTVertexNormBlend4Tex1* out, N3DPrimitive::CDeviceIndependent& mesh);
};
void CAnimatedBlock::ConvertToVBFormatNonBump(CTVertexNormBlend4Tex1* out, N3DPrimitive::CDeviceIndependent& mesh)
{
 unsigned long count=Data->VertexList.size();
 for(unsigned long i=0;i<count;++i){
  const C3DVertex2&src=mesh.PeekVertex(Data->VertexList[i]);
  CTVertexNormBlend4Tex1&vertex=out[i];
  vertex.SetPosition(src.GetPosition());vertex.SetNormal(src.GetNormal());vertex.SetU1(src.GetU());vertex.SetV1(src.GetV());
  unsigned long j;
  for(j=0;j<BonesPerVertex;++j){
   const C3DIndexedBoneWeight&weight=Data->BoneWeights[i*BonesPerVertex+j];
   vertex.BlendingIndices[j]=(unsigned char)(weight.BoneIndex*3);
   vertex.BlendingWeights[j]=(unsigned char)weight.Weight;
  }
  for(;j<4;++j){
   vertex.BlendingIndices[j]=0;
   vertex.BlendingWeights[j]=0;
  }
 }
}
}