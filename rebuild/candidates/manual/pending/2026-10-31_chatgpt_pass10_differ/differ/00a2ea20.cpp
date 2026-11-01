// 00a2ea20 C3DMeshStats::HasHelperPoint
// Leaf: ?HasHelperPoint@C3DMeshStats@@QBE_NK@Z
// Flags:
#define CMovableResource CMovableResourceLayout
#include "engine/CMovableResource.h"
#undef CMovableResource
// Layout: supplied base flattens its vptr; original PDB has these four virtual slots.
class CMovableResource {public:virtual ~CMovableResource();virtual void RelocateData(long,void*,void*,long)=0;virtual bool CanRelocateData(long)const;virtual unsigned long Verify()const;};
namespace std {template<class A,class B>struct pair {A first;B second;};}
class C3DVector {public:float X,Y,Z;};
class C3DHelperPoint {private:C3DVector Position;long BoneIndex;};
template<class K,class V>class CKeyPairCompareLess {};
// Layout: raw original TPI Compare0, Elements4, Size8, Dirty12; native stride20.
template<class K,class V,class C>class CPreAllocVectorMap {
private:C Compare;std::pair<K,V>*Elements;unsigned long Size;bool Dirty;
public:
 const std::pair<K,V>*LowerBound(const K&)const;
 const std::pair<K,V>*end()const{return Elements+Size;}
 const std::pair<K,V>*Find(const K&key)const{
  const std::pair<K,V>*found=LowerBound(key);
  if(found==end()||key<found->first)return end();
  return found;
 }
};
class CMovableResourceMemoryPool;class CCharStringData;
class CCharString {protected:CCharStringData*PStringData;};
class C3DBoundingSphere {public:C3DVector Center;float Radius;};
class C3DBoundingBox {public:C3DVector MinCoord,MaxCoord;};
// Layout: original CMovableResource base and fields; HelperPoints at0x38, remaining members unaccessed.
class C3DMeshStats:public CMovableResource {
protected:CMovableResourceMemoryPool*MemoryPool;long StatsMemoryHandle;
private:CCharString Name;C3DBoundingSphere BoundingSphere;C3DBoundingBox BoundingBox;CPreAllocVectorMap<unsigned long,C3DHelperPoint,CKeyPairCompareLess<unsigned long,C3DHelperPoint> >HelperPoints;
public:virtual~C3DMeshStats();bool HasHelperPoint(unsigned long)const;
};
bool C3DMeshStats::HasHelperPoint(unsigned long id)const{return HelperPoints.Find(id)!=HelperPoints.end()?true:false;}
