// std::__copy_backward(first,last,result,tag,distance) -- Assign counted pointers backward with correct ownership.
// Leaf: ??$__copy_backward@PAV?$CCountedPointer@VCMeshDataBankEntry@@@@PAV1@H@std@@YIPAV?$CCountedPointer@VCMeshDataBankEntry@@@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Flags: /Gr
// Identity: Own fill-insert00a2d960 calls this; resize00a2dbc0 is called by CMeshDataBank::NotifyEntryAdded00a26d40 for DynamicInfoBuffer. Original CArray<CCountedPointer<CMeshDataBankEntry>>.
// Layout: original CCPPointerInfo RefCount0/DeleteFunc4/Data8; wrapper Data0/Info4. Actual callback consumes its pointer in ECX, so fastcall in this TU. Scalar operator delete at BFE9BC releases the control block.
#include <new>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
class CMeshDataBankEntry;
#include "engine/CCountedPointer.h"
namespace std{struct random_access_iterator_tag{random_access_iterator_tag(){}};template<class Input,class Output,class Distance>Output __fastcall __copy_backward(Input first,Input last,Output result,const random_access_iterator_tag&,Distance*){for(Distance count=last-first;count>0;--count)*--result=*--last;return result;}}
typedef CCountedPointer<CMeshDataBankEntry> Element;
template Element* __fastcall std::__copy_backward<Element*,Element*,int>(Element*,Element*,Element*,const std::random_access_iterator_tag&,int*);