// NGraphicAppearance::CTCGraphicAppearance::SetAttachedPrimitiveGraphics(CPrimitive* primitive, CEnginePrimitive* attachTo, CEnginePrimitiveAttachmentInfo* info, CPrimitive* parent) -- attach a primitive to its parent through the engine's primitive manager with the colour-filter-bypass / after-water draw flags
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BD690, length 78 bytes. Callees: CTCGraphicAppearance::IsDrawingAfterWater
// @ 0x004BD3C0 (this chunk), the primitive manager's virtual slot 32 (+0x80; six arguments). The manager is +0x34 of the engine
// object the global @ 0x013B8A18 points to (the landed RefreshPrimitiveAnimationAndPos neighbour reaches it the same way).
// Shape: the flags are an `unsigned short` local built first (0x800 when ByPassColorFilter > 0, `| 8` when drawing after water)
// and widened to the 32-bit parameter at the push (`movzx eax,si ; push eax`); the two primitives are passed by their handle (+4) addresses;
// the manager and its vtable are fetched before the argument pushes.
// Layout (Ego_r.pdb CTCGraphicAppearance, 116 bytes; retail 0x5c: the two STLport maps 12 bytes, the two STLport lists one
// header-node pointer each): CTCBase +0x00..+0x0b (ParentThing +0x04), MeshEffects +0x0c, TintColours +0x18, PTCAnimation +0x24,
// CachedStaticFocalPosOffset +0x28, Scale +0x34, Primitives +0x38, Components +0x3c, LightingChannel +0x40, MainGraphic +0x44,
// ActivePersistenceFlags +0x52, FrameUpdatedWasRenderedFlag +0x54, UpdatePrimitiveReasons +0x58, the flag bits +0x59
// (SelfIlluminating 0, ForceResendEveryRenderFrame 1, ResendPrimsEveryRenderFrame 2, DisableOcclusionTesting 3, DrawAfterWater 4,
// CachedStaticFocalPos 5, RenderedLastFrame 6, PrimitivesDrawn 7), Alpha +0x5a, ByPassColorFilter +0x5b.
// No engine header exists for CTCGraphicAppearance: local model.
struct C3DVector { float X, Y, Z; };

struct CTCAnimationBase;

struct CThing {
    void AddToRenderList();                             // 0x004C7AC0 (manifest CThing::AddToRenderList)
};

namespace NGraphicAppearance {

struct CAGraphicAppearanceComponent;

struct CEnginePersistentPrimitiveBase {                 // CIVCountedPointer<CEnginePersistentPrimitiveBase> target
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual bool WasRenderedLastFrame();                // slot 4 (+0x10)
};

struct CEnginePrimitiveHandle {                         // 8 bytes (Ego_r.pdb): a leading word, then the counted data pointer
    long                            m_header;           // +0x00
    CEnginePersistentPrimitiveBase* Data;               // +0x04 CIVCountedPointer<CEnginePersistentPrimitiveBase>
};

struct CPrimitive {                                     // 16 bytes (Ego_r.pdb)
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

struct CEnginePrimitive;
struct CEnginePrimitiveAttachmentInfo;

struct CEnginePrimitiveManager {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31();
    virtual void AttachPrimitive(NGraphicAppearance::CEnginePrimitiveHandle* primitive, CEnginePrimitive* attachTo,
                                 CEnginePrimitiveAttachmentInfo* info, NGraphicAppearance::CEnginePrimitiveHandle* parent,
                                 long boneIndex, unsigned long flags);    // slot 32 (+0x80)
};

struct CEngineGlobals {
    unsigned char            _pad_0x00[0x34];
    CEnginePrimitiveManager* m_pPrimitiveManager;       // +0x34
};

extern CEngineGlobals* g_pEngineGlobals;                // 0x013B8A18

#pragma pack(push, 2)
struct CEngineGraphic {                                 // 14 bytes (PDB size 14)
    unsigned long BankIndex;                            // +0x00
    float         AnimStep;                             // +0x04
    float         RenderSizeX;                          // +0x08
    bool          AdditiveAlpha;                        // +0x0c
    unsigned char Type;                                 // +0x0d
};
#pragma pack(pop)

namespace NGraphicAppearance {

struct CTCGraphicAppearance {
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
    bool IsDrawingAfterWater();                         // 0x004BD3C0
    void SetAttachedPrimitiveGraphics(CPrimitive* primitive, CEnginePrimitive* attachTo, CEnginePrimitiveAttachmentInfo* info, CPrimitive* parent);
};

void CTCGraphicAppearance::SetAttachedPrimitiveGraphics(CPrimitive* primitive, CEnginePrimitive* attachTo, CEnginePrimitiveAttachmentInfo* info, CPrimitive* parent)
{
    unsigned short flags = 0;
    if (ByPassColorFilter > 0)
        flags = 0x800;
    if (IsDrawingAfterWater())
        flags |= 8;
    g_pEngineGlobals->m_pPrimitiveManager->AttachPrimitive(&primitive->PrimitiveHandle, attachTo, info, &parent->PrimitiveHandle, -1, flags);
}

}