// NGraphicAppearance::CTCGraphicAppearanceAnimatedMesh::GetMeshesUsed(std::vector<CMeshInfo>& out) const -- append the primary mesh of the owner's main graphic.
// Leaf: ?GetMeshesUsed@CTCGraphicAppearanceAnimatedMesh@NGraphicAppearance@@UBEXAAV?$vector@VCMeshInfo@NGraphicAppearance@@V?$allocator@VCMeshInfo@NGraphicAppearance@@@std@@@std@@@Z
// Identity: proven; own RTTI secondary vtable0x0126ACC0 slot2 and PDB GetMeshesUsed(vector<CMeshInfo>&)const; secondary this+12 -> ParentThing-relative-8; MainGraphic retail+0x44, BankIndex+0 and RenderSizeX+8.
// Layout: engine/CTCBase.h flattens the vptr into void* and supplies no constructor/destructor/accessor API. The local actual polymorphic chain is needed for target constructor/vtable/MI behavior; ParentThing+4 and flags+8 agree with that header and PDB. For nonvirtual queries only these inherited data offsets are used.
// Layout: no target header, and GraphicAppearance/CThing headers are quarantined. Real MI supplies the secondary receiver convention; minimal named MainGraphic view reconciles two maps/two lists from donor+92 to retail+68. Actual CMeshInfo bitfield PrimaryMesh=true preserves the retail OR into the otherwise uninitialized flag byte. Genuine same-TU vector overflow, as verified N0230x7091A0.
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __forceinline inline
typedef __SIZE_TYPE__ PlacementSize;
#else
typedef unsigned int PlacementSize;
#endif
inline void* operator new(PlacementSize,void* p) { return p; }
#pragma pack(push,1)
class CEngineGraphic { public: unsigned long BankIndex; float AnimStep,RenderSizeX; bool AdditiveAlpha; unsigned char Type; };
#pragma pack(pop)
namespace NGraphicAppearance { class CMeshInfo { public: long Index; float SizeX; unsigned char PrimaryMesh:1; CMeshInfo(const CEngineGraphic& g):Index(g.BankIndex),SizeX(g.RenderSizeX),PrimaryMesh(true) {} };
class CTCGraphicAppearance { public: unsigned char _unmodeled00[0x44]; CEngineGraphic MainGraphic; };
}
#ifdef _MSC_VER
#define NOINLINE __declspec(noinline)
extern "C" void* __cdecl malloc(unsigned int); extern "C" void __cdecl free(void*);
#else
#define NOINLINE __attribute__((noinline))
#include <stdlib.h>
#endif
namespace std {
struct __false_type { __false_type() {} };
template<class T> class allocator { public: T* allocate(unsigned int n) { return n?(T*)malloc(n*sizeof(T)):0; } void deallocate(T* p,unsigned int) { if(p) free(p); } };
template<class V,class T,class Alloc> class _STLP_alloc_proxy:public Alloc { public: V _M_data; };
template<class T> inline const T& max(const T& a,const T& b) { return a<b?b:a; }
template<class T,class V> __forceinline void _Construct(T* p,const V& value) { new(p) T(value); }
template<class Input,class Output> NOINLINE Output __fastcall __uninitialized_copy(Input first,Input last,Output result,const __false_type&) { Output current=result; for(;first!=last;++first,++current) _Construct(&*current,*first); return current; }
template<class Iter,class Size,class T> NOINLINE Iter __fastcall __uninitialized_fill_n(Iter first,Size count,const T& value,const __false_type&) { for(;count>0;--count,++first) _Construct(&*first,value); return first; }
template<class T,class Alloc=allocator<T> > class vector { public: typedef unsigned int size_type; typedef T* pointer; T* _M_start; T* _M_finish; _STLP_alloc_proxy<T*,T,Alloc> _M_end_of_storage;
void push_back(const T& value) { if(_M_finish!=_M_end_of_storage._M_data) { _Construct(_M_finish,value); ++_M_finish; } else _M_insert_overflow(_M_finish,value,__false_type(),1,true); }
protected: NOINLINE void _M_insert_overflow(pointer position,const T& value,const __false_type&,size_type fillLength,bool atEnd) {
 const size_type oldSize=size_type(_M_finish-_M_start); const size_type length=oldSize+max(oldSize,fillLength); pointer start=_M_end_of_storage.allocate(length);
 pointer finish=__uninitialized_copy(_M_start,position,start,__false_type());
 if(fillLength==1) { _Construct(finish,value); ++finish; } else finish=__uninitialized_fill_n(finish,fillLength,value,__false_type());
 if(!atEnd) finish=__uninitialized_copy(position,_M_finish,finish,__false_type());
 _M_end_of_storage.deallocate(_M_start,_M_end_of_storage._M_data-_M_start); _M_start=start; _M_finish=finish; _M_end_of_storage._M_data=start+length;
 }
public: void InvokeGrow(pointer p,const T& v,size_type n,bool atEnd) { _M_insert_overflow(p,v,__false_type(),n,atEnd); }
}; }
class CThing { public: virtual ~CThing() {} unsigned char _unmodeled04[0x60]; NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance; };
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
 class CBaseClassNonCopyable:public CBaseClass {};
class CTCBase:public CBaseClassNonCopyable { public: CThing* ParentThing; bool ToBeRemoved,IsInGlobalUpdateTCs; };
namespace NGraphicAppearance { class CAGraphicAppearanceComponent { public: virtual ~CAGraphicAppearanceComponent() {} virtual void OnAppearanceClear() {} virtual void GetMeshesUsed(std::vector<CMeshInfo>&) const {} }; }
namespace NGraphicAppearance { class CTCGraphicAppearanceAnimatedMesh:public CTCBase,public CAGraphicAppearanceComponent { public: virtual void GetMeshesUsed(std::vector<CMeshInfo>&) const; };
void CTCGraphicAppearanceAnimatedMesh::GetMeshesUsed(std::vector<CMeshInfo>& out) const { out.push_back(CMeshInfo(ParentThing->PTCGraphicAppearance->MainGraphic)); } }