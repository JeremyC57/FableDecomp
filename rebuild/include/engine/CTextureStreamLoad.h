#ifdef FABLE_RETAIL_TEXTURE_STREAM_LOAD_PASS30
#ifndef FABLE_ENGINE_CTEXTURE_STREAM_LOAD_H
#define FABLE_ENGINE_CTEXTURE_STREAM_LOAD_H
#include "engine/CTexture.h"
#include "engine/CSurfaceLockedRect.h"
#include "engine/Direct3DTexture9.h"
#include "engine/CSystemManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CRenderManager.h"

inline CPixelFormat CTexture::GetPixelFormat() const
{
    _D3DSURFACE_DESC desc;
    HRESULT hr = PD3DTexture->GetLevelDesc(0, &desc);
    (void)hr;
    return CPixelFormat(desc.Format);
}

inline C2DExtentsI CTexture::GetDimensions(unsigned long mipmap_level) const
{
    _D3DSURFACE_DESC desc;
    HRESULT hr = PD3DTexture->GetLevelDesc(mipmap_level, &desc);
    (void)hr;
    return C2DExtentsI(desc.Width, desc.Height);
}

inline CSurfaceLockedRect CTexture::Lock(unsigned long level, unsigned long lock_flags)
{
    C2DExtentsI dimensions = GetDimensions(level);
    tagRECT rect;
    rect.top = 0;
    rect.left = 0;
    rect.bottom = dimensions.Y;
    rect.right = dimensions.X;
    GFGetSystemManager()->GetDisplayManager()->GetRenderManager().WaitWhileResourceIsBusy(PD3DTexture, 1.0f);
    _D3DLOCKED_RECT locked_rect;
    HRESULT hr = PD3DTexture->LockRect(level, &locked_rect, &rect, lock_flags);
    if (hr >= 0)
    {
        CSurfaceLockedRect lock_info;
        lock_info.Dimensions = dimensions;
        lock_info.Pitch = locked_rect.Pitch;
        lock_info.PMem = (unsigned char*)locked_rect.pBits;
        return lock_info;
    }
    return CSurfaceLockedRect();
}

inline void CTexture::Unlock(unsigned long level)
{
    HRESULT hr = PD3DTexture->UnlockRect(level);
    (void)hr;
}

inline long CTexture::GetNoLevels() const
{
    return PD3DTexture->GetLevelCount();
}

inline long CTexture::GetLevelByteLength(long level) const
{
    _D3DSURFACE_DESC desc;
    HRESULT hr = PD3DTexture->GetLevelDesc(level, &desc);
    (void)hr;
    return GetPixelFormat().GetColourDepth() * desc.Width * desc.Height / 8;
}

#endif
#endif
