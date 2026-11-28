// Leaf: ?push_back@?$CArray@VCOptimisedSubMesh@QuadricOptimiserInternals@@@@QAEXABVCOptimisedSubMesh@QuadricOptimiserInternals@@@Z
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
template void CArray<COptimisedSubMesh>::push_back(const COptimisedSubMesh&);

#include <cassert>
#include <cstdio>
using namespace QuadricOptimiserInternals;
namespace std {template<> vector<COptimisedSubMesh>::~vector(){_Destroy(this->_M_start,this->_M_finish);}}
CCharString::CCharString():PStringData(0){}
CCharString::CCharString(const CCharString& a):PStringData(a.PStringData){}
CCharString::~CCharString(){}
int __cdecl main(){CCharString name;CArray<COptimisedSubMesh> v;for(int i=0;i<80;++i){COptimisedSubMesh s;for(int j=0;j<i%7;++j)s.Bones.push_back(COptimisedBone(name,i*7+j));v.push_back(s);assert(v.size()==(unsigned)(i+1));for(int a=0;a<=i;++a){assert(v[a].Primitives.empty()&&v[a].Bones.size()==unsigned(a%7));for(int j=0;j<a%7;++j)assert(v[a].Bones[j].Parent==a*7+j);}}std::puts("00ad8720_TEST PASS");return 0;}