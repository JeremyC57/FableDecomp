// NGraphicAppearance::CTCGraphicAppearance::IsDrawingAfterWater() -- the own DrawAfterWater bit, else (when the owner has interface 0x47 with a bound thing) that thing's def DrawAfterWater bit
// Grade: DIFFER(93v91) -- after the lookup retail loads the component straight into ecx and the holder (+0x10) into eax
//        (`mov ecx,[eax+4] ; mov eax,[ecx+10h] ; test eax,eax`); the lane loads the component into eax, the holder into ecx, then
//        `mov ecx,eax` before the getter call (+2 bytes). Tried: the out-parameter GetInterface template (plain, `__forceinline`),
//        HasTC + pointer-returning FindTC, the holder test through an inline member.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BD3C0, length 91 bytes. Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound
// @ 0x004365B0, the component's bound-thing getter @ 0x00705170 (`m_pHolder ? m_pHolder->m_pObject : 0`; manifest
// "CGuiControlListBox::GetCurrentSelectedEntryData", propagated).
// Owner side (retail CThing): the interface presence bitset +0x20 (type 0x47 = word 2 bit 7: `test cl,cl ; jns` on byte +0x28),
// the sorted interface map {first, last} +0x44, PBaseDef +0x70 -> CThingBaseDef flag byte +0x43 (DrawAfterWater = bit 1,
// Ego_r.pdb +0x47 bool:1 list: DisableOcclusionTesting, DrawAfterWater, ...).
// Shape: the frame word (`push ecx`) holds the lookup key; the owner's inline HasTC (raw masked word) then a
// pointer-returning inline FindTC, which loads the component straight into ecx (the out-parameter GetInterface template of
// 0x007504E0 leaves it in eax here); the component's holder pointer (+0x10) is tested inline before the out-of-line getter; the def bit is
// returned as a one-bit `bool` bit-field (`shr al,1 ; and al,1`).
// Layout (Ego_r.pdb CTCGraphicAppearance, 116 bytes; retail 0x5c: the two STLport maps 12 bytes, the two STLport lists one
// header-node pointer each): CTCBase +0x00..+0x0b (ParentThing +0x04), MeshEffects +0x0c, TintColours +0x18, PTCAnimation +0x24,
// CachedStaticFocalPosOffset +0x28, Scale +0x34, Primitives +0x38, Components +0x3c, LightingChannel +0x40, MainGraphic +0x44,
// ActivePersistenceFlags +0x52, FrameUpdatedWasRenderedFlag +0x54, UpdatePrimitiveReasons +0x58, the flag bits +0x59
// (SelfIlluminating 0, ForceResendEveryRenderFrame 1, ResendPrimsEveryRenderFrame 2, DisableOcclusionTesting 3, DrawAfterWater 4,
// CachedStaticFocalPos 5, RenderedLastFrame 6, PrimitivesDrawn 7), Alpha +0x5a, ByPassColorFilter +0x5b.
// No engine header exists for CTCGraphicAppearance: local model.
struct CTCAnimationBase;

struct CThing;

struct CTCBase {
    unsigned char _pad_0x00[0x10];
    void*         m_pHolder;                            // +0x10 the bound thing's object-pointer holder
    CThing* GetBoundThing();                            // 0x00705170
};

struct CInterfaceMap {                                  // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x004365B0
};

struct CThingBaseDef {
    unsigned char _pad_0x00[0x43];
    bool          DisableOcclusionTesting : 1;          // +0x43 bit 0 (retail)
    bool          DrawAfterWater : 1;                   //        bit 1
};

struct CThing {
    unsigned char  _pad_0x00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20 std::bitset<274> UsingComponents
    CInterfaceMap  m_interfaces;                        // +0x44 Components
    unsigned char  _pad_0x4c[0x24];
    CThingBaseDef* PBaseDef;                            // +0x70
    unsigned long HasTC(long type) const                // inline: the raw masked bitset word
    {
        return m_presentBits[type >> 5] & (1UL << (type & 31));
    }
    CTCBase* FindTC(long type)                          // inline: the map lookup, pointer-returning
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        if (it == map->last || type < it->type)
            it = map->last;
        return it->component;
    }
};

namespace NGraphicAppearance {

struct CAGraphicAppearanceComponent;

struct CEnginePersistentPrimitiveBase {                 // CIVCountedPointer<CEnginePersistentPrimitiveBase> target
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual bool WasRenderedLastFrame();                // slot 4 (+0x10)
};

struct CPrimitive {                                     // 16 bytes (Ego_r.pdb)
    long                            Type;               // +0x00 EPrimitiveType
    long                            PrimitiveHandlePad; // +0x04 CEnginePrimitiveHandle (first word)
    CEnginePersistentPrimitiveBase* PrimitiveData;      // +0x08 CEnginePrimitiveHandle::Data
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

struct C3DVector { float X, Y, Z; };

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
    bool IsDrawingAfterWater();
};

bool CTCGraphicAppearance::IsDrawingAfterWater()
{
    if (DrawAfterWater)
        return true;
    if (ParentThing->HasTC(0x47)) {
        CTCBase* component = ParentThing->FindTC(0x47);
        if (component->m_pHolder)
            return component->GetBoundThing()->PBaseDef->DrawAfterWater;
    }
    return false;
}

}
