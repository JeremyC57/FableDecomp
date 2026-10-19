// CParticleNative::SetPrimitiveID -- store a per-parent particle primitive ID and replace its handle.
// Native00AFE9B0,282bytes, ECX owner / stack parentIndex,slotIndex,handle&,longID / ret16.
// Manifest viewport ClearChildPrimitive is contradicted by4args/nested arrays.
// PDB CParticle PrimIDList132 and ParentHandleList148 match native132/144 after
// donor16-byte arrays become native12. No actual CParticle header is available.
// Behavioral method name is inferred, not a recovered PDB method name.
// Actual CIVCountedPointeeBase and flattened PersistentPrimitiveBase retained;
// native B8FFA0 constructor proves vptr0,count4,Type8. Typed view restores base.
// PersistentBase VT12A3D60 slot1=42D930 const Delete;slot3=B8FFC0 NotifyReleased;
// internal primitive overrideslot3=B8FF70, PDB const handle& signature donorA44470.
// Calls AFE910 outer resize(size,const empty array&),4428F0 long-array resize(size,const long&),443F90 handle resize(size).
// Leaf: SetPrimitiveID
#include "engine/CIVCountedPointeeBase.h"
#include "engine/CEnginePersistentPrimitiveBase.h"
#include <map>
// The available headers flatten the vptr. This projection restores the two genuine
// CIV virtuals and three genuine Persistent virtuals, retaining native fields/layout.
// Counted VT12354D0: scalar deleting destructor4664E0, Delete()const42D930.
// Persistent VT12A3D60: dtorB8FFE0, Delete42D930, pure CollectStatsBFEA08,
// NotifyReleasedB8FFC0, WasRendered()const B8FFD0. No omitted or invented slots.
// CollectStats signature proven by native override B3AA90 and donor A50FB0/AA4CB0.
// The enum's name/type are recovered; no enumerator meanings are inferred here.
enum EEnginePrimitiveStatIndex { NativeStatIndexUnspecified = 0 };
struct PrimitiveHandle;
struct NativeCountedPointee {
 virtual ~NativeCountedPointee();
 virtual void Delete()const;
 unsigned long IVRefCount;
 void DecRef(){--IVRefCount;}
 long GetRefCount()const{return IVRefCount;}
 __forceinline void RemoveReference(){DecRef();if(GetRefCount()==0)Delete();}
};
struct PersistentMethods:NativeCountedPointee {
 long Type;
 virtual void CollectStats(std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<const unsigned long,unsigned long> > >&,EEnginePrimitiveStatIndex,unsigned long)const=0;
 virtual void NotifyReleased(const PrimitiveHandle&);
 virtual bool WasRendered()const;
};
struct PrimitivePointer {
 PersistentMethods* Data;
 // FableWin PDB019093D0: CIVCountedPointer::GetRefCount()const returns unsigned long;
 // pointee accessor01909400 returns signed long. Null pointer has zero owned references.
 unsigned long GetRefCount()const{return Data?Data->GetRefCount():0;}
 PersistentMethods*operator->()const{return Data;} // PDB01909260

 __forceinline void Release(){if(Data){Data->RemoveReference();Data=0;}}
 __forceinline PrimitivePointer& operator=(const PrimitivePointer&other){if(Data!=other.Data){Release();Data=other.Data;if(Data)++Data->IVRefCount;}return *this;}
};
struct PrimitiveHandle {
 int UnusedHandleWord;PrimitivePointer Pointer; // raw PDB firstword int padding, not refcount
 __forceinline void Clear(){if(Pointer.GetRefCount()==1)Pointer->NotifyReleased(*this);Pointer.Release();Pointer.Data=0;}
 __forceinline PrimitiveHandle& operator=(const PrimitiveHandle& other){Clear();Pointer=other.Pointer;return *this;}
};
struct PrimitiveIDs {
 long* Start;long* Finish;long* Limit;
 PrimitiveIDs():Start(0),Finish(0),Limit(0){}
 unsigned long size()const{return Finish-Start;}
 void resize(unsigned long,const long&);
};
struct NestedIDs {PrimitiveIDs* Start;PrimitiveIDs* Finish;PrimitiveIDs* Limit;unsigned long size()const{return Finish-Start;}void resize(unsigned long,const PrimitiveIDs&);};
struct Handles {PrimitiveHandle* Start;PrimitiveHandle* Finish;PrimitiveHandle* Limit;unsigned long size()const{return Finish-Start;}void resize(unsigned long);};
struct CParticleNative {
 unsigned char UnrecoveredPrefix[132];NestedIDs PrimIDList;Handles ParentHandleList;
 void SetPrimitiveID(unsigned long,unsigned long,const PrimitiveHandle&,long);
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(PersistentMethods)==sizeof(CEnginePersistentPrimitiveBase));
FABLE_STATIC_ASSERT(sizeof(NativeCountedPointee)==sizeof(CIVCountedPointeeBase));
FABLE_STATIC_ASSERT(offsetof(NativeCountedPointee,IVRefCount)==offsetof(CIVCountedPointeeBase,IVRefCount));
FABLE_STATIC_ASSERT(offsetof(PersistentMethods,Type)==offsetof(CEnginePersistentPrimitiveBase,Type));
FABLE_STATIC_ASSERT(sizeof(PrimitiveHandle)==8);
FABLE_STATIC_ASSERT(sizeof(PrimitiveIDs)==12);
FABLE_STATIC_ASSERT(offsetof(CParticleNative,PrimIDList)==132);
FABLE_STATIC_ASSERT(offsetof(CParticleNative,ParentHandleList)==144);
#endif
void CParticleNative::SetPrimitiveID(unsigned long parent,unsigned long index,const PrimitiveHandle& handle,long id){
 if(PrimIDList.size()<=parent)PrimIDList.resize(parent+1,PrimitiveIDs());
 if(PrimIDList.Start[parent].size()<=index)PrimIDList.Start[parent].resize(index+1,0);
 PrimIDList.Start[parent].Start[index]=id;
 if(ParentHandleList.size()<=parent)ParentHandleList.resize(parent+1);
 ParentHandleList.Start[parent]=handle;
}