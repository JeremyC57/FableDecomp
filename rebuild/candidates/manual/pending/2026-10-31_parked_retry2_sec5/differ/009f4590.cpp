// CSurface::SaveAsTGA(const CWideString& filename, const C2DExtentsI* size, bool alpha) -- copy the surface into a scratch 8:8:8(:8) image of the given (or own) size, convert its pixels into an uncompressed top-down TGA (24- or 32-bit) and write header + pixels to a new disk file
// Leaf: SaveAsTGA
// Grade: DIFFER(900v864) -- both pixel loops count down from the locked-rect bounds (123 diff lines v 167); left: frame
//   0xb8 vs 0xa0, 0 in ebp, ebx pushed late
// Flags: /GS
// Retail 0x009F4590, 864 bytes, `ret 0xc`, /GS cookie 0x0139C8A8. Callees: IDirect3DSurface9 GetDesc/UnlockRect/Release
// (+0x30/+0x38/+0x08), CPixelFormat::Initialise(long, long, long, long, bool) 0x009E3790 / GetByteColourDepth 0x009E3800,
// CSurface::InitialiseAsImage 0x009F3230 / CopyFromSurface 0x009F37E0 / ReadOnlyLock 0x009F3010 (locked rect by hidden
// pointer), malloc 0x00BFEA0E / free 0x00BFEA14, CDiskFileWin32(const CWideString&, FileAccessType, unsigned long)
// 0x0099AD80 / SetLength 0x0099A560 / Write 0x0099AA90 and the inlined ~CDiskFileWin32 (vtable 0x0122D06C, Close
// 0x0099A920, ~CWideString 0x0099B510, ~CBase 0x0099A300); scratch CSurface vtable 0x0122F84C.
// Layout: CSurface as the landed 0x009F37E0 (PDB member names); CSurfaceLockedRect {DimX, DimY, Pitch, PMem}; the
// pixel buffer is an STLport vector<unsigned char> (malloc-backed, zero-filled).
#include <string.h>
#include <stdlib.h>
struct D3DSURFACE_DESC_Local {
    unsigned long Format; unsigned long Type; unsigned long Usage; unsigned long Pool;
    unsigned long MultiSampleType; unsigned long MultiSampleQuality; unsigned long Width; unsigned long Height;
};
struct IDirect3DSurface9 {
    virtual long __stdcall QueryInterface(const void* iid, void** object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall GetDevice(void** device);
    virtual long __stdcall SetPrivateData(const void* guid, const void* data, unsigned long size, unsigned long flags);
    virtual long __stdcall GetPrivateData(const void* guid, void* data, unsigned long* size);
    virtual long __stdcall FreePrivateData(const void* guid);
    virtual unsigned long __stdcall SetPriority(unsigned long priority);
    virtual unsigned long __stdcall GetPriority();
    virtual void __stdcall PreLoad();
    virtual long __stdcall GetType();
    virtual long __stdcall GetContainer(const void* iid, void** container);
    virtual long __stdcall GetDesc(D3DSURFACE_DESC_Local* desc);             // +0x30
    virtual long __stdcall LockRect(void* lockedRect, const void* rect, unsigned long flags);
    virtual long __stdcall UnlockRect();                                     // +0x38
};
class C2DBoxI { public: long left, top, right, bottom; };
struct C2DExtentsI { long X; long Y; C2DExtentsI(long x, long y) : X(x), Y(y) {} };
struct CSurfaceLockedRect { long DimX; long DimY; long Pitch; unsigned char* PMem; };
struct CRGBColour { unsigned char B, G, R, A; };
class CPixelFormat {
public:
    long FormatIndex;
    CPixelFormat() : FormatIndex(-1) {}
    void Initialise(long r, long g, long b, long a, bool alpha);   // 0x009E3790
    unsigned long GetByteColourDepth() const;                      // 0x009E3800
};
struct CWideString {
    void* PStringData;
    ~CWideString();                                 // 0x0099B510
};
struct CBase {
    virtual void v00();
    ~CBase();                                       // 0x0099A300
};
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
class CASuspendableProcess;
struct CDiskFileWin32 : CBase {                     // vtable 0x0122D06C, 0x1c bytes
    virtual void v00();
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // slot 1
    virtual void Close();                           // 0x0099A920 (slot 2)
    long        AccessType;                         // +0x04
    long        Position;                           // +0x08
    long        Length;                             // +0x0c
    void*       WinFileHandle;                      // +0x10
    bool        Opened;                             // +0x14
    CWideString Pathname;                           // +0x18
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    ~CDiskFileWin32() { Close(); }
    void SetLength(long length);                    // 0x0099A560
    void Write(const void* data, long size, CASuspendableProcess* process);   // 0x0099AA90
};
struct CByteVector {                                // STLport vector<unsigned char>
    unsigned char* _M_start;
    unsigned char* _M_finish;
    unsigned char* _M_end_of_storage;
    CByteVector(unsigned long n) : _M_start(0), _M_finish(0), _M_end_of_storage(0)
    {
        _M_start = n ? (unsigned char*)malloc(n) : 0;
        _M_end_of_storage = _M_start + n;
        _M_finish = _M_start;
        memset(_M_start, 0, n);
        _M_finish = _M_start + n;
    }
    ~CByteVector() { if (_M_start) free(_M_start); }
    unsigned long size() const { return _M_finish - _M_start; }
    unsigned char* data() { return size() ? _M_start : (unsigned char*)this; }
};
class CSurface {
public:
    virtual ~CSurface() { Uninitialise(); }
    IDirect3DSurface9* PD3DSurface;                // +0x04
    unsigned long      AllocationSource;           // +0x08
    void*              PAllocatedMemory;           // +0x0c
    CSurface() : PD3DSurface(0), AllocationSource(0) {}
    void Uninitialise()
    {
        if (PD3DSurface) {
            PD3DSurface->Release();
            PD3DSurface = 0;
            AllocationSource = 0;
        }
    }
    long GetWidth() const
    {
        D3DSURFACE_DESC_Local desc;
        PD3DSurface->GetDesc(&desc);
        return desc.Width;
    }
    long GetHeight() const
    {
        D3DSURFACE_DESC_Local desc;
        PD3DSurface->GetDesc(&desc);
        return desc.Height;
    }
    bool InitialiseAsImage(const C2DExtentsI& dims, const CPixelFormat& format, bool dynamic);              // 0x009F3230
    CSurfaceLockedRect ReadOnlyLock(unsigned long flags);                                                  // 0x009F3010
    void Unlock() { PD3DSurface->UnlockRect(); }
    void CopyFromSurface(const C2DBoxI* dstBox, const CSurface& source, const C2DBoxI* srcBox, unsigned long filter);   // 0x009F37E0
    bool SaveAsTGA(const CWideString& filename, const C2DExtentsI* size, bool alpha);
};

bool CSurface::SaveAsTGA(const CWideString& filename, const C2DExtentsI* size, bool alpha)
{
    C2DExtentsI dims(GetWidth(), GetHeight());
    if (size)
        dims = *size;
    CPixelFormat format;
    format.Initialise(8, 8, 8, alpha ? 8 : 0, true);
    CSurface image;
    image.InitialiseAsImage(dims, format, false);
    image.CopyFromSurface(0, *this, 0, 0xffffffff);
    long bytesPerPixel = (alpha ? 1 : 0) + 3;
    CSurfaceLockedRect locked = image.ReadOnlyLock(0x10);
    long length = locked.DimY * locked.DimX * bytesPerPixel;
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
    header[12] = (unsigned char)locked.DimX;
    header[13] = (unsigned char)(locked.DimX / 256);
    header[14] = (unsigned char)locked.DimY;
    header[15] = (unsigned char)(locked.DimY / 256);
    header[16] = (unsigned char)(bytesPerPixel * 8);
    header[17] = 0x20;
    CByteVector pixels(length);
    if (alpha) {
        unsigned char* dst = pixels.data();
        unsigned char* row = locked.PMem;
        for (long y = locked.DimY; y > 0; --y) {
            unsigned char* src = row;
            for (long x = locked.DimX; x > 0; --x) {
                CRGBColour colour = *(CRGBColour*)src;
                dst[2] = colour.R;
                dst[3] = colour.A;
                dst[1] = colour.G;
                dst[0] = colour.B;
                src += format.GetByteColourDepth();
                dst += 4;
            }
            row += locked.Pitch;
        }
    } else {
        unsigned char* dst = pixels.data();
        unsigned char* row = locked.PMem;
        for (long y = locked.DimY; y > 0; --y) {
            unsigned char* src = row;
            for (long x = locked.DimX; x > 0; --x) {
                CRGBColour colour = *(CRGBColour*)src;
                dst[2] = colour.R;
                dst[1] = colour.G;
                dst[0] = colour.B;
                src += format.GetByteColourDepth();
                dst += 3;
            }
            row += locked.Pitch;
        }
    }
    CDiskFileWin32 file(filename, FILE_ACCESS_WRITE, 2);
    file.SetLength(pixels.size() + 18);
    file.Write(header, 18, 0);
    file.Write(pixels.data(), pixels.size(), 0);
    image.Unlock();
    return true;
}
