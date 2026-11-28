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
#define FABLE_RETAIL_VOLUME_SAVE_LOCK_BODY_PASS30
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