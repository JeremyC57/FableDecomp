// CEnginePrimitiveManagerSprites::AddSprite(CTexture* texture, const C3DVector& position, float sizeX, float sizeY, float angle, EEnginePrimitiveBlendMode blendMode, unsigned char alpha, bool lit, bool sprite3D, float offsetX, float offsetY, bool persistent) -- create a sprite primitive (half sizes, offsets as fractions of the half sizes, bank/frame 0, non-persistent unless asked), register it with the manager and return its handle
// Leaf: ?AddSprite@CEnginePrimitiveManagerSprites@@QAE?AVCEnginePrimitiveHandle@@PAVCTexture@@ABVC3DVector@@MMMW4EEnginePrimitiveBlendMode@@E_N3MM3@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BAE160, length 364 bytes, `ret 34h` (hidden result + 12 dwords).
// Name: FableWin.pdb `?AddSprite@CEnginePrimitiveManagerSprites@@QAE?AVCEnginePrimitiveHandle@@PAVCTexture@@ABVC3DVector@@MMMW4EEngine
//   PrimitiveBlendMode@@E_N3MM3@Z` -- the 12 parameters + hidden result fit `ret 34h`, and each lands on the Ego_r.pdb member of the
//   created object, whose vtable 0x012A5444 is RTTI CEngineInternalPrimitiveSprite. Label correction: the catalog label
//   "CParticleMorphs_CEntry_Constructor" (_global) is wrong. Parameter names are descriptive (the PDB name set carries none).
// Callees: operator new 0x00BFEA1A (0x88 bytes), CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase(long, unsigned long)
//   0x00B8FF00 (landed; type 7, render-layer mask 0x200000 from the sprite's inline default constructor), CBaseClass::CBaseClass
//   0x0099A2D0 / ~CBaseClass 0x0099A2E0 (the handle's base), CEnginePrimitiveManager::AddPrimitive(CEngineInternalPrimitiveBase&,
//   unsigned long) 0x00BACDD0 (FableWin name; catalog "_Cons_val<...CParticleMorphs::CEntry...>"); the primitive's virtual slots
//   3 (+0x0C, CEnginePersistentPrimitiveBase::NotifyReleased(const CEnginePrimitiveHandle&)) and 1 (+0x04,
//   CIVCountedPointeeBase::Delete() const). Vtable 0x0122F598 CEnginePrimitiveHandle. Constant 0.5f @ 0x0122F59C.
// Inlines (FableWin.pdb names): CEnginePrimitiveHandle ctor / copy ctor / dtor -> Clear(); CIVCountedPointer<CEnginePersistent
//   PrimitiveBase> copy ctor, IsNull, GetRefCount, Release, Reset (adopts: no AddRef, as the landed SetData 0x00B38780);
//   CIVCountedPointeeBase::AddRef / Release / GetRefCount. Shape: the handle dtor must be __forceinline (VC7.1 otherwise calls an
//   out-of-line copy); the pointee's Release() decrements, tests its own count and calls Delete() (`mov edi,[ecx+4] ; lea eax,
//   [ecx+4] ; dec edi ; mov edx,edi ; test edx,edx`); the stores follow the source order WorldPos, sizes, Texture, Angle,
//   BlendMode (Angle/BlendMode/Texture or texture-first orders schedule the loads differently).
// Layout: Ego_r.pdb CEngineInternalPrimitiveBase (Alpha +0x27, Enable/NonPersistent bits +0x34) and CEngineInternalPrimitiveSprite
//   (WorldPos +0x48, BankIndex +0x54, Texture +0x58, Frame +0x5C, SizeX/SizeY +0x60/+0x64, OffsetFactorX/Y +0x68/+0x6C, Angle +0x74,
//   BlendMode +0x78, Lit/Sprite3D bits 1/2 of +0x84); CIVCountedPointeeBase IVRefCount +0x04. The flattened engine headers
//   carry the vptr as a data member, so the polymorphic classes are modelled locally. Slot 2 of CEnginePersistentPrimitiveBase
//   has no recovered name (placeholder kept only for the vtable position); the type-7 enumerator likewise (literal).
class CTexture;
class C3DVector { public: float X, Y, Z; };
enum EEnginePrimitiveBlendMode { };
class CEnginePrimitiveHandle;
class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase();                  // slot 0
    virtual void Delete() const;                       // slot 1 (+0x04)
    mutable unsigned long IVRefCount;                  // +0x04
    void AddRef() const { ++IVRefCount; }
    long GetRefCount() const { return IVRefCount; }
    long Release() const { long count = --IVRefCount; if (count == 0) Delete(); return count; }
};
class CEnginePersistentPrimitiveBase : public CIVCountedPointeeBase { public:
    long Type;                                         // +0x08
    virtual void v002();                               // slot 2 (no recovered name)
    virtual void NotifyReleased(const CEnginePrimitiveHandle& handle);   // slot 3 (+0x0c)
    virtual bool WasRendered() const;                  // slot 4
};
template <class T> class CIVCountedPointer { public:
    T* Object;
    CIVCountedPointer() : Object(0) { }
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object) { if (Object) Object->AddRef(); }
    ~CIVCountedPointer() { Release(); }
    T* operator->() const { return Object; }
    bool IsNull() const { return Object == 0; }
    unsigned long GetRefCount() const { return Object->GetRefCount(); }
    void Release()
    {
        if (Object)
            Object->Release();
        Object = 0;
    }
    void Reset(T* object) { Release(); Object = object; }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy
class CEnginePrimitiveHandle : public CBaseClass { public:   // vtable 0x0122F598
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;  // +0x04
    CEnginePrimitiveHandle() { }
    CEnginePrimitiveHandle(const CEnginePrimitiveHandle& other) : CBaseClass(other), Data(other.Data) { }
    __forceinline ~CEnginePrimitiveHandle() { Clear(); }
    void Clear()
    {
        if (!Data.IsNull() && Data.GetRefCount() == 1)
            Data->NotifyReleased(*this);
        Data.Release();
    }
};
class CEngineInternalPrimitiveBase : public CEnginePersistentPrimitiveBase { public:
    unsigned long  EnginePrimitiveFlags;               // +0x0c
    float          AlphaMipBias;                       // +0x10
    unsigned long  FadeDistance[2];                    // +0x14
    unsigned long  OcclusionLayerBitMask;              // +0x1c
    void*          GridCell;                           // +0x20
    unsigned char  GridPos[3];                         // +0x24
    unsigned char  Alpha;                              // +0x27
    unsigned char  AlphaRef;                           // +0x28
    unsigned char  PreloadStat;                        // +0x29
    long           InterpolationSet;                   // +0x2c
    unsigned long  RenderLayerMask;                    // +0x30
    unsigned char  Enable : 1;                         // +0x34
    unsigned char  NonPersistent : 1;
    unsigned char  OverrideFadeDistance : 1;
    unsigned char  PostRenderEnable : 1;
    unsigned short LastFrameRendered;                  // +0x36
    void*          NextPrimitive;                      // +0x38
    void*          RefPrimitive;                       // +0x3c
    void*          NextLayerMask;                      // +0x40
    void*          RefLayerMask;                       // +0x44
    CEngineInternalPrimitiveBase(long type, unsigned long renderLayerMask);   // 0x00B8FF00
};
class CEngineInternalPrimitiveSprite : public CEngineInternalPrimitiveBase { public:   // vtable 0x012A5444
    C3DVector                 WorldPos;                // +0x48
    unsigned long             BankIndex;               // +0x54
    CTexture*                 Texture;                 // +0x58
    unsigned long             Frame;                   // +0x5c
    float                     SizeX;                   // +0x60
    float                     SizeY;                   // +0x64
    float                     OffsetFactorX;           // +0x68
    float                     OffsetFactorY;           // +0x6c
    float                     BSphereRadius;           // +0x70
    float                     Angle;                   // +0x74
    EEnginePrimitiveBlendMode BlendMode;               // +0x78
    long                      BlendOp;                 // +0x7c
    unsigned long             Colour;                  // +0x80
    unsigned char             Modulate2X : 1;          // +0x84
    unsigned char             Lit : 1;
    unsigned char             Sprite3D : 1;
    unsigned char             RotateAroundCentre : 1;
    unsigned char             RotateAroundZAxis : 1;
    unsigned char             IsParticle : 1;
    unsigned char             ZBuffer : 1;
    CEngineInternalPrimitiveSprite() : CEngineInternalPrimitiveBase(7, 0x200000) { }
    virtual ~CEngineInternalPrimitiveSprite();
};
class CEnginePrimitiveManager { public:
    void AddPrimitive(CEngineInternalPrimitiveBase& primitive, unsigned long flags);   // 0x00BACDD0
};
class CEnginePrimitiveManagerSprites : public CEnginePrimitiveManager { public:
    CEnginePrimitiveHandle AddSprite(CTexture* texture, const C3DVector& position, float sizeX, float sizeY, float angle,
                                     EEnginePrimitiveBlendMode blendMode, unsigned char alpha, bool lit, bool sprite3D,
                                     float offsetX, float offsetY, bool persistent);
};
CEnginePrimitiveHandle CEnginePrimitiveManagerSprites::AddSprite(CTexture* texture, const C3DVector& position, float sizeX, float sizeY, float angle,
                                                                 EEnginePrimitiveBlendMode blendMode, unsigned char alpha, bool lit, bool sprite3D,
                                                                 float offsetX, float offsetY, bool persistent)
{
    CEngineInternalPrimitiveSprite* sprite = new CEngineInternalPrimitiveSprite;
    sprite->WorldPos = position;
    sprite->SizeX = sizeX * 0.5f;
    sprite->SizeY = sizeY * 0.5f;
    sprite->Texture = texture;
    sprite->Angle = angle;
    sprite->BlendMode = blendMode;
    sprite->Alpha = alpha;
    sprite->Lit = lit;
    sprite->Sprite3D = sprite3D;
    sprite->NonPersistent = !persistent;
    sprite->BankIndex = 0;
    sprite->Frame = 0;
    sprite->OffsetFactorX = offsetX / sprite->SizeX;
    sprite->OffsetFactorY = offsetY / sprite->SizeY;
    CEnginePrimitiveHandle handle;
    handle.Data.Reset(sprite);
    AddPrimitive(*sprite, 0);
    return handle;
}