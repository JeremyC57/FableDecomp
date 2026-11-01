// CVolumeTexture::SaveToDataStream(CDataOutputStream& stream) -- write the volume texture's 21-byte CVolumeTextureFileInfo header, then every mip level: compressed levels as one block, others slice by slice and row by row through a packed buffer
// Grade: DIFFER(754v753) -- the slice count is spilled before the vector(n) call instead of living in edi as the outer loop counter (residue 42, best of 18 probes)
// Next: lane gap: register allocation (same family as 0x009FB110)
// Leaf: ?SaveToDataStream@CVolumeTexture@@QAEXAAVCDataOutputStream@@@Z
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

struct _D3DVOLUME_DESC {
    _D3DFORMAT    Format;                                   // +0x00
    long          Type;                                     // +0x04
    unsigned long Usage;                                    // +0x08
    long          Pool;                                     // +0x0c
    unsigned int  Width;                                    // +0x10
    unsigned int  Height;                                   // +0x14
    unsigned int  Depth;                                    // +0x18
};
struct IDirect3DVolumeTexture9 {
    virtual void __stdcall Slot00() = 0; virtual void __stdcall Slot01() = 0; virtual void __stdcall Slot02() = 0;
    virtual void __stdcall Slot03() = 0; virtual void __stdcall Slot04() = 0; virtual void __stdcall Slot05() = 0;
    virtual void __stdcall Slot06() = 0; virtual void __stdcall Slot07() = 0; virtual void __stdcall Slot08() = 0;
    virtual void __stdcall Slot09() = 0; virtual void __stdcall Slot10() = 0; virtual void __stdcall Slot11() = 0;
    virtual void __stdcall Slot12() = 0;
    virtual unsigned long __stdcall GetLevelCount() = 0;                                         // slot 13
    virtual void __stdcall Slot14() = 0; virtual void __stdcall Slot15() = 0; virtual void __stdcall Slot16() = 0;
    virtual long __stdcall GetLevelDesc(unsigned int level, _D3DVOLUME_DESC* desc) = 0;           // slot 17
    virtual void __stdcall Slot18() = 0; virtual void __stdcall Slot19() = 0;
    virtual long __stdcall UnlockBox(unsigned int level) = 0;                                   // slot 20
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
enum EVolumePool { EVolumePool_0 = 0, EVolumePool_FORCE_DWORD = 0x7fffffff };
class CVolumeTextureFileInfo { public:                      // 21 bytes (Ego_r.pdb)
    unsigned short   Width;                                 // +0x00
    unsigned short   Height;                                // +0x02
    unsigned short   Depth;                                 // +0x04
    unsigned char    NoLevels;                              // +0x06
    CPixelFormatInit PixelFormatInit;                       // +0x07
    unsigned long    UseageFlags;                           // +0x0d
    EVolumePool      Pool;                                  // +0x11
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

class C3DExtentsI { public:
    long X;
    long Y;
    long Z;
    C3DExtentsI(long x, long y, long z) : X(x), Y(y), Z(z) { }
};
class CVolumeLockedBox { public:                            // 24 bytes (Ego_r.pdb)
    C3DExtentsI    Dimensions;                              // +0x00
    long           RowPitch;                                // +0x0c
    long           SlicePitch;                              // +0x10
    unsigned char* PMem;                                    // +0x14
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

#include "engine/CBaseClass.h"
class CVolumeTexture : public CBaseClass { public:          // 16 bytes (Ego_r.pdb)
    IDirect3DVolumeTexture9* PD3DVolumeTexture;             // +0x04
    long                     ByteLength;                    // +0x08
    void*                    PData;                         // +0x0c
    virtual ~CVolumeTexture();
    long GetNoLevels() const { return PD3DVolumeTexture->GetLevelCount(); }
    C3DExtentsI GetDimensions(unsigned long level) const
    {
        _D3DVOLUME_DESC desc;
        PD3DVolumeTexture->GetLevelDesc(level, &desc);
        return C3DExtentsI(desc.Width, desc.Height, desc.Depth);
    }
    CPixelFormat GetPixelFormat() const
    {
        _D3DVOLUME_DESC desc;
        PD3DVolumeTexture->GetLevelDesc(0, &desc);
        return CPixelFormat(desc.Format);
    }
    long GetLevelByteLength(long level) const;                                                   // 0x00A681F0
    CVolumeLockedBox Lock(unsigned long level, unsigned long flags);                             // 0x00A685E0
    void Unlock(unsigned long level) { PD3DVolumeTexture->UnlockBox(level); }
    void SaveToDataStream(CDataOutputStream& stream);
};

void CVolumeTexture::SaveToDataStream(CDataOutputStream& stream)
{
    _D3DVOLUME_DESC desc;
    PD3DVolumeTexture->GetLevelDesc(0, &desc);
    CVolumeTextureFileInfo info;
    info.Width = (unsigned short)GetDimensions(0).X;
    info.Height = (unsigned short)GetDimensions(0).Y;
    info.Depth = (unsigned short)GetDimensions(0).Z;
    info.NoLevels = (unsigned char)GetNoLevels();
    info.PixelFormatInit = GetPixelFormat().GetInit();
    info.UseageFlags = desc.Usage;
    info.Pool = (EVolumePool)desc.Pool;
    stream.Write(&info, sizeof(info));
    for (long level = 0; level < GetNoLevels(); ++level) {
        CVolumeLockedBox box = Lock(level, 0x10);
        if (GetPixelFormat().IsCompressed()) {
            C3DExtentsI dimensions = GetDimensions(level);
            stream.Write(box.PMem, GetLevelByteLength(level));
        } else {
            long width = box.Dimensions.X;
            if ((unsigned __int64)GetPixelFormat().GetByteColourDepth() * width > 0xffffffff)
                return;
            unsigned long rowBytes = GetPixelFormat().GetByteColourDepth() * width;
            if ((unsigned __int64)((__int64)box.Dimensions.Y * box.Dimensions.Z) * rowBytes > 0xffffffff)
                return;
            unsigned long size = box.Dimensions.Z * box.Dimensions.Y * rowBytes;
            long depth = box.Dimensions.Z;
            std::vector<unsigned char> buffer(size);
            unsigned char* src = box.PMem;
            unsigned char* dst = buffer.begin();
            for (long z = 0; z < depth; ++z) {
                for (long y = box.Dimensions.Y; y > 0; --y) {
                    memcpy(dst, src, rowBytes);
                    dst += rowBytes;
                    src += box.RowPitch;
                }
                src += box.SlicePitch;
            }
            stream.Write(buffer.begin(), size);
        }
        Unlock(level);
    }
}
