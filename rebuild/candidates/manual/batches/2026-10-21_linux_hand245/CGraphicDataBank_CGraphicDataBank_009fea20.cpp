// CGraphicDataBank::CGraphicDataBank() -- construct the async bank-file and resource-bank bases, empty graphic lists, the default-format state block, unset pixel formats, null blank textures, and create the bank's texture manager
// Leaf: ??0CGraphicDataBank@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FEA20, length 337 bytes (`ret`).
// Callees: CBankFileAsync::CBankFileAsync @ 0x009D5F80, CResourceBank::CResourceBank @ 0x009FC5F0, operator new @ 0x00BFEA1A,
//   CTextureManager::CTextureManager @ 0x00A6A360, CCountedPointer<CTextureManager>::operator=(CTextureManager*) @ 0x00A002E0.
// Vtables: CGraphicDataBank 0x0129C8CC (primary) / 0x0129C938 (CResourceBank part, +0x164), CGraphicBankStateBlock 0x0129C888.
// Layout: Ego_r.pdb members at the retail offsets (the landed ~CGraphicDataBank 0x009FEBC0 model: CBankFileAsync 0x164 bytes,
//   CResourceBank 0x7C, 12-byte vectors): GraphicList +0x1E0, GraphicFrameBuffer +0x1EC, Initialised +0x1F8, StateBlock +0x1FC
//   (vptr + 14 pixel formats + ...), TextureSizeWarningMaxWidth +0x260, TextureManager +0x280, PixelFormats +0x288,
//   BlankTextures +0x2B4.
// Shape: CPixelFormat's default constructor stores -1 (the state block's 14 named formats and the 11-entry array are
//   written out one by one), CTexture's zeroes both words (a counted loop over the 11 textures); the state block's base
//   is __declspec(novtable) (no base vptr store).
struct CTextureManager {
    CTextureManager();                          // 0x00A6A360
    unsigned char _body[0x5d4];
};
struct CPixelFormat {
    long FormatIndex;
    CPixelFormat() : FormatIndex(-1) {}
};
struct CTexture {
    void*         PD3DTexture;                  // +0x00
    unsigned long Bits;                         // +0x04 ByteLength:28, AllocationSource:4
    CTexture() : PD3DTexture(0), Bits(0) {}
};
template<class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) {}
};
struct CPointerInfo;
struct CTextureManagerPointer {                 // CCountedPointer<CTextureManager>
    CTextureManager* Data;
    CPointerInfo*    Info;
    CTextureManagerPointer() : Data(0), Info(0) {}
    CTextureManagerPointer& operator=(CTextureManager* object);   // 0x00A002E0
};
struct CPixelFormatSet {                        // the seven per-use formats of a bank state
    CPixelFormat NonAlpha, Alpha, BooleanAlpha, InterpolatedAlpha, BumpMap, BumpMapAlpha, Signed;
};
struct __declspec(novtable) CBankStateBlock {
    virtual void GetDefault(CBankStateBlock& block) = 0;
};
struct CGraphicBankStateBlock : CBankStateBlock {   // vtable 0x0129C888, 0x64 bytes
    CPixelFormatSet BankFormats;                // +0x04
    CPixelFormatSet CompressedBankFormats;      // +0x20
    long         Settings[8];                   // +0x3C CompressionType .. BumpmapFactor
    bool         Flags[8];                      // +0x5C ResourceIsSprite .. EnableSignedFormats
    CGraphicBankStateBlock() {}
    virtual void GetDefault(CBankStateBlock& block);
};
struct CBankFileAsync {
    CBankFileAsync();                           // 0x009D5F80
    virtual ~CBankFileAsync();
    unsigned char _body[0x160];
};
struct CResourceBank {
    CResourceBank();                            // 0x009FC5F0
    virtual ~CResourceBank();
    unsigned char _body[0x78];
};
struct CGraphicInfo;
struct CGraphicInfoFrame;
struct CGraphicDataBank : CBankFileAsync, CResourceBank {
    CRetailVector<CGraphicInfo>      GraphicList;            // +0x1E0
    CRetailVector<CGraphicInfoFrame> GraphicFrameBuffer;     // +0x1EC
    bool                   Initialised;                      // +0x1F8
    CGraphicBankStateBlock StateBlock;                       // +0x1FC
    long                   TextureSizeWarningMaxWidth;       // +0x260
    long                   TextureSizeWarningMaxHeight;      // +0x264
    unsigned long          UnloadFrameDelay;                 // +0x268
    unsigned long          ReduceSizeFrameDelay;             // +0x26C
    unsigned long          TexturePointersValidForFrames;    // +0x270
    long                   MinAvailablePreloadMemory;        // +0x274
    long                   AvailableMemory;                  // +0x278
    bool                   FreezeGraphics;                   // +0x27C
    CTextureManagerPointer TextureManager;                   // +0x280
    CPixelFormat           PixelFormats[11];                 // +0x288
    CTexture               BlankTextures[11];                // +0x2B4
    CGraphicDataBank();
    virtual ~CGraphicDataBank();
};

CGraphicDataBank::CGraphicDataBank() : Initialised(false)
{
    TextureManager = new CTextureManager;
}
