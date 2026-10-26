// NGraphicAppearance::CTCGraphicAppearance::RemovePrimitive(CPrimitive* primitive) -- find the list node holding this primitive, unlink it, destroy the primitive and free the node
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BF920, length 62 bytes. Callees: CPrimitive::~CPrimitive @ 0x004C4160 (manifest
// "CObject::~CObject", propagated), free @ 0x00BFEA14 (the STLport node allocator).
// Shape: the Primitives list (STLport header-node pointer +0x38) walked comparing each node's value ADDRESS (`lea edx,[esi+8]`)
// with the argument; the header stays in eax (no call in the loop); the inlined STLport `erase` (next read first in the source -- the
// reverse declaration order swaps eax/ecx) loads prev into ecx and next into eax, links
// prev->next and next->prev, destroys the value and frees the node; a miss falls out of the loop with its own epilogue.
#include <stdlib.h>
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
    void RemovePrimitive(CPrimitive* primitive);
};

void CTCGraphicAppearance::RemovePrimitive(CPrimitive* primitive)
{
    for (CPrimitiveListNode* node = Primitives.m_head->m_next; node != Primitives.m_head; node = node->m_next) {
        if (&node->m_value == primitive) {
            CPrimitiveListNode* next = node->m_next;
            CPrimitiveListNode* prev = node->m_prev;
            prev->m_next = next;
            next->m_prev = prev;
            node->m_value.~CPrimitive();
            free(node);
            return;
        }
    }
}

}