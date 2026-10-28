// test for 0x00BAE160 CEnginePrimitiveManagerSprites::AddSprite -- fields, flags, registration, handle reference counting
#include <stdio.h>
#include <string.h>
#ifndef _MSC_VER
#define __forceinline inline
#endif
static int g_deleted = 0, g_notified = 0, g_added = 0;
static void* g_addedPrim = 0;
static unsigned long g_addedFlags = 99;
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
class CBaseClass { public:
    CBaseClass();                                      // 0x0099A2D0
    CBaseClass(const CBaseClass&) { }
    virtual ~CBaseClass();                             // 0x0099A2E0
};
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

CIVCountedPointeeBase::~CIVCountedPointeeBase() { }
void CIVCountedPointeeBase::Delete() const { ++g_deleted; delete this; }
void CEnginePersistentPrimitiveBase::v002() { }
void CEnginePersistentPrimitiveBase::NotifyReleased(const CEnginePrimitiveHandle&) { ++g_notified; }
bool CEnginePersistentPrimitiveBase::WasRendered() const { return false; }
CBaseClass::CBaseClass() { }
CBaseClass::~CBaseClass() { }
CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase(long type, unsigned long renderLayerMask)
{
    IVRefCount = 1; Type = type; RenderLayerMask = renderLayerMask; Enable = 1; NonPersistent = 0; Alpha = 0xff;
}
CEngineInternalPrimitiveSprite::~CEngineInternalPrimitiveSprite() { }
void CEnginePrimitiveManager::AddPrimitive(CEngineInternalPrimitiveBase& primitive, unsigned long flags)
{
    ++g_added; g_addedPrim = &primitive; g_addedFlags = flags;
}
int main()
{
    int ok = 1;
    static CEnginePrimitiveManagerSprites manager;
    C3DVector pos; pos.X = 1.0f; pos.Y = 2.0f; pos.Z = 3.0f;
    CTexture* tex = (CTexture*)&pos;
    {
        CEnginePrimitiveHandle h = manager.AddSprite(tex, pos, 4.0f, 8.0f, 0.25f, (EEnginePrimitiveBlendMode)3, 0x80, true, false, 1.0f, 2.0f, false);
        CEngineInternalPrimitiveSprite* s = (CEngineInternalPrimitiveSprite*)h.Data.Object;
        if (!s || g_added != 1 || g_addedPrim != s || g_addedFlags != 0) ok = 0;
        if (s) {
            if (s->WorldPos.X != 1.0f || s->WorldPos.Y != 2.0f || s->WorldPos.Z != 3.0f) ok = 0;
            if (s->SizeX != 2.0f || s->SizeY != 4.0f || s->OffsetFactorX != 0.5f || s->OffsetFactorY != 0.5f) ok = 0;
            if (s->Texture != tex || s->Angle != 0.25f || s->BlendMode != 3 || s->Alpha != 0x80) ok = 0;
            if (!s->Lit || s->Sprite3D || !s->NonPersistent || !s->Enable) ok = 0;
            if (s->BankIndex != 0 || s->Frame != 0 || s->Type != 7 || s->RenderLayerMask != 0x200000) ok = 0;
            if (s->IVRefCount != 1) ok = 0;   // the local handle adopted the creation reference, the result took one, the local released one
        }
        if (g_notified != 0 || g_deleted != 0) ok = 0;
    }
    // the returned handle was the last: notified, then deleted
    if (g_notified != 1 || g_deleted != 1) ok = 0;
    {
        CEnginePrimitiveHandle h = manager.AddSprite(tex, pos, 2.0f, 2.0f, 0.0f, (EEnginePrimitiveBlendMode)0, 0xff, false, true, 0.0f, 0.0f, true);
        CEngineInternalPrimitiveSprite* s = (CEngineInternalPrimitiveSprite*)h.Data.Object;
        if (!s || s->Lit || !s->Sprite3D || s->NonPersistent) ok = 0;
    }
    if (!ok) { printf("00bae160_TEST FAIL\n"); return 1; }
    printf("00bae160_TEST PASS\n");
    return 0;
}