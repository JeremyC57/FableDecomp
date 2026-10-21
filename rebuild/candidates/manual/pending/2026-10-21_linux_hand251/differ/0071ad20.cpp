// Grade: DIFFER(365v365) -- register choice only: retail's temporaries are rotated against ours throughout (draw distance via edx not ecx, camera vtable eax not edx, sprite scale edx / alpha ecx / icon ecx not ecx / eax / eax); tried inline getters (16 combos), expression-order / inline range helper, const& / scoped locals, replace helper, QFE
// CTCEmoteIcon::DrawEmoteTypeIcon(const CEnginePrimitivePrepareInfo& info) -- within the emote icon draw distance of the camera, (re)place the emote's persistent sprite half a unit above the thing's static focal point (icon bank = EmoteIcon, square size EmoteScale, alpha = EmoteAlpha); out of range, release the sprite
// Leaf: DrawEmoteTypeIcon
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071AD20, 368 bytes (`ret 4`).
// Callees: CTCBase::GetCamera @ 0x00686A80, CTCBase::GetDefinitions @ 0x006869D0, CThing::GetPosition @ 0x004C73D0, the camera's
// virtual slot 8 (+0x20, position by value), CTCGraphicAppearance::GetStaticFocalPosIfLoaded(C3DVector*) @ 0x004C11F0,
// CEnginePrimitiveSprite ctor (15 words) @ 0x006558CE, the prepare info's engine (+0x34) virtual slot 23 (+0x5c,
// SetPersistentPrimitive(handle&, primitive, 0)), the persistent primitive's virtual slots 3 and 1 in the inline handle release.
// Globals: 0.5f @ 0x0122F59C. The draw distance is the definitions' +0xd0 def's float at +0x130 (named descriptively).
// Layout = Ego_r.pdb CTCEmoteIcon (as the landed DrawPreparePrimitivesForRendering 0x0071AE90 / DrawEmoteTypeText 0x0071AB20):
// EmoteIcon +0x14, EmoteScale +0x18, EmoteAlpha +0x24, PrimitiveHandle +0x28; CThing appearance TC +0x64.
// Shape: camera, distance and position are named locals fetched in that order; `GetLengthSquared() < d * d` (Z*Z first);
// the old sprite is released only when the handle is valid (`IsValid()` then the inline Release()).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
    float GetLengthSquared() const { float s = Z * Z; s += Y * Y; s += X * X; return s; }
};

class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1 (+0x04)
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3 (+0x0C)
    long RefCount;                                                   // +0x04
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};
class CIVCountedPointer {                          // CIVCountedPointer<CEnginePersistentPrimitiveBase>
public:
    CEnginePersistentPrimitiveBase* Object;
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    __forceinline CIVCountedPointer& operator=(int)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = 0;
        return *this;
    }
};
class CEnginePrimitiveHandle {                     // 8 bytes: vptr (0x0122F598) + Data
public:
    virtual void v000();
    CIVCountedPointer Data;                        // +0x04
    bool IsValid() const { return Data.Object != 0; }
    __forceinline void Release()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
        Data = 0;
    }
};

struct CEnginePrimitiveSprite {                    // 0x48 bytes
    unsigned char _data[0x48];
    CEnginePrimitiveSprite(const C3DVector& pos, unsigned long bank, unsigned long frame, unsigned short flags, float angle,
                           float sizeX, float sizeY, long blendMode, unsigned char a, unsigned char r, unsigned char g,
                           unsigned char b, float fadeStart, long interpolationSet, long blendOp);   // 0x006558CE
};
struct CIEngine {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22();
    virtual void SetPersistentPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitiveSprite& sprite, long flags);   // slot 23 (+0x5c)
};
struct CEnginePrimitivePrepareInfo {
    unsigned char _pad00[0x34];
    CIEngine* Engine;                              // +0x34
};
struct CCamera {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual C3DVector GetPosition() const;         // slot 8 (+0x20)
};
struct CTCGraphicAppearance {
    bool GetStaticFocalPosIfLoaded(C3DVector* pos);   // 0x004C11F0
};
struct CThing {
    unsigned char _pad00[0x64];
    CTCGraphicAppearance* PGraphicAppearance;      // +0x64
    const C3DVector& GetPosition() const;          // 0x004C73D0
};
struct CEmoteDisplayDef {
    unsigned char _pad00[0x130];
    float EmoteIconDrawDistance;                   // +0x130
};
struct CDefinitions {
    unsigned char _pad00[0xd0];
    CEmoteDisplayDef* PDisplayDef;                 // +0xd0
};

struct CTCEmoteIcon {
    virtual void v00();
    CThing*       ParentThing;                     // +0x04 (CTCBase)
    unsigned char _pad08[0x4];
    long          ActiveTimer;                     // +0x0c
    void*         EmoteText;                       // +0x10 CWideString
    long          EmoteIcon;                       // +0x14
    float         EmoteScale;                      // +0x18
    long          EmoteType;                       // +0x1c
    long          EmotePriority;                   // +0x20
    unsigned char EmoteAlpha;                      // +0x24
    bool          Replaceable;                     // +0x25
    bool          Enabled;                         // +0x26
    CEnginePrimitiveHandle PrimitiveHandle;        // +0x28
    const CCamera* GetCamera() const;              // 0x00686A80 (CTCBase)
    const CDefinitions* GetDefinitions();          // 0x006869D0 (CTCBase)
    void DrawEmoteTypeIcon(const CEnginePrimitivePrepareInfo& info);
};

void CTCEmoteIcon::DrawEmoteTypeIcon(const CEnginePrimitivePrepareInfo& info)
{
    const CCamera* camera = GetCamera();
    float drawDistance = GetDefinitions()->PDisplayDef->EmoteIconDrawDistance;
    const C3DVector& pos = ParentThing->GetPosition();
    if ((camera->GetPosition() - pos).GetLengthSquared() < drawDistance * drawDistance) {
        C3DVector focal;
        if (ParentThing->PGraphicAppearance->GetStaticFocalPosIfLoaded(&focal)) {
            focal.Z += 0.5f;
            CEnginePrimitiveSprite sprite(focal, EmoteIcon, 0, 0x90, 0.0f, EmoteScale, EmoteScale, 3, EmoteAlpha, 255, 255, 255,
                                          0.0f, 0, 0);
            if (PrimitiveHandle.IsValid())
                PrimitiveHandle.Release();
            info.Engine->SetPersistentPrimitive(PrimitiveHandle, sprite, 0);
        }
    } else if (PrimitiveHandle.IsValid())
        PrimitiveHandle.Release();
}
