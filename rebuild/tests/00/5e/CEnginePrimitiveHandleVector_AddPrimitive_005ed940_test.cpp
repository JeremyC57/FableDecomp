// CEnginePrimitiveHandleVector::AppendDefault -- append a default handle with placement-copy or004413c0 overflow helper. Retail005ed940 complete181bytes+11int3. OriginalAddPrimitive label is propagated; shared persistent headers and genuine polymorphic lifetime model from00443f90. Handles8bytes. Copy construction increments refcount; default temporary cleanup notifies last-reference before rereading its current pointee. Flags[].
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
 CHandlePointer(const CHandlePointer& p):Object(p.Object){if(Object)++Object->IVRefCount;}
 __forceinline ~CHandlePointer(){if(Object){CPersistentPrimitiveMethods* p=Object;if(!p->RemoveReference())p->Delete();}Object=0;}
};
class CEnginePrimitiveHandle:public CBase { public:
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

#include <cstdio>
#include <stddef.h>
static int failures=0,ctors=0,dtors=0,overflows=0,notifies=0,deletes=0,mode=0;
static CEnginePrimitiveHandle* temporary=0;
static CPersistentPrimitiveMethods* attach=0;static CPersistentPrimitiveMethods* replacement=0;
static void check(bool b){if(!b)++failures;}
CBase::CBase(){++ctors;temporary=reinterpret_cast<CEnginePrimitiveHandle*>(this);}
CBase::~CBase(){++dtors;}
CPersistentPrimitiveMethods::~CPersistentPrimitiveMethods(){}
void CPersistentPrimitiveMethods::Delete()const{check(IVRefCount==0);++deletes;}
void CPersistentPrimitiveMethods::ReservedSlot2(){check(false);}
void CPersistentPrimitiveMethods::NotifyReleased(const CEnginePrimitiveHandle& h){check(&h==temporary && IVRefCount==1);++notifies;if(mode==1)const_cast<CEnginePrimitiveHandle&>(h).Data.Object=0;if(mode==2)const_cast<CEnginePrimitiveHandle&>(h).Data.Object=replacement;}
void CEnginePrimitiveHandleVector::InsertOverflow(CEnginePrimitiveHandle* at,const CEnginePrimitiveHandle& fill,const CFalseType&,unsigned long n,bool copy){check(at==Last && Last==End && &fill==temporary && !fill.Data.Object && n==1 && copy);++overflows;if(attach)const_cast<CEnginePrimitiveHandle&>(fill).Data.Object=attach;}
static void run(bool full,unsigned long refs,int m){
 union Storage {long double align;unsigned char bytes[sizeof(CEnginePrimitiveHandle)*4];} storage;
 CEnginePrimitiveHandleVector v;v.First=reinterpret_cast<CEnginePrimitiveHandle*>(storage.bytes);v.Last=v.First;v.End=v.First+(full?0:4);
 CPersistentPrimitiveMethods object,other;object.IVRefCount=refs;other.IVRefCount=1;attach=refs?&object:0;replacement=&other;mode=m;ctors=dtors=overflows=notifies=deletes=0;
 v.AppendDefault();check(ctors==1 && dtors==1 && overflows==(full?1:0));
 check(v.First==reinterpret_cast<CEnginePrimitiveHandle*>(storage.bytes) && v.Last==v.First+(full?0:1));
 if(!full){check(!v.First->Data.Object);v.First->~CEnginePrimitiveHandle();}
 check(notifies==(refs==1?1:0));if(refs==1 && m==1)check(object.IVRefCount==1 && deletes==0);else if(refs==1 && m==2)check(object.IVRefCount==1 && other.IVRefCount==0 && deletes==1);else if(refs)check(object.IVRefCount==refs-1 && deletes==(refs==1?1:0));else check(deletes==0);
}
int main(){
#ifdef _MSC_VER
 check(sizeof(CEnginePrimitiveHandle)==8);
#endif
 run(false,0,0);run(true,0,0);run(true,3,0);run(true,1,0);run(true,1,1);run(true,1,2);
 // Copy constructor owns a reference even when the source is nonempty.
 {CPersistentPrimitiveMethods object;object.IVRefCount=2;CEnginePrimitiveHandle h;h.Data.Object=&object;{CEnginePrimitiveHandle copy(h);check(object.IVRefCount==3 && copy.Data.Object==&object);}check(object.IVRefCount==2);h.Data.Object=0;}
 if(failures){std::printf("005ed940_TEST FAIL %d\n",failures);return 1;}std::printf("005ed940_TEST PASS\n");return 0;
}