// CTCDegradableBase::ChangeMesh(const CEngineGraphic&) -- install the stage's graphic as the parent's main appearance graphic, flag the appearance changed, and rebuild the physics mesh info from it
// Leaf: ChangeMesh
//
// Retail 0x008EC9C0, 135 bytes (`ret 4`). Ego_r.pdb `?ChangeMesh@CTCDegradableBase@@AAEXABVCEngineGraphic@@@Z`.
// Callees: CTCGraphicAppearance::SetAsChanged(unsigned long) @ 0x004BF9E0, CVectorMap<ETCInterfaceType,
// CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface for interface 2, bit 2 of +0x20),
// CTCPhysicsBase::CreatePPhysicsMeshInfo(const CEngineGraphic&) @ 0x006B08E0.
// Layout: CEngineGraphic is the PDB's 14-byte record (BankIndex, AnimStep, RenderSizeX, AdditiveAlpha,
// Type; packed to 2, so the implicit copy is three dwords and a word); retail CThing keeps its
// graphic-appearance component at +0x64; retail CTCGraphicAppearance MainGraphic +0x44 (PDB +0x5c: the
// containers ahead of it are smaller in retail).
// Shape: the physics component comes from a `__forceinline` helper around the out-parameter
// GetInterface whose uninitialised out pointer shares the dead argument slot with the lookup key, so
// the absent path calls through whatever the slot holds (`mov eax,[esp+0Ch]`) and both arms keep their
// own call and `ret 4`.
#pragma pack(push, 2)
struct CEngineGraphic {
    unsigned long BankIndex;                                // +0x00
    float         AnimStep;                                 // +0x04
    float         RenderSizeX;                              // +0x08
    bool          AdditiveAlpha;                            // +0x0c
    unsigned char Type;                                     // +0x0d
};
#pragma pack(pop)

struct CTCBase;

struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

struct CTCGraphicAppearance {
    unsigned char  _pad00[0x44];
    CEngineGraphic MainGraphic;                             // +0x44
    void SetAsChanged(unsigned long reasons);               // 0x004BF9E0
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* PGraphicAppearance;               // +0x64
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CTCPhysicsBase {
    void CreatePPhysicsMeshInfo(const CEngineGraphic& graphic);   // 0x006B08E0
};

__forceinline CTCPhysicsBase* GetPhysics(CThing* thing)
{
    CTCPhysicsBase* physics;
    thing->GetInterface(&physics, 2);                       // TC_PHYSICS
    return physics;
}

struct CTCDegradableBase {
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    void ChangeMesh(const CEngineGraphic& graphic);
};

void CTCDegradableBase::ChangeMesh(const CEngineGraphic& graphic)
{
    ParentThing->PGraphicAppearance->MainGraphic = graphic;
    ParentThing->PGraphicAppearance->SetAsChanged(4);
    GetPhysics(ParentThing)->CreatePPhysicsMeshInfo(graphic);
}
