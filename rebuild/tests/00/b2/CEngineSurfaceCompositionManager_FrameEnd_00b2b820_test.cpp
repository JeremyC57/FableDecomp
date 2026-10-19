// CEngineSurfaceCompositionManager::FrameEnd -- retain shared surface caches, release unused caches and end banks.
// Retail00B2B820,233 bytes. Native method this is CEngineComponent subobject +0x7c into complete manager.
// Shared donor header is a flattened record; this local nonpolymorphic view adds no virtual methods.
// Native list head full88, banks398/6a4, CurrentFrame9d0, TextureManager9d4, CacheSize10d8.
// Donor list88 but16-byte vectors/larger banks shift later members (banks41c/7a4, frameb50).
// Calls frame Sizeof009FCBF0, texture FrameEnd00A698B0, resource bank009FC430,
// graphic banks009FD660, operator delete00BFE9BC and free00BFEA14. CCPPointerInfo callback is void __fastcall(void*) at4, Data8.
#ifdef _MSC_VER
#include "engine/CEngineSurfaceCompositionManager.h"
#include "engine/CGraphicFrame.h"
#else
#define __forceinline inline
#define __fastcall
struct CGraphicFrameData;
struct CGraphicFrame { CGraphicFrameData* FrameData; };
#endif
extern "C" void free(void*); // 0x00BFEA14
void operator delete(void*) throw(); // 0x00BFE9BC
struct CGraphicFrameData { unsigned long Sizeof(); }; // 0x009FCBF0
struct CGraphicFrameMethods : CGraphicFrame {
 unsigned long Sizeof() {return FrameData ? FrameData->Sizeof()+sizeof(CGraphicFrame) : sizeof(CGraphicFrame);}
};
struct CCompositeSurfaceCache { unsigned char ResourceAndMorphs[0x4c]; CGraphicFrameMethods GraphicFrame; };
struct CCPPointerInfo { unsigned long RefCount; void (__fastcall *DeleteFunc)(void*); void* Data; };
struct CCachePointer {
 CCompositeSurfaceCache* Data; CCPPointerInfo* Info;
 __forceinline unsigned long GetRefCount() const { return Info ? Info->RefCount : 0; }
 __forceinline ~CCachePointer() {
  if(Info) {--Info->RefCount; if(Info->RefCount==0) { Info->DeleteFunc(Info->Data); delete Info; }}
  Data=0;Info=0;
 }
};
struct CacheNode {CacheNode* Next;CacheNode* Prev;CCachePointer Value;};
struct CResourceBank {unsigned char State[0x7c];void FrameEnd();}; // 0x009FC430
struct CGraphicDataBank {unsigned char State[0x30c];void FrameEnd();}; // 0x009FD660
struct CTextureManager {unsigned char State[0x5d4];void FrameEnd();}; // 0x00A698B0
struct CEngineComponentParams;
struct CSurfaceCompositionFrameEnd {
 unsigned char ComponentData[0xc];
 CacheNode* CachedSurfaces;
 unsigned char IntermediateBankAndRecords[0x30c];
 CGraphicDataBank CompletedBankFile;
 CGraphicDataBank CustomTextureBankFile;
 unsigned char GeneratedMorphsAndNames[0x20];
 unsigned long CurrentFrame;
 CTextureManager TextureManager;
 unsigned char PixelFormatsHandlesAndOptions[0x130];
 unsigned long CacheSizeBytes;
 CResourceBank* GetResourceBank() {return reinterpret_cast<CResourceBank*>(reinterpret_cast<unsigned char*>(this)-0x7c);}
 void FrameEnd(const CEngineComponentParams&);
};
void CSurfaceCompositionFrameEnd::FrameEnd(const CEngineComponentParams&)
{
 CacheSizeBytes=0;
 CacheNode* node=CachedSurfaces->Next;
 while(node!=CachedSurfaces) {
  CacheNode* next=node->Next;
  if(node->Value.GetRefCount()>1) CacheSizeBytes+=node->Value.Data->GraphicFrame.Sizeof();
  else {
   CacheNode* previous=node->Prev;previous->Next=next;next->Prev=previous;
   node->Value.~CCachePointer();free(node);
  }
  node=next;
 }
 TextureManager.FrameEnd();GetResourceBank()->FrameEnd();CompletedBankFile.FrameEnd();CustomTextureBankFile.FrameEnd();++CurrentFrame;
}
#include <stdio.h>
extern "C" void* malloc(unsigned long);
#include <new>
struct CEngineComponentParams {};
static CSurfaceCompositionFrameEnd* activeManager;
static bool mutateSize,mutateFrame;
static void* order[8];static int orderCount;static int destroyed;static unsigned long payloadSize=40;
unsigned long CGraphicFrameData::Sizeof() {order[orderCount++]=this;if(mutateSize)activeManager->CacheSizeBytes=100;return payloadSize;}
void CTextureManager::FrameEnd() {order[orderCount++]=this;if(mutateFrame)activeManager->CurrentFrame=700;}
void CResourceBank::FrameEnd() {order[orderCount++]=this;}
void CGraphicDataBank::FrameEnd() {order[orderCount++]=this;}
static void* lastDeleted;
static void __fastcall DeleteCache(void* data) {++destroyed;lastDeleted=data;delete static_cast<CCompositeSurfaceCache*>(data);}
struct TestOwner {
#ifndef _MSC_VER
 unsigned char hostAlignment[4];
#endif
 CResourceBank bank;CSurfaceCompositionFrameEnd view;
};
static CacheNode* makeNode(CCompositeSurfaceCache* data,unsigned long count,bool hasInfo) {
 CacheNode* n=new(malloc(sizeof(CacheNode))) CacheNode;
 n->Next=n->Prev=0;n->Value.Data=data;n->Value.Info=0;
 if(hasInfo) {n->Value.Info=new CCPPointerInfo;n->Value.Info->RefCount=count;n->Value.Info->DeleteFunc=&DeleteCache;n->Value.Info->Data=data;}
 return n;
}
int main() {
 TestOwner owner;CSurfaceCompositionFrameEnd& manager=owner.view;CEngineComponentParams params;activeManager=&manager;
 CacheNode head;head.Value.Data=0;head.Value.Info=0;head.Next=head.Prev=&head;manager.CachedSurfaces=&head;
 manager.CacheSizeBytes=999;manager.CurrentFrame=10;orderCount=0;destroyed=0;manager.FrameEnd(params);
 bool ok=manager.CacheSizeBytes==0&&manager.CurrentFrame==11&&orderCount==4&&order[0]==&manager.TextureManager&&order[1]==&owner.bank&&order[2]==&manager.CompletedBankFile&&order[3]==&manager.CustomTextureBankFile;
 CGraphicFrameData frame;CCompositeSurfaceCache* retained=new CCompositeSurfaceCache;retained->GraphicFrame.FrameData=&frame;
 CCompositeSurfaceCache* empty=new CCompositeSurfaceCache;empty->GraphicFrame.FrameData=0;
 CacheNode* nodes[4]={makeNode(new CCompositeSurfaceCache,1,true),makeNode(retained,2,true),makeNode(0,0,false),makeNode(empty,3,true)};
 void* expectedDeleted=nodes[0]->Value.Info->Data; nodes[0]->Value.Data=retained;
 head.Next=nodes[0];head.Prev=nodes[3];for(int i=0;i<4;++i){nodes[i]->Prev=i?nodes[i-1]:&head;nodes[i]->Next=i<3?nodes[i+1]:&head;}
 manager.CurrentFrame=~0UL;orderCount=0;manager.FrameEnd(params);
 ok=ok&&manager.CurrentFrame==0&&manager.CacheSizeBytes==payloadSize+2*sizeof(CGraphicFrame)&&destroyed==1&&lastDeleted==expectedDeleted&&orderCount==5&&order[0]==&frame&&order[1]==&manager.TextureManager&&order[2]==&owner.bank&&order[3]==&manager.CompletedBankFile&&order[4]==&manager.CustomTextureBankFile;
 ok=ok&&head.Next==nodes[1]&&head.Prev==nodes[3]&&nodes[1]->Next==nodes[3]&&nodes[3]->Prev==nodes[1]&&nodes[1]->Prev==&head&&nodes[3]->Next==&head&&nodes[1]->Value.Info->RefCount==2&&nodes[3]->Value.Info->RefCount==3;

 orderCount=0;mutateSize=true;mutateFrame=true;manager.FrameEnd(params);
 ok=ok&&manager.CacheSizeBytes==100+payloadSize+2*sizeof(CGraphicFrame)&&manager.CurrentFrame==701&&destroyed==1&&orderCount==5;
 mutateSize=mutateFrame=false;
#ifdef _MSC_VER
 ok=ok&&sizeof(CCPPointerInfo)==12&&(unsigned char*)&manager.CacheSizeBytes-(unsigned char*)&manager==0x105c&&(unsigned char*)&manager.CurrentFrame-(unsigned char*)&manager==0x954;
#endif
 for(int j=1;j<4;j+=2){delete nodes[j]->Value.Info;nodes[j]->Value.Info=0;nodes[j]->Value.Data=0;nodes[j]->~CacheNode();free(nodes[j]);}

 // Native zero refcount wraps; its callback/info are retained while the list node is removed.
 head.Next=head.Prev=&head;
 CacheNode* zero=makeNode(0,0,true);CCPPointerInfo* zeroInfo=zero->Value.Info;
 head.Next=head.Prev=zero;zero->Next=zero->Prev=&head;
 orderCount=0;manager.FrameEnd(params);
 ok=ok&&head.Next==&head&&head.Prev==&head&&zeroInfo->RefCount==~0UL&&destroyed==1&&manager.CacheSizeBytes==0&&orderCount==4;
 delete zeroInfo;
 delete retained;delete empty;
 printf("00b2b820_TEST %s\n",ok?"PASS":"FAIL");return ok?0:1;
}