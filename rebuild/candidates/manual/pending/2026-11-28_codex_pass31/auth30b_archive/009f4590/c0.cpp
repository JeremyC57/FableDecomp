// 009f4590 CSurface::SaveAsTGA
#define FABLE_RETAIL_TGA_DISK_POSITION_PASS29
#define FABLE_RETAIL_TGA_PIXEL_RECORDS_PASS29
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CPIXELFORMAT_SIGNED
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CSYSTEMMANAGER
// Leaf: ?SaveAsTGA@CSurface@@QBE_NABVCWideString@@PBVC2DExtentsI@@_N@Z
// Flags: /O2 /Oy /Gr /GS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDISKFILEWIN32
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CSURFACE_COPYTOMEMORY_PASS26
#define FABLE_RETAIL_CSURFACE_COPYFROMSURFACE
#define FABLE_RETAIL_CSURFACE_READONLYLOCK
#define FABLE_RETAIL_CSURFACE_LOCK
#define FABLE_RETAIL_CSURFACE_SAVEASTGA
#define FABLE_RETAIL_CSURFACE_BITMAP
#define FABLE_RETAIL_CSURFACE_DIMENSIONS_INLINE
#define FABLE_RETAIL_C2DEXTENTSI_ORIGINAL_CTOR_BODIES_PASS22
#define FABLE_RETAIL_CSURFACELOCKEDRECT_ORIGINAL_IMPLICIT_CTORS_PASS26
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_BYTECOLOURDEPTH_OUTOFLINE
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_GETBYTESIZE
#define FABLE_RETAIL_STLP_VECTOR
#include "engine/Direct3DSurface9.h"
#include "engine/CSurfaceLockedRect.h"
#include "engine/CPixelFormat.h"
#include "engine/CSurface.h"
#include "engine/CDiskFileWin32.h"
#include "engine/CArray.h"
#include "engine/SurfaceTGAPixels.h"
#include "engine/IDirect3DDevice9.h"
#include "engine/CDisplayManager.h"
#include "engine/CSystemManager.h"
CSurfaceLockedRect CSurface::ReadOnlyLock(unsigned long flags) const
{
    C2DExtentsI dimensions = GetDimensions();
    tagRECT rect;
    rect.left = rect.top = 0;
    rect.bottom = dimensions.Y;
    rect.right = dimensions.X;
    _D3DLOCKED_RECT locked;
    if (PD3DSurface->LockRect(&locked, &rect, flags | 0x10) >= 0) {
        CSurfaceLockedRect result;
        result.Dimensions = dimensions;
        result.Pitch = locked.Pitch;
        result.PMem = static_cast<unsigned char*>(locked.pBits);
        return result;
    }
    CSurfaceLockedRect result;
    result.Dimensions = C2DExtentsI(0,0);
    result.Pitch = 0;
    result.PMem = 0;
    return result;
}

bool CSurface::InitialiseAsImage(const C2DExtentsI& size, const CPixelFormat& format, bool)
{
    Uninitialise();
    C2DExtentsI dimensions(size);
    IDirect3DDevice9* device = GFGetSystemManager()->GetDisplayManager()->GetD3DDevice();
    long result = device->CreateOffscreenPlainSurface(dimensions.X, dimensions.Y, format.GetAsD3DPixelFormat(), 3, &PD3DSurface, 0);
    AllocationSource = 1;
    return result >= 0 ? true : false;
}
bool CSurface::SaveAsTGA(const CWideString& filename,const C2DExtentsI* pextents,bool save_alpha) const {
    // Inline-budget: the two native GetDesc calls are the original GetWidth and GetHeight accessor expansions.
    C2DExtentsI extents(GetWidth(),GetHeight());
    if(pextents) extents=*pextents;
    CPixelFormat dest_pf;
    dest_pf.Initialise(8,8,8,save_alpha?8:0,true);
    CSurface tsurface;
    tsurface.InitialiseAsImage(extents,dest_pf,false);
    tsurface.CopyFromSurface(0,*this,0,0xffffffff);
    unsigned long bytes_per_pixel=save_alpha?4:3;
    CSurfaceLockedRect lock=tsurface.ReadOnlyLock(0x10);
    unsigned char header[18];
    header[0] = 0;
    header[1] = 0;
    header[2] = 2;
    header[3] = 0;
    header[4] = 0;
    header[5] = 0;
    header[6] = 0;
    header[7] = 0;
    header[8] = 0;
    header[9] = 0;
    header[10] = 0;
    header[11] = 0;
    header[12] = (unsigned char)lock.Dimensions.X;
    header[13] = (unsigned char)(lock.Dimensions.X / 256);
    header[14] = (unsigned char)lock.Dimensions.Y;
    header[15] = (unsigned char)(lock.Dimensions.Y / 256);
    header[16] = (unsigned char)(bytes_per_pixel * 8);
    header[17] = 0x20;

    unsigned char* psrc=lock.PMem;
    long pitch=lock.Pitch;
    CArray<unsigned char> dst(lock.Dimensions.X*lock.Dimensions.Y*bytes_per_pixel);
    if(save_alpha) {
        CATGAPixel* tga_pixel=(CATGAPixel*)dst.GetAsCArray();
        for(long cy=0;cy<lock.Dimensions.Y;++cy) {
            psrc=lock.PMem+pitch*cy;
            for(long cx=0;cx<lock.Dimensions.X;++cx) {
                CRGBALong temp_rgba_long=*(const CRGBALong*)psrc;
                tga_pixel->R=temp_rgba_long.R;
                tga_pixel->G=temp_rgba_long.G;
                tga_pixel->B=temp_rgba_long.B;
                tga_pixel->A=temp_rgba_long.A;
                psrc+=dest_pf.GetByteColourDepth();
                ++tga_pixel;
            }
        }
    } else {
        CNATGAPixel* tga_pixel=(CNATGAPixel*)dst.GetAsCArray();
        for(long cy=0;cy<lock.Dimensions.Y;++cy) {
            psrc=lock.PMem+pitch*cy;
            for(long cx=0;cx<lock.Dimensions.X;++cx) {
                CRGBALong temp_rgba_long=*(const CRGBALong*)psrc;
                tga_pixel->R=temp_rgba_long.R;
                tga_pixel->G=temp_rgba_long.G;
                tga_pixel->B=temp_rgba_long.B;

                psrc+=dest_pf.GetByteColourDepth();
                ++tga_pixel;
            }
        }
    }
    CDiskFileWin32 file(filename,FILE_ACCESS_CREATE,2);
    file.SetLength(dst.GetByteSize()+18);
    file.Write(header,18,0);
    file.Write(dst.GetAsCArray(),dst.GetByteSize(),0);
    bool success=true;
    tsurface.Unlock();
    return success;
}
