// CTextureRenderer::DoRenderSample(IMediaSample* sample) -- under the video system's lock, copy the sample's 24-bit frame into the locked render texture (A8R8G8B8: byte copy with opaque alpha; A1R5G5B5: packed with the top bit set), unlock, publish the texture and signal the render event
// Leaf: DoRenderSample
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A3B730, length 481 bytes (`ret 4`, returns S_OK). DirectShow
//   CBaseVideoRenderer override; the body is the DirectX SDK Texture3D9 sample's DoRenderSample.
// Callees: EnterCriticalSection / LeaveCriticalSection / SetEvent (IAT 0x0143FE0C / 0x0143FE10 / 0x0143FDE0); IMediaSample::GetPointer
//   (slot 3, __stdcall); CTexture::Lock 0x009FA450 (by-value locked-rect record, pitch +8, bits +0xc) and Unlock 0x009F9DE0.
// Layout: CTextureRenderer (Ego_r.pdb names past the 0x160-byte renderer base) PVideoSys +0x160, Texture +0x164, TextureFormat +0x168,
//   Width +0x16c, Height +0x170, Pitch +0x174; CVideoSys CriticalSection +0x64, RenderEvent +0x7c, PRenderTexture +0x80 (PDB +0x6c..+0x88,
//   retail 8 bytes lower: the load sites here). Local model: no engine header for either class.
// Shape: the texture bits are read before the pitch (the pitch load lands after the first format compare); the loops are the SDK's
//   saved-row-pointer form.
struct CRITICAL_SECTION_18 { unsigned char Data[0x18]; };
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(CRITICAL_SECTION_18* cs);   // IAT 0x0143FE0C
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(CRITICAL_SECTION_18* cs);   // IAT 0x0143FE10
extern "C" __declspec(dllimport) int __stdcall SetEvent(void* event);                            // IAT 0x0143FDE0
struct IMediaSample {
    virtual long __stdcall QueryInterface(const void* iid, void** object);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
    virtual long __stdcall GetPointer(unsigned char** buffer);   // slot 3 (+0xc)
};
struct CLockedRect {
    long Width;
    long Height;
    long Pitch;                                             // +0x08
    unsigned char* Bits;                                    // +0x0c
};
struct CTexture {
    CLockedRect Lock(long level, unsigned long flags);      // 0x009FA450
    void Unlock(long level);                                // 0x009F9DE0
};
struct CVideoSys {
    unsigned char _pad00[0x64];
    CRITICAL_SECTION_18 CriticalSection;                    // +0x64
    void* RenderEvent;                                      // +0x7c
    CTexture* PRenderTexture;                               // +0x80
};
enum { D3DFMT_A8R8G8B8 = 21, D3DFMT_A1R5G5B5 = 25 };
struct CTextureRenderer {
    virtual long DoRenderSample(IMediaSample* sample);
    unsigned char _pad04[0x15c];
    CVideoSys* PVideoSys;                                   // +0x160
    CTexture* Texture;                                      // +0x164
    long TextureFormat;                                     // +0x168
    int Width;                                              // +0x16c
    int Height;                                             // +0x170
    int Pitch;                                              // +0x174
};

long CTextureRenderer::DoRenderSample(IMediaSample* sample)
{
    unsigned char* bmpBuffer;
    EnterCriticalSection(&PVideoSys->CriticalSection);
    sample->GetPointer(&bmpBuffer);
    CLockedRect locked = Texture->Lock(0, 0);
    unsigned char* txtBuffer = locked.Bits;
    long txtPitch = locked.Pitch;
    if (TextureFormat == D3DFMT_A8R8G8B8) {
        for (int y = 0; y < Height; y++) {
            unsigned char* bmpBufferOld = bmpBuffer;
            unsigned char* txtBufferOld = txtBuffer;
            for (int x = 0; x < Width; x++) {
                txtBuffer[0] = bmpBuffer[0];
                txtBuffer[1] = bmpBuffer[1];
                txtBuffer[2] = bmpBuffer[2];
                txtBuffer[3] = 0xff;
                txtBuffer += 4;
                bmpBuffer += 3;
            }
            bmpBuffer = bmpBufferOld + Pitch;
            txtBuffer = txtBufferOld + txtPitch;
        }
    }
    if (TextureFormat == D3DFMT_A1R5G5B5) {
        for (int y = 0; y < Height; y++) {
            unsigned char* bmpBufferOld = bmpBuffer;
            unsigned char* txtBufferOld = txtBuffer;
            for (int x = 0; x < Width; x++) {
                *(unsigned short*)txtBuffer = (unsigned short)
                    (0x8000 +
                    ((bmpBuffer[2] & 0xF8) << 7) +
                    ((bmpBuffer[1] & 0xF8) << 2) +
                    (bmpBuffer[0] >> 3));
                txtBuffer += 2;
                bmpBuffer += 3;
            }
            bmpBuffer = bmpBufferOld + Pitch;
            txtBuffer = txtBufferOld + txtPitch;
        }
    }
    Texture->Unlock(0);
    PVideoSys->PRenderTexture = Texture;
    SetEvent(PVideoSys->RenderEvent);
    LeaveCriticalSection(&PVideoSys->CriticalSection);
    return 0;
}
