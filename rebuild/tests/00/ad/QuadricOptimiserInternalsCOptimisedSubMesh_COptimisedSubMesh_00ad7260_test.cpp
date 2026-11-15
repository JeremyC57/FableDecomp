// Exact empty destructor body; observable string and counted-ownership boundaries.
#include <vector>
#include <assert.h>
#include <stdio.h>
static bool Observe;
static int BoneDtors,PointerDtors,ExpectedBones;
struct State {int count,deletions;};
class CCharString {public:~CCharString(){if(Observe){assert(PointerDtors==0);++BoneDtors;}}};
template<class T>class CArray:public std::vector<T>{};
template<class T>class CCountedPointer {public:State* Info;CCountedPointer():Info(0){}CCountedPointer(const CCountedPointer& x):Info(x.Info){if(Info)++Info->count;}
 ~CCountedPointer(){if(Observe){assert(BoneDtors==ExpectedBones);++PointerDtors;}if(Info){--Info->count;if(!Info->count)++Info->deletions;}}
};
namespace QuadricOptimiserInternals {
class COptimisedPrimitive;
class COptimisedBone {public:CCharString BoneName;long Parent;bool Remapped;long NewIndex;};
class COptimisedSubMesh {public:CArray<CCountedPointer<COptimisedPrimitive> > Primitives;CArray<COptimisedBone> Bones;~COptimisedSubMesh();};
}
QuadricOptimiserInternals::COptimisedSubMesh::~COptimisedSubMesh() {}
int main(){for(int bones=0;bones<=4;++bones)for(int primitives=0;primitives<=4;++primitives){
 State state[2];for(int i=0;i<2;++i){state[i].count=0;state[i].deletions=0;}
 QuadricOptimiserInternals::COptimisedSubMesh* mesh=new QuadricOptimiserInternals::COptimisedSubMesh;
 mesh->Bones.resize(bones);mesh->Primitives.resize(primitives);
 for(int i=0;i<primitives;++i)if(i%3!=0){mesh->Primitives[i].Info=&state[i%2];++state[i%2].count;}
 int used[2]={state[0].count!=0,state[1].count!=0};ExpectedBones=bones;BoneDtors=PointerDtors=0;Observe=true;
 delete mesh;Observe=false;
 assert(BoneDtors==bones && PointerDtors==primitives);for(int i=0;i<2;++i){assert(state[i].count==0);assert(state[i].deletions==used[i]);}
 }puts("00ad7260_TEST PASS");return 0;}