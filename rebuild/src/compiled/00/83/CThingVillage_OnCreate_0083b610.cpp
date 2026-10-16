// CThingVillage::OnCreate() -- add the graphic-appearance and static-mesh components, copy the village's current graphic into the appearance component, run the game-object OnCreate and give the village 100 MaxHealth
// Leaf: OnCreate@CThingVillage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0083B610, length 160 bytes (plain `ret`). Vtable CThingVillage 0x012451BC
// slot 8 (manifest "vfunc_8"; slot 8 is OnCreate as for CThingGameObject 0x00838A40).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("CTCGraphicAppearance" @ 0x012398DC,
// "CTCGraphicAppearanceStaticMesh" @ 0x0123BC4C), CThing::AddTC(const CCharString&, bool, CTCParamsBase*) @ 0x004C9D60,
// CCharString::~CCharString @ 0x0099EAE0, CThingVillage::GetCurrentGraphic() @ 0x0083B520, CThingGameObject::OnCreate
// @ 0x00838A40.
// Layout (retail): CThing::PTCGraphicAppearance +0x64 [PDB +0x74]; the appearance component's CEngineGraphic (14 bytes,
// copied as three dwords and a word) at +0x44; CThingPhysical::MaxHealth +0xb0 [PDB +0xc8] (100.0f = 0x42C80000).
// Owner note (rendering layer): CEngineGraphic is the Ego_r.pdb 14-byte record (BankIndex, AnimStep, RenderSizeX,
// AdditiveAlpha, Type; 2-byte packed, as the retail word copy shows); the appearance component pointer is CThing's
// PTCGraphicAppearance at retail +0x64.
// Shape: each component name is a named CCharString in its own block (built before the AddTC pushes and re-addressed
// with `lea`); the appearance pointer is a named local read before GetCurrentGraphic (kept in edi); the 14-byte
// struct assignment copies 3 dwords + 1 word; the health store sits in the success arm.
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CTCBase;
struct CTCParamsBase;
#pragma pack(push, 2)
struct CEngineGraphic {                                // Ego_r.pdb: 14 bytes (2-byte packed), copied as 3 dwords + a word
    unsigned long BankIndex;                           // +0x00
    float         AnimStep;                            // +0x04
    float         RenderSizeX;                         // +0x08
    bool          AdditiveAlpha;                       // +0x0c
    unsigned char Type;                                // +0x0d
};
#pragma pack(pop)
struct CTCGraphicAppearance {
    unsigned char  _pad_0x00[0x44];
    CEngineGraphic Graphic;                            // +0x44
};
struct CThingGameObject {
    unsigned char         _pad_0x00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;        // +0x64 CThing [PDB +0x74]
    unsigned char         _pad_0x68[0xb0 - 0x68];
    float                 MaxHealth;                   // +0xb0 CThingPhysical [PDB +0xc8]
    CTCBase* AddTC(const CCharString& name, bool a2, CTCParamsBase* params);   // 0x004C9D60 (CThing)
    bool OnCreate();                                   // 0x00838A40
};
struct CThingVillage : CThingGameObject {
    const CEngineGraphic* GetCurrentGraphic();         // 0x0083B520
    bool OnCreate();
};

bool CThingVillage::OnCreate()
{
    {
        CCharString name("CTCGraphicAppearance");
        AddTC(name, false, 0);
    }
    {
        CCharString name("CTCGraphicAppearanceStaticMesh");
        AddTC(name, false, 0);
    }
    CTCGraphicAppearance* appearance = PTCGraphicAppearance;
    appearance->Graphic = *GetCurrentGraphic();
    if (CThingGameObject::OnCreate()) {
        MaxHealth = 100.0f;
        return true;
    }
    return false;
}