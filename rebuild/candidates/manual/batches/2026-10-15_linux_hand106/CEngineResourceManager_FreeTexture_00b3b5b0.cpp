// CEngineResourceManager::FreeTexture(CTexture* texture) -- deletes a texture made by AllocateTexture (the inline destructor runs CTexture::Uninitialise); `this` is unused
// Leaf: ?FreeTexture@CEngineResourceManager
// Retail: Fable.exe @ 0x00B3B5B0, 29 bytes (+ int3 padding).
// Callees: CTexture::Uninitialise @ 0x009F9F70, operator delete @ 0x00BFE9BC.
// Label: the manifest's autonamed "CTexture::Destroy" is the target of CEngine::FreeTextureFromEngineResourcePools
// (0x00B23F50: `mov ecx,[0x01436E98] ; jmp 0x00B3B5B0`) -- a thiscall member of the engine resource record taking the
// texture (`ret 4`), named after that forwarder.
// Shape: `delete texture` of a non-polymorphic class whose destructor is inline: null test, Uninitialise, operator delete.
struct IDirect3DTexture9;

class CTexture {
public:
    IDirect3DTexture9* PD3DTexture;                // +0x00
    unsigned long      ByteLengthAndSource;        // +0x04 ByteLength:28 | AllocationSource:4
    ~CTexture() { Uninitialise(); }
    void Uninitialise();                           // 0x009F9F70
};

struct CEngineResourceManager {
    void FreeTexture(CTexture* texture);
};

void CEngineResourceManager::FreeTexture(CTexture* texture)
{
    delete texture;
}
