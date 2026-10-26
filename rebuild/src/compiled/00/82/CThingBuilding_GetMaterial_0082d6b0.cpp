// CThingBuilding::GetMaterial() -- the building def's wall material (virtual slot 70)
// Leaf: ?GetMaterial@CThingBuilding
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082D6B0, 64 bytes.
// Callees: CThing::GetDefinitionManager @ 0x004C7990 (propagated label "CViewBase::GetDisplayEngine"),
//          CDefinitionManager::GetDef<CThingBuildingDef> @ 0x004AB840 (propagated "<CThingPhysicalSwitchDef>"), the def's
//          Destroy (vtable slot 1) on the last release.
// Layout: CThing::DefGlobalIndex (unsigned short) at retail +0x8c; PDB CThingBuildingDef has WallMaterial +0x8c,
//   FloorMaterial +0x90, ReverbEnvironmentType +0x94, ReverbEnvironmentLevel +0x98, the flag bits at +0xa4: retail
//   reads all of them 4 bytes lower (one STL container narrower in a base).
// Shape: an inline GetBuildingDef() builds a CDefPointer local (zeroed after the argument pushes), fills it through
//   the manager and returns the RAW object pointer, so the local's release (`dec [esi+4] ; jne ; call [eax+4]`, the
//   counted release living on the def object) runs BEFORE the field read (the landed wave-24m rule).
// Identity: CThingBuilding vtable 0x0124509C slot 70 (rtti-vtables.tsv); the slot's name is not in the PDB dump, the
//   descriptive name follows the CThingObjectDef "Material" field it serves for other things.
class CDefPointeeBase { public:                // counted def object: vptr, reference count at +4
    virtual void v00();
    virtual void Destroy();             // slot 1
    long m_refCount;                    // +0x04
    void Release()
    {
        if (--m_refCount == 0)
            Destroy();
    }
};

class CThingBuildingDef : public CDefPointeeBase { public:    // retail offsets = PDB - 4
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

template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        T* object = Object;
        if (object) {
            long& count = object->m_refCount;
            if (--count == 0)
                object->Destroy();
        }
    }
};

class CDefinitionManager { public:
    bool GetDef(long globalIndex, CDefPointer<CThingBuildingDef>* out);   // 0x004AB840 (GetDef<CThingBuildingDef>)
};
void* FindDef_CDefinitionManager_GetDef_0();   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only inline, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long globalIndex, CDefPointer<CThingBuildingDef>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)

class CThingBuilding { public:
    unsigned char _pad00[0x8c];
    unsigned short DefGlobalIndex;      // +0x8c (CThing; PDB +0x9c)
    CDefinitionManager* GetDefinitionManager();     // 0x004C7990 (CThing)
    const CThingBuildingDef* GetBuildingDef()
    {
        CDefPointer<CThingBuildingDef> def;
        GetDefinitionManager()->GetDef(DefGlobalIndex, &def);
        return def.Object;
    }
    long GetMaterial();
};

long CThingBuilding::GetMaterial()
{
    return GetBuildingDef()->WallMaterial;
}