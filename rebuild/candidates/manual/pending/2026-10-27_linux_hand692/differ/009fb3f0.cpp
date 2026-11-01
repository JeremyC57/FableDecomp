// CTexture::LoadFromDataStream(CDataInputStream& stream) -- read the 19-byte CTextureFileInfo header, create the texture from it, then fill every mip level: compressed levels as one block, others through a packed row buffer
// Grade: DIFFER(850v850) -- same size; the stream reference sits in ebx where retail uses ebp (and the matching size register ebp vs ebx), plus the src/dst slot pair swapped before the row loop (residue 160, best same-size of 23 probes; p16 with a height local gets the ebp stream but spills height, 863v850 residue 95)
// Next: lane gap: register allocation (try the inline CDataInputStream::Read from the real engine header once it is de-flattened)
// Leaf: ?LoadFromDataStream@CTexture@@QAEXAAVCDataInputStream@@@Z
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
    CPixelFormat(const CPixelFormatInit& init) { Initialise(init); }
    void Initialise(const CPixelFormatInit& init);          // 0x009E3640
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

class CDataInputStream {                                    // vtable 0x0129A6B0, 0x18 bytes
public:
    virtual ~CDataInputStream();
    unsigned long        StreamPos;                         // +0x04
    long                 StreamSize;                        // +0x08
    const unsigned char* CurrentSourcePtr;                  // +0x0c
    unsigned long        SourceChunkStreamPos;              // +0x10
    long                 SourceChunkBytesRemaining;         // +0x14
    static bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        if (size > 0) {
            if (!IsValidPosition((__int64)(StreamPos + size)))
                return;
            if (size <= SourceChunkBytesRemaining) {
                memcpy(buffer, CurrentSourcePtr, size);
                CurrentSourcePtr += size;
                SourceChunkBytesRemaining -= size;
                StreamPos += size;
            }
            else
                ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
        }
    }
private:
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
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
    bool Initialise(const C2DExtentsI& extents, long levels, const CPixelFormat& format, unsigned long usage,
                    ESurfacePool pool, bool flagA, bool flagB);                                  // 0x009FA280
    CSurfaceLockedRect Lock(unsigned long level, unsigned long flags);                           // 0x009FA450
    void Unlock(unsigned long level) { PD3DTexture->UnlockRect(level); }
    void LoadFromDataStream(CDataInputStream& stream);
};

void CTexture::LoadFromDataStream(CDataInputStream& stream)
{
    CTextureFileInfo info;
    stream.Read(&info, sizeof(info));
    CPixelFormat format(info.PixelFormatInit);
    Initialise(C2DExtentsI(info.Width, info.Height), info.NoLevels, format, info.UseageFlags, info.Pool, false, false);
    for (long level = 0; level < GetNoLevels(); ++level) {
        CSurfaceLockedRect rect = Lock(level, 0x10);
        if (format.IsCompressed()) {
            C2DExtentsI dimensions = GetDimensions(level);
            stream.Read(rect.PMem, GetLevelByteLength(level));
        } else {
            if ((unsigned __int64)format.GetByteColourDepth() * rect.Dimensions.X > 0xffffffff)
                return;
            long width = rect.Dimensions.X;
            unsigned long rowBytes = format.GetByteColourDepth() * width;
            if ((unsigned __int64)rect.Dimensions.Y * rowBytes > 0xffffffff)
                return;
            unsigned long size = rect.Dimensions.Y * rowBytes;
            std::vector<unsigned char> buffer(size);
            unsigned char* src = buffer.begin();
            unsigned char* dst = rect.PMem;
            stream.Read(src, size);
            for (long y = rect.Dimensions.Y; y > 0; --y) {
                memcpy(dst, src, rowBytes);
                src += rowBytes;
                dst += rect.Pitch;
            }
        }
        Unlock(level);
    }
}
