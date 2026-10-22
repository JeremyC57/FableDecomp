// Behaviour test for 0x00A3B730: the reconstructed body (pragmas stripped) driven through recording stubs.
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __stdcall
#define __cdecl
#define __fastcall
#define __forceinline inline
#endif
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
#define EnterCriticalSection Test_EnterCriticalSection
#define LeaveCriticalSection Test_LeaveCriticalSection
#define SetEvent Test_SetEvent
struct CRITICAL_SECTION_18 { unsigned char Data[0x18]; };
extern "C" void __stdcall EnterCriticalSection(CRITICAL_SECTION_18* cs);   // IAT 0x0143FE0C
extern "C" void __stdcall LeaveCriticalSection(CRITICAL_SECTION_18* cs);   // IAT 0x0143FE10
extern "C" int __stdcall SetEvent(void* event);                            // IAT 0x0143FDE0
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

static int g_enter = 0, g_leave = 0, g_events = 0, g_unlocks = 0;
extern "C" void __stdcall Test_EnterCriticalSection(CRITICAL_SECTION_18* cs) { ++g_enter; }
extern "C" void __stdcall Test_LeaveCriticalSection(CRITICAL_SECTION_18* cs) { if (g_enter > g_leave) ++g_leave; }
extern "C" int __stdcall Test_SetEvent(void* event) { if (event == (void*)0x1234) ++g_events; return 1; }
static unsigned char g_frame[2 * 8];     // 2 rows, pitch 8 (2 pixels of 3 bytes + padding)
static unsigned char g_tex[2 * 16];      // 2 rows, pitch 16
long __stdcall IMediaSample::QueryInterface(const void*, void**) { return 0; }
unsigned long __stdcall IMediaSample::AddRef() { return 1; }
unsigned long __stdcall IMediaSample::Release() { return 0; }
long __stdcall IMediaSample::GetPointer(unsigned char** buffer) { *buffer = g_frame; return 0; }
CLockedRect CTexture::Lock(long level, unsigned long flags) { CLockedRect r; r.Width = 2; r.Height = 2; r.Pitch = 16; r.Bits = g_tex; return r; }
void CTexture::Unlock(long level) { ++g_unlocks; }

static int g_fail = 0;
static void Check(bool c, const char* what) { if (!c) { printf("FAIL %s\n", what); g_fail = 1; } }
int main()
{
    for (int i = 0; i < 16; ++i) g_frame[i] = (unsigned char)(0x10 + i * 9);
    CVideoSys video;
    memset(&video, 0, sizeof(video));
    video.RenderEvent = (void*)0x1234;
    CTexture texture;
    IMediaSample sample;
    CTextureRenderer renderer;
    renderer.PVideoSys = &video;
    renderer.Texture = &texture;
    renderer.Width = 2; renderer.Height = 2; renderer.Pitch = 8;

    renderer.TextureFormat = D3DFMT_A8R8G8B8;
    memset(g_tex, 0, sizeof(g_tex));
    Check(renderer.DoRenderSample(&sample) == 0, "S_OK");
    Check(g_tex[0] == g_frame[0] && g_tex[1] == g_frame[1] && g_tex[2] == g_frame[2] && g_tex[3] == 0xff, "first pixel");
    Check(g_tex[4] == g_frame[3] && g_tex[6] == g_frame[5] && g_tex[7] == 0xff, "second pixel");
    Check(g_tex[16] == g_frame[8] && g_tex[20] == g_frame[11] && g_tex[8] == 0, "second row honours both pitches");
    Check(video.PRenderTexture == &texture && g_events == 1 && g_unlocks == 1 && g_enter == 1 && g_leave == 1, "publish and unlock");

    renderer.TextureFormat = D3DFMT_A1R5G5B5;
    memset(g_tex, 0, sizeof(g_tex));
    renderer.DoRenderSample(&sample);
    unsigned short expect0 = (unsigned short)(0x8000 + ((g_frame[2] & 0xF8) << 7) + ((g_frame[1] & 0xF8) << 2) + (g_frame[0] >> 3));
    unsigned short expect3 = (unsigned short)(0x8000 + ((g_frame[13] & 0xF8) << 7) + ((g_frame[12] & 0xF8) << 2) + (g_frame[11] >> 3));
    unsigned short got0, got3;
    memcpy(&got0, g_tex, 2); memcpy(&got3, g_tex + 18, 2);
    Check(got0 == expect0, "packed first pixel");
    Check(got3 == expect3, "packed last pixel");

    renderer.TextureFormat = 3;
    memset(g_tex, 0, sizeof(g_tex));
    renderer.DoRenderSample(&sample);
    Check(g_tex[0] == 0 && g_events == 3 && g_leave == 3, "other formats copy nothing but still publish");
    if (!g_fail) printf("00a3b730_TEST PASS\n");
    return g_fail;
}