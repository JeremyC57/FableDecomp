// CGraphicInfoFrame::InitialiseFrame -- initialise resource frame, replace shared graphic and managed texture
// Leaf: InitialiseFrame
// Retail 009FE550, 208 bytes. CFrameData ctor 009FE380; managed texture assignment A69B20 and ClearHandle A69AD0.
// No CGraphicInfoFrame/CManagedTexture headers exist. CResource header retained at native size 1C.
#include "engine/CTexture.h"
struct C2DExtentsI { long X,Y; C2DExtentsI(); };
// Native method view: generated CTexture has the right fields but no declarations.
struct CTextureMethods : CTexture { C2DExtentsI GetDimensions(unsigned long mip) const; }; // 0x009F9DB0
void __cdecl operator delete(void*); // 0x00BFE9BC
struct CCPPointerInfo { long References; void (__fastcall* Destroy)(void*); void* Object; };
struct CountedTexture {
 CTextureMethods* Data; CCPPointerInfo* Info;
 __forceinline void Release() { if(Info) { --Info->References; if(Info->References==0) { Info->Destroy(Info->Object); operator delete(Info); } } }
 __forceinline ~CountedTexture() { Release(); Data=0; Info=0; }
 __forceinline void Assign(const CountedTexture& p) { CTextureMethods* data=p.Data; CCPPointerInfo* info=p.Info; if(Info!=info) { Release(); Data=data; Info=info; if(info) ++info->References; } }
};
// No engine/CGraphicFrameData.h exists; PDB fields agree with all native offsets.
struct CGraphicFrameData {
 unsigned long ReferenceCount; void* Data; CountedTexture PTexture;
 unsigned short TLX,TLY,Width,Height,Pitch;
 unsigned char Alpha:1,BooleanAlpha:1,UsesSurface:1,SectionOfTPage:1,IsInitialised:1,ScreenGraphic:1;
 void Uninitialise(); // 0x009FCBA0
 __forceinline ~CGraphicFrameData() { if(IsInitialised) Uninitialise(); }
 bool InitialiseAsSurface(const CountedTexture&,unsigned long,unsigned long,bool,bool);
};
#include "engine/CResource.h"
#include "engine/CIVCountedPointeeBase.h"
// Genuine virtual member ABI: native single-inheritance PMFs occupy4 bytes;
// host table entries retain the full member-pointer representation.
struct CIVCalls;
#pragma pointers_to_members(full_generality,single_inheritance)
union IVSlot {
 void* Address;
 void* (CIVCalls::*ScalarDeletingDestructor)(unsigned int);
 void (CIVCalls::*Delete)() const;
};
#ifdef _MSC_VER
FABLE_STATIC_ASSERT(sizeof(IVSlot)==4);
#endif
struct CIVCalls:CIVCountedPointeeBase {
 __forceinline void Delete() const {(this->*reinterpret_cast<const IVSlot*>(__vftable)[1].Delete)();}
};
__forceinline void IVRelease(CIVCountedPointeeBase* p) { if(--p->IVRefCount==0) static_cast<CIVCalls*>(p)->Delete(); }
struct SharedFrame {
 CGraphicFrameData* FrameData;
 __forceinline void Assign(const SharedFrame& p) { CGraphicFrameData* old=FrameData; if(old && --old->ReferenceCount==0) { old->~CGraphicFrameData(); operator delete(old); } FrameData=p.FrameData; if(FrameData) ++FrameData->ReferenceCount; }
};
struct CManagedTexture { void* VTable; CIVCountedPointeeBase* Data; CManagedTexture& operator=(const CManagedTexture&); bool ClearHandle() const; };
struct CGraphicInfoFrame;
struct CFrameData : CResource {
 SharedFrame Frame; CManagedTexture Texture; CManagedTexture SmallTexture; void* LoadInfo; void* LoadContext;
 signed char CurrentPreloadMipmap; unsigned char Unknown39; signed char RequestedMipmap; unsigned char Unknown3B; long ResourcePriority; CGraphicInfoFrame* Owner;
 CFrameData(CGraphicInfoFrame*);
};
struct ResourcePointer { CFrameData* Object;
 __forceinline void Assign(CFrameData* p) { if(Object) { IVRelease((CIVCountedPointeeBase*)Object); Object=0; } Object=p; }
};
struct CGraphicInfoFrame {
 unsigned long Unknown; unsigned short FrameNum; signed char SmallMipmapLevel; unsigned char Unknown7;
 ResourcePointer Data; void* GraphicInfo;
 void InitialiseFrame(const SharedFrame&,const CManagedTexture&,signed char);
};
void CGraphicInfoFrame::InitialiseFrame(const SharedFrame& frame,const CManagedTexture& texture,signed char level) {
 if(!Data.Object) Data.Assign(new CFrameData(this));
 Data.Object->Frame.Assign(frame);
 Data.Object->Texture=texture;
 texture.ClearHandle();
 Data.Object->CurrentPreloadMipmap=level;
}