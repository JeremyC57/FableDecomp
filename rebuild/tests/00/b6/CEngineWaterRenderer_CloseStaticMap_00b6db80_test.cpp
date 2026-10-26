// CEngineWaterRenderer::CloseStaticMap -- release sea geometry, discard async data and release loading pointers.
// Retail00b6db80 178bytes. Shared donor offsets differ: HaveBackgroundSea2aa vs276, LoadingSea5fc vs5b8.
#ifdef _MSC_VER
#include "engine/CEngineWaterRenderer.h"
#include "engine/CIndexBuffer.h"
#include "engine/CVertexBufferWin32.h"
#else
#define __fastcall
class CIndexBuffer { public:unsigned char State[0x14];};
class CVertexBufferWin32 { public:unsigned char State[0x34];};
#define __forceinline inline
#endif
class CCPPointerInfo { public:unsigned long RefCount;void (__fastcall *DeleteFunc)(void*);void* Data;};
struct CAsyncPointer {void* Data;CCPPointerInfo* Info;
 __forceinline void Reset(){if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}}Info=0;Data=0;}
};
void __fastcall DiscardAsyncData(CAsyncPointer*); //009d68a0
struct CVertexBufferNative:CVertexBufferWin32 {void Uninitialise();}; //00a63200
struct CIndexBufferNative:CIndexBuffer {void Uninitialise();}; //00ac1ee0
struct CWaterStaticMap {
 unsigned char Foreground[0x68];CIndexBufferNative SeaBackgroundIndexBuffer;CVertexBufferNative SeaBackgroundVertexBuffer;
 long SeaBackgroundVertexCount,SeaBackgroundPolyCount;unsigned char ShadersAndLists[0x276-0xb8];bool HaveBackgroundSea;unsigned char SettingsAndTesselators[0x5a8-0x277];
 void* StaticMapBankFile;void* StaticMapSeaEntryPrefix;CAsyncPointer LoadingSeaEntry,LoadingSea;
 void CloseStaticMap();
};
void CWaterStaticMap::CloseStaticMap(){
 if(HaveBackgroundSea){SeaBackgroundVertexBuffer.Uninitialise();SeaBackgroundVertexCount=0;SeaBackgroundIndexBuffer.Uninitialise();SeaBackgroundPolyCount=0;HaveBackgroundSea=false;}
 if(LoadingSea.Data){DiscardAsyncData(&LoadingSea);LoadingSea.Reset();LoadingSeaEntry.Reset();}
 StaticMapBankFile=0;
}

#include <cstdio>
#include <cstddef>
static CWaterStaticMap* active;static int eventCount,discardCount,deleted;static bool ok=true;
static int events[5];
static bool clearOnDiscard;
void CVertexBufferNative::Uninitialise(){ok=ok&&this==&active->SeaBackgroundVertexBuffer&&active->HaveBackgroundSea;events[eventCount++]=1;}
void CIndexBufferNative::Uninitialise(){ok=ok&&this==&active->SeaBackgroundIndexBuffer&&active->SeaBackgroundVertexCount==0&&active->HaveBackgroundSea;events[eventCount++]=2;}
void __fastcall DiscardAsyncData(CAsyncPointer* p){ok=ok&&p==&active->LoadingSea&&p->Data;events[eventCount++]=3;++discardCount;if(clearOnDiscard){p->Data=0;p->Info=0;}}
void __fastcall DeletePayload(void* p){ok=ok&&(p==(void*)11||p==(void*)22);events[eventCount++]=p==(void*)11?4:5;++deleted;}
int main(){
#ifdef _MSC_VER
 ok=ok&&offsetof(CWaterStaticMap,HaveBackgroundSea)==0x276&&offsetof(CWaterStaticMap,LoadingSea)==0x5b8&&offsetof(CWaterStaticMap,LoadingSeaEntry)==0x5b0&&offsetof(CWaterStaticMap,StaticMapBankFile)==0x5a8;
#endif
 for(int mode=0;mode<4;++mode){CWaterStaticMap r;active=&r;eventCount=discardCount=deleted=0;r.HaveBackgroundSea=(mode!=0);r.SeaBackgroundVertexCount=31;r.SeaBackgroundPolyCount=17;r.StaticMapBankFile=(void*)3;r.StaticMapSeaEntryPrefix=(void*)4;
 CCPPointerInfo* dataInfo=new CCPPointerInfo;dataInfo->RefCount=mode==2?2:1;dataInfo->DeleteFunc=DeletePayload;dataInfo->Data=(void*)11;
 CCPPointerInfo* entryInfo=new CCPPointerInfo;entryInfo->RefCount=mode==2?2:1;entryInfo->DeleteFunc=DeletePayload;entryInfo->Data=(void*)22;
 r.LoadingSea.Data=mode==0?0:(void*)11;r.LoadingSea.Info=dataInfo;r.LoadingSeaEntry.Data=(void*)22;r.LoadingSeaEntry.Info=entryInfo;
 r.CloseStaticMap();ok=ok&&!r.StaticMapBankFile&&r.StaticMapSeaEntryPrefix==(void*)4;
 if(mode==0){ok=ok&&eventCount==0&&r.SeaBackgroundVertexCount==31&&r.SeaBackgroundPolyCount==17&&r.LoadingSea.Info==dataInfo&&r.LoadingSeaEntry.Info==entryInfo;delete dataInfo;delete entryInfo;}
 else{ok=ok&&!r.HaveBackgroundSea&&r.SeaBackgroundVertexCount==0&&r.SeaBackgroundPolyCount==0&&discardCount==1&&!r.LoadingSea.Info&&!r.LoadingSea.Data&&!r.LoadingSeaEntry.Info&&!r.LoadingSeaEntry.Data&&events[0]==1&&events[1]==2&&events[2]==3;
 if(mode==2){ok=ok&&deleted==0&&dataInfo->RefCount==1&&entryInfo->RefCount==1;delete dataInfo;delete entryInfo;}else ok=ok&&deleted==2&&events[3]==4&&events[4]==5;}
 }
 // Non-null loading data with absent geometry and null info still discards/clears.
 {CWaterStaticMap r;active=&r;eventCount=discardCount=deleted=0;
 r.HaveBackgroundSea=false;r.SeaBackgroundVertexCount=31;r.SeaBackgroundPolyCount=17;
 r.StaticMapBankFile=(void*)3;r.StaticMapSeaEntryPrefix=(void*)4;
 r.LoadingSea.Data=(void*)11;r.LoadingSea.Info=0;r.LoadingSeaEntry.Data=(void*)22;r.LoadingSeaEntry.Info=0;
 r.CloseStaticMap();ok=ok&&discardCount==1&&eventCount==1&&!deleted&&!r.LoadingSea.Data&&!r.LoadingSeaEntry.Data&&!r.StaticMapBankFile&&r.SeaBackgroundVertexCount==31&&r.SeaBackgroundPolyCount==17;}
 // Zero reference count follows native unsigned decrement, without deleting payload.
 {CWaterStaticMap r;active=&r;eventCount=discardCount=deleted=0;r.HaveBackgroundSea=false;
 CCPPointerInfo* info=new CCPPointerInfo;info->RefCount=0;info->DeleteFunc=DeletePayload;info->Data=(void*)11;
 r.StaticMapBankFile=(void*)3;r.LoadingSea.Data=(void*)11;r.LoadingSea.Info=info;r.LoadingSeaEntry.Data=0;r.LoadingSeaEntry.Info=0;
 r.CloseStaticMap();ok=ok&&info->RefCount==~0UL&&!deleted&&!r.LoadingSea.Info&&!r.StaticMapBankFile;delete info;}
 // Discard helper may already clear its live pointer; subsequent reset must reload it.
 {CWaterStaticMap r;active=&r;eventCount=discardCount=deleted=0;r.HaveBackgroundSea=false;
 CCPPointerInfo* info=new CCPPointerInfo;info->RefCount=1;info->DeleteFunc=DeletePayload;info->Data=(void*)11;
 r.StaticMapBankFile=(void*)3;r.LoadingSea.Data=(void*)11;r.LoadingSea.Info=info;r.LoadingSeaEntry.Data=0;r.LoadingSeaEntry.Info=0;
 clearOnDiscard=true;r.CloseStaticMap();clearOnDiscard=false;
 ok=ok&&info->RefCount==1&&!deleted&&!r.LoadingSea.Info&&!r.LoadingSea.Data&&!r.StaticMapBankFile;delete info;}
 std::printf("00b6db80_TEST %s\n",ok?"PASS":"FAIL");return ok?0:1;}