// 006c8780 CPersistTraits<CIntelligentPointer<CThing>>::TransferBinaryIn
// Flags: /O2 /Oy /W3
// Leaf: TransferBinaryIn
// Layout: shared CDataInputStream/CDefinitionManager; CThing flags+0x3c,interfaces+0x44; serialisation component ThingUID+0x10 from retail.
#include "engine/CMemoryDataInputStream.h"
#include <stdlib.h>
#include <string.h>
#include "engine/CDefinitionManager.h"
void* __cdecl operator new[](unsigned int);
void __cdecl operator delete[](void*);
namespace X_006c8780 {
extern "C" void* __cdecl memcpy(void*,const void*,unsigned int);
#pragma intrinsic(memcpy)
struct CMemoryDataInputStreamAdapter: ::CMemoryDataInputStream {
 void ReadWithSrcChunkOverflow(unsigned char*,int);
 __forceinline void Read(void* out,int size) {
  __int64 next=(__int64)(unsigned)(position+size);
  if(next<0 || next>0x7fffffff) return;
  if(chunkLength>=size) {memcpy(out,(const void*)chunkBase,size);chunkBase+=size;chunkLength-=size;position+=size;}
  else ReadWithSrcChunkOverflow((unsigned char*)out,size);
 }
 __forceinline unsigned int ReadWord() {unsigned int v;Read(&v,4);return v;}
 __forceinline unsigned __int64 ReadUID(){unsigned __int64 value;Read(&value,8);return value;}
};


#pragma pack(push,1)
template<class Char>struct CBasicString{Char*PData;unsigned StringLength,DataLength;bool UseFastExtend;};
struct CCharStringData{CBasicString<char>Data;int NoRefs;};
#pragma pack(pop)
extern int StringInstances;
struct CCharString {CCharStringData*Data;__declspec(noinline) CCharString(const char*p,int n=-1):Data(0){++StringInstances;if(!p||!*p)return;if(n<0)n=(int)strlen(p);Data=new CCharStringData;Data->Data.PData=new char[n+1];memcpy(Data->Data.PData,p,n);Data->Data.PData[n]=0;Data->Data.StringLength=n;Data->Data.DataLength=n+1;Data->Data.UseFastExtend=true;Data->NoRefs=1;}__declspec(noinline) CCharString(const CCharString&p):Data(0){++StringInstances;if(this!=&p&&p.Data){Data=p.Data;++Data->NoRefs;}}__declspec(noinline) ~CCharString(){if(Data&&--Data->NoRefs<=0){if(Data->Data.PData){::operator delete[](Data->Data.PData);Data->Data.PData=0;}Data->Data.StringLength=0;Data->Data.DataLength&=0x80000000u;delete Data;}Data=0;--StringInstances;}};
struct CDefinitionManagerAdapter:CDefinitionManager {unsigned int GetDefGlobalIndexFromName(const CCharString&)const;};
struct CGameDefinitionManager {unsigned char Padding[16];CDefinitionManagerAdapter* Manager;};
struct CThingManager {unsigned char Padding[36];CGameDefinitionManager* DefinitionManager;};
extern CThingManager* g_pThingManager;
struct CTCThingUIDDummyForSerialisation {unsigned int Words[4];unsigned __int64 ThingUID;};
struct Entry {int Type;CTCThingUIDDummyForSerialisation* Value;};
struct Components {Entry* Start;Entry* Finish;Entry* End;__declspec(noinline) Entry* LowerBound(const int* key){Entry*a=Start;unsigned n=Finish-Start;while(n){unsigned half=n/2;Entry*m=a+half;if(m->Type<*key){a=m+1;n-=half+1;}else n=half;}return a;}};
struct CThing {unsigned char Padding[0x2c];unsigned int NoneTypeFlags;unsigned char Padding30[12];unsigned int Flags;unsigned int Pad40;Components Interfaces;bool Activate();};
struct C3DVector{float x,y,z;__forceinline C3DVector(float a,float b,float c):x(a),y(b),z(c){}};
int GetMapNumberForPosition(const C3DVector&);
CThing* __fastcall CreateThingFromDef(unsigned int,int,int,bool);
struct CTCDNone{__declspec(noinline) static CThing* __fastcall Create(unsigned int index,const C3DVector&p,bool ignoreMap){int map=ignoreMap?0:GetMapNumberForPosition(p);CThing*thing=CreateThingFromDef(index,4,map,true);if(thing&&(thing->NoneTypeFlags&1)&&thing->Activate())return thing;return 0;}};
struct CBaseObjectPointer{void*Object;unsigned Refs;};
struct CBaseIntelligentPointer {CBaseObjectPointer*Link;__declspec(noinline) CBaseIntelligentPointer():Link(0){}__declspec(noinline) virtual ~CBaseIntelligentPointer(){Release();}__forceinline void Release(){if(Link){if(!--Link->Refs&&!Link->Object)delete Link;Link=0;}}__declspec(noinline) void* GetPItem()const{if(!Link)return 0;if(Link->Object)return Link->Object;CBaseIntelligentPointer*self=const_cast<CBaseIntelligentPointer*>(this);self->Release();return 0;}__declspec(noinline) void SetPItem(void*p){if(Link&&Link->Object==p&&p)return;Release();if(p){CBaseObjectPointer*&node=*(CBaseObjectPointer**)((char*)p+4);if(!node){node=new CBaseObjectPointer;node->Object=p;node->Refs=0;}Link=node;++node->Refs;}}};
template<class T>struct CIntelligentPointer:CBaseIntelligentPointer {__forceinline CIntelligentPointer(){}__forceinline CIntelligentPointer(T* p){SetPItem(p);}};
template<class T>struct CPersistTraits {T DefaultVal;void TransferBinaryIn(CMemoryDataInputStreamAdapter&,T&);};
template<class T>void CPersistTraits<T>::TransferBinaryIn(CMemoryDataInputStreamAdapter& in,T& out){
 unsigned __int64 uid=in.ReadUID();
 if(uid>0){
  CThing* thing;{CCharString name("THING_DUMMY_FOR_SERIALISATION");CThingManager*system=g_pThingManager;C3DVector position(0.0f,0.0f,0.0f);CDefinitionManagerAdapter*manager=system->DefinitionManager->Manager;
  thing=CTCDNone::Create(manager->GetDefGlobalIndexFromName(name),position,true);}
  CTCThingUIDDummyForSerialisation* tc=0;
  if(thing->Flags&0x800){int type=0xeb;Components& map=thing->Interfaces;Entry* p=map.LowerBound(&type);tc=(p==map.Finish||type<p->Type?map.Finish:p)->Value;}
  tc->ThingUID=uid;
  T temporary(thing);out.SetPItem(temporary.GetPItem());
 }
}
template void CPersistTraits<CIntelligentPointer<CThing> >::TransferBinaryIn(CMemoryDataInputStreamAdapter&,CIntelligentPointer<CThing>&);

}

void* __cdecl operator new[](unsigned int);

#include <new>

#include <cstdio>
#include <cassert>
namespace X_006c8780 {
int StringInstances;static CThing object;static int creates,getIndexCalls;
unsigned int CDefinitionManagerAdapter::GetDefGlobalIndexFromName(const CCharString&s)const{assert(s.Data&&strcmp(s.Data->Data.PData,"THING_DUMMY_FOR_SERIALISATION")==0);++getIndexCalls;return 23;}
int GetMapNumberForPosition(const C3DVector&p){assert(p.x==0&&p.y==0&&p.z==0);return 7;}CThing* __fastcall CreateThingFromDef(unsigned index,int type,int map,bool flag){assert(index==23&&type==4&&map==0&&flag);++creates;return &object;}bool CThing::Activate(){return true;}
CThingManager*g_pThingManager;
void CMemoryDataInputStreamAdapter::ReadWithSrcChunkOverflow(unsigned char*out,int size){memcpy(out,(const void*)chunkBase,size);chunkBase+=size;position+=size;chunkLength=0;}
}

static void*repairWatched;static int repairDeletes;void __cdecl operator delete[](void*p){if(p==repairWatched)++repairDeletes;free(p);}
namespace X_006c8780{static void OwnershipContractProbe(){{CCharString x("ownership");x.Data->Data.UseFastExtend=false;repairWatched=x.Data->Data.PData;{CCharString copy(x);assert(x.Data->NoRefs==2);}assert(x.Data->NoRefs==1&&!repairDeletes);}assert(repairDeletes==1);{CCharString x("ownership");x.Data->Data.UseFastExtend=false;x.Data->NoRefs=0;repairWatched=x.Data->Data.PData;}assert(repairDeletes==2);}}
int main(){X_006c8780::OwnershipContractProbe();using namespace X_006c8780;assert(sizeof(X_006c8780::CCharStringData)==17);{CCharString shared("shared");CCharString*copies=(CCharString*)operator new(140*sizeof(CCharString));for(int i=0;i<140;++i)new(copies+i)CCharString(shared);assert(shared.Data->NoRefs==141);for(int j=0;j<140;++j)copies[j].~CCharString();operator delete(copies);assert(shared.Data->NoRefs==1&&StringInstances==1);}assert(!StringInstances);CTCThingUIDDummyForSerialisation actual,fallback;memset(&actual,0,sizeof actual);memset(&fallback,0,sizeof fallback);Entry entries[]={{1,0},{0xeb,&actual},{0xff,0},{0,&fallback}};memset(&object,0,sizeof object);object.NoneTypeFlags=1;object.Flags=0x800;object.Interfaces.Start=entries;object.Interfaces.Finish=entries+3;object.Interfaces.End=entries+4;CDefinitionManagerAdapter manager;CGameDefinitionManager holder;memset(&holder,0,sizeof holder);holder.Manager=&manager;CThingManager sys;memset(&sys,0,sizeof sys);sys.DefinitionManager=&holder;g_pThingManager=&sys;unsigned __int64 uid=0x100000023;CMemoryDataInputStreamAdapter in;memset(&in,0,sizeof in);CPersistTraits<CIntelligentPointer<CThing> > traits;{CIntelligentPointer<CThing> out;for(int mode=0;mode<3;++mode){uid=mode==0?0x100000023:(mode==1?0:0xabcdef0000000078);in.chunkBase=(long)&uid;in.chunkLength=mode==2?0:8;int before=in.position;traits.TransferBinaryIn(in,out);assert(in.position==before+8&&StringInstances==0);if(uid)assert(actual.ThingUID==uid&&out.GetPItem()==&object);else assert(creates==1&&out.GetPItem()==&object);}assert(creates==2&&getIndexCalls==2);CBaseObjectPointer*link=*(CBaseObjectPointer**)((char*)&object+4);assert(link&&link->Object==&object&&link->Refs==1);out.SetPItem(0);assert(link->Refs==0);out.SetPItem(&object);assert(link->Refs==1);}CBaseObjectPointer*link=*(CBaseObjectPointer**)((char*)&object+4);assert(link->Refs==0);delete link;*(CBaseObjectPointer**)((char*)&object+4)=0;std::puts("006c8780_TEST PASS");return 0;}