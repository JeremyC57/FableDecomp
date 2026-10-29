#include <stdio.h>
#ifndef _MSC_VER
#define __fastcall
#endif
// std::__pop_heap_aux<CMeshInfo*,CMeshInfo,CSortPrimaryMeshesFirst> -- original STLport pop_heap front end
// Leaf: ??$__pop_heap_aux@PAVCMeshInfo@NGraphicAppearance@@V12@VCSortPrimaryMeshesFirst@@@std@@YIXPAVCMeshInfo@NGraphicAppearance@@00VCSortPrimaryMeshesFirst@@@Z
// Flags:
// Retail: 004c3bf0,103 bytes. True mesh-info/primary-sort identities from original PDB donor family and actual helper byte8 comparator.
#include <stddef.h>
namespace NGraphicAppearance {
class CMeshInfo {public:long Index;float SizeX;unsigned char PrimaryMesh:1;CMeshInfo(long,float,bool);};
}
using NGraphicAppearance::CMeshInfo;
class CSortPrimaryMeshesFirst {public:bool operator()(const CMeshInfo& a,const CMeshInfo&){return a.PrimaryMesh!=0;}};
class CSortPrimaryMeshesLast {public:bool operator()(const CMeshInfo& a,const CMeshInfo&){return a.PrimaryMesh==0;}};
namespace std {
template<class Iterator,class Distance,class T,class Compare>
void __fastcall __adjust_heap(Iterator,Distance,Distance,T,Compare); // actual004c2c70, exact existing std binding
// Original4.6.2 _heap.h comparator overload. The actual VC7.1 pointer difference type is int.
template<class Iterator,class T,class Compare,class Distance>
inline void __fastcall __pop_heap(Iterator first,Iterator last,Iterator result,T value,Compare comp,Distance*)
{
 *result=*first;
 __adjust_heap(first,Distance(0),Distance(last-first),value,comp);
}
template<class Iterator,class T,class Compare>
inline void __fastcall __pop_heap_aux(Iterator first,Iterator last,T*,Compare comp)
{
 __pop_heap(first,last-1,last-1,T(*(last-1)),comp,(ptrdiff_t*)0);
}
template void __fastcall __pop_heap_aux<CMeshInfo*,CMeshInfo,CSortPrimaryMeshesFirst>(CMeshInfo*,CMeshInfo*,CMeshInfo*,CSortPrimaryMeshesFirst);
}

static int calls;static bool bad;static CMeshInfo* expected;
CMeshInfo::CMeshInfo(long i,float x,bool p):Index(i),SizeX(x),PrimaryMesh(p){}
namespace std {
template<class Iterator,class Distance,class T,class Compare>
void __fastcall __adjust_heap(Iterator first,Distance hole,Distance len,T value,Compare){++calls;if(first!=expected||hole!=0||len!=2||value.Index!=30||value.SizeX!=31.5f||!value.PrimaryMesh)bad=true;}
}
int main(){CMeshInfo v[3]={CMeshInfo(10,11.5f,false),CMeshInfo(20,21.5f,false),CMeshInfo(30,31.5f,true)};
 CSortPrimaryMeshesFirst first;CSortPrimaryMeshesLast last;
 if(first(v[0],v[2])||!first(v[2],v[0])||!last(v[0],v[2])||last(v[2],v[0]))return 1;
 expected=v;std::__pop_heap_aux<CMeshInfo*,CMeshInfo,CSortPrimaryMeshesFirst>(v,v+3,(CMeshInfo*)0,CSortPrimaryMeshesFirst());
 if(sizeof(CMeshInfo)!=12||calls!=1||bad||v[2].Index!=10||v[2].SizeX!=11.5f||v[2].PrimaryMesh)return 2;
 puts("004c3bf0_TEST PASS");return 0;}