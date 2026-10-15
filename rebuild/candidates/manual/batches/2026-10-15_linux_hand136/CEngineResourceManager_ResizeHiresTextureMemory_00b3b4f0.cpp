// CEngineResourceManager::ResizeHiresTextureMemory(long sizeKB) -- flushes the graphic bank and the surface-composition cache, then resizes the main bank's texture pool to as many 512x512 DXT1 textures as fit in the given kilobytes (bank level forced to 0 meanwhile); returns the pool's result
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B3B4F0, length 136 bytes.
// Signature from Ego_r.pdb: ?ResizeHiresTextureMemory@CEngineResourceManager@@QAE_NJ@Z.
// Callees: 0x009FD0C0 CGraphicDataBank::ChangeScene() (bank at +0x954), 0x00B2AD80 the surface-composition manager's
// flush (global @ 0x01436EAC; the manifest's _Cons_val label is propagated), 0x009E3830 CPixelFormat::Initialise(
// D3DFORMAT) with D3DFMT_DXT1 ('DXT1' = 0x31545844), 0x00A698F0 CalculateTextureSize(unsigned long, unsigned long,
// bool, const CPixelFormat&) (`__fastcall`), 0x009FD100 the main bank's level setter (the landed FrameEnd 0x00B3B870
// calls it with 3; the manifest label is propagated), 0x00A6ABE0 the texture manager's pool resize by format
// (bank +0x280: texture size, texture count) -> bool.
// Layout: the retail resource manager keeps its two graphic banks at +0x954 / +0x958 (the PDB's GraphicBanks[1] /
// MainGraphicBank sit at +0xa88 / +0xa8c in engine/CEngineResourceManager.h, whose mesh-bank members are wider than
// retail's), so `this` is modelled locally with the retail offsets; the bank's level word is at +0x26c and its
// texture manager at +0x280.
// Shape: the pixel format is the single `push ecx` frame word; the main bank's level is read into a local (ebp)
// before the level is forced to 0; the count is a signed division of the byte size by the texture size; the
// bool result rides in bl across the final setter call.

enum D3DFORMAT { D3DFMT_DXT1 = 0x31545844 };
class CPixelFormat {
public:
    long m_format;
    void Initialise(D3DFORMAT format);                   // 0x009E3830
};
long __fastcall CalculateTextureSize(unsigned long width, unsigned long height, bool mipmaps,
                                     const CPixelFormat& format);   // 0x00A698F0
class CTextureManager {
public:
    bool ResizePoolByFormat(long textureSize, long textureCount);   // 0x00A6ABE0
};
class CGraphicDataBank {
public:
    unsigned char    _pad00[0x26c];
    long             m_level;                            // +0x26c
    unsigned char    _pad270[0x10];
    CTextureManager* m_pTextureManager;                  // +0x280
    void ChangeScene();                                  // 0x009FD0C0
    void SetLevel(long level);                           // 0x009FD100
};
class CEngineSurfaceCompositionManager {
public:
    void FlushCache();                                   // 0x00B2AD80
};
extern CEngineSurfaceCompositionManager* g_pSurfaceComposition_01436eac;   // 0x01436EAC

class CEngineResourceManager {
public:
    unsigned char     _pad00[0x954];
    CGraphicDataBank* m_pGraphicBank;                    // +0x954
    CGraphicDataBank* m_pMainGraphicBank;                // +0x958
    bool ResizeHiresTextureMemory(long sizeKB);
};

bool CEngineResourceManager::ResizeHiresTextureMemory(long sizeKB)
{
    m_pGraphicBank->ChangeScene();
    g_pSurfaceComposition_01436eac->FlushCache();
    CPixelFormat format;
    format.Initialise(D3DFMT_DXT1);
    long textureSize = CalculateTextureSize(0x200, 0x200, true, format);
    long level = m_pMainGraphicBank->m_level;
    m_pMainGraphicBank->SetLevel(0);
    bool result = m_pMainGraphicBank->m_pTextureManager->ResizePoolByFormat(textureSize, (sizeKB << 10) / textureSize);
    m_pMainGraphicBank->SetLevel(level);
    return result;
}
