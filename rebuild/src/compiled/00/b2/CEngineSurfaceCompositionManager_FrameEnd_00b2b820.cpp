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