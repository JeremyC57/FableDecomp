// NGraphicAppearance::CTCGraphicAppearance::ClearAllMeshEffects() -- walk the mesh-effect map and clear every effect that was inherited from a carrier
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BFF60, length 96 bytes. Callees: CTCGraphicAppearance::ClearMeshEffect(void* key)
// @ 0x004BFE20 (it erases the entry, so the iterator is advanced first).
// Layout: MeshEffects (+0x0c) is an STLport map<void*, CMeshEffectPriorityEntry> whose first word is the allocated header node;
// STLport node: colour +0x00, parent +0x04, left +0x08, right +0x0c, value pair +0x10 (key +0x10, CMeshEffectPriorityEntry
// (Ego_r.pdb, 12 bytes): PEffect +0x14, Priority +0x18, EffectsCarriedItems +0x1c, EffectsWeapons +0x1d, InheritedFromCarrier +0x1e).
// Shape: `cur = it; ++it;` with the STLport `_M_increment` inlined (right subtree's leftmost, else climb while the node is its
// parent's right child, then the header guard `if (node->right != parent) node = parent`); the end (header) re-read at the bottom
// because of the call; cur in ecx, it in esi. Declaring `cur = begin` first and then `it.m_node = cur` (cur also the
// loop variable, `cur = it` at the bottom) puts begin in ecx and copies it to esi before the entry test, as retail does.
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

struct CMeshEffectPriorityEntry {                       // 12 bytes (Ego_r.pdb)
    void*         PEffect;                              // +0x00 CIVCountedPointer<CEngineMeshEffect>
    long          Priority;                             // +0x04 EAppearanceMeshEffectPriority
    bool          EffectsCarriedItems;                  // +0x08
    bool          EffectsWeapons;                       // +0x09
    bool          InheritedFromCarrier;                 // +0x0a
};

struct SMeshEffectNode {                                // STLport _Rb_tree_node<pair<void* const, CMeshEffectPriorityEntry> >
    long                     m_color;                   // +0x00
    SMeshEffectNode*         m_parent;                  // +0x04
    SMeshEffectNode*         m_left;                    // +0x08
    SMeshEffectNode*         m_right;                   // +0x0c
    void*                    m_key;                     // +0x10
    CMeshEffectPriorityEntry m_value;                   // +0x14
};

struct CMeshEffectMapIterator {
    SMeshEffectNode* m_node;
    void Increment()                                    // STLport _Rb_tree_base_iterator::_M_increment
    {
        if (m_node->m_right != 0) {
            m_node = m_node->m_right;
            while (m_node->m_left != 0)
                m_node = m_node->m_left;
        } else {
            SMeshEffectNode* parent = m_node->m_parent;
            while (m_node == parent->m_right) {
                m_node = parent;
                parent = parent->m_parent;
            }
            if (m_node->m_right != parent)
                m_node = parent;
        }
    }
};

struct CMeshEffectMap {                                 // STLport map (12 bytes): the header-node pointer first
    SMeshEffectNode* m_header;
    unsigned long    m_reserved;
    unsigned long    m_count;
};

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
    CMeshEffectMap MeshEffects;                         // +0x0c
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
    void ClearMeshEffect(void* key);                    // 0x004BFE20
    void ClearAllMeshEffects();
};

void CTCGraphicAppearance::ClearAllMeshEffects()
{
    SMeshEffectNode* cur = MeshEffects.m_header->m_left;
    CMeshEffectMapIterator it;
    it.m_node = cur;
    while (cur != MeshEffects.m_header) {
        it.Increment();
        if (cur->m_value.InheritedFromCarrier)
            ClearMeshEffect(cur->m_key);
        cur = it.m_node;
    }
}

}
