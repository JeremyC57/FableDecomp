// CTexture::Swizzle(long level, CSurfaceLockedRect& rect) -- Apply the surface operation to one texture level.
// Leaf: ?Swizzle@CTexture@@QAEXJAAVCSurfaceLockedRect@@@Z
// Identity: PDB exact overload and own calls009f9e00 GetSurfaceLevel(long)const and009f2e20 Uninitialise. Surface helper009f2e40 is original Swizzle(CSurfaceLockedRect&), replacing E2 placeholder.
#define CSurface CSurface_HeaderLayout
#include "engine/CSurface.h"
#undef CSurface
#define CTexture CTexture_HeaderLayout
#include "engine/CTexture.h"
#undef CTexture

class CSurfaceLockedRect;
// Layout: active CSurface header is flattened; real polymorphic CSurface has vptr+0, PD3DSurface+4, AllocationSource+8, PAllocatedMemory+12. Own vtable122f84c proves virtual destructor and no base. CTexture retains its active header layout.
class CSurface {public:
 IDirect3DSurface9* PD3DSurface;unsigned long AllocationSource;void* PAllocatedMemory;
 CSurface();CSurface(const CSurface&);virtual ~CSurface(){Uninitialise();}
 void Uninitialise();void Unswizzle(unsigned long);void Swizzle(CSurfaceLockedRect&);void Unswizzle(CSurfaceLockedRect&);
};
class CTexture:public CTexture_HeaderLayout {public:
 CSurface GetSurfaceLevel(long) const;
 void Unswizzle(long,long);void Swizzle(long,CSurfaceLockedRect&);void Unswizzle(long,CSurfaceLockedRect&);
};
void CTexture::Swizzle(long level,CSurfaceLockedRect& rect){GetSurfaceLevel(level).Swizzle(rect);}