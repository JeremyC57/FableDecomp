// Leaf: ?DeleteData@?$CCountedPointer@VCOptimisedPrimitive@QuadricOptimiserInternals@@@@SIXPAX@Z
// Flags: /Gr
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_CARRAY_ORIGINAL_USER_SPECIAL_MEMBERS_PASS22
#define FABLE_RETAIL_CARRAY_ORIGINAL_INDEX_BASE_PASS22
#define FABLE_RETAIL_CARRAY_ORIGINAL_COMPLETE_DECLARATIONS_PASS22
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_C3DMESHFILESUBMESHCHUNK
#define FABLE_RETAIL_C3DMESHFILEPRIMITIVECHUNK
#define FABLE_RETAIL_C3DMESHFILEBONECHUNK_GETBONE
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_QUADRIC_CUNIQUEVERTEX
#define FABLE_RETAIL_QUADRIC_COPTIMISEDVERTEX_GROUPS
#define FABLE_RETAIL_QUADRIC_COPTIMISEDFACE
#define FABLE_RETAIL_QUADRIC_COPTIMISEDEDGE
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE
#define FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE_CTOR
#define FABLE_RETAIL_COPTIMISEDSUBMESH_PASS22
#define FABLE_RETAIL_QUADRIC_MESH_CTOR_PASS30
#include "engine/CArray.h"
#include "engine/C3DMeshFileRootChunk.h"
#include "engine/C3DMeshFile.h"
#include "engine/C3DMeshFileSubMeshChunk.h"
#include "engine/C3DMeshFilePrimitiveChunk.h"
#include "engine/C3DMeshFileBoneChunk.h"
#include "engine/QuadricOptimiserTypes.h"
#include "engine/COptimisedPrimitive.h"
#include "engine/COptimisedMesh.h"


template void CCountedPointer<QuadricOptimiserInternals::COptimisedPrimitive>::DeleteData(void*);

namespace QuadricOptimiserInternals {
COptimisedMesh::COptimisedMesh(C3DMeshFile& mesh)
{
    C3DMeshFileRootChunk& root = *mesh.GetRoot();
    CCountedPointer<C3DMeshFileSubMeshChunk> src_submesh = root.FindFirstSubMesh();
    while (src_submesh.Get()) {
        COptimisedSubMesh opt_submesh;
        CCountedPointer<C3DMeshFilePrimitiveChunk> src_prim = src_submesh->FindFirstPrimitive();
        while (src_prim.Get()) {
            CCountedPointer<COptimisedPrimitive> opt_prim = new COptimisedPrimitive(*src_prim);
            opt_submesh.Primitives.push_back(opt_prim);
            src_prim = src_submesh->FindNextPrimitive(src_prim);
        }
        CCountedPointer<C3DMeshFileBoneChunk> src_bone = src_submesh->FindFirstBone();
        while (src_bone.Get()) {
            opt_submesh.Bones.push_back(COptimisedBone(src_bone->GetName(),src_bone->GetParentIndex()));
            src_bone = src_submesh->FindNextBone(src_bone);
        }
        SubMeshes.push_back(opt_submesh);
        src_submesh = root.FindNextSubMesh(src_submesh);
    }
}
}
#include <cassert>
#include <cstdio>
#include <cstring>

using namespace QuadricOptimiserInternals;
#ifndef FABLE_MESH_EXTERNAL_VECTOR_DTOR
namespace std {
template<> vector<QuadricOptimiserInternals::COptimisedSubMesh>::~vector(){_Destroy(this->_M_start,this->_M_finish);}
}
#endif
namespace {
const int MaxSubs=5, MaxChildren=7;
struct RawChunk { double words[96]; };
RawChunk fileStorage,rootStorage,subStorage[MaxSubs],primStorage[MaxSubs][MaxChildren],boneStorage[MaxSubs][MaxChildren];
unsigned long infoStorage[2+MaxSubs*(1+2*MaxChildren)][3];
int tokens[MaxSubs][MaxChildren],stringRefs[MaxSubs][MaxChildren],parentIndex[MaxSubs][MaxChildren];
int subs,prims[MaxSubs],bones[MaxSubs],created,destroyed,rootCalls,subNext,primNext,boneNext,firstPrims,firstBones,validations;
const COptimisedPrimitive* built[MaxSubs][MaxChildren];
int releasedSources;bool ephemeral;
void FABLE_FASTCALL releaseSource(void* p){assert(p);++releasedSources;}
void FABLE_FASTCALL lostBorrow(void*){assert(false);}
template<class T>CCountedPointer<T> provide(T* object, unsigned id){
 CCountedPointer<T> result;
 if(object && ephemeral && id){result.Data=object;result.Info=new CCPPointerInfo(1,releaseSource,object);}
 else if(object){result.Data=object;result.Info=(CCPPointerInfo*)infoStorage[id];assert(result.Info->RefCount>=1);++result.Info->RefCount;}
 return result;
}
int subNumber(const C3DMeshFileSubMeshChunk* p){for(int i=0;i<subs;++i)if(p==(C3DMeshFileSubMeshChunk*)&subStorage[i])return i;assert(false);return 0;}
int sourceSlot(int s,bool bone,int j){return 2+s*(1+2*MaxChildren)+1+(bone?MaxChildren:0)+j;}
unsigned tokenIndex(const void* p){return (unsigned)(((const char*)p-(const char*)tokens)/sizeof(int));}
}
CBaseClass::CBaseClass(){}
CBaseClass::~CBaseClass(){}
CBaseClassNonCopyable::CBaseClassNonCopyable(){}
CBaseClassNonCopyable::~CBaseClassNonCopyable(){}
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const CCharString& other):PStringData(other.PStringData){
 if(PStringData){unsigned z=tokenIndex(PStringData);assert(z<MaxSubs*MaxChildren);++stringRefs[z/MaxChildren][z%MaxChildren];}
}
CCharString::~CCharString(){
 if(PStringData){unsigned z=tokenIndex(PStringData);assert(z<MaxSubs*MaxChildren);assert(stringRefs[z/MaxChildren][z%MaxChildren]>0);--stringRefs[z/MaxChildren][z%MaxChildren];}
}
CCountedPointer<C3DMeshFileRootChunk> C3DMeshFile::GetRoot(){assert(this==(C3DMeshFile*)&fileStorage);++rootCalls;return provide((C3DMeshFileRootChunk*)&rootStorage,0);}
CCountedPointer<C3DMeshFileSubMeshChunk> C3DMeshFileRootChunk::FindFirstSubMesh(){assert(this==(C3DMeshFileRootChunk*)&rootStorage);return provide(subs?(C3DMeshFileSubMeshChunk*)&subStorage[0]:0,2);}
CCountedPointer<C3DMeshFileSubMeshChunk> C3DMeshFileRootChunk::FindNextSubMesh(const CCountedPointer<const C3DMeshFileSubMeshChunk>& old){assert(this==(C3DMeshFileRootChunk*)&rootStorage);int s=subNumber(old.Get());assert(old.Info->RefCount>=2);++subNext;++s;return provide(s<subs?(C3DMeshFileSubMeshChunk*)&subStorage[s]:0,2+s*(1+2*MaxChildren));}
CCountedPointer<C3DMeshFilePrimitiveChunk> C3DMeshFileSubMeshChunk::FindFirstPrimitive(){int s=subNumber(this);++firstPrims;return provide(prims[s]?(C3DMeshFilePrimitiveChunk*)&primStorage[s][0]:0,sourceSlot(s,false,0));}
CCountedPointer<C3DMeshFilePrimitiveChunk> C3DMeshFileSubMeshChunk::FindNextPrimitive(const CCountedPointer<const C3DMeshFilePrimitiveChunk>& old){int s=subNumber(this),j=0;while(j<prims[s]&&old.Get()!=(C3DMeshFilePrimitiveChunk*)&primStorage[s][j])++j;assert(j<prims[s]&&old.Info->RefCount>=2);++primNext;++j;return provide(j<prims[s]?(C3DMeshFilePrimitiveChunk*)&primStorage[s][j]:0,sourceSlot(s,false,j));}
CCountedPointer<C3DMeshFileBoneChunk> C3DMeshFileSubMeshChunk::FindFirstBone(){int s=subNumber(this);++firstBones;return provide(bones[s]?(C3DMeshFileBoneChunk*)&boneStorage[s][0]:0,sourceSlot(s,true,0));}
CCountedPointer<C3DMeshFileBoneChunk> C3DMeshFileSubMeshChunk::FindNextBone(const CCountedPointer<const C3DMeshFileBoneChunk>& old){int s=subNumber(this),j=0;while(j<bones[s]&&old.Get()!=(C3DMeshFileBoneChunk*)&boneStorage[s][j])++j;assert(j<bones[s]&&old.Info->RefCount>=2);++boneNext;++j;return provide(j<bones[s]?(C3DMeshFileBoneChunk*)&boneStorage[s][j]:0,sourceSlot(s,true,j));}
COptimisedPrimitive::COptimisedPrimitive(C3DMeshFilePrimitiveChunk& input){
 for(int s=0;s<subs;++s)for(int j=0;j<prims[s];++j)if(&input==(C3DMeshFilePrimitiveChunk*)&primStorage[s][j]){assert(!built[s][j]);built[s][j]=this;++created;return;}assert(false);
}
COptimisedPrimitive::~COptimisedPrimitive(){++destroyed;}
CUniqueVertex::~CUniqueVertex(){}
COptimisedVertex::~COptimisedVertex(){}
void COptimisedMesh::RemoveBones(const std::set<CCharString>&){}
void COptimisedMesh::CopyTo(C3DMeshFile&){
 ++validations;assert(SubMeshes.size()==(unsigned)subs);
 for(int s=0;s<subs;++s){const COptimisedSubMesh& m=SubMeshes[s];assert(m.Primitives.size()==(unsigned)prims[s]&&m.Bones.size()==(unsigned)bones[s]);
  for(int j=0;j<prims[s];++j){assert(m.Primitives[j].Get()==built[s][j]);assert(m.Primitives[j].Info->RefCount==1);}
  for(int j=0;j<bones[s];++j){const COptimisedBone& b=m.Bones[j];const void* text;std::memcpy(&text,&b.BoneName,sizeof(text));assert(text==&tokens[s][j]&&b.Parent==parentIndex[s][j]&&!b.Remapped&&b.NewIndex==-1);assert(stringRefs[s][j]==1);}
 }
}
int __cdecl main(){
 CCountedPointer<COptimisedPrimitive>::DeleteData(0);assert(destroyed==0);
 for(int scenario=0;scenario<512;++scenario){std::memset(built,0,sizeof(built));std::memset(stringRefs,0,sizeof(stringRefs));
  for(unsigned k=0;k<sizeof(infoStorage)/sizeof(infoStorage[0]);++k)new(infoStorage[k])CCPPointerInfo(1,lostBorrow,0);
  releasedSources=0;ephemeral=(scenario&8)!=0;subs=scenario%(MaxSubs+1);created=destroyed=rootCalls=subNext=primNext=boneNext=firstPrims=firstBones=validations=0;int np=0,nb=0;
  for(int s=0;s<subs;++s){prims[s]=(scenario/(s+1)+s*5)%(MaxChildren+1);bones[s]=(scenario/(s+2)+s*3)%(MaxChildren+1);np+=prims[s];nb+=bones[s];
   for(int j=0;j<bones[s];++j){parentIndex[s][j]=(scenario+j+s)%11-5;char* p=(char*)&boneStorage[s][j];void* name=&tokens[s][j];std::memcpy(p+0x6C,&name,sizeof(name));std::memcpy(p+0x30,&parentIndex[s][j],4);}
  }
  C3DMeshFile& file=*(C3DMeshFile*)&fileStorage;
  {COptimisedMesh mesh(file);mesh.CopyTo(file);assert(validations==1&&rootCalls==1&&subNext==subs&&firstBones==subs&&firstPrims==subs);assert(primNext==np&&created==np&&destroyed==0&&boneNext==nb);}
  assert(destroyed==created);assert(releasedSources==(ephemeral?subs+np+nb:0));
  for(int s=0;s<subs;++s)for(int j=0;j<bones[s];++j)assert(!stringRefs[s][j]);
  for(unsigned k=0;k<sizeof(infoStorage)/sizeof(infoStorage[0]);++k)assert(((CCPPointerInfo*)infoStorage[k])->RefCount==1);
 }std::puts("00ad8810_TEST PASS");return 0;
}
