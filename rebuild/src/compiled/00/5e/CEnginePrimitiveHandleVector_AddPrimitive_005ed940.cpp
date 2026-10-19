// CEnginePrimitiveHandleVector::AppendDefault -- append a default handle with placement-copy or004413c0 overflow helper. Retail005ed940 complete181bytes+11int3. OriginalAddPrimitive label is propagated; shared persistent headers and genuine polymorphic lifetime model from00443f90. Handles8bytes. Copy construction increments refcount; default temporary cleanup notifies last-reference before rereading its current pointee. Flags[].
#ifndef _MSC_VER
#define __forceinline inline
#endif
#ifdef _MSC_VER
#include "engine/CEnginePersistentPrimitiveBase.h"
#include "engine/CIVCountedPointeeBase.h"
#endif
struct CEnginePrimitiveHandle;
struct CPersistentPrimitiveMethods {
 virtual ~CPersistentPrimitiveMethods();
 virtual void Delete()const;
 virtual void ReservedSlot2();
 virtual void NotifyReleased(const CEnginePrimitiveHandle&);
 unsigned long IVRefCount;
 long Type;
 __forceinline unsigned long RemoveReference(){return --IVRefCount;}
};
struct CBase {CBase();virtual ~CBase();}; //0099a2d0/0099a2e0
struct CHandlePointer {
 CPersistentPrimitiveMethods* Object;
 CHandlePointer():Object(0){}
 CHandlePointer(const CHandlePointer& p):Object(p.Object){if(Object)++Object->IVRefCount;}
 __forceinline ~CHandlePointer(){if(Object){CPersistentPrimitiveMethods* p=Object;if(!p->RemoveReference())p->Delete();}Object=0;}
};
struct CEnginePrimitiveHandle:CBase {
 CHandlePointer Data;
 CEnginePrimitiveHandle():CBase(),Data(){}
 __forceinline virtual ~CEnginePrimitiveHandle(){if(Data.Object && Data.Object->IVRefCount==1)Data.Object->NotifyReleased(*this);}
};
struct CFalseType {CFalseType(){}};
struct CEnginePrimitiveHandleVector {
 CEnginePrimitiveHandle* First;CEnginePrimitiveHandle* Last;CEnginePrimitiveHandle* End;
 void InsertOverflow(CEnginePrimitiveHandle*,const CEnginePrimitiveHandle&,const CFalseType&,unsigned long,bool); //004413c0
 void AppendDefault();
};
#include <new>
void CEnginePrimitiveHandleVector::AppendDefault()
{
 CEnginePrimitiveHandle value;
 if(Last!=End){if(Last)new(Last) CEnginePrimitiveHandle(value);++Last;}
 else InsertOverflow(Last,value,CFalseType(),1,true);
}