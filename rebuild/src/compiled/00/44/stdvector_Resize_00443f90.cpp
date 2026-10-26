// CEnginePrimitiveHandleVector::Resize -- constructs a default engine handle, erases excess elements or inserts fill copies, then releases the temporary. Retail00443f90 complete177bytes ret4+15int3 padding. Manifest AddPrimitive is propagatedwrong: 66callers pass scalarcount and native8byteelement arithmetic; RTTI0122f598=CEnginePrimitiveHandle. Helpers00440d70 erase(begin,end),00442350 insert(position,count,fill); CBase ctor0099a2d0/dtor0099a2e0. Shared flat persistentprimitive/CIVbase headers included for native layout; virtual methodview restores slots0dtor/1Delete/3NotifyReleased(consthandle&) callback and refcount+4. No vector/handle/CBase header exists.
#ifndef _MSC_VER
#define __forceinline inline
#endif
#ifdef _MSC_VER
#include "engine/CEnginePersistentPrimitiveBase.h"
#include "engine/CIVCountedPointeeBase.h"
#endif
class CEnginePrimitiveHandle;
struct CPersistentPrimitiveMethods {
 virtual ~CPersistentPrimitiveMethods();
 virtual void Delete()const;
 virtual void ReservedSlot2();
 virtual void NotifyReleased(const CEnginePrimitiveHandle&);
 unsigned long IVRefCount;
 long Type;
 __forceinline unsigned long RemoveReference(){return --IVRefCount;}
};
namespace N_0099a2d0 { struct CBase {CBase();virtual ~CBase();}; } using N_0099a2d0::CBase; //0099a2d0/0099a2e0
struct CHandlePointer {
 CPersistentPrimitiveMethods* Object;
 CHandlePointer():Object(0){}
 __forceinline ~CHandlePointer(){if(Object){CPersistentPrimitiveMethods* p=Object;if(!p->RemoveReference())p->Delete();}Object=0;}
};
class CEnginePrimitiveHandle:public CBase { public:
 CHandlePointer Data;
 CEnginePrimitiveHandle():CBase(),Data(){}
 __forceinline virtual ~CEnginePrimitiveHandle(){if(Data.Object && Data.Object->IVRefCount==1)Data.Object->NotifyReleased(*this);}
};
struct CEnginePrimitiveHandleVector {
 CEnginePrimitiveHandle* First;CEnginePrimitiveHandle* Last;CEnginePrimitiveHandle* End;
 unsigned long size()const{return (unsigned long)(Last-First);}
 void Erase(CEnginePrimitiveHandle*,CEnginePrimitiveHandle*); //00440d70
 void Insert(CEnginePrimitiveHandle*,unsigned long,const CEnginePrimitiveHandle&); //00442350
 void Resize(unsigned long);
};
void CEnginePrimitiveHandleVector::Resize(unsigned long count)
{
 CEnginePrimitiveHandle fill;
 if(count<size())Erase(First+count,Last);
 else Insert(Last,count-size(),fill);
}