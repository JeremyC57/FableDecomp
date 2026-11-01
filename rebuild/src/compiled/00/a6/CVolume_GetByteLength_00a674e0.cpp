// CVolume::GetByteLength() const -- bytes per texel of the volume's pixel format times width, height and depth
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A674E0, length 77 bytes (+ int3 padding).
// Leaf: GetByteLength
// Naming wave 25: CPixelFormat::Initialise 0x009E3830 is ?Initialise@CPixelFormat@@QAEXW4_D3DFORMAT@@@Z (Ego_r.pdb; no constructor:
//   the body returns nothing), the format a _D3DFORMAT; a CPixelFormat built from a format is the inline constructor calling it.
// Callees: IDirect3DVolume9::GetDesc (COM slot 8, +0x20) twice -- once for the dimensions, once for the format (the
// inlined GetDimensions and the format query are separate calls, the interface pointer re-read for the second),
// CPixelFormat::Initialise(D3DFORMAT) @ 0x009E3830, CPixelFormat::GetByteColourDepth @ 0x009E3800.
// Shape: frame = pixel format (+4), first desc (+8), second desc (+0x24); the product is emitted as `imul Depth ; imul
// Height ; imul Width`, which VC7.1 produces from the source order `bytes * Width * Height * Depth` (it multiplies the
// last operand first; the reverse source order gives Width, Height, Depth).
enum _D3DFORMAT { D3DFMT_UNKNOWN = 0, D3DFMT_A8R8G8B8 = 21, D3DFMT_D24S8 = 75, D3DFMT_DXT1 = 0x31545844, D3DFMT_DXT3 = 0x33545844, D3DFMT_DXT5 = 0x35545844, D3DFMT_FORCE_DWORD = 0x7fffffff };   // naming wave 25: the D3D format enum (Initialise takes it)
struct IUnknownLike {
    virtual long __stdcall QueryInterface(const void* iid, void** out);
    virtual unsigned long __stdcall AddRef();
    virtual unsigned long __stdcall Release();
};
struct D3DVOLUME_DESC { _D3DFORMAT Format; long Type; unsigned long Usage; long Pool; unsigned long Width; unsigned long Height; unsigned long Depth; };
struct IDirect3DVolume9 : IUnknownLike {
    virtual long __stdcall GetDevice(void** device);
    virtual long __stdcall SetPrivateData(const void* guid, const void* data, unsigned long size, unsigned long flags);
    virtual long __stdcall GetPrivateData(const void* guid, void* data, unsigned long* size);
    virtual long __stdcall FreePrivateData(const void* guid);
    virtual long __stdcall GetContainer(const void* iid, void** container);
    virtual long __stdcall GetDesc(D3DVOLUME_DESC* desc);             // slot 8 (+0x20)
};
struct CBase {
    virtual ~CBase();
    CBase();                                // 0x0099A2D0
};
class CPixelFormat { public:
    long m_format;
    void Initialise(_D3DFORMAT format);               // 0x009E3830
    unsigned long GetByteColourDepth() const;   // 0x009E3800
};
class CVolume : public CBase { public:
    IDirect3DVolume9* m_pVolume;
    unsigned long GetByteLength() const;
};
unsigned long CVolume::GetByteLength() const
{
    D3DVOLUME_DESC dims;
    m_pVolume->GetDesc(&dims);
    D3DVOLUME_DESC desc;
    m_pVolume->GetDesc(&desc);
    CPixelFormat format;
    format.Initialise(desc.Format);
    return format.GetByteColourDepth() * dims.Width * dims.Height * dims.Depth;
}