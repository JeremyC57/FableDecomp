// Standalone behavioural dependencies. The byte grade uses full shared headers.
#include <stdio.h>
#include <stdlib.h>
#define CHECK(c) do{if(!(c)){printf("FAIL %d\n",__LINE__);abort();}}while(0)
class CEnginePrimitiveHandle;
static int eventCount,events[20];
static void Event(int e){CHECK(eventCount<20);events[eventCount++]=e;}
class CIVCountedPointeeBase {
public:
    mutable unsigned long IVRefCount;
    CIVCountedPointeeBase():IVRefCount(1){}
    virtual ~CIVCountedPointeeBase(){}
    virtual void Delete()const{Event(2);}
    void AddRef()const{++IVRefCount;}
    long Release()const{return --IVRefCount;}
    long GetRefCount()const{return IVRefCount;}
};
class CEnginePersistentPrimitiveBase:public CIVCountedPointeeBase {
public:
    mutable int Notifies;
    CEnginePersistentPrimitiveBase():Notifies(0){}
    virtual void NotifyReleased(const CEnginePrimitiveHandle&);
};
template<class T>class CIVCountedPointer {
public:
    CIVCountedPointeeBase* Object;
    CIVCountedPointer():Object(0){}
    bool IsNull()const{return Object==0;}
    T* operator->()const{return static_cast<T*>(Object);}
    unsigned long GetRefCount()const{return Object?Object->GetRefCount():0;}
    void Release(){CIVCountedPointeeBase* object=Object;if(object){if(object->Release()==0)object->Delete();Object=0;}}
    void Reset(T* object){Release();Object=object;if(Object)Object->AddRef();}
};
class CEnginePrimitiveHandle {
public:
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;
    bool IsNull()const{return Data.IsNull();}
    void Clear(){if(Data.GetRefCount()==1)Data->NotifyReleased(*this);Data.Reset(0);}
};
static CEnginePrimitiveHandle* expectedHandle;
static CEnginePersistentPrimitiveBase* notificationReplacement;
static bool replaceDuringNotify;
void CEnginePersistentPrimitiveBase::NotifyReleased(const CEnginePrimitiveHandle& h){
 CHECK(&h==expectedHandle && h.Data.Object==this && IVRefCount==1);++Notifies;Event(1);
 if(replaceDuringNotify)expectedHandle->Data.Object=notificationReplacement;
}
template<class T>class CArray {
public:
    T* Start;T* Finish;
    CArray():Start(0),Finish(0){}
    unsigned int size()const{return (unsigned int)(Finish-Start);}
    T* begin(){return Start;}
    const T* begin()const{return Start;}
    T& At(unsigned int i){return *(this->begin()+i);}
    const T& operator[](unsigned int i)const{return *(this->begin()+i);}
};
class CEngineInternalPrimitiveGroup {
public:
    CArray<CEnginePrimitiveHandle> Primitives;
    virtual void ClearChildPrimitive(unsigned long index);
    bool HasChildPrimitive(unsigned long index)const{return index<Primitives.size() && !Primitives[index].IsNull();}
    void UpdateRenderLayerMask(){CHECK(expectedHandle->IsNull());Event(3);}
};
void CEngineInternalPrimitiveGroup::ClearChildPrimitive(unsigned long index)
{
    if(HasChildPrimitive(index)){
        Primitives.At(index).Clear();
        UpdateRenderLayerMask();
    }
}
int main(){
 for(int count=0;count<=4;++count)for(unsigned long index=0;index<6;++index)for(int mode=0;mode<5;++mode){
  CEnginePrimitiveHandle handles[4];CEnginePersistentPrimitiveBase value,replacement;
  value.IVRefCount=mode==2?3:1;replacement.IVRefCount=2;
  CEngineInternalPrimitiveGroup group;group.Primitives.Start=handles;group.Primitives.Finish=handles+count;
  eventCount=0;replaceDuringNotify=mode>=3;notificationReplacement=mode==3?0:&replacement;
  expectedHandle=index<4?handles+index:0;
  if(index<4 && mode!=0)handles[index].Data.Object=&value;
  group.ClearChildPrimitive(index);
  if(index>=(unsigned long)count || mode==0){CHECK(eventCount==0 && value.Notifies==0);}
  else {
   CHECK(handles[index].IsNull());
   if(mode==1){CHECK(eventCount==3 && events[0]==1 && events[1]==2 && events[2]==3 && value.IVRefCount==0);}
   if(mode==2){CHECK(eventCount==1 && events[0]==3 && value.IVRefCount==2 && value.Notifies==0);}
   if(mode>=3){CHECK(eventCount==2 && events[0]==1 && events[1]==3 && value.IVRefCount==1);CHECK(replacement.IVRefCount==(mode==4?1UL:2UL));}
   int before=eventCount;group.ClearChildPrimitive(index);CHECK(eventCount==before);
  }
  int before=eventCount;group.ClearChildPrimitive(0xFFFFFFFFUL);CHECK(eventCount==before);
 }
 puts("00bc62d0_TEST PASS");return 0;
}