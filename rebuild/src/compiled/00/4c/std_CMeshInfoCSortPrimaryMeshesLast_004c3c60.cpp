// std::__pop_heap_aux<CMeshInfo*,CMeshInfo,CSortPrimaryMeshesLast> -- original STLport pop_heap front end
// Leaf: ??$__pop_heap_aux@PAVCMeshInfo@NGraphicAppearance@@V12@VCSortPrimaryMeshesLast@@@std@@YIXPAVCMeshInfo@NGraphicAppearance@@00VCSortPrimaryMeshesLast@@@Z
// Flags:
// Retail: 004c3c60,103 bytes. True mesh-info/primary-sort identities from original PDB donor family and actual helper byte8 comparator.
#include <stddef.h>
namespace NGraphicAppearance {
class CMeshInfo {public:long Index;float SizeX;unsigned char PrimaryMesh:1;CMeshInfo(long,float,bool);};
}
using NGraphicAppearance::CMeshInfo;
class CSortPrimaryMeshesFirst {public:bool operator()(const CMeshInfo& a,const CMeshInfo&){return a.PrimaryMesh!=0;}};
class CSortPrimaryMeshesLast {public:bool operator()(const CMeshInfo& a,const CMeshInfo&){return a.PrimaryMesh==0;}};
namespace std {
template<class Iterator,class Distance,class T,class Compare>
void __fastcall __adjust_heap(Iterator,Distance,Distance,T,Compare); // actual004c2d10, exact existing std binding
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
template void __fastcall __pop_heap_aux<CMeshInfo*,CMeshInfo,CSortPrimaryMeshesLast>(CMeshInfo*,CMeshInfo*,CMeshInfo*,CSortPrimaryMeshesLast);
}