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