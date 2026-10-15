// CThingBuilding::UsesHighDetailQuadTree() -- the building def's UseHighDetailQuadTree bit (virtual slot 57)
// Grade: DIFFER(74v66) -- one extra instruction: the detach store `mov dword ptr [esp+4],0` (a dead store to the address-taken handle) that makes the lane load the object straight into esi and release with `dec dword ptr [esi+4]` exactly like retail; without the store the release becomes the load/dec/store form (76v66)
// Leaf: UsesHighDetailQuadTree
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082D870, 66 bytes.
// Callees: CThing::GetDefinitionManager @ 0x004C7990 (propagated label "CViewBase::GetDisplayEngine"),
//          CDefinitionManager::GetDef<CThingBuildingDef> @ 0x004AB840 (propagated "<CThingPhysicalSwitchDef>"), the def's
//          Destroy (vtable slot 1) on the last release.
// Layout: CThing::DefGlobalIndex (unsigned short) at retail +0x8c; PDB CThingBuildingDef has WallMaterial +0x8c,
//   FloorMaterial +0x90, ReverbEnvironmentType +0x94, ReverbEnvironmentLevel +0x98, the flag bits at +0xa4: retail
//   reads all of them 4 bytes lower (one STL container narrower in a base).
// Shape: an inline GetBuildingDef() builds a CDefPointer local (zeroed after the argument pushes), fills it through
//   the manager and returns the RAW object pointer, so the local's release (`dec [esi+4] ; jne ; call [eax+4]`, the
//   counted release living on the def object) runs BEFORE the field read (the landed wave-24m rule).
// Identity: CThingBuilding vtable 0x0124509C slot 57; the `mov al ; and al,1` getter is an unsigned char bitfield.
struct CDefPointeeBase {                // counted def object: vptr, reference count at +4
    virtual void v00();
    virtual void Destroy();             // slot 1
    long m_refCount;                    // +0x04
    void Release()
    {
        if (--m_refCount == 0)
            Destroy();
    }
};

struct CThingBuildingDef : CDefPointeeBase {    // retail offsets = PDB - 4
    unsigned char _pad08[0x80];
    long WallMaterial;                  // +0x88 (PDB +0x8c)
    long FloorMaterial;                 // +0x8c (PDB +0x90)
    long ReverbEnvironmentType;         // +0x90 EReverbEnvironmentType (PDB +0x94)
    long ReverbEnvironmentLevel;        // +0x94 float, moved as a dword (PDB +0x98)
    long OutsideOcclusionLevel;         // +0x98 float
    long CameraManagerSetIndex;         // +0x9c
    unsigned char UseHighDetailQuadTree : 1;    // +0xa0 bit 0 (PDB +0xa4)
    unsigned char IsSelfIlluminating : 1;
    unsigned char IsPartOfHeroGuild : 1;
    unsigned char HasWeatherMask : 1;
};

template<class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
};

struct CDefinitionManager {
    bool GetDef(long globalIndex, CDefPointer<CThingBuildingDef>* out);   // 0x004AB840 (GetDef<CThingBuildingDef>)
};

struct CThingBuilding {
    unsigned char _pad00[0x8c];
    unsigned short DefGlobalIndex;      // +0x8c (CThing; PDB +0x9c)
    CDefinitionManager* GetDefinitionManager();     // 0x004C7990 (CThing)
    __forceinline const CThingBuildingDef* GetBuildingDef()
    {
        CDefPointer<CThingBuildingDef> def;
        GetDefinitionManager()->GetDef(DefGlobalIndex, &def);
        CThingBuildingDef* object = def.Object;
        def.Object = 0;
        if (object)
            object->Release();
        return object;
    }
    unsigned char UsesHighDetailQuadTree();
};

unsigned char CThingBuilding::UsesHighDetailQuadTree()
{
    return GetBuildingDef()->UseHighDetailQuadTree;
}
