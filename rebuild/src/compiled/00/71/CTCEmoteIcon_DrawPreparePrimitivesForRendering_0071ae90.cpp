// CTCEmoteIcon::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) -- while the emote is active, enabled, the GUI allows it and the parent is drawable, draw it as an icon (type 1) or text (type 0); otherwise release the emote primitive
// Leaf: DrawPreparePrimitivesForRendering
//
// Retail 0x0071AE90, 147 bytes (`ret 4`). Ego_r.pdb `?DrawPreparePrimitivesForRendering@CTCEmoteIcon@@UAEXABV
// CEnginePrimitivePrepareInfo@@@Z`. Callees: the player GUI's 0x004374F0 (manifest
// "C3DMeshInfo::HasPhysicsMesh" is propagated; it returns `[this+294h] != 1`) on g_pPlayerGui @
// 0x013B8790, the parent thing's virtual slot 27 (+0x6c, a bool), CTCEmoteIcon::DrawEmoteTypeIcon @
// 0x0071AD20 and DrawEmoteTypeText @ 0x0071AB20; the primitive's virtual slots 3 and 1 in the release.
// Layout = Ego_r.pdb CTCEmoteIcon (0x30, no retail shift): ActiveTimer +0xc, EmoteType +0x1c, Enabled
// +0x26, PrimitiveHandle +0x28; the handle model and its inline Release() are the landed CTCEmoteIcon
// FrameUpdate's (0x0071AF30).
// Shape: one `&&` condition, two typed draw calls each with its own `ret 4`, every other path falls
// into the inline handle release (`this` dead, so esi becomes the handle: `add esi,28h`).
#include "engine/CTCBase.h"

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

class CEnginePrimitivePrepareInfo;

class CPlayerGui { public:
    bool IsEmoteDrawingAllowed() const;            // 0x004374F0
};
extern CPlayerGui* g_pPlayerGui;                   // 0x013B8790

class CThing { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26();
    virtual bool IsDrawable();                     // slot 27 (+0x6c)
};

class CTCEmoteIcon : public CTCBase { public:
    long          ActiveTimer;                     // +0x0c
    void*         EmoteText;                       // +0x10 CWideString
    long          EmoteIcon;                       // +0x14
    float         EmoteScale;                      // +0x18
    long          EmoteType;                       // +0x1c EEmoteType
    long          EmotePriority;                   // +0x20 EEmotePriority
    unsigned char EmoteAlpha;                      // +0x24
    bool          Replaceable;                     // +0x25
    bool          Enabled;                         // +0x26
    CEnginePrimitiveHandle PrimitiveHandle;        // +0x28
    void DrawEmoteTypeText(const CEnginePrimitivePrepareInfo& info);   // 0x0071AB20
    void DrawEmoteTypeIcon(const CEnginePrimitivePrepareInfo& info);   // 0x0071AD20
    void DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info);
};

void CTCEmoteIcon::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info)
{
    if (ActiveTimer != 0 && Enabled && g_pPlayerGui->IsEmoteDrawingAllowed() && ParentThing->IsDrawable())
    {
        if (EmoteType == 1)
        {
            DrawEmoteTypeIcon(info);
            return;
        }
        if (EmoteType == 0)
        {
            DrawEmoteTypeText(info);
            return;
        }
    }
    PrimitiveHandle.Release();
}