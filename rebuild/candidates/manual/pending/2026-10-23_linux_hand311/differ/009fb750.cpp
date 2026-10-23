// Grade: DIFFER(789v789) -- same length, registers only: retail homes the stream in ebp and the row count/total in ebx (ours swaps them), and the row-copy loop keeps src in eax / dst in edx with dst advanced first (ours swaps the pair); tried reader-cast vs local CDataInputStream model, signed level, size<=remaining compare, product order, src/dst declaration and increment order, inline width/height
// CTexture::LoadFromDataStreamToPreallocatedSurface(CDataInputStream& in) -- read the texture header, then for every mip level lock the preallocated surface and stream the level in: compressed formats as one block sized from the level dimensions, others row by row through a temporary buffer honouring the lock pitch (overflowing sizes abandon the load)
// Leaf: ?LoadFromDataStreamToPreallocatedSurface@CTexture@@QAEXAAVCDataInputStream@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x009FB750, 787 bytes, ret 4.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (inline Read as landed 0x00994670),
//   CPixelFormat::Initialise(const CPixelFormatInit&) 0x009E3640 / (D3DFORMAT) 0x009E3830, IsCompressed 0x009E3940,
//   GetColourDepth 0x009E3820, GetByteColourDepth 0x009E3800, CTexture::Lock(ulong, ulong) 0x009FA450 (CSurfaceLockedRect by
//   value), vector<unsigned char>(n) 0x005136B0, _allmul 0x00BFEB50, free 0x00BFEA14; IDirect3DTexture9 GetLevelCount
//   (+0x34), GetLevelDesc (+0x44), UnlockRect (+0x50).
// Layout: Ego_r.pdb CTexture {PD3DTexture +0, ByteLength:28 / AllocationSource:4 +4}; CSurfaceLockedRect {Dimensions
//   (C2DExtentsI) +0, Pitch +8, PMem +0xC}; CPixelFormatInit 6 bytes at header +5 (header 0x13 bytes).
#include <stdlib.h>
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

// Layout: Ego_r.pdb CDataInputStream (StreamPos +4, StreamSize +8, CurrentSourcePtr +0xc, SourceChunkStreamPos +0x10,
//   SourceChunkBytesRemaining +0x14); local model because engine/CDataInputStream.h is data-only and cannot carry the
//   inline Read this function inlines (as the landed 0x00B16F60).
struct CDataInputStream {
    void* __vftable;
    unsigned long StreamPos;                            // +0x04
    long StreamSize;                                    // +0x08
    const unsigned char* CurrentSourcePtr;              // +0x0c
    unsigned long SourceChunkStreamPos;                 // +0x10
    long SourceChunkBytesRemaining;                     // +0x14
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (size <= SourceChunkBytesRemaining) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

enum D3DFORMAT { D3DFMT_UNKNOWN = 0, D3DFMT_FORCE_DWORD = 0x7fffffff };
struct D3DSURFACE_DESC {
    D3DFORMAT     Format;
    long          Type;
    unsigned long Usage;
    long          Pool;
    long          MultiSampleType;
    unsigned long MultiSampleQuality;
    unsigned int  Width;
    unsigned int  Height;
};
struct IDirect3DTexture9 {
    virtual long __stdcall QueryInterface(); virtual unsigned long __stdcall AddRef(); virtual unsigned long __stdcall Release();
    virtual long __stdcall GetDevice(); virtual long __stdcall SetPrivateData(); virtual long __stdcall GetPrivateData();
    virtual long __stdcall FreePrivateData(); virtual unsigned long __stdcall SetPriority(); virtual unsigned long __stdcall GetPriority();
    virtual void __stdcall PreLoad(); virtual long __stdcall GetType(); virtual unsigned long __stdcall SetLOD();
    virtual unsigned long __stdcall GetLOD();
    virtual unsigned long __stdcall GetLevelCount();                                  // +0x34
    virtual long __stdcall SetAutoGenFilterType(); virtual long __stdcall GetAutoGenFilterType();
    virtual void __stdcall GenerateMipSubLevels();
    virtual long __stdcall GetLevelDesc(unsigned int level, D3DSURFACE_DESC* desc);  // +0x44
    virtual long __stdcall GetSurfaceLevel(); virtual long __stdcall LockRect();
    virtual long __stdcall UnlockRect(unsigned int level);                            // +0x50
};

#pragma pack(push, 1)
struct CPixelFormatInit { unsigned char Bytes[6]; };
struct CTextureHeader {                       // 0x13 bytes on disk
    unsigned char    _pad00[5];
    CPixelFormatInit PixelFormat;             // +0x05
    unsigned char    _pad0b[8];
};
#pragma pack(pop)
struct CPixelFormat {
    long FormatIndex;
    CPixelFormat() {}
    void Initialise(const CPixelFormatInit& init);   // 0x009E3640
    void Initialise(D3DFORMAT format);               // 0x009E3830
    bool IsCompressed() const;                       // 0x009E3940
    unsigned long GetColourDepth() const;            // 0x009E3820
    unsigned long GetByteColourDepth() const;        // 0x009E3800
};
struct C2DExtentsI { long Width; long Height; };
struct CSurfaceLockedRect {
    C2DExtentsI    Dimensions;                // +0x00
    long           Pitch;                     // +0x08
    unsigned char* PMem;                      // +0x0C
};
struct CByteBuffer {
    unsigned char* _M_start;
    unsigned char* _M_finish;
    unsigned char* _M_end_of_storage;
    explicit CByteBuffer(unsigned long n);    // 0x005136B0
    ~CByteBuffer() { if (_M_start) free(_M_start); }
};

class CTexture {
public:
    IDirect3DTexture9* PD3DTexture;           // +0x00
    unsigned long      ByteLength : 28;       // +0x04
    unsigned long      AllocationSource : 4;
    CSurfaceLockedRect Lock(unsigned long level, unsigned long flags);   // 0x009FA450
    void LoadFromDataStreamToPreallocatedSurface(CDataInputStream& in);
};

void CTexture::LoadFromDataStreamToPreallocatedSurface(CDataInputStream& in)
{
    CTextureHeader header;
    in.Read(&header, sizeof(header));
    CPixelFormat format;
    format.Initialise(header.PixelFormat);
    for (long level = 0; level < (long)PD3DTexture->GetLevelCount(); ++level)
    {
        CSurfaceLockedRect locked = Lock(level, 0);
        if (format.IsCompressed())
        {
            D3DSURFACE_DESC levelDesc;
            PD3DTexture->GetLevelDesc(level, &levelDesc);
            D3DSURFACE_DESC desc;
            PD3DTexture->GetLevelDesc(level, &desc);
            D3DSURFACE_DESC topDesc;
            PD3DTexture->GetLevelDesc(0, &topDesc);
            CPixelFormat levelFormat;
            levelFormat.Initialise(topDesc.Format);
            long bytes = (levelFormat.GetColourDepth() * desc.Width * desc.Height) >> 3;
            if (bytes > 0)
                in.Read(locked.PMem, bytes);
        }
        else
        {
            long width = locked.Dimensions.Width;
            if ((unsigned __int64)format.GetByteColourDepth() * (__int64)width > 0xffffffffu)
                return;
            unsigned long rowBytes = format.GetByteColourDepth() * width;
            long height = locked.Dimensions.Height;
            if ((__int64)height * (unsigned __int64)rowBytes > 0xffffffffu)
                return;
            long total = height * rowBytes;
            CByteBuffer buffer(total);
            unsigned char* dst = locked.PMem;
            unsigned char* src = buffer._M_start;
            if (total > 0)
                in.Read(buffer._M_start, total);
            for (long row = locked.Dimensions.Height; row > 0; --row)
            {
                memcpy(dst, src, rowBytes);
                dst += locked.Pitch;
                src += rowBytes;
            }
        }
        PD3DTexture->UnlockRect(level);
    }
}
