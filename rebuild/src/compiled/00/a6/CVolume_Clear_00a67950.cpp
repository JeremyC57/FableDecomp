// CVolume::Clear(const CRGBColour& colour) -- locks the volume and writes the colour, converted to the volume's pixel format, into every texel (pixel by pixel, rows by the locked row pitch, slices by the slice pitch on top)
// Leaf: ?Clear@CVolume@@QAEXABVCRGBColour@@@Z
// Naming wave 25: CPixelFormat::Initialise 0x009E3830 is ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z (Ego_r.pdb; no constructor:
//   the body returns nothing), the format a _D3DFORMAT; a CPixelFormat built from a format is the inline constructor calling it.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A67950, 194 bytes (padded to 208), `ret 4`.
//   PDB name ?Clear@CVolume@@QAEXABVCRGBColour@@@Z.
// Callees: CVolume::Lock(unsigned long) 0x00A677D0, IDirect3DVolume9 GetDesc (slot 8) / UnlockBox (slot 10, inline
//   CVolume::Unlock), CPixelFormat::Initialise(D3DFORMAT) 0x009E3830, CPixelFormat::ConvertRGBToPFDData(const
//   CRGBColour&, unsigned char*) 0x009E3F60 (catalog "Graphics_CopyPixelsToBuffer"), GetByteColourDepth 0x009E3800
//   (called again for every texel). CVolume::Lock has its same-TU copy below: with only a declaration VC7.1 hoisted the
//   box's dimension loads out of the row/slice loops.
// Shape: one running destination pointer (read from the box before GetDesc) advanced by the row pitch after each row
//   and by the slice pitch after each slice; the three loop counts are strength-reduced count-downs.
// Layout: CVolume (Ego_r.pdb, 8 bytes) vptr +0, PD3DVolume +4.
// Re-port (2026-11-19): the row's local CRGBColour / CPixelFormat / C3DExtentsI models (and its local _D3DFORMAT enum) are replaced by the
//   shared engine/CRGBColour.h, engine/CPixelFormat.h and engine/C3DExtentsI.h (Ego_d.pdb: a C2DExtentsI base plus long Z). CPixelFormat
//   opt-ins: FABLE_RETAIL_CPIXELFORMAT, _BYTECOLOURDEPTH_OUTOFLINE, _CONVERTRGBTOPFDDATA_OUTOFLINE: both callees out of line, as retail calls them here.
//   The format is built by the header's inline CPixelFormat(_D3DFORMAT) constructor (FableWin.pdb ??0CPixelFormat@@QAE@W4_D3DFORMAT@@@Z: Initialise only, no -1 store).
// Same-TU copy: CVolume::Lock is the retail TU-mate 0x00A677D0, written verbatim as the landed owner row over the owner's own D3D /
//   CVolume declarations (the earlier noinline stand-in body is gone); it stays a call here, as in retail (`call 0A677D0h`), with no
//   inline control.
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_BYTECOLOURDEPTH_OUTOFLINE
#define FABLE_RETAIL_CPIXELFORMAT_CONVERTRGBTOPFDDATA_OUTOFLINE
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
class CVolume:public CBaseClass{public:IDirect3DVolume9*PD3DVolume;virtual~CVolume();C3DExtentsI GetDimensions()const{_D3DVOLUME_DESC desc;PD3DVolume->GetDesc(&desc);return C3DExtentsI(desc.Width,desc.Height,desc.Depth);}CVolumeLockedBox Lock(unsigned long);CVolumeLockedBox ReadOnlyLock(unsigned long)const;void Unlock()const{PD3DVolume->UnlockBox();}void Clear(const CRGBColour& colour);};
FABLE_STATIC_ASSERT(sizeof(C3DExtentsI)==12);FABLE_STATIC_ASSERT(sizeof(CVolumeLockedBox)==24);FABLE_STATIC_ASSERT(sizeof(CVolume)==8);
// Fidelity: same-TU strong definition of 0x00A677D0 (retail TU-mate)
CVolumeLockedBox CVolume::Lock(unsigned long flags){
 C3DExtentsI dimensions=GetDimensions();_D3DBOX box;box.Front=0;box.Top=0;box.Left=0;box.Bottom=dimensions.Y;box.Right=dimensions.X;box.Back=dimensions.Z;_D3DLOCKED_BOX locked;
 if(PD3DVolume->LockBox(&locked,&box,flags)>=0){CVolumeLockedBox result;result.Dimensions=dimensions;result.RowPitch=locked.RowPitch;result.SlicePitch=locked.SlicePitch;result.PMem=(unsigned char*)locked.pBits;return result;}return CVolumeLockedBox();
}
void CVolume::Clear(const CRGBColour& colour)
{
    CVolumeLockedBox box = Lock(0);
    unsigned char* dst = box.PMem;
    _D3DVOLUME_DESC desc;
    PD3DVolume->GetDesc(&desc);
    CPixelFormat format(desc.Format);
    for (long z = box.Dimensions.Z; z > 0; --z) {
        for (long y = box.Dimensions.Y; y > 0; --y) {
            unsigned char* texel = dst;
            for (long x = box.Dimensions.X; x > 0; --x) {
                format.ConvertRGBToPFDData(colour, texel);
                texel += format.GetByteColourDepth();
            }
            dst += box.RowPitch;
        }
        dst += box.SlicePitch;
    }
    Unlock();
}