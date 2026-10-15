// CEngineResourceManager::AllocateTexture(unsigned long id, const C2DExtentsI& size, long levels, const CPixelFormat& format) -- allocates a zeroed CTexture with new and initialises it (no usage flags, managed pool, no dynamic/render-target flags); `this` and `id` are unused
// Leaf: ?AllocateTexture@CEngineResourceManager
// Retail: Fable.exe @ 0x00B3B1F0, 70 bytes (+ int3 padding).
// Callees: operator new @ 0x00BFEA1A, CTexture::Initialise(const C2DExtentsI&, long, const CPixelFormat&, ulong, ESurfacePool, bool, bool) @ 0x009FA280.
// Label: the manifest's autonamed "CTexture::Create" is the target of CEngine::AllocateTextureFromEngineResourcePools
// (0x00B23F40: `mov ecx,[0x01436E98] ; jmp 0x00B3B1F0`), i.e. a thiscall member of the engine resource record at
// 0x01436E98 taking the same (ulong, const C2DExtentsI&, long, const CPixelFormat&) arguments (`ret 10h`) -- named
// after that forwarder; the counterpart is FreeTexture @ 0x00B3B5B0.
// Shape: `new CTexture` with an inline constructor zeroing both words (null test, `mov esi,eax` / `xor esi,esi`), then
// the seven Initialise arguments pushed right to left with the three references passed as their addresses.
// Layout: PDB CTexture (8 bytes: PD3DTexture +0, the ByteLength:28/AllocationSource:4 word +4); engine/CTexture.h
// cannot carry the constructor, so it is modelled locally (the bitfield word as one dword the constructor clears).
struct IDirect3DTexture9;
struct C2DExtentsI;
struct CPixelFormat;
enum ESurfacePool { SURFACE_POOL_DEFAULT = 0, SURFACE_POOL_MANAGED = 1 };

class CTexture {
public:
    IDirect3DTexture9* PD3DTexture;                // +0x00
    unsigned long      ByteLengthAndSource;        // +0x04 ByteLength:28 | AllocationSource:4
    CTexture() : PD3DTexture(0), ByteLengthAndSource(0) { }
    bool Initialise(const C2DExtentsI& size, long levels, const CPixelFormat& format, unsigned long usage,
                    ESurfacePool pool, bool dynamic, bool renderTarget);   // 0x009FA280
};

struct CEngineResourceManager {
    CTexture* AllocateTexture(unsigned long id, const C2DExtentsI& size, long levels, const CPixelFormat& format);
};

CTexture* CEngineResourceManager::AllocateTexture(unsigned long id, const C2DExtentsI& size, long levels, const CPixelFormat& format)
{
    CTexture* texture = new CTexture;
    texture->Initialise(size, levels, format, 0, SURFACE_POOL_MANAGED, false, false);
    return texture;
}