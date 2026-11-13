#ifdef FABLE_RETAIL_CSURFACE
#ifndef FABLE_ENGINE_CSURFACE_H
#define FABLE_ENGINE_CSURFACE_H
#include "fable_compiler.h"
struct IDirect3DSurface9;
// Layout: original size16; real virtual destructor, fields4/8/12. Last pointer is not initialized by default ctor.
class CSurface {
public:
 CSurface() :PD3DSurface(0),AllocationSource(0) {}
#ifdef FABLE_RETAIL_CSURFACE_COPYFROMMEMORY
 // Retail opt-in (chunk 792, row 0x00AB8E10 CStaticFontBank::CStaticFontBank): the destructor is inline in retail (the
 // GetSurfaceLevel temporary is destroyed in place: vtable 0x0122F84C stored, then ?Uninitialise@CSurface@@QAEXXZ 0x009F2E20
 // called; no out-of-line ??1CSurface is bound) and Ego_r.pdb ?CopyFromMemory@CSurface@@QAEXPBVC2DBoxI@@PBX0ABVCPixelFormat@@JK@Z
 // (0x009F4390, six arguments: destination box, source pixels, source box, source format, source pitch, colour key).
#ifdef FABLE_RETAIL_CSURFACE_FORCEINLINE_DTOR
 // Per-TU inline opt-in (chunk 886, row 0x0041E6D3 NUISystem::CManager::Action): the GetSurfaceLevel temporary of LOAD_PHOTO is destroyed in
 // place at 0x00426747 (vtable stored, Uninitialise 0x009F2E20 called) past VC7.1's caller-size inline budget, which calls ??1CSurface there.
 FABLE_FORCEINLINE
#endif
 virtual ~CSurface() { Uninitialise(); }
 void CopyFromMemory(const class C2DBoxI* destRect, const void* source, const class C2DBoxI* sourceRect,
                     const class CPixelFormat& sourceFormat, long sourcePitch, unsigned long colourKey);   // 0x009F4390
#else
 virtual ~CSurface();
#endif
 void Uninitialise();                                     // 0x009F2E20
#ifdef FABLE_RETAIL_CSURFACE_ASSIGN
 // Retail opt-in (row 0x00B89C30 ApplyOutline): ??4CSurface@@QAEAAV0@ABV0@@Z = retail 0x009F2E70 (landed CSurface_operator_009f2e70),
 // called twice by the inline (implicit) CRenderTarget::operator=. Declaration only.
 CSurface& operator=(const CSurface& other);              // 0x009F2E70
#endif
#ifdef FABLE_RETAIL_CSURFACE_READONLYLOCK
 // Retail opt-in (chunk Q9, row 0x009F3010): Ego_r.pdb / FableWin.pdb ?ReadOnlyLock@CSurface@@QBE?AVCSurfaceLockedRect@@K@Z (hidden result,
 // `ret 8`). Needs CSurfaceLockedRect complete (engine/CSurfaceLockedRect.h) before the include; GetDimensions comes from
 // FABLE_RETAIL_CSURFACE_BITMAP. Declaration only.
 CSurfaceLockedRect ReadOnlyLock(unsigned long flags) const;            // 0x009F3010
#endif
#ifdef FABLE_RETAIL_CSURFACE_BITMAP
 // Retail opt-in (chunk 858, row 0x00494900 CFrontendGameComponent::Output): Ego_r.pdb ?GetDimensions@CSurface@@QBE?AVC2DExtentsI@@XZ
 // (0x00B46C70 = the landed retail 0x009F2F90, hidden result) and ?LoadFromBitmapFileToPreallocatedSurface@CSurface@@QAE_NABVCWideString@@K@Z
 // (0x00B46AA0 = the landed retail 0x009F2DD0). Needs C2DExtentsI / CWideString complete at the call. Declarations only.
 C2DExtentsI GetDimensions() const;                       // 0x009F2F90
 bool LoadFromBitmapFileToPreallocatedSurface(const class CWideString& filename, unsigned long filter);   // 0x009F2DD0
 // FableWin.pdb ?GetWidth@CSurface@@QBEKXZ / ?GetHeight@CSurface@@QBEKXZ (debug only, inline in release: Output 0x00494900 calls
 // GetDimensions 0x009F2F90 once per extent and keeps the one field). Needs C2DExtentsI complete before the include.
#ifdef FABLE_RETAIL_CSURFACE_SIZE_OUTOFLINE
 // Retail opt-in (agentNF20, 2026-11-11, rows 0x00415BD0 / 0x00415BF0): retail also keeps out-of-line copies (`sub esp,8 ; push
 // the temporary ; call 0x009F2F90 ; mov eax,[eax] / [eax+4]`), Ego_r.pdb (0x00414F70 / 0x00414F90) + FableWin.pdb ?GetWidth@CSurface@@QBEKXZ / ?GetHeight@CSurface@@QBEKXZ;
 // CMainGameComponent::AppReinitFunc 0x004167DA calls GetHeight 0x00415BF0. Declarations only; the rows define them.
 unsigned long GetWidth() const;                          // 0x00415BD0
 unsigned long GetHeight() const;                         // 0x00415BF0
#else
#ifdef FABLE_RETAIL_CSURFACE_GETWIDTH_FORCEINLINE
 // Per-TU inline opt-in (agentNF20, 2026-11-11, row 0x004167DA CMainGameComponent::AppReinitFunc): the size-optimised TU expands
 // GetWidth in place (its C2DExtentsI temporary keeps its own slot) while it calls the out-of-line GetHeight 0x00415BF0.
 FABLE_FORCEINLINE
#endif
 unsigned long GetWidth() const { return GetDimensions().X; }
#ifdef FABLE_RETAIL_CSURFACE_GETHEIGHT_OUTOFLINE
 // Per-TU opt-in (agentNF20, 2026-11-11, row 0x004167DA): the call to the out-of-line copy 0x00415BF0 (see SIZE_OUTOFLINE above).
 unsigned long GetHeight() const;                         // 0x00415BF0
#else
 unsigned long GetHeight() const { return GetDimensions().Y; }
#endif
#endif
#endif
#ifdef FABLE_RETAIL_CSURFACE_PIXELFORMAT
 // chunk 751 (row 0x009BEE40): Ego_r.pdb ?GetPixelFormat@CSurface@@QBE?AVCPixelFormat@@XZ (0x009F2F60, by value through the hidden
 // pointer). Needs CPixelFormat complete (engine/CPixelFormat.h) before the include.
 CPixelFormat GetPixelFormat() const;
#endif
#ifdef FABLE_RETAIL_CSURFACE_CLEAR
 // Retail opt-in (Codex pass 17): original SDK interface and PDB methods, complete native fields unchanged.
 CSurfaceLockedRect Lock(unsigned long);
 void Clear();
 void Unlock()const{PD3DSurface->UnlockRect();}
 unsigned long GetByteLength()const {
  _D3DSURFACE_DESC desc;PD3DSurface->GetDesc(&desc);
  return GetPixelFormat().GetColourDepth()*desc.Width*desc.Height/8;
 }
#endif
#ifdef FABLE_RETAIL_CSURFACE_SWIZZLE
 // Retail opt-in (agentNF20, 2026-11-11, rows 0x009F4330 / 0x009F4360): Ego_r.pdb (0x00B48030 / 0x00B48060, the order of retail) +
 // FableWin.pdb ?Swizzle@CSurface@@QAEXK@Z / ?Unswizzle@CSurface@@QAEXK@Z; the PC bodies only lock (Lock 0x009F33E0) and Unlock.
 // Needs FABLE_RETAIL_CSURFACE_CLEAR (Lock / Unlock). Declarations only.
 void Swizzle(unsigned long lockFlags);                   // 0x009F4330
 void Unswizzle(unsigned long lockFlags);                 // 0x009F4360
#endif
private:
 IDirect3DSurface9* PD3DSurface;
 unsigned long AllocationSource;
 void* PAllocatedMemory;
};
#endif
#else
#ifndef FABLE_ENGINE_CSURFACE_H
#define FABLE_ENGINE_CSURFACE_H
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CSurface = 0x10.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

struct IDirect3DSurface9;

#pragma pack(push, 1)
class CSurface {
public:
    void*              __vftable;                            // +0x00 vptr, or first dword of a flattened base subobject
    IDirect3DSurface9* PD3DSurface;                          // +0x04
    unsigned long      AllocationSource;                     // +0x08
    void*              PAllocatedMemory;                     // +0x0c
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CSurface) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CSurface, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CSurface, PD3DSurface) == 0x4);
FABLE_STATIC_ASSERT(offsetof(CSurface, AllocationSource) == 0x8);
FABLE_STATIC_ASSERT(offsetof(CSurface, PAllocatedMemory) == 0xc);

#endif // FABLE_ENGINE_CSURFACE_H

#endif
