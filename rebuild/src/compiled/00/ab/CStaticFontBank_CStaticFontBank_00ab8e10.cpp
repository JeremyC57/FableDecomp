// CStaticFontBank::CStaticFontBank(CFontDataBank* bank, unsigned long index) -- read a static font from its bank entry: name, height, weight, italics, max height, texture size, character range, the glyph banks (each glyph read by CGlyph::Read), then the Targa glyph image, copied into a new compressed-alpha texture
// Leaf: ??0CStaticFontBank@@QAE@PAVCFontDataBank@@K@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AB8E10, 2197 bytes, thiscall `ret 8`, /GS cookie (0x0139C8A8, __security_check_cookie 0x00BFE9F9).
// Identity: Ego_r.pdb / FableWin.pdb ??0CStaticFontBank@@QAE@PAVCFontDataBank@@K@Z; the vtable stores 0x0129AB38 (CFontBank) then
//   0x0129EAD4 (CStaticFontBank, RTTI .?AVCStaticFontBank@@) after ??0CBaseClassNonCopyable@@IAE@XZ 0x0099A2F0 and CCharString()
//   0x0099E4B0 on Name.
// Callees: CBankFile::GetEntryDataSize 0x009CC240 / ReadEntryData 0x009CC2A0, malloc / free 0x00BFEA0E / 0x00BFEA14 (the two
//   CArray<unsigned char> buffers), CMemoryDataInputStream(const void*, unsigned long) 0x00994930,
//   CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (the inline reads' slow path), ReadNullTerminatedString 0x00995D30
//   (devirtualised on the local stream), CCharString::operator= 0x0099EFB0 / ~CCharString 0x0099EAE0, operator new[] 0x00BFEB22
//   (the CGlyph arrays, 0x18 bytes each, no cookie), CGlyph::Read 0x00AB96B0 (?Read@CGlyph@@QAEXAAVCDataInputStream@@@Z),
//   GFGetSystemManager 0x009A4EC0 (twice: the first result is unused), CPixelFormat::Initialise 0x009E3790 (8,8,8,8,false),
//   CDisplayManager::GetBestCompressedAlphaTextureFormat 0x009BE870, operator new 0x00BFEA1A (the 8-byte CTexture, inline sized
//   constructor -> CTexture::Initialise 0x009FA280), CTexture::GetSurfaceLevel 0x009F9E00, CSurface::CopyFromMemory 0x009F4390
//   (?CopyFromMemory@CSurface@@QAEXPBVC2DBoxI@@PBX0ABVCPixelFormat@@JK@Z), CSurface::Uninitialise 0x009F2E20 (inline ~CSurface),
//   ~CDataStream 0x00993B10.
// Shape: every scalar is an inline bounds-checked 4-byte (Italics: 1-byte, ReadEBOOL) read; the bank loop counts down; the image
//   pointer is taken before the `size - 18 > 0` test and again for the copy; the destination box is a temporary built in the
//   call (its stores sink to just before GetSurfaceLevel), the source box a named local stored early.
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER_TEXTUREFORMATS
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_SIZED
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CDATASTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_BASE
#define FABLE_RETAIL_CSTATICFONTBANK
#define FABLE_RETAIL_CSTATICFONTBANK_CTOR
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CPixelFormat.h"
#include "engine/CTexture.h"
#include "engine/CSurface.h"
#include "engine/C2DBoxI.h"
#include "engine/CArray.h"
#include "engine/CMemoryDataInputStream.h"
#include "engine/CStaticFontBank.h"
void* __cdecl operator new[](unsigned int bytes);             // 0x00BFEB22 (MSVCR71 import thunk): the CGlyph arrays
extern "C" void* __cdecl memset(void* dest, int value, unsigned int bytes);
#pragma intrinsic(memset)

// engine/CBankFile.h is on HOLD and declares neither entry accessor: Ego_r.pdb ?GetEntryDataSize@CBankFile@@QBEKK@Z (0x009CC240)
// and ?ReadEntryData@CBankFile@@QAEXKKPAXK@Z (0x009CC2A0), the landed rows' spellings.
class CBankFile {
public:
    unsigned long GetEntryDataSize(unsigned long index) const;                                   // 0x009CC240
    void ReadEntryData(unsigned long index, unsigned long offset, void* buffer, unsigned long size);   // 0x009CC2A0
};
class CFontDataBank : public CBankFile { };

// Ego_r.pdb TargaHeader (18 bytes).
#pragma pack(push, 1)
struct TargaHeader {
    unsigned char  IDLength;
    unsigned char  ColormapType;
    unsigned char  ImageType;
    unsigned char  ColormapSpecification[5];
    unsigned short XOrigin;
    unsigned short YOrigin;
    unsigned short ImageWidth;
    unsigned short ImageHeight;
    unsigned char  PixelDepth;
    unsigned char  ImageDescriptor;
};
#pragma pack(pop)

CStaticFontBank::CStaticFontBank(CFontDataBank* bank, unsigned long index)
{
    memset(GlyphBanks, 0, sizeof(GlyphBanks));
    unsigned long size = bank->GetEntryDataSize(index);
    CArray<unsigned char> data(size);
    bank->ReadEntryData(index, 0, data.GetAsCArray(), size);
    CMemoryDataInputStream stream(data.GetAsCArray(), size);
    Name = stream.ReadNullTerminatedString();
    FontHeight = stream.ReadULONG();
    Weight = stream.ReadSLONG();
    Italics = stream.ReadEBOOL();
    MaxHeight = stream.ReadULONG();
    TextureSize.X = stream.ReadSLONG();
    TextureSize.Y = stream.ReadSLONG();
    MinChar = stream.ReadSLONG();
    MaxChar = stream.ReadSLONG();
    unsigned long noBanks = stream.ReadULONG();
    for (unsigned long i = 0; i < noBanks; ++i)
    {
        GlyphBank& glyphBank = GlyphBanks[stream.ReadULONG()];
        glyphBank.FirstGlyph = (unsigned short)stream.ReadULONG();
        glyphBank.NoGlyphs = (unsigned short)stream.ReadULONG();
        glyphBank.Glyphs = new CGlyph[glyphBank.NoGlyphs];
        for (unsigned long j = 0; j < glyphBank.NoGlyphs; ++j)
            glyphBank.Glyphs[j].Read(stream);
    }
    unsigned long imageSize = stream.ReadULONG();
    TargaHeader header;
    stream.Read(&header, sizeof(header));
    CArray<unsigned char> image(imageSize);
    unsigned char* pixels = image.GetAsCArray();
    long bytes = imageSize - sizeof(TargaHeader);
    if (bytes > 0)
        stream.Read(pixels, bytes);
    CDisplayManager* displayManager = GFGetSystemManager()->GetDisplayManager();
    CPixelFormat sourceFormat(8, 8, 8, 8);
    CPixelFormat textureFormat;
    GFGetSystemManager()->GetDisplayManager()->GetBestCompressedAlphaTextureFormat(16, textureFormat);
    PTexture = new CTexture(TextureSize, 1, textureFormat, 0, SURFACE_POOL_MANAGED);
    C2DBoxI sourceRect(0, 0, TextureSize.X, TextureSize.Y);
    PTexture->GetSurfaceLevel(0).CopyFromMemory(&C2DBoxI(0, 0, TextureSize.X, TextureSize.Y), image.GetAsCArray(), &sourceRect, sourceFormat, TextureSize.X * 4, 0xFFFFFFFF);
    (void)displayManager;
}