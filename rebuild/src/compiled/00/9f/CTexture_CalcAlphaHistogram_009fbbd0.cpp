// CTexture::CalcAlphaHistogram(vector<unsigned long>& histogram) const -- lock mip level 0 and count, for each of the 256 alpha values, how many texels carry it
// Leaf: ?CalcAlphaHistogram@CTexture
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FBBD0, 250 bytes (+2 int3), `ret 4`.
// Callees: IDirect3DTexture9::GetLevelDesc (COM slot 17, +0x44; twice), LockRect (slot 19, +0x4c) and UnlockRect (slot 20,
//   +0x50); CPixelFormat::Initialise(D3DFORMAT) @ 0x009E3830 (through a converting constructor), CPixelFormat::
//   ConvertPFDDataToRGB(unsigned char*, CRGBColour*) const @ 0x009E3DF0, CPixelFormat::GetByteColourDepth() const
//   @ 0x009E3800; vector<unsigned long>::resize(size_t, const unsigned long&) @ 0x004428F0.
// Layout: PDB CTexture (PD3DTexture +0) and CPixelFormat (FormatIndex); D3DSURFACE_DESC (Width +0x18, Height +0x1C),
//   D3DLOCKED_RECT {Pitch, pBits}, RECT; CRGBColour {B, G, R, A} (alpha the fourth byte).
// Shape: the level description is fetched twice through the inline GetPixelFormat (0x009F9D80) and GetDimensions(0)
//   (0x009F9DB0) bodies, whose result extents live on in the description's frame words; the lock rectangle is
//   {0, 0, width, height} (written top, left, right, bottom: retail's store order); the histogram is resized to 256 zeroes through a by-reference temporary that shares its frame
//   word with the per-texel colour; the inner loop counts the width down; the row pointer is re-based as
//   `pBits + Pitch * y` with the row just finished (retail's arithmetic, kept as is).
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