// CVolume::CopyFromMemoryAutoSwizzle(void* data, long rowPitch, long slicePitch) -- locks the volume and copies a linear source image into it row by row (width * bytes-per-pixel per row, the source and destination pitches stepped separately); no swizzle on PC
// Leaf: ?CopyFromMemoryAutoSwizzle@CVolume@@QAEXPAXJJ@Z
// Naming wave 25: CPixelFormat::Initialise 0x009E3830 is ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z (Ego_r.pdb; no constructor:
//   the body returns nothing), the format a _D3DFORMAT; a CPixelFormat built from a format is the inline constructor calling it.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A67860, 237 bytes (padded to 240), `ret 0Ch`.
//   PDB name ?CopyFromMemoryAutoSwizzle@CVolume@@QAEXPAXJJ@Z.
// Callees: CVolume::Lock(unsigned long) 0x00A677D0 (CVolumeLockedBox by value), IDirect3DVolume9 GetDesc (slot 8) and
//   UnlockBox (slot 10, the inline CVolume::Unlock), CPixelFormat::Initialise(D3DFORMAT) 0x009E3830,
//   CPixelFormat::GetByteColourDepth 0x009E3800; the row copy is the memcpy intrinsic (`rep movsd ; rep movsb`).
// Shape: two slice pointers stepped by the two slice pitches, two row pointers by the row pitches; the loop counts
//   are strength-reduced into count-down frame words (the row count homed in the dead `data` argument slot).
// Layout: CVolume (Ego_r.pdb, 8 bytes) vptr +0, PD3DVolume +4 (landed CVolume siblings 0x00A674E0 / 0x00A67480).
// Re-port (2026-11-19): the row's local CRGBColour / CPixelFormat / C3DExtentsI models (and its local _D3DFORMAT enum) are replaced by the
//   shared engine/CRGBColour.h, engine/CPixelFormat.h and engine/C3DExtentsI.h (Ego_d.pdb: a C2DExtentsI base plus long Z). CPixelFormat
//   opt-ins: FABLE_RETAIL_CPIXELFORMAT, _BYTECOLOURDEPTH_OUTOFLINE: GetByteColourDepth out of line, as retail calls it here.
//   The format is built by the header's inline CPixelFormat(_D3DFORMAT) constructor (FableWin.pdb ??0CPixelFormat@@QAE@W4_D3DFORMAT@@@Z: Initialise only, no -1 store).
// Same-TU copy: CVolume::Lock is the retail TU-mate 0x00A677D0, written verbatim as the landed owner row over the owner's own D3D /
//   CVolume declarations (the earlier noinline stand-in body is gone); it stays a call here, as in retail (`call 0A677D0h`), with no
//   inline control.
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_BYTECOLOURDEPTH_OUTOFLINE
#include "engine/Direct3D9Types.h"
#include "engine/CRGBColour.h"
#include "engine/CPixelFormat.h"
#include <string.h>
#include "rebuild_abi.h"
struct _GUID;struct IDirect3DDevice9;
enum _D3DRESOURCETYPE{D3DRTYPE_VOLUME=2};enum _D3DPOOL{D3DPOOL_DEFAULT=0};
struct _D3DVOLUME_DESC{_D3DFORMAT Format;_D3DRESOURCETYPE Type;unsigned long Usage;_D3DPOOL Pool;unsigned int Width,Height,Depth;};
struct _D3DBOX{unsigned int Left,Top,Right,Bottom,Front,Back;};struct _D3DLOCKED_BOX{int RowPitch,SlicePitch;void*pBits;};
struct IUnknown{virtual long __stdcall QueryInterface(const _GUID&,void**)=0;virtual unsigned long __stdcall AddRef()=0;virtual unsigned long __stdcall Release()=0;};
struct IDirect3DVolume9:public IUnknown{virtual long __stdcall GetDevice(IDirect3DDevice9**)=0;virtual long __stdcall SetPrivateData(const _GUID&,const void*,unsigned long,unsigned long)=0;virtual long __stdcall GetPrivateData(const _GUID&,void*,unsigned long*)=0;virtual long __stdcall FreePrivateData(const _GUID&)=0;virtual long __stdcall GetContainer(const _GUID&,void**)=0;virtual long __stdcall GetDesc(_D3DVOLUME_DESC*)=0;virtual long __stdcall LockBox(_D3DLOCKED_BOX*,const _D3DBOX*,unsigned long)=0;virtual long __stdcall UnlockBox()=0;};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
#include "engine/C3DExtentsI.h"
class CVolumeLockedBox{public:C3DExtentsI Dimensions;long RowPitch,SlicePitch;unsigned char*PMem;CVolumeLockedBox(){}CVolumeLockedBox(const CVolumeLockedBox&o):Dimensions(o.Dimensions),RowPitch(o.RowPitch),SlicePitch(o.SlicePitch),PMem(o.PMem){}};
class CVolume:public CBaseClass{public:IDirect3DVolume9*PD3DVolume;virtual~CVolume();C3DExtentsI GetDimensions()const{_D3DVOLUME_DESC desc;PD3DVolume->GetDesc(&desc);return C3DExtentsI(desc.Width,desc.Height,desc.Depth);}CVolumeLockedBox Lock(unsigned long);CVolumeLockedBox ReadOnlyLock(unsigned long)const;void Unlock()const{PD3DVolume->UnlockBox();}void CopyFromMemoryAutoSwizzle(void* data,long rowPitch,long slicePitch);};
FABLE_STATIC_ASSERT(sizeof(C3DExtentsI)==12);FABLE_STATIC_ASSERT(sizeof(CVolumeLockedBox)==24);FABLE_STATIC_ASSERT(sizeof(CVolume)==8);
// Fidelity: same-TU strong definition of 0x00A677D0 (retail TU-mate)
CVolumeLockedBox CVolume::Lock(unsigned long flags){
 C3DExtentsI dimensions=GetDimensions();_D3DBOX box;box.Front=0;box.Top=0;box.Left=0;box.Bottom=dimensions.Y;box.Right=dimensions.X;box.Back=dimensions.Z;_D3DLOCKED_BOX locked;
 if(PD3DVolume->LockBox(&locked,&box,flags)>=0){CVolumeLockedBox result;result.Dimensions=dimensions;result.RowPitch=locked.RowPitch;result.SlicePitch=locked.SlicePitch;result.PMem=(unsigned char*)locked.pBits;return result;}return CVolumeLockedBox();
}
void CVolume::CopyFromMemoryAutoSwizzle(void* data, long rowPitch, long slicePitch)
{
    CVolumeLockedBox box = Lock(0);
    _D3DVOLUME_DESC desc;
    PD3DVolume->GetDesc(&desc);
    unsigned long bytesPerPixel;
    {
        CPixelFormat format(desc.Format);
        bytesPerPixel = format.GetByteColourDepth();
    }
    for (long z = 0; z < box.Dimensions.Z; ++z)
        for (long y = 0; y < box.Dimensions.Y; ++y)
            memcpy(box.PMem + z * box.SlicePitch + y * box.RowPitch, (unsigned char*)data + z * slicePitch + y * rowPitch,
                   box.Dimensions.X * bytesPerPixel);
    Unlock();
}