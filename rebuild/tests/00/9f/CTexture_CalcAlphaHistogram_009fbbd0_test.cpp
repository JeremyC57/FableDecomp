#include <cstdio>
#include <cstdlib>
#ifndef _MSC_VER
#define __stdcall
#endif
struct CRGBColour { unsigned char B, G, R, A; };
struct D3DSURFACE_DESC {
    long          Format;                        // +0x00 D3DFORMAT
    long          Type;                          // +0x04
    unsigned long Usage;                         // +0x08
    long          Pool;                          // +0x0c
    long          MultiSampleType;               // +0x10
    unsigned long MultiSampleQuality;            // +0x14
    unsigned long Width;                         // +0x18
    unsigned long Height;                        // +0x1c
};
struct D3DLOCKED_RECT {
    long  Pitch;
    void* pBits;
};
struct RECT { long left, top, right, bottom; };
struct IDirect3DTexture9 {                       // IUnknown + IDirect3DResource9 + IDirect3DBaseTexture9 + texture slots
    virtual long __stdcall QueryInterface(const void* iid, void** object);   // slot 0
    virtual unsigned long __stdcall AddRef();                                // slot 1
    virtual unsigned long __stdcall Release();                               // slot 2
    virtual long __stdcall GetDevice(void** device);                         // slot 3
    virtual long __stdcall SetPrivateData(const void* guid, const void* data, unsigned long size, unsigned long flags);
    virtual long __stdcall GetPrivateData(const void* guid, void* data, unsigned long* size);
    virtual long __stdcall FreePrivateData(const void* guid);
    virtual unsigned long __stdcall SetPriority(unsigned long priority);
    virtual unsigned long __stdcall GetPriority();
    virtual void __stdcall PreLoad();
    virtual long __stdcall GetType();
    virtual unsigned long __stdcall SetLOD(unsigned long lod);
    virtual unsigned long __stdcall GetLOD();
    virtual unsigned long __stdcall GetLevelCount();
    virtual long __stdcall SetAutoGenFilterType(long filter);
    virtual long __stdcall GetAutoGenFilterType();
    virtual void __stdcall GenerateMipSubLevels();                           // slot 16
    virtual long __stdcall GetLevelDesc(unsigned long level, D3DSURFACE_DESC* desc);   // slot 17 (+0x44)
    virtual long __stdcall GetSurfaceLevel(unsigned long level, void** surface);       // slot 18 (+0x48)
    virtual long __stdcall LockRect(unsigned long level, D3DLOCKED_RECT* locked, const RECT* rect, unsigned long flags);   // slot 19 (+0x4c)
    virtual long __stdcall UnlockRect(unsigned long level);                  // slot 20 (+0x50)
};
struct C2DExtentsI {
    long X, Y;
    C2DExtentsI(long x, long y) : X(x), Y(y) { }
};
struct CPixelFormat {
    long FormatIndex;
    CPixelFormat(long d3dFormat) { Initialise(d3dFormat); }
    void Initialise(long d3dFormat);                                         // 0x009E3830
    void ConvertPFDDataToRGB(unsigned char* data, CRGBColour* colour) const; // 0x009E3DF0
    unsigned long GetByteColourDepth() const;                                // 0x009E3800
};
template<class T> struct vector {               // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T& operator[](unsigned long i) { return *(_M_start + i); }
    void resize(unsigned int size, const T& value);                          // 0x004428F0
};
template<> void vector<unsigned long>::resize(unsigned int n, const unsigned long& v);   // test: declared before use
class CTexture {
public:
    IDirect3DTexture9* PD3DTexture;                // +0x00
    unsigned long      ByteLength : 28;            // +0x04
    unsigned long      AllocationSource : 4;       // +0x04
    CPixelFormat GetPixelFormat() const          // 0x009F9D80 (inlined here)
    {
        D3DSURFACE_DESC desc;
        PD3DTexture->GetLevelDesc(0, &desc);
        return CPixelFormat(desc.Format);
    }
    C2DExtentsI GetDimensions(unsigned long level) const   // 0x009F9DB0 (inlined here)
    {
        D3DSURFACE_DESC desc;
        PD3DTexture->GetLevelDesc(level, &desc);
        return C2DExtentsI(desc.Width, desc.Height);
    }
    void CalcAlphaHistogram(vector<unsigned long>& histogram) const;
};

void CTexture::CalcAlphaHistogram(vector<unsigned long>& histogram) const
{
    CPixelFormat format = GetPixelFormat();
    C2DExtentsI size = GetDimensions(0);
    D3DLOCKED_RECT locked;
    RECT rect;
    rect.top = 0;
    rect.left = 0;
    rect.right = size.X;
    rect.bottom = size.Y;
    long result = PD3DTexture->LockRect(0, &locked, &rect, 0);
    histogram.resize(256, 0);
    if (result >= 0) {
        unsigned char* texel = (unsigned char*)locked.pBits;
        for (long y = 0; y < size.Y; ++y) {
            for (long x = 0; x < size.X; ++x) {
                CRGBColour colour;
                format.ConvertPFDDataToRGB(texel, &colour);
                ++histogram[colour.A];
                texel += format.GetByteColourDepth();
            }
            texel = (unsigned char*)locked.pBits + locked.Pitch * y;
        }
        PD3DTexture->UnlockRect(0);
    }
}
// ---- test doubles ----
static unsigned char g_texels[4 * 4 * 4];       // 4x4 32-bit texels, alpha in byte 3, pitch 16
struct CTestTexture : IDirect3DTexture9 {
    int locks, unlocks; bool fail;
    virtual long __stdcall QueryInterface(const void*, void**) { return 0; }
    virtual unsigned long __stdcall AddRef() { return 1; }
    virtual unsigned long __stdcall Release() { return 0; }
    virtual long __stdcall GetDevice(void**) { return 0; }
    virtual long __stdcall SetPrivateData(const void*, const void*, unsigned long, unsigned long) { return 0; }
    virtual long __stdcall GetPrivateData(const void*, void*, unsigned long*) { return 0; }
    virtual long __stdcall FreePrivateData(const void*) { return 0; }
    virtual unsigned long __stdcall SetPriority(unsigned long) { return 0; }
    virtual unsigned long __stdcall GetPriority() { return 0; }
    virtual void __stdcall PreLoad() {}
    virtual long __stdcall GetType() { return 0; }
    virtual unsigned long __stdcall SetLOD(unsigned long) { return 0; }
    virtual unsigned long __stdcall GetLOD() { return 0; }
    virtual unsigned long __stdcall GetLevelCount() { return 1; }
    virtual long __stdcall SetAutoGenFilterType(long) { return 0; }
    virtual long __stdcall GetAutoGenFilterType() { return 0; }
    virtual void __stdcall GenerateMipSubLevels() {}
    virtual long __stdcall GetLevelDesc(unsigned long, D3DSURFACE_DESC* d) { d->Format = 21; d->Width = 4; d->Height = 3; return 0; }
    virtual long __stdcall GetSurfaceLevel(unsigned long, void**) { return 0; }
    virtual long __stdcall LockRect(unsigned long, D3DLOCKED_RECT* l, const RECT* r, unsigned long)
    { ++locks; if (fail) return -1; if (r->left || r->top || r->right != 4 || r->bottom != 3) return -1; l->Pitch = 16; l->pBits = g_texels; return 0; }
    virtual long __stdcall UnlockRect(unsigned long) { ++unlocks; return 0; }
};
long __stdcall IDirect3DTexture9::QueryInterface(const void* iid, void** object) { return 0; }
unsigned long __stdcall IDirect3DTexture9::AddRef() { return 0; }
unsigned long __stdcall IDirect3DTexture9::Release() { return 0; }
long __stdcall IDirect3DTexture9::GetDevice(void** device) { return 0; }
long __stdcall IDirect3DTexture9::SetPrivateData(const void* guid, const void* data, unsigned long size, unsigned long flags) { return 0; }
long __stdcall IDirect3DTexture9::GetPrivateData(const void* guid, void* data, unsigned long* size) { return 0; }
long __stdcall IDirect3DTexture9::FreePrivateData(const void* guid) { return 0; }
unsigned long __stdcall IDirect3DTexture9::SetPriority(unsigned long priority) { return 0; }
unsigned long __stdcall IDirect3DTexture9::GetPriority() { return 0; }
void __stdcall IDirect3DTexture9::PreLoad() {}
long __stdcall IDirect3DTexture9::GetType() { return 0; }
unsigned long __stdcall IDirect3DTexture9::SetLOD(unsigned long lod) { return 0; }
unsigned long __stdcall IDirect3DTexture9::GetLOD() { return 0; }
unsigned long __stdcall IDirect3DTexture9::GetLevelCount() { return 0; }
long __stdcall IDirect3DTexture9::SetAutoGenFilterType(long filter) { return 0; }
long __stdcall IDirect3DTexture9::GetAutoGenFilterType() { return 0; }
void __stdcall IDirect3DTexture9::GenerateMipSubLevels() {}
long __stdcall IDirect3DTexture9::GetLevelDesc(unsigned long level, D3DSURFACE_DESC* desc) { return 0; }
long __stdcall IDirect3DTexture9::GetSurfaceLevel(unsigned long level, void** surface) { return 0; }
long __stdcall IDirect3DTexture9::LockRect(unsigned long level, D3DLOCKED_RECT* locked, const RECT* rect, unsigned long flags) { return 0; }
long __stdcall IDirect3DTexture9::UnlockRect(unsigned long level) { return 0; }
static long g_format = 0;
void CPixelFormat::Initialise(long f) { FormatIndex = f; g_format = f; }
void CPixelFormat::ConvertPFDDataToRGB(unsigned char* d, CRGBColour* c) const { c->B = d[0]; c->G = d[1]; c->R = d[2]; c->A = d[3]; }
unsigned long CPixelFormat::GetByteColourDepth() const { return 4; }
template<> void vector<unsigned long>::resize(unsigned int n, const unsigned long& v)
{ _M_start = (unsigned long*)std::malloc(n * sizeof(unsigned long)); for (unsigned int i = 0; i < n; ++i) _M_start[i] = v; _M_finish = _M_start + n; _M_end_of_storage = _M_finish; }
int main()
{
    for (int i = 0; i < 16; ++i) { g_texels[i * 4 + 3] = (unsigned char)(i < 4 ? 10 : i < 8 ? 20 : 30); }
    CTestTexture d3d; d3d.locks = d3d.unlocks = 0; d3d.fail = false;
    CTexture t; t.PD3DTexture = &d3d;
    vector<unsigned long> h; h._M_start = h._M_finish = h._M_end_of_storage = 0;
    t.CalcAlphaHistogram(h);
    // rows: row 0 from pBits, row 1 re-reads row 0 (retail re-bases with the finished row), row 2 reads row 1
    bool ok = g_format == 21 && d3d.locks == 1 && d3d.unlocks == 1 && (h._M_finish - h._M_start) == 256
        && h._M_start[10] == 8 && h._M_start[20] == 4 && h._M_start[30] == 0;
    d3d.fail = true; vector<unsigned long> h2; h2._M_start = 0;
    t.CalcAlphaHistogram(h2);
    ok = ok && d3d.unlocks == 1 && h2._M_start != 0 && h2._M_start[10] == 0;
    if (!ok) { std::printf("009fbbd0_TEST FAIL\n"); return 1; }
    std::printf("009fbbd0_TEST PASS\n");
    return 0;
}