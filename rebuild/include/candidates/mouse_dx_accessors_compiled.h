#ifndef FABLETLC_MOUSE_DX_ACCESSORS_COMPILED_H
#define FABLETLC_MOUSE_DX_ACCESSORS_COMPILED_H

#include <stddef.h>
#include "rebuild_abi.h"

class C2DVector
{
public:
    float X;
    float Y;
};

struct CMousePoint { long X; long Y; };
struct CMouseRect { long Left; long Top; long Right; long Bottom; };

class CMousePrimitiveOwner
{
public:
    fable_u8 Unknown00[0x94];
    void* Window;
};

class CMousePositionContext
{
public:
    CMousePrimitiveOwner* ResolvePrimitiveOwner();
};

#ifdef FABLE_RETAIL_CMOUSEDX_MOUSECURSOR
// Retail opt-in (agentNF27, 2026-11-12, row 0x00AB5390 CMouseDX::SetMouseCursor): MouseCursor is Ego_r.pdb's
// CCountedPointer<CSurface> (engine/CCountedPointer.h; Data +0, Info +4) and its out-of-line Reset 0x00AB5E80 is FableWin.pdb
// ?Reset@?$CCountedPointer@VCSurface@@@@QAEXPAVCSurface@@@Z (declared only: FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL), in place of
// the stand-in CCountedSurfacePointer; CSurface's words carry their Ego_r.pdb names (vptr slot +0, PD3DSurface +4, AllocationSource
// +8, PAllocatedMemory +0xC; the row passes Data->PD3DSurface to the cursor primitive). Same size and offsets as the default.
#define FABLE_RETAIL_CCOUNTEDPOINTER_RESET_CAMERA_DECL
#include "engine/CCountedPointer.h"
class CSurface
{
public:
    void* _padding_;                                                     // +0x0 (the vptr word)
    void* PD3DSurface;                                                   // +0x4 IDirect3DSurface9*
    fable_u32 AllocationSource;                                          // +0x8
    void* PAllocatedMemory;                                              // +0xC
    CSurface(const CSurface& other);
};
#else
class CSurface
{
public:
    fable_u8 Storage[0x10];
    CSurface(const CSurface& other);
};
#endif

struct CCountedSurfaceRep
{
    fable_u32 Unknown00;
    void* Value;
};

class CCountedSurfacePointer
{
public:
    CCountedSurfaceRep* Rep;
    void Reset(CSurface* surface);
};

class CMouseEnginePrimitive;
typedef void (__stdcall *CMousePrimitiveSlot28)(
    CMouseEnginePrimitive* primitive, int zeroA, int zeroB, void* cursorSurface);
typedef void (__stdcall *CMousePrimitiveSlot30)(CMouseEnginePrimitive* primitive, int enabled);

struct CMousePrimitiveDispatch
{
    fable_u8 Unknown00[0x28];
    CMousePrimitiveSlot28 SetCursorSurface;
    fable_u8 Unknown2C[4];
    CMousePrimitiveSlot30 SetCursorEnabled;
};

class CMouseEnginePrimitive
{
public:
    CMousePrimitiveDispatch* Dispatch;
};

struct CMousePrimitiveSlotOwner
{
    fable_u8 Unknown00[0x58];
    CMouseEnginePrimitive* Primitive;
};

struct CMousePrimitiveHierarchy
{
    fable_u8 Unknown00[0x60];
    CMousePrimitiveSlotOwner* Owner;
};

CMousePrimitiveHierarchy* ResolveMousePrimitiveHierarchy();

typedef int (__stdcall *CMouseClientToScreenFunction)(void*, CMousePoint*);
typedef int (__stdcall *CMouseGetClientRectFunction)(void*, CMouseRect*);
typedef int (__stdcall *CMouseOffsetRectFunction)(CMouseRect*, int, int);
typedef int (__stdcall *CMouseSetCursorPosFunction)(int, int);

extern CMouseClientToScreenFunction FableClientToScreen;
extern CMouseGetClientRectFunction FableGetClientRect;
extern CMouseOffsetRectFunction FableOffsetRect;
extern CMouseSetCursorPosFunction FableSetCursorPos;

class CMouseDX
{
public:
    virtual void GetPos(C2DVector* output) const;
    virtual void SetPos(const C2DVector* position);
    const C2DVector& PeekPos() const;
    void SetInputOn(bool inputOn);
    void SetRecentringOn(bool recentringOn);
    bool IsRecentringOn() const;
    void SetDrawCursor(bool drawCursor);
    bool IsDrawingCursor() const;
    void SetMouseFiltering(bool filtering);
    bool IsLMBDown() const;
    bool IsMMBDown() const;
    bool IsRMBDown() const;
    void SetMouseCursor(const CSurface& surface);

    fable_u8 Unknown004[0x3410];
    float PositionX;
    float PositionY;
    fable_u8 Unknown341C[0x18];
    float CachedPositionX;
    float CachedPositionY;
    fable_u8 DisableCursorWarp;
    bool InputOn;
    bool RecentringOn;
    bool DrawCursor;
    fable_u8 Unknown3440[0x1408];
    bool LMBDown;
    bool MMBDown;
    bool RMBDown;
    bool MouseFiltering;
    fable_u8 Unknown484C[0x0C];
#ifdef FABLE_RETAIL_CMOUSEDX_MOUSECURSOR
    CCountedPointer<CSurface> MouseCursor;
#else
    CCountedSurfacePointer MouseCursor;
#endif
};

FABLE_STATIC_ASSERT(sizeof(C2DVector) == 8);
FABLE_STATIC_ASSERT(offsetof(CMouseDX, PositionX) == 0x3414);
FABLE_STATIC_ASSERT(offsetof(CMouseDX, PositionY) == 0x3418);
FABLE_STATIC_ASSERT(offsetof(CMouseDX, CachedPositionX) == 0x3434);
FABLE_STATIC_ASSERT(offsetof(CMouseDX, CachedPositionY) == 0x3438);
FABLE_STATIC_ASSERT(offsetof(CMouseDX, DisableCursorWarp) == 0x343C);
FABLE_STATIC_ASSERT(offsetof(CMouseDX, InputOn) == 0x343D);
FABLE_STATIC_ASSERT(offsetof(CMouseDX, RecentringOn) == 0x343E);
FABLE_STATIC_ASSERT(offsetof(CMouseDX, DrawCursor) == 0x343F);
FABLE_STATIC_ASSERT(offsetof(CMouseDX, LMBDown) == 0x4848);
FABLE_STATIC_ASSERT(offsetof(CMouseDX, MMBDown) == 0x4849);
FABLE_STATIC_ASSERT(offsetof(CMouseDX, RMBDown) == 0x484A);
FABLE_STATIC_ASSERT(offsetof(CMouseDX, MouseFiltering) == 0x484B);
FABLE_STATIC_ASSERT(offsetof(CMouseDX, MouseCursor) == 0x4858);
FABLE_STATIC_ASSERT(sizeof(CSurface) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CCountedSurfaceRep, Value) == 0x04);
FABLE_STATIC_ASSERT(offsetof(CMousePrimitiveDispatch, SetCursorSurface) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CMousePrimitiveDispatch, SetCursorEnabled) == 0x30);
FABLE_STATIC_ASSERT(offsetof(CMousePrimitiveSlotOwner, Primitive) == 0x58);
FABLE_STATIC_ASSERT(offsetof(CMousePrimitiveHierarchy, Owner) == 0x60);
FABLE_STATIC_ASSERT(offsetof(CMousePrimitiveOwner, Window) == 0x94);

#endif
