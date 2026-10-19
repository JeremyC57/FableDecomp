// CEnginePrimitiveHandleVector::Resize -- constructs a default engine handle, erases excess elements or inserts fill copies, then releases the temporary. Retail00443f90 complete177bytes ret4+15int3 padding. Manifest AddPrimitive is propagatedwrong: 66callers pass scalarcount and native8byteelement arithmetic; RTTI0122f598=CEnginePrimitiveHandle. Helpers00440d70 erase(begin,end),00442350 insert(position,count,fill); CBase ctor0099a2d0/dtor0099a2e0. Shared flat persistentprimitive/CIVbase headers included for native layout; virtual methodview restores slots0dtor/1Delete/3NotifyReleased(consthandle&) callback and refcount+4. No vector/handle/CBase header exists.
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
 __forceinline ~CHandlePointer(){if(Object){CPersistentPrimitiveMethods* p=Object;if(!p->RemoveReference())p->Delete();}Object=0;}
};
struct CEnginePrimitiveHandle:CBase {
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

#include <cstdio>
#include <stddef.h>
static int failures=0,baseCtors=0,baseDtors=0,eraseCalls=0,insertCalls=0,lastCalls=0,deleteCalls=0;
static unsigned long expectedOld=0,expectedNew=0;
static CEnginePrimitiveHandle* lastFill=0;
static CPersistentPrimitiveMethods* attach=0;
static CPersistentPrimitiveMethods* replacement=0;
static int callbackMode=0;
static int sequence=0;
static void check(bool b){if(!b)++failures;}
CBase::CBase(){++baseCtors;lastFill=reinterpret_cast<CEnginePrimitiveHandle*>(this);}
CBase::~CBase(){++baseDtors;sequence=3;}
CPersistentPrimitiveMethods::~CPersistentPrimitiveMethods(){}
void CPersistentPrimitiveMethods::Delete()const{++deleteCalls;check(IVRefCount==0);sequence=2;}
void CPersistentPrimitiveMethods::ReservedSlot2(){check(false);}
void CPersistentPrimitiveMethods::NotifyReleased(const CEnginePrimitiveHandle& handle){++lastCalls;check(&handle==lastFill && handle.Data.Object==this && IVRefCount==1);sequence=1;if(callbackMode==1)const_cast<CEnginePrimitiveHandle&>(handle).Data.Object=0;else if(callbackMode==2)const_cast<CEnginePrimitiveHandle&>(handle).Data.Object=replacement;}
void CEnginePrimitiveHandleVector::Erase(CEnginePrimitiveHandle* first,CEnginePrimitiveHandle* last)
{
 ++eraseCalls;check(first==First+expectedNew && last==Last && size()==expectedOld && !lastFill->Data.Object);Last=first;if(attach)lastFill->Data.Object=attach;
}
void CEnginePrimitiveHandleVector::Insert(CEnginePrimitiveHandle* position,unsigned long count,const CEnginePrimitiveHandle& fill)
{
 ++insertCalls;check(position==Last && size()==expectedOld && count==expectedNew-expectedOld && &fill==lastFill && !fill.Data.Object);Last=position+count;if(attach)const_cast<CEnginePrimitiveHandle&>(fill).Data.Object=attach;
}
static void run(unsigned long oldSize,unsigned long newSize,unsigned long refs,int mode)
{
 // Preconstructed storage lets the stub helpers validate resize forwarding without reimplementing STL allocation.
 CEnginePrimitiveHandle storage[8];CEnginePrimitiveHandleVector vector;vector.First=storage;vector.Last=storage+oldSize;vector.End=storage+8;
 CPersistentPrimitiveMethods pointee;pointee.IVRefCount=refs;pointee.Type=97;CPersistentPrimitiveMethods other;other.IVRefCount=1;other.Type=211;
 expectedOld=oldSize;expectedNew=newSize;attach=refs?&pointee:0;replacement=&other;callbackMode=mode;sequence=baseCtors=baseDtors=eraseCalls=insertCalls=lastCalls=deleteCalls=0;lastFill=0;
 vector.Resize(newSize);
 check(vector.First==storage && vector.Last==storage+newSize && vector.End==storage+8);
 check(baseCtors==1 && baseDtors==1 && sequence==3 && eraseCalls==(newSize<oldSize?1:0) && insertCalls==(newSize>=oldSize?1:0));
 check(lastCalls==(refs==1?1:0));
 if(refs==1 && mode==1)check(pointee.IVRefCount==1 && deleteCalls==0);
 else if(refs==1 && mode==2)check(pointee.IVRefCount==1 && other.IVRefCount==0 && deleteCalls==1);
 else if(refs)check(pointee.IVRefCount==refs-1 && deleteCalls==(refs==1?1:0));
 else check(deleteCalls==0);
 check(pointee.Type==97 && other.Type==211);
}
int main(){
#ifdef _MSC_VER
 check(sizeof(CEnginePrimitiveHandle)==8 && sizeof(CPersistentPrimitiveMethods)==sizeof(CEnginePersistentPrimitiveBase));
#endif
 run(4,2,0,0);run(2,6,0,0);run(3,3,0,0);run(0,0,0,0);run(4,1,1,0);run(1,4,3,0);run(4,4,1,1);run(0,2,1,2);
 if(failures){std::printf("00443f90_TEST FAIL %d\n",failures);return 1;}std::printf("00443f90_TEST PASS\n");return 0;
}