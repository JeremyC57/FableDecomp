// NGraphicAppearance::CTCGraphicAppearance::Draw(const CEnginePrimitivePrepareInfo& info, const C3DPositionInterpolator& position, const C3DOrientationInterpolator& orientation) -- mark the primitives drawn, optionally latch the force-resend bit, and re-prepare the primitives when anything is pending
// Leaf: ?Draw@CTCGraphicAppearance@NGraphicAppearance@@QAEXABVCEnginePrimitivePrepareInfo@@ABVC3DPositionInterpolator@@ABVC3DOrientationInterpolator@@@Z
// Identity: Ego_r.pdb ?Draw@CTCGraphicAppearance@NGraphicAppearance@@QAEXABVCEnginePrimitivePrepareInfo@@ABVC3DPositionInterpolator@@
// ABVC3DOrientationInterpolator@@@Z (the three-argument overload of the landed Draw(info) 0x004BE4E0); its tail call is
// the protected SetPrimitive(info, position, orientation) with the same three const references (Ego_r.pdb), and the
// inline reasons getter is the PDB's protected GetUpdatePrimitiveReasons() const. Callers CThingObject
// DrawPreparePrimitivesForRendering (0x00703751) and a CThingMarker virtual (0x0083D41F) pass their own interpolators.
// "PrepareForRenderIfNeeded" was a forge autoname with no PDB counterpart.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BE490, length 69 bytes. Callees: CTCGraphicAppearance::SetPrimitive @ 0x004BD6E0
// (the three arguments forwarded: a tail jump). Globals: two engine bytes at 0x013B8A14 (latch ForceResendEveryRenderFrame into
// ResendPrimsEveryRenderFrame) and 0x01375480 (primitive preparation enabled).
// Shape: PrimitivesDrawn set through a register copy of the flag byte (load / or 80h / store) that the latch reuses
// (`shl dl,1 ; xor ; and dl,4 ; xor`: bit 1 -> bit 2); the pending test is an inline
// `unsigned long` reasons getter (a constant when the reasons byte is zero but ResendPrimsEveryRenderFrame is set, else the
// byte widened at the return): the byte is tested first, the non-zero path re-tests the widened value (`jne ; ...
// movzx eax,al ; test eax,eax ; je`), the constant path jumps straight to the call.
// Layout (Ego_r.pdb CTCGraphicAppearance, 116 bytes; retail 0x5c: the two STLport maps 12 bytes, the two STLport lists one
// header-node pointer each): CTCBase +0x00..+0x0b (ParentThing +0x04), MeshEffects +0x0c, TintColours +0x18, PTCAnimation +0x24,
// CachedStaticFocalPosOffset +0x28, Scale +0x34, Primitives +0x38, Components +0x3c, LightingChannel +0x40, MainGraphic +0x44,
// ActivePersistenceFlags +0x52, FrameUpdatedWasRenderedFlag +0x54, UpdatePrimitiveReasons +0x58, the flag bits +0x59
// (SelfIlluminating 0, ForceResendEveryRenderFrame 1, ResendPrimsEveryRenderFrame 2, DisableOcclusionTesting 3, DrawAfterWater 4,
// CachedStaticFocalPos 5, RenderedLastFrame 6, PrimitivesDrawn 7), Alpha +0x5a, ByPassColorFilter +0x5b.
// No engine header exists for CTCGraphicAppearance: local model.
class C3DVector { public: float X, Y, Z; };

class CTCAnimationBase;

class CThing { public:
    void AddToRenderList();                             // 0x004C7AC0 (manifest CThing::AddToRenderList)
};

namespace NGraphicAppearance {

class CAGraphicAppearanceComponent;

struct CEnginePersistentPrimitiveBase {                 // CIVCountedPointer<CEnginePersistentPrimitiveBase> target
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual bool WasRenderedLastFrame();                // slot 4 (+0x10)
};

struct CEnginePrimitiveHandle {                         // 8 bytes (Ego_r.pdb): a leading word, then the counted data pointer
    long                            m_header;           // +0x00
    CEnginePersistentPrimitiveBase* Data;               // +0x04 CIVCountedPointer<CEnginePersistentPrimitiveBase>
};

class CPrimitive { public:                                     // 16 bytes (Ego_r.pdb)
    long                            Type;               // +0x00 EPrimitiveType
    CEnginePrimitiveHandle          PrimitiveHandle;    // +0x04
    long                            BankIndex;          // +0x0c
    ~CPrimitive();                                      // 0x004C4160 (manifest "CObject::~CObject")
};

}


struct CPrimitiveListNode {
    CPrimitiveListNode*            m_next;              // +0x00
    CPrimitiveListNode*            m_prev;              // +0x04
    NGraphicAppearance::CPrimitive m_value;             // +0x08
};

struct CPrimitiveList {                                 // STLport list<CPrimitive>: the header-node pointer
    CPrimitiveListNode* m_head;
    bool empty() const { return m_head->m_next == m_head; }
};

struct CComponentListNode {
    CComponentListNode*                              m_next;    // +0x00
    CComponentListNode*                              m_prev;    // +0x04
    NGraphicAppearance::CAGraphicAppearanceComponent* m_value;  // +0x08
};

struct CComponentList {                                 // STLport list<CAGraphicAppearanceComponent*>: the header-node pointer
    CComponentListNode* m_head;
};

class CEnginePrimitivePrepareInfo;
class C3DPositionInterpolator;
class C3DOrientationInterpolator;

extern bool g_LatchForceResendPrimitives;               // 0x013B8A14
extern bool g_PreparePrimitivesEnabled;                 // 0x01375480

#pragma pack(push, 2)
class CEngineGraphic { public:                                 // 14 bytes (PDB size 14)
    unsigned long BankIndex;                            // +0x00
    float         AnimStep;                             // +0x04
    float         RenderSizeX;                          // +0x08
    bool          AdditiveAlpha;                        // +0x0c
    unsigned char Type;                                 // +0x0d
};
#pragma pack(pop)

namespace NGraphicAppearance {

class CTCGraphicAppearance { public:
    void*          __vftable;                           // +0x00 (CTCBase)
    CThing*        ParentThing;                         // +0x04
    bool           ToBeRemoved;                         // +0x08
    bool           IsInGlobalUpdateTCs;                 // +0x09
    unsigned char  MeshEffects[0x0c];                   // +0x0c map<void*, CMeshEffectPriorityEntry> (STLport)
    unsigned char  TintColours[0x0c];                   // +0x18 map<void*, CRGBColour> (STLport)
    CTCAnimationBase* PTCAnimation;                     // +0x24
    C3DVector      CachedStaticFocalPosOffset;          // +0x28
    float          Scale;                               // +0x34
    CPrimitiveList Primitives;                          // +0x38 list<CPrimitive> (header-node pointer)
    CComponentList Components;                          // +0x3c list<CAGraphicAppearanceComponent*> (header-node pointer)
    unsigned long  LightingChannel;                     // +0x40 CEnginePrimitiveThemeChannelDesc
    CEngineGraphic MainGraphic;                         // +0x44
    unsigned short ActivePersistenceFlags;              // +0x52
    long           FrameUpdatedWasRenderedFlag;         // +0x54
    unsigned char  UpdatePrimitiveReasons;              // +0x58
    unsigned char  SelfIlluminating : 1;                // +0x59 bit 0
    unsigned char  ForceResendEveryRenderFrame : 1;     //        bit 1
    unsigned char  ResendPrimsEveryRenderFrame : 1;     //        bit 2
    unsigned char  DisableOcclusionTesting : 1;         //        bit 3
    unsigned char  DrawAfterWater : 1;                  //        bit 4
    unsigned char  CachedStaticFocalPos : 1;            //        bit 5
    unsigned char  RenderedLastFrame : 1;               //        bit 6
    unsigned char  PrimitivesDrawn : 1;                 //        bit 7
    unsigned char  Alpha;                               // +0x5a
    signed char    ByPassColorFilter;                   // +0x5b
    unsigned long GetUpdatePrimitiveReasons() const     // inline: a resend-every-frame appearance always has a reason
    {
        if (!UpdatePrimitiveReasons && ResendPrimsEveryRenderFrame)
            return 1;
        return UpdatePrimitiveReasons;
    }
    void SetPrimitive(const CEnginePrimitivePrepareInfo& info, const C3DPositionInterpolator& position,
                      const C3DOrientationInterpolator& orientation);   // 0x004BD6E0
    void Draw(const CEnginePrimitivePrepareInfo& info, const C3DPositionInterpolator& position,
              const C3DOrientationInterpolator& orientation);
};

void CTCGraphicAppearance::Draw(const CEnginePrimitivePrepareInfo& info, const C3DPositionInterpolator& position,
                                const C3DOrientationInterpolator& orientation)
{
    PrimitivesDrawn = 1;
    if (g_LatchForceResendPrimitives)
        ResendPrimsEveryRenderFrame = ForceResendEveryRenderFrame;
    if (g_PreparePrimitivesEnabled && GetUpdatePrimitiveReasons())
        SetPrimitive(info, position, orientation);
}

}