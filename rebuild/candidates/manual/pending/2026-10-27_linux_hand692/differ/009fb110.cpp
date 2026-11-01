// CTexture::SaveToDataStream(CDataOutputStream& stream) -- write the texture's 19-byte CTextureFileInfo header, then every mip level: compressed levels as one block, others row by row through a packed buffer
// Grade: DIFFER(737v732) -- row-copy loop prologue: retail keeps the row count in edi across the vector(n) call and spills it to [esp+10h] after, ours spills it before; retail's src lives in the [esp+18h] slot and dst in eax, ours the reverse (residue 26 non-call lines, best of 27 probes)
// Next: lane gap: register allocation (try a different dimension local placement or the real STLport vector(size_type, const allocator&) ctor; every loop form / declaration order gave the same 26)
// Leaf: ?SaveToDataStream@CTexture@@QAEXAAVCDataOutputStream@@@Z
#include <string.h>
#pragma intrinsic(memcpy)
extern "C" void __cdecl free(void* block);                       // 0x00BFEA14

enum _D3DFORMAT { D3DFMT_UNKNOWN = 0, D3DFMT_FORCE_DWORD = 0x7fffffff };
enum ESurfacePool { ESurfacePool_0 = 0, ESurfacePool_FORCE_DWORD = 0x7fffffff };

struct _D3DSURFACE_DESC {
    _D3DFORMAT    Format;                                   // +0x00
    long          Type;                                     // +0x04
    unsigned long Usage;                                    // +0x08
    long          Pool;                                     // +0x0c
    long          MultiSampleType;                          // +0x10
    unsigned long MultiSampleQuality;                       // +0x14
    unsigned int  Width;                                    // +0x18
    unsigned int  Height;                                   // +0x1c
};

struct IDirect3DTexture9 {
    virtual void __stdcall Slot00() = 0; virtual void __stdcall Slot01() = 0; virtual void __stdcall Slot02() = 0;
    virtual void __stdcall Slot03() = 0; virtual void __stdcall Slot04() = 0; virtual void __stdcall Slot05() = 0;
    virtual void __stdcall Slot06() = 0; virtual void __stdcall Slot07() = 0; virtual void __stdcall Slot08() = 0;
    virtual void __stdcall Slot09() = 0; virtual void __stdcall Slot10() = 0; virtual void __stdcall Slot11() = 0;
    virtual void __stdcall Slot12() = 0;
    virtual unsigned long __stdcall GetLevelCount() = 0;                                         // slot 13
    virtual void __stdcall Slot14() = 0; virtual void __stdcall Slot15() = 0; virtual void __stdcall Slot16() = 0;
    virtual long __stdcall GetLevelDesc(unsigned int level, _D3DSURFACE_DESC* desc) = 0;          // slot 17
    virtual void __stdcall Slot18() = 0; virtual void __stdcall Slot19() = 0;
    virtual long __stdcall UnlockRect(unsigned int level) = 0;                                  // slot 20
};

namespace std {
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(unsigned int __n);                          // 0x005136B0
    ~vector() { }
    _Tp* begin() { return this->_M_start; }
};
}

class C2DExtentsI { public:
    long X;
    long Y;
    C2DExtentsI(long x, long y) : X(x), Y(y) { }
};

#pragma pack(push, 1)
class CPixelFormatInit { public:                            // 6 bytes (Ego_r.pdb)
    unsigned char Type : 4;
    unsigned char Linear : 1;
    unsigned char ColourDepth;
    unsigned char RBits;
    unsigned char GBits;
    unsigned char BBits;
    unsigned char ABits;
};
class CTextureFileInfo { public:                            // 19 bytes (Ego_r.pdb)
    unsigned short   Width;                                 // +0x00
    unsigned short   Height;                                // +0x02
    unsigned char    NoLevels;                              // +0x04
    CPixelFormatInit PixelFormatInit;                       // +0x05
    unsigned long    UseageFlags;                           // +0x0b
    ESurfacePool     Pool;                                  // +0x0f
};
#pragma pack(pop)

class CPixelFormat { public:
    long FormatIndex;
    CPixelFormat(_D3DFORMAT format) { Initialise(format); }
    void Initialise(_D3DFORMAT format);                     // 0x009E3830
    unsigned long GetColourDepth() const;                   // 0x009E3820
    unsigned long GetByteColourDepth() const;               // 0x009E3800
    CPixelFormatInit GetInit() const;                       // 0x009E3870
    bool IsCompressed() const;                              // 0x009E3940
};

class CSurfaceLockedRect { public:                          // 16 bytes (Ego_r.pdb)
    C2DExtentsI    Dimensions;                              // +0x00
    long           Pitch;                                   // +0x08
    unsigned char* PMem;                                    // +0x0c
};

class CDataStream {
public:
    virtual ~CDataStream();
};
class CDataOutputStream : public CDataStream {              // vtable 0x0129A6E0
public:
    virtual void Close() = 0;                               // slot 1
    virtual unsigned long GetPosition() const = 0;          // slot 2
    virtual void SetPosition(unsigned long position) = 0;   // slot 3
    virtual void Write(const void* data, long size) = 0;    // slot 4
};

class CTexture { public:                                    // 8 bytes (Ego_r.pdb)
    IDirect3DTexture9* PD3DTexture;                         // +0x00
    unsigned long      ByteLength : 28;                     // +0x04
    unsigned long      AllocationSource : 4;
    long GetNoLevels() const { return PD3DTexture->GetLevelCount(); }
    C2DExtentsI GetDimensions(unsigned long level) const
    {
        _D3DSURFACE_DESC desc;
        PD3DTexture->GetLevelDesc(level, &desc);
        return C2DExtentsI(desc.Width, desc.Height);
    }
    CPixelFormat GetPixelFormat() const
    {
        _D3DSURFACE_DESC desc;
        PD3DTexture->GetLevelDesc(0, &desc);
        return CPixelFormat(desc.Format);
    }
    long GetLevelByteLength(long level) const
    {
        _D3DSURFACE_DESC desc;
        PD3DTexture->GetLevelDesc(level, &desc);
        return desc.Width * desc.Height * GetPixelFormat().GetColourDepth() / 8;
    }
    CSurfaceLockedRect Lock(unsigned long level, unsigned long flags);                           // 0x009FA450
    void Unlock(unsigned long level) { PD3DTexture->UnlockRect(level); }
    void SaveToDataStream(CDataOutputStream& stream);
};

void CTexture::SaveToDataStream(CDataOutputStream& stream)
{
    _D3DSURFACE_DESC desc;
    PD3DTexture->GetLevelDesc(0, &desc);
    CTextureFileInfo info;
    info.Width = (unsigned short)GetDimensions(0).X;
    info.Height = (unsigned short)GetDimensions(0).Y;
    info.NoLevels = (unsigned char)GetNoLevels();
    info.PixelFormatInit = GetPixelFormat().GetInit();
    info.UseageFlags = desc.Usage;
    info.Pool = (ESurfacePool)desc.Pool;
    stream.Write(&info, sizeof(info));
    for (long level = 0; level < GetNoLevels(); ++level) {
        CSurfaceLockedRect rect = Lock(level, 0x10);
        if (GetPixelFormat().IsCompressed()) {
            C2DExtentsI dimensions = GetDimensions(level);
            stream.Write(rect.PMem, GetLevelByteLength(level));
        } else {
            if ((unsigned __int64)GetPixelFormat().GetByteColourDepth() * rect.Dimensions.X > 0xffffffff)
                return;
            long width = rect.Dimensions.X;
            unsigned long rowBytes = GetPixelFormat().GetByteColourDepth() * width;
            if ((unsigned __int64)rect.Dimensions.Y * rowBytes > 0xffffffff)
                return;
            unsigned long size = rect.Dimensions.Y * rowBytes;
            long height = rect.Dimensions.Y;
            std::vector<unsigned char> buffer(size);
            unsigned char* src = rect.PMem;
            unsigned char* dst = buffer.begin();
            for (long y = 0; y < height; ++y) {
                memcpy(dst, src, rowBytes);
                src += rect.Pitch;
                dst += rowBytes;
            }
            stream.Write(buffer.begin(), size);
        }
        Unlock(level);
    }
}
