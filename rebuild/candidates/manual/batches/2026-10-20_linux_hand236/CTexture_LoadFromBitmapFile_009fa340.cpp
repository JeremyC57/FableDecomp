// CTexture::LoadFromBitmapFile(const CWideString& filename, const C2DExtentsI* size, const CPixelFormat* format, unsigned long mipLevels, unsigned long usage, long pool, unsigned long filter, unsigned long mipFilter, CTextureFileInfo* info, long) -- release the old texture and load the file through D3DXCreateTextureFromFileExW (default size/format unless given), report the loaded size, mip count and format, and recompute the byte length
// Leaf: ?LoadFromBitmapFile@CTexture
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FA340, 256 bytes (+16 int3), `ret 28h` (ten arguments, the last unused).
// Callees: ConvertPathToFullPath(const CWideString&) @ 0x00999110 (`__fastcall`, CWideString by value through ecx),
//   IDirect3DTexture9::Release (COM slot 2), GFGetSystemManager @ 0x009A4EC0 (device = system +0x60 -> +0x58),
//   CWideString::operator const wchar_t* @ 0x0099AF00, D3DXCreateTextureFromFileExW @ 0x00BFEFD4 (static D3DX,
//   `__stdcall`, 14 arguments), CPixelFormat::Initialise(D3DFORMAT) @ 0x009E3830, CTexture::CalcByteLength @ 0x009F9EE0,
//   CWideString::~CWideString @ 0x0099B510. Globals: the 0x24-byte pixel-format table @ 0x0129BA40.
// Layout: PDB CTexture (PD3DTexture +0, ByteLength:28 / AllocationSource:4 at +4); D3DXIMAGE_INFO (Width, Height, Depth,
//   MipLevels, Format, ...) in the frame; the caller's info block {C2DExtentsI Size, MipLevels, CPixelFormat}.
// Shape: the full path is a named CWideString built in the dead filename slot; the inline Uninitialise (release, null,
//   AllocationSource = 0); width/height default to D3DX_DEFAULT (-1) and the format to D3DFMT_UNKNOWN unless given; the
//   device is a named local fetched after them (kept in esi, `this` spilled); the result is tested by two separate
//   `return false` statements (`jl ; je`), each destroying the path; the reported size is a C2DExtentsI temporary copy.
struct CWideString {
    void* PData;
    ~CWideString();                                         // 0x0099B510
    operator const unsigned short*() const;                 // 0x0099AF00
};
CWideString __fastcall ConvertPathToFullPath(const CWideString& path);   // 0x00999110
struct IDirect3DDevice9;
struct CRenderSystem {
    unsigned char      _pad00[0x58];
    IDirect3DDevice9*  m_pDevice;           // +0x58
};
struct CSystemManager {
    unsigned char  _pad00[0x60];
    CRenderSystem* m_pRender;               // +0x60
};
CSystemManager* __fastcall GFGetSystemManager();   // 0x009A4EC0
struct IDirect3DTexture9 {
    virtual long __stdcall QueryInterface(const void* iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1
    virtual unsigned long __stdcall Release();                               // slot 2
};
struct D3DXIMAGE_INFO {
    unsigned long Width;                    // +0x00
    unsigned long Height;                   // +0x04
    unsigned long Depth;                    // +0x08
    unsigned long MipLevels;                // +0x0c
    long          Format;                   // +0x10 D3DFORMAT
    long          ResourceType;             // +0x14
    long          ImageFileFormat;          // +0x18
};
extern "C" long __stdcall D3DXCreateTextureFromFileExW(IDirect3DDevice9* device, const unsigned short* srcFile,
    unsigned long width, unsigned long height, unsigned long mipLevels, unsigned long usage, long format, long pool,
    unsigned long filter, unsigned long mipFilter, unsigned long colorKey, D3DXIMAGE_INFO* srcInfo, void* palette,
    IDirect3DTexture9** texture);           // 0x00BFEFD4
struct SPixelFormatInfo { long d3dFormat; long rest[8]; };   // 0x24 bytes
extern SPixelFormatInfo g_pixelFormatInfo[];                 // 0x0129BA40
struct C2DExtentsI {
    long X, Y;
    C2DExtentsI() {}
    C2DExtentsI(long x, long y) : X(x), Y(y) {}
};
struct CPixelFormat {
    long FormatIndex;
    void Initialise(long d3dFormat);                         // 0x009E3830
};
struct CTextureFileInfo {
    C2DExtentsI  Size;                      // +0x00
    long         MipLevels;                 // +0x08
    CPixelFormat Format;                    // +0x0c
};
class CTexture {
public:
    IDirect3DTexture9* PD3DTexture;                // +0x00
    unsigned long      ByteLength : 28;            // +0x04
    unsigned long      AllocationSource : 4;       // +0x04
    void Uninitialise()                            // 0x009F9F70 (inlined here)
    {
        if (PD3DTexture) {
            PD3DTexture->Release();
            PD3DTexture = 0;
            AllocationSource = 0;
        }
    }
    void CalcByteLength();                         // 0x009F9EE0
    bool LoadFromBitmapFile(const CWideString& filename, const C2DExtentsI* size, const CPixelFormat* format,
                            unsigned long mipLevels, unsigned long usage, long pool, unsigned long filter,
                            unsigned long mipFilter, CTextureFileInfo* info, long);
};

bool CTexture::LoadFromBitmapFile(const CWideString& filename, const C2DExtentsI* size, const CPixelFormat* format,
                                  unsigned long mipLevels, unsigned long usage, long pool, unsigned long filter,
                                  unsigned long mipFilter, CTextureFileInfo* info, long)
{
    CWideString path = ConvertPathToFullPath(filename);
    Uninitialise();
    unsigned long width = (unsigned long)-1;
    unsigned long height = (unsigned long)-1;
    long d3dFormat = 0;
    if (size) {
        width = size->X;
        height = size->Y;
    }
    if (format)
        d3dFormat = g_pixelFormatInfo[format->FormatIndex].d3dFormat;
    IDirect3DDevice9* device = GFGetSystemManager()->m_pRender->m_pDevice;
    D3DXIMAGE_INFO imageInfo;
    long result = D3DXCreateTextureFromFileExW(device, path, width, height,
                                               mipLevels, usage, d3dFormat, pool, filter, mipFilter, 0, &imageInfo, 0,
                                               &PD3DTexture);
    if (result < 0)
        return false;
    if (result != 0)
        return false;
    if (info) {
        info->Size = C2DExtentsI(imageInfo.Width, imageInfo.Height);
        info->MipLevels = imageInfo.MipLevels;
        info->Format.Initialise(imageInfo.Format);
    }
    CalcByteLength();
    return true;
}
