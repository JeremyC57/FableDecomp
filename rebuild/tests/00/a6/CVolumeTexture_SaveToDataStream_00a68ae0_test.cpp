// 00a68ae0 CVolumeTexture::SaveToDataStream
// Leaf: ?SaveToDataStream@CVolumeTexture@@QAEXAAVCDataOutputStream@@@Z
// Flags:
// The shared Lock body is visible here and remains a call. VC7.1 keeps the
// locked depth in EDI across construction of the staging vector (167 matching bytes).
#define FABLE_RETAIL_VOLUME_SERIALIZATION_PASS30
#define FABLE_RETAIL_CVOLUMETEXTURE_INITIALISE
#define FABLE_RETAIL_CVOLUMETEXTURE_GETASTEXTURELIST
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPIXELFORMAT_INIT
#define FABLE_RETAIL_CPIXELFORMAT_BYTECOLOURDEPTH_OUTOFLINE
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_VECTOR_N_OUTOFLINE
#define FABLE_RETAIL_CTEXTURE
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
// The fixture supplies the external Lock result to exercise successful locks, pitched data and overflow exits.
#define FABLE_RETAIL_VOLUME_LOCKED_BOX_CTORS_PASS30
#include "stlport/fable_containers.h"
#include "engine/CVolumeTexture.h"
#include "engine/CVolumeTextureFileInfo.h"
#include "engine/CVolumeLockedBox.h"
#include "engine/CDataOutputStream.h"
#include "stlport/fable_containers.h"
#include <string.h>
#ifdef _MSC_VER
#pragma intrinsic(memcpy)
typedef unsigned __int64 VolumeByteCount;
#else
#include <stdint.h>
typedef uint64_t VolumeByteCount;
#endif

namespace std { template<> vector<unsigned char>::vector(unsigned int); }

void CVolumeTexture::SaveToDataStream(CDataOutputStream& stream)
{
    _D3DVOLUME_DESC desc;
    long hr = PD3DVolumeTexture->GetLevelDesc(0, &desc);
    (void)hr;
    CVolumeTextureFileInfo file_info;
    file_info.Width = (unsigned short)GetDimensions(0).X;
    file_info.Height = (unsigned short)GetDimensions(0).Y;
    file_info.Depth = (unsigned short)GetDimensions(0).Z;
    file_info.NoLevels = (unsigned char)GetNoLevels();
    file_info.PixelFormatInit = GetPixelFormat().GetInit();
    file_info.UseageFlags = desc.Usage;
    file_info.Pool = (EVolumePool)desc.Pool;
    stream.Write(&file_info, sizeof(file_info));
    for (long c1 = 0; c1 < GetNoLevels(); ++c1) {
        CVolumeLockedBox locked_box = Lock(c1, 0x10);
        if (GetPixelFormat().IsCompressed()) {
            _D3DVOLUME_DESC level_desc;
            long hr = PD3DVolumeTexture->GetLevelDesc(c1, &level_desc);
            (void)hr;
            stream.Write(locked_box.PMem, GetLevelByteLength(c1));
        } else {
            VolumeByteCount overflow_check = (VolumeByteCount)GetPixelFormat().GetByteColourDepth() * locked_box.Dimensions.X;
            if (overflow_check > 0xffffffffUL)
                return;
            unsigned long line_length = GetPixelFormat().GetByteColourDepth() * locked_box.Dimensions.X;
            VolumeByteCount overflow_check2 = (VolumeByteCount)line_length * locked_box.Dimensions.Y * locked_box.Dimensions.Z;
            if (overflow_check2 > 0xffffffffUL)
                return;
            unsigned long data_length = line_length * locked_box.Dimensions.Y * locked_box.Dimensions.Z;
            std::vector<unsigned char> src_buffer(data_length);
            unsigned char* dst = &src_buffer[0];
            unsigned char* src = locked_box.PMem;
            for (long cz = 0; cz < locked_box.Dimensions.Z; ++cz) {
                for (long cy = 0; cy < locked_box.Dimensions.Y; ++cy) {
                    memcpy(dst, src, line_length);
                    dst += line_length;
                    src += locked_box.RowPitch;
                }
                src += locked_box.SlicePitch;
            }
            stream.Write(&src_buffer[0], data_length);
        }
        Unlock(c1);
    }
}

#include <stdio.h>
#include <stdlib.h>

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CDataStream::~CDataStream() {}
CDataOutputStream::~CDataOutputStream() {}
CCharString::CCharString() : PStringData(0) {}
CCharString::~CCharString() {}
CVolumeTexture::~CVolumeTexture() {}

namespace {
void require(bool ok) { if (!ok) abort(); }
unsigned int byteDepth, formatIndex, lockCount, unlockCount, allocationCount;
unsigned int levelCount, descCalls[3], rawLengths[3];
bool packedBlocks;
_D3DVOLUME_DESC descriptions[3];
CVolumeLockedBox boxes[3];
unsigned char storage[3][120];
}

namespace std {
template<> vector<unsigned char>::vector(unsigned int length)
    : _Vector_base<unsigned char, allocator<unsigned char> >(allocator<unsigned char>())
{
    ++allocationCount;
    require(length < 512);
    this->_M_start = (unsigned char*)malloc(length ? length : 1);
    require(this->_M_start != 0);
    this->_M_finish = this->_M_start + length;
    this->_M_end_of_storage._M_data = this->_M_finish;
}
}
void CPixelFormat::Initialise(_D3DFORMAT format) { FormatIndex = (long)format; }
bool CPixelFormat::IsCompressed() const { require((unsigned long)FormatIndex == formatIndex); return packedBlocks; }
unsigned long CPixelFormat::GetByteColourDepth() const { require((unsigned long)FormatIndex == formatIndex); return byteDepth; }
CPixelFormatInit CPixelFormat::GetInit() const {
    require((unsigned long)FormatIndex == formatIndex);
    CPixelFormatInit value;
    memset(&value, 0, sizeof(value));
    value.Type = 5; value.Linear = 1; value.ColourDepth = 16;
    value.RBits = 5; value.GBits = 6; value.BBits = 5;
    return value;
}
CVolumeLockedBox CVolumeTexture::Lock(unsigned long level, unsigned long flags) {
    require(level == lockCount && level < levelCount && flags == 0x10);
    ++lockCount;
    return boxes[level];
}
long CVolumeTexture::GetLevelByteLength(long level) const {
    require(level >= 0 && (unsigned long)level < levelCount && packedBlocks);
    return rawLengths[level];
}

namespace {
class TestVolume : public IDirect3DVolumeTexture9 {
public:
    long FABLE_STDCALL QueryInterface(const _GUID&, void**) { abort(); return -1; }
    unsigned long FABLE_STDCALL AddRef() { abort(); return 0; }
    unsigned long FABLE_STDCALL Release() { abort(); return 0; }
    long FABLE_STDCALL GetDevice(IDirect3DDevice9**) { abort(); return -1; }
    long FABLE_STDCALL SetPrivateData(const _GUID&, const void*, DWORD, DWORD) { abort(); return -1; }
    long FABLE_STDCALL GetPrivateData(const _GUID&, void*, DWORD*) { abort(); return -1; }
    long FABLE_STDCALL FreePrivateData(const _GUID&) { abort(); return -1; }
    DWORD FABLE_STDCALL SetPriority(DWORD) { abort(); return 0; }
    DWORD FABLE_STDCALL GetPriority() { abort(); return 0; }
    void FABLE_STDCALL PreLoad() { abort(); }
    D3DRESOURCETYPE FABLE_STDCALL GetType() { abort(); return D3DRTYPE_SURFACE; }
    DWORD FABLE_STDCALL SetLOD(DWORD) { abort(); return 0; }
    DWORD FABLE_STDCALL GetLOD() { abort(); return 0; }
    DWORD FABLE_STDCALL GetLevelCount() { return levelCount; }
    long FABLE_STDCALL SetAutoGenFilterType(D3DTEXTUREFILTERTYPE) { abort(); return -1; }
    D3DTEXTUREFILTERTYPE FABLE_STDCALL GetAutoGenFilterType() { abort(); return D3DTEXF_NONE; }
    void FABLE_STDCALL GenerateMipSubLevels() { abort(); }
    long FABLE_STDCALL GetLevelDesc(UINT level, D3DVOLUME_DESC* desc) {
        require(level < 3); ++descCalls[level]; *desc = descriptions[level]; return 0;
    }
    long FABLE_STDCALL GetVolumeLevel(UINT, IDirect3DVolume9**) { abort(); return -1; }
    long FABLE_STDCALL LockBox(UINT, D3DLOCKED_BOX*, const D3DBOX*, DWORD) { abort(); return -1; }
    long FABLE_STDCALL UnlockBox(UINT level) { require(level == unlockCount && level < lockCount); ++unlockCount; return 0; }
    long FABLE_STDCALL AddDirtyBox(const D3DBOX*) { abort(); return -1; }
};
class TestOutput : public CDataOutputStream {
public:
    unsigned int nw, sizes[4];
    unsigned char data[4][128];
    TestOutput() : nw(0) { memset(data, 0xcd, sizeof(data)); }
    void SetPosition(unsigned long) { abort(); }
    unsigned long GetPosition() const { abort(); return 0; }
    void Close() { abort(); }
    void Write(const void* source, long size) {
        require(nw < 4 && size >= 0 && size <= 128);
        sizes[nw] = (unsigned int)size;
        if (size) memcpy(data[nw], source, size);
        ++nw;
    }
};
void RunCase(unsigned int levels, bool compressed, long width, long height, long depth, unsigned int bytes, bool overflow) {
    levelCount = levels; packedBlocks = compressed; byteDepth = bytes;
    formatIndex = (unsigned int)D3DFMT_A8R8G8B8;
    lockCount = unlockCount = allocationCount = 0; memset(descCalls, 0, sizeof(descCalls));
    for (unsigned int i = 0; i < 3; ++i) {
        D3DVOLUME_DESC& d = descriptions[i];
        memset(&d, 0, sizeof(d)); d.Format = (_D3DFORMAT)formatIndex;
        d.Width = 0x12345; d.Height = 0x67; d.Depth = 0x89; d.Usage = 0x76543210; d.Pool = D3DPOOL_SYSTEMMEM;
        CVolumeLockedBox& b = boxes[i];
        b.Dimensions.Initialise(width, height, depth);
        b.RowPitch = overflow ? 0 : (long)bytes * width + 3;
        b.SlicePitch = 5;
        b.PMem = storage[i]; rawLengths[i] = 7+i;
        for (unsigned int j=0; j<sizeof(storage[i]); ++j) storage[i][j]=(unsigned char)(i*13+j);
    }
    TestVolume device; CVolumeTexture volume; volume.PD3DVolumeTexture = &device;
    TestOutput out; volume.SaveToDataStream(out);
    const unsigned char header[] = {0x45,0x23,0x67,0,0x89,0,(unsigned char)levels,0x15,16,5,6,5,0,0x10,0x32,0x54,0x76,2,0,0,0};
    require(out.sizes[0] == sizeof(header) && !memcmp(out.data[0],header,sizeof(header)));
    if (overflow) { require(out.nw == 1 && lockCount == 1 && unlockCount == 0 && allocationCount == 0); return; }
    require(lockCount == levels && unlockCount == levels && out.nw == 1+levels && allocationCount == (compressed ? 0u : levels));
    for (unsigned int i=0; i<levels; ++i) {
        if (compressed) {require(out.sizes[i+1] == rawLengths[i] && !memcmp(out.data[i+1],storage[i],rawLengths[i])); continue;}
        unsigned int n = 0, idx = 0;
        for (long z=0; z<depth; ++z) {
            for (long y=0; y<height; ++y) {
                for (unsigned int x=0; x<bytes*(unsigned int)width; ++x) require(out.data[i+1][n++] == storage[i][idx+x]);
                idx += boxes[i].RowPitch;
            }
            idx += boxes[i].SlicePitch;
        }
        require(out.sizes[i+1] == n);
    }
    require(descCalls[0]>=5);
}
}
int main() {
    RunCase(0,false,0,0,0,1,false);
    RunCase(2,true,3,2,2,2,false);
    RunCase(2,false,3,2,2,2,false);
    RunCase(1,false,2,0,3,1,false);
    RunCase(1,false,2,3,0,1,false);
    RunCase(1,false,0,1,1,1,false);
    RunCase(1,false,2,1,1,0x80000000u,true);
    RunCase(1,false,65536,65536,1,1,true);
    puts("00a68ae0_TEST PASS");
    return 0;
}