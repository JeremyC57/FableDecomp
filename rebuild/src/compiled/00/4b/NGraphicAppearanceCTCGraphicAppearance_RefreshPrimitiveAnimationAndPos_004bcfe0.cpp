// NGraphicAppearance::CTCGraphicAppearance::RefreshPrimitiveAnimationAndPos(CPrimitive* primitive, const C3DAnimationInterpolator& animation, const C3DPositionInterpolator& position, const C3DOrientationInterpolator& orientation) -- hand the primitive manager a temporary CEnginePrimitiveAnimation built from the three interpolators
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BCFE0, length 92 bytes. Callees: CEnginePrimitiveAnimation::CEnginePrimitiveAnimation
// @ 0x004C3D30, the primitive manager's virtual slot 24 (+0x60), the interpolator base destructor @ 0x0099A2E0 (manifest
// "CBase::CBase", propagated; run for the orientation and position members), C3DAnimationInterpolator::~C3DAnimationInterpolator
// @ 0x004C2E90. The manager is +0x34 of the engine object the global @ 0x013B8A18 points to.
// Layout (Ego_r.pdb CEnginePrimitiveAnimation, 100 bytes): Anim (C3DAnimationInterpolator, 20) +0x00, WorldPos
// (C3DPositionInterpolator, 28) +0x14, Orientation (C3DOrientationInterpolator, 52) +0x30.
// Shape: one statement -- the virtual call with the animation temporary built in the 0x64-byte frame; VC fetches the
// manager's vtable (esi) before building the temporary and re-reads the manager into ecx after it; the temporary's implicit
// destructor runs its members' destructors in reverse order.
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

class CBaseClass { public:                              // vptr only; out-of-line destructor
    virtual ~CBaseClass();                               // 0x0099A2E0
};

class CRightHandedSet { public: float m[6]; };

class C3DAnimationInterpolator { public:                       // 20 bytes
    void*         __vftable;
    unsigned long StartState[2];                        // C3DAnimationBlendState
    unsigned long EndState[2];
    ~C3DAnimationInterpolator();                        // 0x004C2E90
};

class C3DPositionInterpolator { public:                        // 28 bytes
    CBaseClass m_base;
    C3DVector         StartPosition;
    C3DVector         EndPosition;
};

class C3DOrientationInterpolator { public:                     // 52 bytes
    CBaseClass m_base;
    CRightHandedSet   StartOrientation;
    CRightHandedSet   EndOrientation;
};

class CEnginePrimitiveAnimation { public:                      // 100 bytes
    C3DAnimationInterpolator   Anim;                    // +0x00
    C3DPositionInterpolator    WorldPos;                // +0x14
    C3DOrientationInterpolator Orientation;             // +0x30
    CEnginePrimitiveAnimation(const C3DAnimationInterpolator& anim, const C3DPositionInterpolator& position,
                              const C3DOrientationInterpolator& orientation);   // 0x004C3D30
};

class CEnginePrimitiveManager { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void SetPrimitiveAnimation(NGraphicAppearance::CEnginePrimitiveHandle* primitive,
                                       const CEnginePrimitiveAnimation& animation);   // slot 24 (+0x60)
};

struct CEngineGlobals {
    unsigned char            _pad_0x00[0x34];
    CEnginePrimitiveManager* m_pPrimitiveManager;       // +0x34
};

extern CEngineGlobals* g_pEngineGlobals_013b8a18;                // 0x013B8A18

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
    void RefreshPrimitiveAnimationAndPos(CPrimitive* primitive, const C3DAnimationInterpolator& animation,
                                         const C3DPositionInterpolator& position, const C3DOrientationInterpolator& orientation);
};

void CTCGraphicAppearance::RefreshPrimitiveAnimationAndPos(CPrimitive* primitive, const C3DAnimationInterpolator& animation,
                                                           const C3DPositionInterpolator& position, const C3DOrientationInterpolator& orientation)
{
    g_pEngineGlobals_013b8a18->m_pPrimitiveManager->SetPrimitiveAnimation(&primitive->PrimitiveHandle,
                                                                 CEnginePrimitiveAnimation(animation, position, orientation));
}

}