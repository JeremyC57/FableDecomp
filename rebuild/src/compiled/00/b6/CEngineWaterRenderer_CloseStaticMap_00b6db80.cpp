// CEngineWaterRenderer::CloseStaticMap -- release sea geometry, discard async data and release loading pointers.
// Leaf: ?CloseStaticMap@CEngineWaterRenderer@@QAEXXZ
// Naming wave 25 (c692 queue): CIndexBuffer::Uninitialise 0x00AC1EE0 / CVertexBufferWin32::Uninitialise 0x00A63200 (Ego_r.pdb) on the PDB
//   classes (were the stand-ins CIndexBufferNative / CVertexBufferNative); the row's class is the PDB CEngineWaterRenderer (Ego_r
//   ?CloseStaticMap@CEngineWaterRenderer@@QAEXXZ; was a local CWaterStaticMap; the retail layout stays local, as 0x00B6D480 models it);
//   the engine layout headers are included under a layout alias. The Leaf line is new (the row had none).
// Retail00b6db80 178bytes. Shared donor offsets differ: HaveBackgroundSea2aa vs276, LoadingSea5fc vs5b8.
#ifdef _MSC_VER
#define CEngineWaterRenderer CEngineWaterRendererLayout
#define CIndexBuffer CIndexBufferLayout
#define CVertexBufferWin32 CVertexBufferWin32Layout
#include "engine/CEngineWaterRenderer.h"
#include "engine/CIndexBuffer.h"
#include "engine/CVertexBufferWin32.h"
#undef CEngineWaterRenderer
#undef CIndexBuffer
#undef CVertexBufferWin32
#else
#define __fastcall
#define __forceinline inline
#endif
class CCPPointerInfo { public:unsigned long RefCount;void (__fastcall *DeleteFunc)(void*);void* Data;};
struct CAsyncPointer {void* Data;CCPPointerInfo* Info;
 __forceinline void Reset(){if(Info){--Info->RefCount;if(Info->RefCount==0){Info->DeleteFunc(Info->Data);delete Info;}}Info=0;Data=0;}
};
void __fastcall DiscardAsyncData(CAsyncPointer*); //009d68a0
class CVertexBufferWin32 { public:unsigned char State[0x34];void Uninitialise();}; //00a63200
class CIndexBuffer { public:unsigned char State[0x14];void Uninitialise();}; //00ac1ee0
class CEngineWaterRenderer { public:
 unsigned char Foreground[0x68];CIndexBuffer SeaBackgroundIndexBuffer;CVertexBufferWin32 SeaBackgroundVertexBuffer;
 long SeaBackgroundVertexCount,SeaBackgroundPolyCount;unsigned char ShadersAndLists[0x276-0xb8];bool HaveBackgroundSea;unsigned char SettingsAndTesselators[0x5a8-0x277];
 void* StaticMapBankFile;void* StaticMapSeaEntryPrefix;CAsyncPointer LoadingSeaEntry,LoadingSea;
 void CloseStaticMap();
};
void CEngineWaterRenderer::CloseStaticMap(){
 if(HaveBackgroundSea){SeaBackgroundVertexBuffer.Uninitialise();SeaBackgroundVertexCount=0;SeaBackgroundIndexBuffer.Uninitialise();SeaBackgroundPolyCount=0;HaveBackgroundSea=false;}
 if(LoadingSea.Data){DiscardAsyncData(&LoadingSea);LoadingSea.Reset();LoadingSeaEntry.Reset();}
 StaticMapBankFile=0;
}