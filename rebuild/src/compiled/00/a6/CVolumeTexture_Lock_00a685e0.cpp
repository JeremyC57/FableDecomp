// 00a685e0 CVolumeTexture::Lock
// Leaf: ?Lock@CVolumeTexture@@QAE?AVCVolumeLockedBox@@KK@Z
// Flags:
// Re-port (2026-11-19): the local C3DExtentsI model replaced by the shared engine/C3DExtentsI.h, the local CCountedPointer by engine/CCountedPointer.h.
// Re-port (audit h3c2b, 2026-11-20; widened shared-header policy): the nine local class models (CBaseClassNonCopyable, CInitBaseClass,
//   CRenderStateManager, CRenderManagerCore, CRenderManager2D, CRenderManager, CDisplayManager, CSystemManager, CVolumeTexture) are gone:
//   the manager chain is the shared engine/CSystemManager.h / CDisplayManager.h / CRenderManager.h (the define set of the landed
//   0x009FB750 CTexture::LoadFromDataStreamToPreallocatedSurface, which reaches WaitWhileResourceIsBusy the same way), read through the
//   headers' inline accessors CSystemManager::GetDisplayManager() and FableWin.pdb's CDisplayManager::GetRenderManager() (the members
//   are private there; same bytes as the earlier `PDisplayManager.Data->PRenderManager->`); CVolumeTexture is engine/CVolumeTexture.h
//   (FABLE_RETAIL_CVOLUMETEXTURE_GETASTEXTURELIST declares Lock and GetDimensions; GetDimensions' inline body is defined here over this
//   row's IDirect3DVolumeTexture9 model, as row 0x00A68910 does); the D3D base interfaces are engine/Direct3DTexture9.h's.
//   Not shared (no header exists): CVolumeLockedBox, IDirect3DVolumeTexture9 and the three D3D volume structs.
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_PASS22_D3D_TEXTURE_ORIGINAL_BASES
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_PASS22_CORE_COMPLETE_ORIGINAL_DECLARATION
#define FABLE_RETAIL_PASS22_CORE_ORIGINAL_BUFFER_ACCESSORS
#define FABLE_RETAIL_PASS22_RENDERSTATE_ORIGINAL_METHOD_ORDER
#define FABLE_RETAIL_PASS22_SHADER_ORIGINAL_METHOD_ORDER
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CRENDERMANAGER2D
#define FABLE_RETAIL_CVOLUMETEXTURE_GETASTEXTURELIST
#include <string.h>
#include "fable_compiler.h"
#include "stlport/fable_containers.h"
#include "engine/C3DExtentsI.h"
#include "engine/Direct3DTexture9.h"
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "engine/CVolumeTexture.h"
struct _D3DVOLUME_DESC{_D3DFORMAT Format;_D3DRESOURCETYPE Type;unsigned long Usage;_D3DPOOL Pool;unsigned int Width,Height,Depth;};struct _D3DBOX{unsigned int Left,Top,Right,Bottom,Front,Back;};struct _D3DLOCKED_BOX{int RowPitch,SlicePitch;void*pBits;};
struct IDirect3DVolume9;struct IDirect3DVolumeTexture9:public IDirect3DBaseTexture9{virtual long __stdcall GetLevelDesc(unsigned int,_D3DVOLUME_DESC*)=0;virtual long __stdcall GetVolumeLevel(unsigned int,IDirect3DVolume9**)=0;virtual long __stdcall LockBox(unsigned int,_D3DLOCKED_BOX*,const _D3DBOX*,unsigned long)=0;virtual long __stdcall UnlockBox(unsigned int)=0;virtual long __stdcall AddDirtyBox(const _D3DBOX*)=0;};
class CVolumeLockedBox{public:C3DExtentsI Dimensions;long RowPitch,SlicePitch;unsigned char*PMem;CVolumeLockedBox(){}CVolumeLockedBox(const CVolumeLockedBox&o):Dimensions(o.Dimensions),RowPitch(o.RowPitch),SlicePitch(o.SlicePitch),PMem(o.PMem){}};
inline C3DExtentsI CVolumeTexture::GetDimensions(unsigned long level)const{_D3DVOLUME_DESC desc;PD3DVolumeTexture->GetLevelDesc(level,&desc);return C3DExtentsI(desc.Width,desc.Height,desc.Depth);}
CVolumeLockedBox CVolumeTexture::Lock(unsigned long level,unsigned long flags){C3DExtentsI dimensions=GetDimensions(level);_D3DBOX box;box.Front=0;box.Top=0;box.Left=0;box.Bottom=dimensions.Y;box.Right=dimensions.X;box.Back=dimensions.Z;CSystemManager*system=GFGetSystemManager();system->GetDisplayManager()->GetRenderManager().WaitWhileResourceIsBusy(PD3DVolumeTexture,1.0f);_D3DLOCKED_BOX locked;if(PD3DVolumeTexture->LockBox(level,&locked,&box,flags)>=0){CVolumeLockedBox result;result.Dimensions=dimensions;result.RowPitch=locked.RowPitch;result.SlicePitch=locked.SlicePitch;result.PMem=(unsigned char*)locked.pBits;return result;}return CVolumeLockedBox();}
FABLE_STATIC_ASSERT(sizeof(CVolumeLockedBox)==24);