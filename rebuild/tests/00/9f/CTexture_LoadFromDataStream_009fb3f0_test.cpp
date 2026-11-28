#define FABLE_RETAIL_CCHARSTRING
// 009fb3f0 CTexture::LoadFromDataStream
// Leaf: ?LoadFromDataStream@CTexture@@QAEXAAVCDataInputStream@@@Z
// Flags:
#define FABLE_RETAIL_TEXTURE_STREAM_LOAD_PASS30
#define FABLE_RETAIL_CDATAINPUTSTREAM_ORIGINAL_READ_SCOPE
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_LOCK
#define FABLE_RETAIL_CTEXTURE_LOADFROMDATASTREAM
#define FABLE_RETAIL_CTEXTURE_LEVELBYTELENGTH
#define FABLE_RETAIL_CTEXTURE_GETNOLEVELS
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_INIT
#define FABLE_RETAIL_CPIXELFORMAT_COLOURDEPTH
#define FABLE_RETAIL_CPIXELFORMAT_BYTECOLOURDEPTH_OUTOFLINE
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE
#define FABLE_RETAIL_PASS22_D3D_TEXTURE_ORIGINAL_BASES
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CSYSTEMMANAGER
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_PASS22_CORE_COMPLETE_ORIGINAL_DECLARATION
#define FABLE_RETAIL_PASS22_CORE_ORIGINAL_BUFFER_ACCESSORS
#define FABLE_RETAIL_PASS22_RENDERSTATE_ORIGINAL_METHOD_ORDER
#define FABLE_RETAIL_PASS22_SHADER_ORIGINAL_METHOD_ORDER
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CRENDERMANAGER2D
#include "fable_compiler.h"
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/C2DExtentsI.h"
#include "engine/CSurfaceLockedRect.h"
#include "engine/CPixelFormat.h"
#include "engine/CTexture.h"
#include "engine/CDataInputStream.h"
#include "engine/CTextureFileInfo.h"
#include "engine/Direct3DTexture9.h"
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"
#include "fable_string.h"
namespace std { template<> vector<unsigned char>::vector(unsigned int); }

#if defined(_MSC_VER)
typedef unsigned __int64 TextureByteCount;
typedef __int64 TextureSignedCount;
#else
#include <stdint.h>
typedef uint64_t TextureByteCount;
typedef int64_t TextureSignedCount;
#endif

void CTexture::LoadFromDataStream(CDataInputStream& stream)
{
    CTextureFileInfo file_info;
    stream.Read(&file_info, sizeof(file_info));
    CPixelFormat pf(file_info.PixelFormatInit);
    Initialise(C2DExtentsI(file_info.Width, file_info.Height), file_info.NoLevels, pf,
               file_info.UseageFlags, file_info.Pool, false, false);
    for (long c1 = 0; c1 < GetNoLevels(); ++c1)
    {
        CSurfaceLockedRect locked_rect = Lock(c1, 0x10);
        if (pf.IsCompressed())
        {
            _D3DSURFACE_DESC desc;
            HRESULT hr = PD3DTexture->GetLevelDesc(c1, &desc);
            (void)hr;
            stream.Read(locked_rect.PMem, GetLevelByteLength(c1));
        }
        else
        {
            TextureByteCount overflow_check = (TextureByteCount)pf.GetByteColourDepth() * (TextureSignedCount)locked_rect.Dimensions.X;
            if (overflow_check > 0xffffffffu)
                return;
            unsigned long line_length = pf.GetByteColourDepth() * locked_rect.Dimensions.X;
            TextureByteCount overflow_check2 = (TextureSignedCount)locked_rect.Dimensions.Y * (TextureByteCount)line_length;
            if (overflow_check2 > 0xffffffffu)
                return;
            unsigned long data_length = locked_rect.Dimensions.Y * line_length;
            std::vector<unsigned char> src_buffer(data_length);
            unsigned char* src = &src_buffer[0];
            unsigned char* dst = locked_rect.PMem;
            stream.Read(src, data_length);
            for (long cy = 0; cy < locked_rect.Dimensions.Y; ++cy)
            {
                memcpy(dst, src, line_length);
                dst += locked_rect.Pitch;
                src += line_length;
            }
        }
        Unlock(c1);
    }
}
#include <stdio.h>
#include <stdlib.h>

namespace {
void check(bool condition) { if (!condition) abort(); }
unsigned char inputBytes[256], outputBytes[3][80];
long inputChunk, activeLevels, width, height, depth, pitch, locks, unlocks, blocks, allocations, initialisations, descCalls;
unsigned long logicalStart, initFormat, usage;
bool compressed, initialised;
}
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CDataStream::~CDataStream() {}
CCharString::CCharString() : PStringData(0) {}
CCharString::~CCharString() {}
CWideString::CWideString() {}
CDataInputStream::CDataInputStream()
    : StreamPos(logicalStart), StreamSize(256), CurrentSourcePtr(inputBytes),
      SourceChunkStreamPos(logicalStart), SourceChunkBytesRemaining(inputChunk) {}
void CDataInputStream::Close() {}
void CDataInputStream::SetPosition(unsigned long) { abort(); }
void CDataInputStream::ReadCompressed(void*,long) { abort(); }
void CDataInputStream::ReadZlibCompressed(void*,long) { abort(); }
CCharString CDataInputStream::ReadNullTerminatedString() { abort(); return CCharString(); }
CWideString CDataInputStream::ReadNullTerminatedWString() { abort(); return CWideString(); }
void CDataInputStream::InternalReadNextSourceChunk(const unsigned char*&,long&) { abort(); }
bool CDataInputStream::NeedsBufferedRead(long) { abort(); return false; }
void CDataInputStream::InternalReadUnbuffered(unsigned char*,long) { abort(); }
void CDataInputStream::ReadWithSrcChunkOverflow(unsigned char* dest,long size)
{
    check(size>SourceChunkBytesRemaining && StreamPos-logicalStart+(unsigned long)size<=sizeof(inputBytes));
    memcpy(dest, inputBytes+(StreamPos-logicalStart), size);
    StreamPos+=(unsigned long)size;
    CurrentSourcePtr=inputBytes+(StreamPos-logicalStart);
    SourceChunkBytesRemaining=0;
    ++blocks;
}
namespace std {
template<> vector<unsigned char>::vector(unsigned int length)
    : _Vector_base<unsigned char, allocator<unsigned char> >(allocator<unsigned char>())
{
    check(length <= 96); ++allocations;
    this->_M_start=(unsigned char*)malloc(length?length:1);
    check(this->_M_start!=0); memset(this->_M_start,0xb7,length);
    this->_M_finish=this->_M_start+length;
    this->_M_end_of_storage._M_data=this->_M_finish;
}
}
void CPixelFormat::Initialise(const CPixelFormatInit& in)
{
    check(in.Type==4 && in.Linear==1 && in.ColourDepth==16 && in.RBits==5 && in.GBits==6 && in.BBits==5 && in.ABits==0);
    FormatIndex=23;
}
bool CPixelFormat::IsCompressed() const { check(FormatIndex==23); return compressed; }
unsigned long CPixelFormat::GetByteColourDepth() const { check(FormatIndex==23); return (unsigned long)depth; }
namespace {
class TestTextureDevice : public IDirect3DTexture9 {
public:
    long FABLE_STDCALL QueryInterface(const _GUID& riid, void** ppvObj) { abort(); return (long)0; }
    unsigned long FABLE_STDCALL AddRef() { abort(); return (unsigned long)0; }
    unsigned long FABLE_STDCALL Release() { abort(); return (unsigned long)0; }
    long FABLE_STDCALL GetDevice(IDirect3DDevice9** ppDevice) { abort(); return (long)0; }
    long FABLE_STDCALL SetPrivateData(const _GUID& refguid,const void* pData,unsigned long SizeOfData,unsigned long Flags) { abort(); return (long)0; }
    long FABLE_STDCALL GetPrivateData(const _GUID& refguid,void* pData,unsigned long* pSizeOfData) { abort(); return (long)0; }
    long FABLE_STDCALL FreePrivateData(const _GUID& refguid) { abort(); return (long)0; }
    unsigned long FABLE_STDCALL SetPriority(unsigned long PriorityNew) { abort(); return (unsigned long)0; }
    unsigned long FABLE_STDCALL GetPriority() { abort(); return (unsigned long)0; }
    void FABLE_STDCALL PreLoad() { abort(); }
    _D3DRESOURCETYPE FABLE_STDCALL GetType() { abort(); return (_D3DRESOURCETYPE)0; }
    unsigned long FABLE_STDCALL SetLOD(unsigned long LODNew) { abort(); return (unsigned long)0; }
    unsigned long FABLE_STDCALL GetLOD() { abort(); return (unsigned long)0; }
    unsigned long FABLE_STDCALL GetLevelCount() { abort(); return (unsigned long)0; }
    long FABLE_STDCALL SetAutoGenFilterType(_D3DTEXTUREFILTERTYPE FilterType) { abort(); return (long)0; }
    _D3DTEXTUREFILTERTYPE FABLE_STDCALL GetAutoGenFilterType() { abort(); return (_D3DTEXTUREFILTERTYPE)0; }
    void FABLE_STDCALL GenerateMipSubLevels() { abort(); }
    long FABLE_STDCALL GetLevelDesc(unsigned int Level,_D3DSURFACE_DESC* pDesc) { check(compressed && Level < (unsigned long)activeLevels); ++descCalls; memset(pDesc,0,sizeof(*pDesc)); return 0; }
    long FABLE_STDCALL GetSurfaceLevel(unsigned int Level,IDirect3DSurface9** ppSurfaceLevel) { abort(); return (long)0; }
    long FABLE_STDCALL LockRect(unsigned int Level,_D3DLOCKED_RECT* pLockedRect,const tagRECT* pRect,unsigned long Flags) { abort(); return (long)0; }
    long FABLE_STDCALL UnlockRect(unsigned int Level) { abort(); return (long)0; }
    long FABLE_STDCALL AddDirtyRect(const tagRECT* pDirtyRect) { abort(); return (long)0; }
};
IDirect3DTexture9* testDevice;
}
void CTexture::Uninitialise() {}
bool CTexture::Initialise(const C2DExtentsI& dim,long levels,const CPixelFormat& pf,unsigned long flags,ESurfacePool pool,bool a,bool b)
{
    check(dim.X==1031 && dim.Y==487 && levels==activeLevels && flags==usage && pool==SURFACE_POOL_MANAGED && !a && !b);
    check(pf.IsCompressed()==compressed);
    ++initialisations;
    PD3DTexture=testDevice;
    return initialised;
}
long CTexture::GetNoLevels() const { return activeLevels; }
CSurfaceLockedRect CTexture::Lock(unsigned long level,unsigned long flags)
{
    check(level==(unsigned long)locks && flags==0x10 && level<3);
    ++locks;
    CSurfaceLockedRect r; r.Dimensions.X=width; r.Dimensions.Y=height; r.Pitch=pitch; r.PMem=outputBytes[level]; return r;
}
void CTexture::Unlock(unsigned long level) { check(level==(unsigned long)unlocks && unlocks<locks); ++unlocks; }
long CTexture::GetLevelByteLength(long level) const { check(compressed && level>=0 && level<activeLevels); return 5+level; }

static void run(long levels,long w,long h,long bytes,bool comp,long chunk,unsigned long start,bool initResult)
{
    activeLevels=levels; width=w; height=h; depth=bytes; compressed=comp; inputChunk=chunk;
    logicalStart=start; initialised=initResult; locks=unlocks=blocks=allocations=initialisations=descCalls=0; usage=0x974d0102UL;
    bool overflow=!comp && ((TextureByteCount)bytes*(TextureSignedCount)w>0xffffffffUL ||
        (TextureSignedCount)h*(TextureByteCount)((unsigned long)bytes*(unsigned long)w)>0xffffffffUL);
    pitch=(!comp && !overflow && h>0 && w>=0) ? w*bytes+3 : 0;
    CTextureFileInfo file;
    memset(&file,0,sizeof(file)); file.Width=1031; file.Height=487; file.NoLevels=(unsigned char)levels;
    file.Pool=SURFACE_POOL_MANAGED; file.UseageFlags=usage;
    file.PixelFormatInit.Type=4; file.PixelFormatInit.Linear=1; file.PixelFormatInit.ColourDepth=16;
    file.PixelFormatInit.RBits=5; file.PixelFormatInit.GBits=6; file.PixelFormatInit.BBits=5;
    for(unsigned int i=0;i<sizeof(inputBytes);++i)inputBytes[i]=(unsigned char)(i*7+3);
    memcpy(inputBytes,&file,sizeof(file)); memset(outputBytes,0x96,sizeof(outputBytes));
    unsigned char expected[3][80]; memset(expected,0x96,sizeof(expected));
    TestTextureDevice device; testDevice = &device;
    CDataInputStream in; CTexture texture; texture.LoadFromDataStream(in);
    unsigned int pos=sizeof(file); long slow=chunk<(long)pos;
    int remaining=slow?0:chunk-(long)pos;
    int done=0;
    for(;done<levels && !overflow;++done) {
        unsigned int n=comp?5+done:(unsigned int)((unsigned long)w*(unsigned long)bytes*(unsigned long)h);
        bool read=n && (start+pos+n)<=0x7fffffffUL;
        if(comp && read)memcpy(expected[done],inputBytes+pos,n);
        if(!comp && w && h)for(long y=0;y<h;++y) {
            if(read)memcpy(expected[done]+y*pitch,inputBytes+pos+y*w*bytes,w*bytes);
            else memset(expected[done]+y*pitch,0xb7,w*bytes);
        }
        if(read) {pos+=n; if((long)n>remaining){++slow;remaining=0;}else remaining-=(long)n;}
    }
    check(memcmp(expected,outputBytes,sizeof(expected))==0);
    check(in.GetPosition()==start+pos && blocks==slow && initialisations==1 && locks==done+(levels>0&&overflow));
    check(unlocks==done && allocations==(comp?0:done) && descCalls==(comp?done:0));
}
int main()
{
    for(int n=0;n<=3;++n) for(int m=0;m<2;++m) for(int c=0;c<3;++c) for(int init=0;init<2;++init) {
        int chunk=c==0?0:c==1?20:200;
        run(n,3,2,2,m!=0,chunk,0,init!=0);
        run(n,0,2,2,m!=0,chunk,0,init!=0);
        run(n,3,0,2,m!=0,chunk,0,init!=0);
        run(n,3,2,2,m!=0,chunk,0x7fffffecUL,init!=0);
    }
    run(2,-1,0,1,false,24,0,true); run(2,1,-1,1,false,24,0,true);
    run(2,0x55555556L,1,3,false,24,0,true); run(2,1,0x7fffffffL,3,false,24,0,true);
    run(2,0x55555555L,0,3,false,24,0,true);
    puts("009fb3f0_TEST PASS"); return 0;
}