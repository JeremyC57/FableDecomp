// CThingBuilding::OnCreate() -- hands the def's graphic record and the building's scale to the graphic appearance component, clears its +0xD4 bit 0, runs the game-object creation and, when that succeeds, seeds health and maximum health from the def
// Leaf: OnCreate
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082D6F0, 218 bytes (+6 int3). `ret`, bool in al. CThingBuilding vtable 0x0124509C slot 8 (+0x20).
//         Catalog name CThingBuilding::vfunc_8 (CThing::OnCreate 0x004C9560 is slot 8).
// Callees: CThing::GetDefinitionManager() const 0x004C7990, CDefinitionManager::GetDef(long, CDefPointer<const CThingBuildingDef>*)
//          0x004AB840 (x2; manifest "GetDef<CThingPhysicalSwitchDef>" propagated), CDefClassBase virtual slot 1 (Delete), CTCGraphic
//          Appearance::SetScale(float) 0x004BFA50, CThingGameObject::OnCreate() 0x00838A40.
// Shape: the appearance pointer is loaded into ebx before the first PeekPDef (inlined: zeroed holder, GetDef, release, raw read);
//        the 14-byte graphic record is copied as three dwords and a word through `add edi,78h`; the def's max health is read
//        after the second PeekPDef and stored to Health then MaxHealth (`fst` / `fstp`, `MaxHealth = Health = ...`); the
//        success path is the `if (OnCreateGameObject()) { ... }` block so the false return is laid out last.
//        GetDef gets a same-TU `auto_inline(off)` stand-in body that writes only the out slot: VC7.1 then knows the holder
//        does not escape and keeps the def in edi from the call through the release to the member reads, as retail.
// Layout: CThing PTCGraphicAppearance +0x64, DefGlobalIndex +0x8C; CThingPhysical Scale +0x9C, MaxHealth +0xB0, Health +0xB4
//         (Ego_r.pdb +0xC8/+0xCC); the +0xD4 flag byte (descriptive); CTCGraphicAppearance::Graphic +0x44; CThingBuildingDef
//         MaxHealth +0x50 and Graphic +0x78 (PDB +0x7C, def classes pack to 1).
class CDefClassBase { public:
    virtual void v00();
    virtual void Delete();                           // slot 1 (+0x04)
    long RefCount;                                   // +0x04
};
#pragma pack(push, 1)
class CEngineGraphic { public:                              // 14 bytes
    long  Bank;
    long  Index;
    long  Flags;
    short Lod;
};
class CThingBuildingDef : public CDefClassBase { public:
    unsigned char  _pad08[0x50 - 0x08];
    float          MaxHealth;                        // +0x50 (CThingPhysicalDef)
    unsigned char  _pad54[0x78 - 0x54];
    CEngineGraphic Graphic;                          // +0x78
};
#pragma pack(pop)
struct CDefPointer {                                 // CDefPointer<const CThingBuildingDef>
    CThingBuildingDef* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        CDefClassBase* old = Object;                  // released through the unpacked base (the def classes pack to 1)
        if (old) {
            if (--old->RefCount == 0)
                old->Delete();
        }
    }
};
class CDefinitionManager { public:
    bool GetDef_004ab840(long index, CDefPointer* out);       // 0x004AB840
};
void* FindDef_CDefinitionManager_GetDef(long index);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef_004ab840(long index, CDefPointer* out)
{
    void* def = FindDef_CDefinitionManager_GetDef(index);
    if (!def)
        return false;
    out->Object = (CThingBuildingDef*)def;
    return true;
}
#pragma auto_inline(on)
struct CTCGraphicAppearance {
    unsigned char  _pad00[0x44];
    CEngineGraphic Graphic;                          // +0x44
    void SetScale(float scale);                      // 0x004BFA50
};
class CThingBuilding { public:
    unsigned char         _pad00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;      // +0x64
    unsigned char         _pad68[0x8c - 0x68];
    unsigned short        DefGlobalIndex;            // +0x8c
    unsigned char         _pad8e[0x9c - 0x8e];
    float                 Scale;                     // +0x9c
    unsigned char         _pada0[0xb0 - 0xa0];
    float                 MaxHealth;                 // +0xb0
    float                 Health;                    // +0xb4
    unsigned char         _padb8[0xd4 - 0xb8];
    unsigned char         m_flagD4 : 1;              // +0xd4 bit 0 (descriptive)
    CDefinitionManager* GetDefinitionManager() const;   // 0x004C7990
    __forceinline const CThingBuildingDef* PeekPDef() const
    {
        CDefPointer def;
        GetDefinitionManager()->GetDef_004ab840(DefGlobalIndex, &def);
        return def.Object;
    }
    bool OnCreateGameObject();                       // 0x00838A40 CThingGameObject::OnCreate (called non-virtually)
    bool OnCreate();
};
bool CThingBuilding::OnCreate()
{
    CTCGraphicAppearance* appearance = PTCGraphicAppearance;
    appearance->Graphic = PeekPDef()->Graphic;
    appearance->SetScale(Scale);
    m_flagD4 = false;
    if (OnCreateGameObject()) {
        MaxHealth = Health = PeekPDef()->MaxHealth;
        return true;
    }
    return false;
}