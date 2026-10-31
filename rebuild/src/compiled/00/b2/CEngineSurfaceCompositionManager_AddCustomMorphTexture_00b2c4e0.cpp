// CEngineSurfaceCompositionManager::AddCustomMorphTexture(CTexture& texture, const CCharString& name) -- reopen the custom texture bank for update, add an entry for `name` (with a default graphic state block) if it has none, copy the texture into a power-of-two compressed texture clamped to the composite size limits, generate its mips, write it to the bank entry, reopen the bank read-only with the engine's texture manager and return the entry index tagged as a composed handle (top bit)
// Leaf: AddCustomMorphTexture
// Grade: RELOCATION_MATCH -- moved onto the shared engine/CCountedPointer.h (was DIFFER 1009v1042 with the local
//   counted-pointer model)
// Layout: local model; retail offsets: CustomTextureBankFile +0x6a4 (CGraphicDataBank: CBankFile vptr +0, Size +4), CustomTextureBankFileName +0x9b8,
//         CompressedOpaquePixelFormat +0xfb0 / CompressedAlphaPixelFormat +0xfb4, MaxCompositeTextureSize +0x10c8, MinRenderTargetSize +0x10cc
//         (Ego_r.pdb names, retail offsets = PDB - 0x1c0 here); CGraphicBankStateBlock 0x64 bytes (vtable 0x0129C888, landed 0x009FEA20 model);
//         pixel-format info table 0x0129BA4C (0x24-byte rows, alpha bit count first).
#include <stddef.h>
void* __cdecl operator new(size_t size);
void __cdecl operator_delete_00bfe9bc(void* p);         // 0x00BFE9BC

#include "engine/CCountedPointer.h"



class CCharString { public:
    void* PStringData;
    CCharString(const char* s, int len = -1);           // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CWideString { public: void* PStringData; };

class C2DExtentsI { public:
    long X, Y;
};
struct CPixelFormatInfo {                               // 0x24-byte rows @ 0x0129BA4C
    unsigned long AlphaBits;
    unsigned long _rest[8];
};
extern CPixelFormatInfo g_PixelFormatInfo[];            // 0x0129BA4C
class CPixelFormat { public:
    long FormatIndex;
    CPixelFormat() : FormatIndex(-1) {}
    bool HasAlpha() const { return g_PixelFormatInfo[FormatIndex].AlphaBits > 0; }
};
struct CPixelFormatSet {
    CPixelFormat NonAlpha, Alpha, BooleanAlpha, InterpolatedAlpha, BumpMap, BumpMapAlpha, Signed;
};
struct __declspec(novtable) CBankStateBlock {
    virtual void v00();
    virtual void GetDefault(CBankStateBlock& block) = 0;
};
class CGraphicBankStateBlock : public CBankStateBlock { public:      // vtable 0x0129C888, 0x64 bytes
    CPixelFormatSet BankFormats;                        // +0x04
    CPixelFormatSet CompressedBankFormats;              // +0x20
    long            Settings[8];                        // +0x3c
    bool            Flags[8];                           // +0x5c
    CGraphicBankStateBlock() {}
    virtual void v00();
    virtual void GetDefault(CBankStateBlock& block);
};

struct IDirect3DSurface9;
class C2DBoxI;
class CSurface { public:                                       // vtable 0x0122F84C
    IDirect3DSurface9* PD3DSurface;
    unsigned long      AllocationSource;
    void*              PAllocatedMemory;
    virtual void v00();
    ~CSurface() { Uninitialise(); }
    void Uninitialise();                                // 0x009F2E20
    void CopyFromSurface(const C2DBoxI* destBox, const CSurface* source, const C2DBoxI* sourceBox, unsigned long flags);   // 0x009F37E0
};
class CTexture { public:
    void*         PD3DTexture;                          // +0x00
    unsigned long Bits;                                 // +0x04
    CTexture() : PD3DTexture(0), Bits(0) {}
    ~CTexture() { Uninitialise(); }
    void Uninitialise();                                // 0x009F9F70
    bool Initialise(const C2DExtentsI* size, long levels, const CPixelFormat* format, unsigned long usage, long pool, bool a, bool b);   // 0x009FA280
    CSurface GetSurface(long level);                    // 0x009F9E00
    C2DExtentsI GetDimensions(unsigned long level) const;   // 0x009F9DB0
    CPixelFormat GetPixelFormat() const;                // 0x009F9D80
    void GenerateMipmaps(unsigned long filter, unsigned long flags);   // 0x009F9E60
};

class CTextureManager;
class CGraphicDataBank { public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual bool OpenFileReadOnly(const CWideString& fileName, unsigned long flags);   // slot 3
    virtual void OpenFileReadWrite(const CWideString& fileName, unsigned long create, unsigned long flags);   // slot 4
    virtual void Close();                               // slot 5
    unsigned long Size;                                 // +0x04
    unsigned char _pad[0x278];
    CCountedPointer<CTextureManager> TextureManager;    // +0x280
    void BeginUpdateEntries();                          // 0x009CE8D0
    void EndUpdateEntries();                            // 0x009CEA70
    unsigned long FindEntry(const CCharString& symbol); // 0x009CCDC0
    void AddEntry(const CCharString& symbol, unsigned long type, const CCharString& filename, CCountedPointer<CBankStateBlock> state);   // 0x009CD620
    void SetTextureManager(const CCountedPointer<CTextureManager>& manager);   // 0x009FD870
    CCountedPointer<CTextureManager> GetPTextureManager() { return TextureManager; }
};

struct CEngineData_01436e98 {
    unsigned char     _pad[0x954];
    CGraphicDataBank* GraphicBank;                      // +0x954
};
extern CEngineData_01436e98* g_pEngine_01436e98;        // 0x01436E98

__forceinline long HighestSetBit(unsigned long value)
{
    if (value == 0)
        return -1;
    if (value >= 0x80000000)
        return 31;
    ++value;
    long r = ((long)((1 << 16) - value) >> 31) & 16;
    r += ((long)((1 << (r + 8)) - value) >> 31) & 8;
    r += ((long)((1 << (r + 4)) - value) >> 31) & 4;
    r += ((long)((1 << (r + 2)) - value) >> 31) & 2;
    r += ((long)((1 << (r + 1)) - value) >> 31) & 1;
    return r;
}
__forceinline unsigned long NextPowerOfTwo(unsigned long value)
{
    if (value == 0)
        return 0;
    return 1 << HighestSetBit(value * 2 - 1);
}

class CEngineSurfaceCompositionManager { public:
    unsigned char    _pad[0x6a4];
    CGraphicDataBank CustomTextureBankFile;             // +0x6a4
    unsigned char    _pad6a4[0x9b8 - 0x6a4 - sizeof(CGraphicDataBank)];
    CWideString      CustomTextureBankFileName;         // +0x9b8
    unsigned char    _pad9bc[0xfb0 - 0x9bc];
    CPixelFormat     CompressedOpaquePixelFormat;       // +0xfb0
    CPixelFormat     CompressedAlphaPixelFormat;        // +0xfb4
    unsigned char    _padfb8[0x10c8 - 0xfb8];
    unsigned long    MaxCompositeTextureSize;           // +0x10c8
    unsigned long    MinRenderTargetSize;               // +0x10cc
    void WriteTextureToBank(CTexture& texture, CGraphicDataBank& bank, unsigned long index, bool transparent);   // 0x00B2BD80
    unsigned long AddCustomMorphTexture(CTexture& texture, const CCharString& name);
};

unsigned long CEngineSurfaceCompositionManager::AddCustomMorphTexture(CTexture& texture, const CCharString& name)
{
    unsigned long create = 0;
    if (CustomTextureBankFile.Size == 0)
        create = 1;
    CustomTextureBankFile.Close();
    CustomTextureBankFile.OpenFileReadWrite(CustomTextureBankFileName, create, 0x1000);
    CustomTextureBankFile.BeginUpdateEntries();
    unsigned long index = CustomTextureBankFile.FindEntry(name);
    if (index == 0) {
        CCountedPointer<CBankStateBlock> state(new CGraphicBankStateBlock);
        state->GetDefault(*state);
        {
            CCharString filename("");
            CustomTextureBankFile.AddEntry(name, 0, filename, state);
        }
        index = CustomTextureBankFile.FindEntry(name);
    }
    unsigned long width = NextPowerOfTwo(texture.GetDimensions(0).X);
    unsigned long height = NextPowerOfTwo(texture.GetDimensions(0).Y);
    if (width < MinRenderTargetSize)
        width = MinRenderTargetSize;
    else if (width > MaxCompositeTextureSize)
        width = MaxCompositeTextureSize;
    if (height < MinRenderTargetSize)
        height = MinRenderTargetSize;
    else if (height > MaxCompositeTextureSize)
        height = MaxCompositeTextureSize;
    bool hasAlpha = texture.GetPixelFormat().HasAlpha();
    CPixelFormat format;
    if (hasAlpha)
        format = CompressedAlphaPixelFormat;
    else
        format = CompressedOpaquePixelFormat;
    CTexture composite;
    C2DExtentsI size;
    size.X = width;
    size.Y = height;
    composite.Initialise(&size, -1, &format, 0, 1, false, false);
    composite.GetSurface(0).CopyFromSurface(0, &texture.GetSurface(0), 0, (unsigned long)-1);
    composite.GenerateMipmaps(5, 0);
    WriteTextureToBank(composite, CustomTextureBankFile, index, hasAlpha);
    CustomTextureBankFile.EndUpdateEntries();
    CustomTextureBankFile.Close();
    CustomTextureBankFile.OpenFileReadOnly(CustomTextureBankFileName, 0x48);
    CustomTextureBankFile.SetTextureManager(g_pEngine_01436e98->GraphicBank->GetPTextureManager());
    return index | 0x80000000;
}