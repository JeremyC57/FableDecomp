// 00a69f70 CTextureManager::ReduceMipmapLevel
// Leaf: ?ReduceMipmapLevel@CTextureManager@@QAE_NAAVCGraphicFrame@@AAVCManagedTexture@@K@Z
// Flags: /O2 /Oy
#define FABLE_RETAIL_ASSERT_DEAD_ARM
#include "fable_assert.h"
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_ISNULL
#define FABLE_RETAIL_TEXTURE_MIP_REDUCE_PASS29
#define FABLE_RETAIL_CTEXTUREMANAGER_CLASS
#define FABLE_RETAIL_CTEXTUREMANAGER_ALLOCATETEXTURE
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CMANAGEDTEXTUREDATA_RELEASETEXTURE
#define FABLE_RETAIL_CMANAGEDTEXTURE_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CGRAPHICFRAMEDATA_TEXTUREQUADUVS
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CTEXTURE_SIZED
#define FABLE_RETAIL_CGRAPHICFRAME_CTOR
#define FABLE_RETAIL_CTEXTURE_GETNOLEVELS
#define FABLE_RETAIL_CTEXTURE_FORMATQUERIES
#define FABLE_RETAIL_CSURFACE
#define FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
#define FABLE_RETAIL_CSURFACE_COPYFROMSURFACE
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_CTORS
#define private public
#define protected public
#include "engine/C2DBoxI.h"
#include "engine/CTexture.h"
#include "engine/CSurface.h"
#include "engine/CManagedTexture.h"

// Inline-budget: the member initializer expands in the caller. The closing brace selects an out-of-line destructor in the native deleting destructor.
inline CManagedTexture::CManagedTexture() {
#ifdef _MSC_VER
#pragma inline_depth(0)
#endif
}
#ifdef _MSC_VER
#pragma inline_depth()
#endif

inline bool CManagedTexture::Release()
{
    if (Data.IsNull()) return true;
    bool was_released = false;
    if (Data.Get() && Data.GetRefCount() == 2) {
        FABLE_RETAIL_ASSERT(Data->IsAllocated());
        Data->ReleaseTexture();
        was_released = true;
    }
    Data.Reset(0);
    return was_released;
}
inline CManagedTexture::~CManagedTexture()
{
    if (Data.GetRefCount() == 2)
        Release();
}
inline const CManagedTexture& CManagedTexture::operator=(const CManagedTexture& src)
{
    if (Data.Get() != src.Data.Get()) {
        Release();
        Data = src.Data;
    }
    return *this;
}

#include "engine/CGraphicFrame.h"
#include "engine/CTextureManager.h"
bool CTextureManager::ReduceMipmapLevel(CGraphicFrame& frame, CManagedTexture& managed_texture, unsigned long skip_levels)
{
    CTexture& src_texture = *frame.GetTexture();
    unsigned long old_levels = src_texture.GetNoLevels();
    unsigned long new_levels = old_levels - skip_levels;
    C2DExtentsI dimensions(frame.GetWidth(), frame.GetHeight());
    for (unsigned long c_skip=0; c_skip<skip_levels; ++c_skip) {
        dimensions.X /= 2;
        dimensions.Y /= 2;
    }
    managed_texture.Data->SetLocked(true);
    CManagedTexture new_managed_texture;
    if (!AllocateTexture(new_managed_texture, dimensions.X, dimensions.Y, new_levels, src_texture.GetPixelFormat(), managed_texture.Data->GetOwner())) {
        managed_texture.Data->SetLocked(false);
        return false;
    }
    CCountedPointer<CTexture> dest_texture = new_managed_texture.GetTexture();
    C2DBoxI surface_area(0,0,dimensions.X,dimensions.Y);
    for (unsigned long c_level=0; c_level<new_levels; ++c_level) {
        CSurface dest_surface = dest_texture->GetSurfaceLevel(c_level);
        CSurface src_surface = src_texture.GetSurfaceLevel(c_level+skip_levels);
        dest_surface.CopyFromSurface(&surface_area,src_surface,&surface_area,1);
        surface_area.BRX /= 2;
        surface_area.BRY /= 2;
    }
    CGraphicFrame old_frame(frame);
    frame.UnInitialise();
    frame.InitialiseAsSurface(dest_texture,dimensions.X,dimensions.Y,old_frame.IsAlpha(),old_frame.IsBooleanAlpha());
    managed_texture.Data->SetOwner(0);
    managed_texture.Data->SetLocked(false);
    managed_texture = new_managed_texture;
    return true;
}

#undef private
#undef protected
#include <stdio.h>
#include "engine/CManagedTextureOwner.h"
static int errors, allocations, fills, copies, releases, source_levels, skip_levels, flags, w, h;
static CTexture* source_texture;
static CTexture* target_texture;
static CManagedTextureData* old_data;
static CManagedTextureData* new_data;
static CManagedTextureOwner* expected_owner;
bool FABLE_FASTCALL GFAssertFailed(const char*,const char*,const char*,unsigned long) { ++errors; return false; }
void CTexture::Uninitialise() {}
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CTextureManager::CTextureManager() {}
CTextureManager::~CTextureManager() {}
CResource::CResource() { IVRefCount = 1; }
CResource::~CResource() {}
bool CResource::UnloadResource() { return false; }
unsigned long CResource::GetUnloadableMemory() const {return 0;}
bool CResource::IsResourceLoaded() const {return true;}
void CResource::OnResourceLoaded() {}
bool CResource::CanAbortResourceLoad(long) const { return false; }
void CResource::OnResourceLoadAborted() {}
unsigned long CResource::GetDebugMemoryInUse() const {return 0;}
void CResource::ResetDebugMemoryInUse() {}
void CIVCountedPointeeBase::Delete() const { ++errors; }
CManagedTextureOwner::~CManagedTextureOwner() {}
CManagedTextureData::~CManagedTextureData() {}
void CManagedTextureData::ReleaseTexture() { ++releases; Allocated=false; }
class FixtureOwner : public CManagedTextureOwner { public: void ReleaseTexture() {} };
void CGraphicFrameData::Uninitialise() { IsInitialised=false; }
void CSurface::Uninitialise() {}
long CTexture::GetNoLevels() const {
    if (this!=source_texture) ++errors;
    return source_levels;
}
CPixelFormat CTexture::GetPixelFormat() const {
    CPixelFormat f;
    if (this!=source_texture) ++errors;
    f.FormatIndex=source_levels+10;
    return f;
}
CSurface CTexture::GetSurfaceLevel(long n) const {
    CSurface v;
    v.AllocationSource=(this==source_texture?128U:0U)+n;
    return v;
}
void CSurface::CopyFromSurface(const C2DBoxI* out,const CSurface& src,const C2DBoxI* in,unsigned long filter) {
    if (AllocationSource!=unsigned(copies) || src.AllocationSource!=unsigned(128+skip_levels+copies) || in!=out || filter!=1 ||
        in->TLX!=0 || in->TLY!=0 || in->BRX!=(w>>(skip_levels+copies)) || in->BRY!=(h>>(skip_levels+copies))) ++errors;
    ++copies;
}
bool CTextureManager::AllocateTexture(CManagedTexture& m,unsigned long x,unsigned long y,long levels,const CPixelFormat& f,CManagedTextureOwner* owner) {
    ++allocations;
    if (!old_data->Locked || x!=unsigned(w>>skip_levels) || y!=unsigned(h>>skip_levels) || levels!=source_levels-skip_levels ||
        f.FormatIndex!=source_levels+10 || owner!=expected_owner || !m.Data.IsNull()) ++errors;
    if (flags&4) {
        m.Data.Reset(new_data);
        return true;
    }
    return false;
}
bool CGraphicFrame::InitialiseAsSurface(const CCountedPointer<CTexture>& texture,long width,long height,bool alpha,bool boolean_alpha) {
    ++fills;
    if (texture.Data!=target_texture || FrameData || width!=(w>>skip_levels) || height!=(h>>skip_levels) ||
        alpha!=((flags&1)!=0) || boolean_alpha!=((flags&2)!=0)) ++errors;
    FrameData=new CGraphicFrameData();
    FrameData->ReferenceCount=1; FrameData->IsInitialised=true;
    FrameData->PTexture.Data=texture.Data; FrameData->Width=(unsigned short)width; FrameData->Height=(unsigned short)height;
    FrameData->Alpha=alpha; FrameData->BooleanAlpha=boolean_alpha;
    return true;
}
int main() {
    FixtureOwner owner;
    expected_owner=&owner;
    CTexture source, dest;
    source_texture=&source;target_texture=&dest;
    CTextureManager manager;
    for (source_levels=0;source_levels<=7;++source_levels)
    for (skip_levels=0;skip_levels<=source_levels;++skip_levels)
    for (flags=0;flags<8;++flags)
    for (int parity=0;parity<4;++parity)
    for (int holders=2;holders<=3;++holders) {
        w=106+(parity&1);h=90+((parity>>1)&1);
        CManagedTextureData before,after;
        old_data=&before;new_data=&after;
        before.IVRefCount=holders;after.IVRefCount=3;before.SetOwner(&owner);after.SetOwner(&owner);
        before.Locked=after.Locked=false;before.Allocated=after.Allocated=true;
        before.Texture.Data=&source;after.Texture.Data=&dest;
        CManagedTexture handle;handle.Data.Reset(&before);
        CGraphicFrame frame;frame.FrameData=new CGraphicFrameData();
        frame.FrameData->ReferenceCount=1;frame.FrameData->PTexture.Data=&source;
        frame.FrameData->Width=(unsigned short)w;frame.FrameData->Height=(unsigned short)h;
        frame.FrameData->Alpha=(flags&1)!=0;frame.FrameData->BooleanAlpha=(flags&2)!=0;frame.FrameData->IsInitialised=true;
        allocations=fills=copies=releases=errors=0;
        const bool ok=manager.ReduceMipmapLevel(frame,handle,skip_levels);
        if (ok!=((flags&4)!=0) || allocations!=1 || before.Locked || errors) return 1;
        if (ok) {
            if (copies!=source_levels-skip_levels || fills!=1 || handle.Data.Get()!=&after || before.Owner ||
                releases!=(holders==2?1:0) || before.IVRefCount!=unsigned(holders-1) || frame.GetTexture()!=&dest) return 2;
        } else if (copies || fills || releases || before.Owner!=&owner || handle.Data.Get()!=&before || frame.GetTexture()!=&source) return 3;
        handle.Data.Reset(0);
    }
    puts("00a69f70_TEST PASS");
    return 0;
}