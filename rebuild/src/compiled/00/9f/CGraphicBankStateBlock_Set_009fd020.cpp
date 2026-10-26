// CGraphicBankStateBlock::Set(const CGraphicBankStateBlock& other) -- copies the other block's texture-bank state field by field (ResourceIsSprite is assigned twice, as in the original listing)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009FD020, length 140 bytes (padded to 144).
// Callees: none.
// Shape: the two 28-byte CPixelFormats are trivially copied (`rep movsd` x7 each); the remaining members are separate
// assignments in source order -- the stray `ResourceIsSprite` copy between MaxTextureHeight and ResourceSelectionIndex is
// a second assignment of the same member, kept by VC7.1.
// Layout: engine/CGraphicBankStateBlock.h keeps the state opaque (`State[0x60]` at +4), so the CStateBlock members are
// modelled from Ego_r.pdb (BankFormats +0, CompressedBankFormats +0x1c, CompressionType +0x38 ... EnableSignedFormats +0x5f).
struct CPixelFormats {                             // 28 bytes
    long m_formats[7];
};
struct CStateBlock {
    CPixelFormats BankFormats;                     // +0x00
    CPixelFormats CompressedBankFormats;           // +0x1c
    long          CompressionType;                 // +0x38 (ECompressionType)
    long          CompressionQuality;              // +0x3c
    long          MinTextureWidth;                 // +0x40
    long          MinTextureHeight;                // +0x44
    long          MaxTextureWidth;                 // +0x48
    long          MaxTextureHeight;                // +0x4c
    long          ResourceSelectionIndex;          // +0x50
    long          BumpmapFactor;                   // +0x54
    bool          ResourceIsSprite;                // +0x58
    bool          ResourceIsMultiFile;             // +0x59
    bool          ResourceIsBumpMapWithAlpha;      // +0x5a
    bool          EnableDXT1Format;                // +0x5b
    bool          EnableSizeReduction;             // +0x5c
    bool          EnableCompression;               // +0x5d
    bool          EnableSurfaceSection;            // +0x5e
    bool          EnableSignedFormats;             // +0x5f
};
class CGraphicBankStateBlock { public:
    virtual void v00();
    CStateBlock State;                             // +0x04
    void Set(const CGraphicBankStateBlock& other);
};

void CGraphicBankStateBlock::Set(const CGraphicBankStateBlock& other)
{
    State.BankFormats = other.State.BankFormats;
    State.CompressedBankFormats = other.State.CompressedBankFormats;
    State.CompressionType = other.State.CompressionType;
    State.CompressionQuality = other.State.CompressionQuality;
    State.MinTextureWidth = other.State.MinTextureWidth;
    State.MinTextureHeight = other.State.MinTextureHeight;
    State.MaxTextureWidth = other.State.MaxTextureWidth;
    State.MaxTextureHeight = other.State.MaxTextureHeight;
    State.ResourceIsSprite = other.State.ResourceIsSprite;
    State.ResourceSelectionIndex = other.State.ResourceSelectionIndex;
    State.BumpmapFactor = other.State.BumpmapFactor;
    State.ResourceIsSprite = other.State.ResourceIsSprite;
    State.ResourceIsMultiFile = other.State.ResourceIsMultiFile;
    State.ResourceIsBumpMapWithAlpha = other.State.ResourceIsBumpMapWithAlpha;
    State.EnableDXT1Format = other.State.EnableDXT1Format;
    State.EnableSizeReduction = other.State.EnableSizeReduction;
    State.EnableCompression = other.State.EnableCompression;
    State.EnableSurfaceSection = other.State.EnableSurfaceSection;
    State.EnableSignedFormats = other.State.EnableSignedFormats;
}