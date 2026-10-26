// CDisplayManager::CopyBackBufferToTexture()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009BF5E0, oracle length 18 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; lea eax,[ecx+2Ch] ; lea esi,[ecx+4Ch] ; push eax ; push esi ; call CDisplayManager::BlitRenderTargetSurfaceToTexture ; mov eax,esi ; pop esi ; ret
// Blits the back-buffer surface (+0x2C) into the capture texture (+0x4C) and returns that texture.
// The callee @ 0x009BF350 is __fastcall(CTexture*, CSurface*).
class CSurface;
class CTexture;

struct CDisplayManager_Retail {
    unsigned char _pad_0x00[0x2C];
    unsigned char BackBufferSurface[0x20];  // +0x2C CSurface (name from use)
    unsigned char BackBufferTexture[0x04];  // +0x4C CTexture (name from use)
    void BlitRenderTargetSurfaceToTexture(CTexture* texture, CSurface* surface);   // 0x009BF350
    CTexture* CopyBackBufferToTexture();
};

CTexture* CDisplayManager_Retail::CopyBackBufferToTexture()
{
    CTexture* texture = reinterpret_cast<CTexture*>(&this->BackBufferTexture[0]);
    BlitRenderTargetSurfaceToTexture(texture, reinterpret_cast<CSurface*>(&this->BackBufferSurface[0]));
    return texture;
}