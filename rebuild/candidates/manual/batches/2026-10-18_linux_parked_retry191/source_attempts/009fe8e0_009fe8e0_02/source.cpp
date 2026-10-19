// CGraphicInfo::UnloadFrame(ulong) -- PDB00B58F10
// Leaf: UnloadFrame
#include "engine/CIVCountedPointeeBase.h"
#include "engine/CResource.h"
#include "engine/CPixelFormat.h"
#define CGraphicDataBank CGraphicDataBankDonor
#include "engine/_quarantine/CGraphicDataBank.h"
#undef CGraphicDataBank
// Genuine hierarchy from native RTTI: FrameData -> ManagedTextureOwner -> Resource -> CIVCountedPointeeBase.
struct CountedNative {CountedNative();virtual ~CountedNative();virtual void Delete()const;mutable unsigned long IVRefCount;long Release()const{return --IVRefCount;}}; // ctor PDB019F60F0, Delete const01934D60
struct ResourceNative:CountedNative {ResourceNative();virtual ~ResourceNative();virtual bool UnloadResource();virtual unsigned long GetUnloadableMemory()const;virtual bool IsResourceLoaded()const;virtual void OnResourceLoaded();virtual bool CanAbortResourceLoad(long)const;virtual void OnResourceLoadAborted();virtual unsigned long GetDebugMemoryInUse()const;virtual void ResetDebugMemoryInUse();void*ResourceList;ResourceNative*Prev,*Next;unsigned long ResourceSize,LastUsedFrame;unsigned long GetUnusedFrames()const;}; //009FBF70/PDB00B6BE20
struct ManagedTextureOwner:ResourceNative {ManagedTextureOwner();virtual ~ManagedTextureOwner();virtual void ReleaseTexture();}; // genuine ctor02D878F0
struct GraphicInfoFrame;
struct FrameData:ManagedTextureOwner {FrameData(const GraphicInfoFrame*);virtual ~FrameData();virtual unsigned long GetUnloadableMemory()const;virtual bool IsResourceLoaded()const;virtual void OnResourceLoaded();virtual unsigned long GetDebugMemoryInUse()const;virtual void ResetDebugMemoryInUse();virtual void ReleaseTexture();bool StopLoading();bool FreeTextureMemory(bool);unsigned char NativeTail[40];}; //009FDC20/PDB00B58150 bool nonconst;009FE460/PDB00B58A80 bool(bool); ctor00B589A0
// Native FrameData VT0129C89C: A010A0,42D930,42D940,9FD830,9FD7F0,9FF860,42D970,42D980,9FDCB0,9FD4D0,9FEA00. Full11 real slots, no padding virtuals.
struct FramePointer {FrameData*P;void Reset(){if(P){if(P->Release()==0)P->Delete();P=0;}P=0;}};
struct CGraphicInfo;
struct GraphicInfoFrame {void*VTable;unsigned short FrameNum;signed char SmallMipmapLevel;FramePointer Data;CGraphicInfo*GraphicInfo;}; //native16
#pragma pack(push,1)
struct GraphicHeader {unsigned short Width,Height,Depth,FrameWidth,FrameHeight,FrameCount;CPixelFormat PixelFormat;unsigned char TransparencyType,MipmapLevels,Flags,Padding;unsigned long FrameDataSize,CompressedMipmapSizes;};
#pragma pack(pop)
struct BankNative {unsigned char NativeState[0x268];unsigned long UnloadFrameDelay;}; //retail+268 vs donor+2E4, actual donor header retained
struct CGraphicInfo {CGraphicInfo();GraphicHeader Header;long TextureType;BankNative*Bank;long Index;GraphicInfoFrame*Frames;void UnloadFrame(unsigned long);}; //actual default PDB00B57D00; no repository header
__forceinline const unsigned long& MinAge(const unsigned long&a,const unsigned long&b){return a<b?a:b;}
void CGraphicInfo::UnloadFrame(unsigned long index){
 unsigned long count=Header.FrameCount;
 if(count>1){
 unsigned long age=~0UL;
 for(unsigned long i=0;i<count;++i){GraphicInfoFrame&f=Frames[i];if(f.Data.P&&!f.Data.P->StopLoading())return;if(f.Data.P){unsigned long unused=f.Data.P->GetUnusedFrames();age=MinAge(unused,age);}}
 if(age>=Bank->UnloadFrameDelay)for(unsigned long i=0;i<count;++i)Frames[i].Data.Reset();
 }else{GraphicInfoFrame&f=Frames[index];if(f.Data.P&&!f.Data.P->StopLoading())return;if(f.Data.P){unsigned long age=f.Data.P->GetUnusedFrames();if(f.Data.P)f.Data.P->FreeTextureMemory(age>=Bank->UnloadFrameDelay);}}
}
