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
#include "engine/CTextureStreamLoad.h"

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