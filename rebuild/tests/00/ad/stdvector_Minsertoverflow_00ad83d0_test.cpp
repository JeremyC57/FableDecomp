// Leaf: ?_M_insert_overflow@?$vector@VCOptimisedSubMesh@QuadricOptimiserInternals@@V?$allocator@VCOptimisedSubMesh@QuadricOptimiserInternals@@@std@@@std@@IAEXPAVCOptimisedSubMesh@QuadricOptimiserInternals@@ABV34@ABU__false_type@2@I_N@Z
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


using QuadricOptimiserInternals::COptimisedSubMesh;
template void std::vector<COptimisedSubMesh>::_M_insert_overflow(COptimisedSubMesh*,const COptimisedSubMesh&,const std::__false_type&,unsigned int,bool);

#include <cassert>
#include <cstdio>
using namespace QuadricOptimiserInternals;
namespace std {template<> vector<COptimisedSubMesh>::~vector(){_Destroy(this->_M_start,this->_M_finish);}}
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const CCharString& a):PStringData(a.PStringData){}
CCharString::~CCharString(){}
struct TestedVector : std::vector<COptimisedSubMesh> {
 void fill(unsigned position,unsigned count,const COptimisedSubMesh& value){_M_insert_overflow(begin()+position,value,std::__false_type(),count,position==size());}
};
int __cdecl main(){CCharString name;for(unsigned n=0;n<6;++n)for(unsigned pos=0;pos<=n;++pos)for(unsigned count=1;count<4;++count){TestedVector v;for(unsigned i=0;i<n;++i){COptimisedSubMesh s;s.Bones.push_back(COptimisedBone(name,i));v.push_back(s);}COptimisedSubMesh q;q.Bones.push_back(COptimisedBone(name,-8));q.Bones.push_back(COptimisedBone(name,-7));v.fill(pos,count,q);assert(v.size()==n+count);for(unsigned j=0;j<n+count;++j){bool ins=j>=pos&&j<pos+count;assert(v[j].Bones.size()==(ins?2U:1U));assert(v[j].Bones[0].Parent==(ins?-8:int(j<pos?j:j-count)));if(ins)assert(v[j].Bones[1].Parent==-7);assert(v[j].Primitives.empty());}}std::puts("00ad83d0_TEST PASS");return 0;}