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