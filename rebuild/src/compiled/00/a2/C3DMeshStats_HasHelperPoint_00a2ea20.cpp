// C3DMeshStats::HasHelperPoint(unsigned long id) -- true when the mesh's HelperPoints vector map holds the helper point id
// Leaf: ?HasHelperPoint@C3DMeshStats@@QBE_NK@Z
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe @ 0x00A2EA20, 63 bytes (`ret 4`). Callee: CPreAllocVectorMap<unsigned long,C3DHelperPoint,
//   CKeyPairCompareLess<..>>::LowerBound 0x00A316B0 (PDB spelling, bound once in reloc_bindings.tsv).
// Name: Ego_r/FableWin ?HasHelperPoint@C3DMeshStats@@QBE_NK@Z (public const).
// Shape: the inline Find (`found == end() || key < found->first` -> end()) and a second end() for the test. Written
//   `if (Find(id) != end()) return true; return false;` (codex50 lesson "HasTC as if(bit) return true; return false"):
//   the ternary/plain-bool forms reorder the second end()'s Size/Elements loads (63v63, 6 lines) or grow (67v63).
// Layout: ChatGPT pass 10's model (PDB TPI: CPreAllocVectorMap Compare +0, Elements +4, Size +8, Dirty +0xC; 20-byte
//   pair<unsigned long,C3DHelperPoint>; HelperPoints at +0x38 of C3DMeshStats).
#define CMovableResource CMovableResourceLayout
#include "engine/CMovableResource.h"
#undef CMovableResource
// Layout: supplied base flattens its vptr; original PDB has these four virtual slots.
class CMovableResource {public:virtual ~CMovableResource();virtual void RelocateData(long,void*,void*,long)=0;virtual bool CanRelocateData(long)const;virtual unsigned long Verify()const;};
namespace std {template<class A,class B>struct pair {A first;B second;};}
#include "engine/C3DVector.h"
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
bool C3DMeshStats::HasHelperPoint(unsigned long id)const{if(HelperPoints.Find(id)!=HelperPoints.end())return true;return false;}