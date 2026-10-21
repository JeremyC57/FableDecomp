// NEnvironmentInternal::CEnvironmentLookupTexture::CEnvironmentLookupTexture(CWideString filename) -- load the environment colour lookup texture: read the TGA header for the width and height, load the bitmap into an A8R8G8B8 image surface and copy its pixels into a freshly allocated 32-bit buffer
// Leaf: ??0CEnvironmentLookupTexture
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00885F50, length 384 bytes (body 0x178), `ret 4`, /GS (cookie 0x0139C8A8, check
//   0x00BFE9F9).
// PDB: ??0CEnvironmentLookupTexture@NEnvironmentInternal@@QAE@VCWideString@@@Z.
// Callees: CBaseClassNonCopyable ctor @ 0x0099A2F0 / dtor @ 0x0099A300, CWideString() @ 0x0099AED0 / ~CWideString @ 0x0099B510,
//   CDiskFileWin32::Open(const CWideString&, FileAccessType, unsigned long) @ 0x0099A6A0, Read(void*, long,
//   CASuspendableProcess*) @ 0x0099A960, Close @ 0x0099A920 (vtable 0x0122D06C), CPixelFormat::Initialise(long, long, long,
//   long, bool) @ 0x009E3790, the `__fastcall` extents helper @ 0x009F3220 (hidden result in ecx, size in edx, format on the
//   stack; landed as "CopyC2DVector"), CSurface::InitialiseAsImage(const C2DExtentsI&, const CPixelFormat&, bool)
//   @ 0x009F3230, LoadFromBitmapFileToPreallocatedSurface(const CWideString&, unsigned long) @ 0x009F2DD0, the pixel copy to
//   memory @ 0x009F3520, Uninitialise @ 0x009F2E20 (vtable 0x0122F84C), operator new[] @ 0x00BFEB22.
// Layout: Ego_r.pdb CEnvironmentLookupTexture: Width +0, Height +4, Pitch +8, Data +0xc; CDiskFileWin32 (0x1c bytes, Opened
//   +0x14, Pathname +0x18); CSurface (0x10).
// Shape: a local CDiskFileWin32 (inline ctor clearing Opened, inline virtual dtor = Close), the 18-byte TGA header buffer,
//   two CPixelFormat(8,8,8,8) temporaries sharing one slot, the size as a C2DCoordI local.
class CWideString {
public:
    void* PStringData;
    CWideString();                                  // 0x0099AED0
    ~CWideString();                                 // 0x0099B510
};
class CBaseClassNonCopyable {
public:
    CBaseClassNonCopyable();                        // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();               // 0x0099A300
};
enum FileAccessType { FILE_ACCESS_READ = 1 };
class CASuspendableProcess;
class CDiskFileWin32 : public CBaseClassNonCopyable {   // vtable 0x0122D06C, 0x1c bytes
public:
    FileAccessType AccessType;                      // +0x04
    long           Position;                        // +0x08
    long           Length;                          // +0x0c
    void*          WinFileHandle;                   // +0x10
    bool           Opened;                          // +0x14
    CWideString    Pathname;                        // +0x18
    CDiskFileWin32() { Opened = false; }
    virtual ~CDiskFileWin32() { Close(); }
    virtual void Open(const CWideString& name, FileAccessType access, unsigned long flags);   // 0x0099A6A0
    virtual void Read(void* buffer, long size, CASuspendableProcess* process);              // 0x0099A960
    virtual void Close();                           // 0x0099A920
};

class CPixelFormat {
public:
    long FormatIndex;
    void Initialise(long r, long g, long b, long a, bool flag);   // 0x009E3790
    CPixelFormat(long r, long g, long b, long a, bool flag) { Initialise(r, g, b, a, flag); }
};
struct C2DCoordI { long X, Y; C2DCoordI(long x, long y) : X(x), Y(y) { } };
struct C2DExtentsI { long X, Y; C2DExtentsI() { } };
C2DExtentsI __fastcall GetSurfaceExtents(const C2DCoordI& size, const CPixelFormat& format);   // 0x009F3220

class CSurface {                                    // vtable 0x0122F84C, 0x10 bytes
public:
    void*         PD3DSurface;                      // +0x04
    unsigned long AllocationSource;                 // +0x08
    void*         PAllocatedMemory;                 // +0x0c
    CSurface() : PD3DSurface(0), AllocationSource(0) { }
    virtual void v00();
    virtual ~CSurface() { Uninitialise(); }
    void Uninitialise();                            // 0x009F2E20
    void InitialiseAsImage(const C2DExtentsI& extents, const CPixelFormat& format, bool flag);   // 0x009F3230
    void LoadFromBitmapFileToPreallocatedSurface(const CWideString& filename, unsigned long flags);   // 0x009F2DD0
    void CopyToMemory(void* destination, long pitch);   // 0x009F3520
};

void* __cdecl operator new[](unsigned int size);    // 0x00BFEB22

namespace NEnvironmentInternal {
struct CEnvironmentLookupTexture {
    long           Width;                           // +0x00
    long           Height;                          // +0x04
    long           Pitch;                           // +0x08
    unsigned char* Data;                            // +0x0c
    CEnvironmentLookupTexture(CWideString filename);
};

CEnvironmentLookupTexture::CEnvironmentLookupTexture(CWideString filename)
{
    {
        CDiskFileWin32 file;
        file.Open(filename, FILE_ACCESS_READ, 2);
        unsigned char header[0x12];
        file.Read(header, 0x12, 0);
        Width = header[0xc] + (header[0xd] << 8);
        Height = header[0xe] + (header[0xf] << 8);
        file.Close();
        CSurface surface;
        C2DExtentsI extents = GetSurfaceExtents(C2DCoordI(Width, Height), CPixelFormat(8, 8, 8, 8, false));
        surface.InitialiseAsImage(extents, CPixelFormat(8, 8, 8, 8, false), false);
        surface.LoadFromBitmapFileToPreallocatedSurface(filename, 1);
        Pitch = extents.X * 4;
        Data = (unsigned char*)operator new[](Pitch * extents.Y);
        surface.CopyToMemory(Data, Pitch);
    }
}
}