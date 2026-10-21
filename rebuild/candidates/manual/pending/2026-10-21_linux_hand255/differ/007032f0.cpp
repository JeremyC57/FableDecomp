// Grade: DIFFER(383v380) -- flow, graphic copy through edi, base/validate calls, die-off timer, rotation-time test, same-TU LowerBound stand-in (frame packs to 8 like retail) and SameText compare match; the lane keeps the literal's address (the CCharString ctor's eax) in a register for the compare (edi, pushing ebp for the component) where retail re-reads the data word from the frame (`mov ecx,[esp+0Ch]`) with the component in edi; tried: temporary vs block-scoped named literal, bool local, both GetTC forms, nested data-block equality
// CThingObject::OnCreate() -- a live object takes its def's graphic and its own scale into its graphic appearance, runs the game-object creation and the physical-thing validation, takes the def's die-off timer (dying off when it is positive) and, when the def gives a rotation time, sets a sprite appearance (component 0x5c) rotating once per that many seconds
// Leaf: ?OnCreate@CThingObject
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007032F0, length 380 bytes (+4 int3), plain `ret`.
//   Name from Ego_r.pdb / the catalog (`CThingObject::OnCreate`).
// Callees: CTCGraphicAppearance::SetScale(float) @ 0x004BFA50, CThingGameObject::OnCreate @ 0x00838A40,
//   CThingPhysical::ValidateInitialisation @ 0x006A4D60, LowerBound @ 0x0040F020, CCharString(const char*, long)
//   @ 0x0099EBF0 ("CTCGraphicAppearanceSprite" @ 0x0123BCA0), the component's virtual slot 22 (+0x58, its class name as
//   a CCharString by hidden pointer), Compare @ 0x00411570 (`__fastcall`), ~CCharString @ 0x0099EAE0,
//   CTCGraphicAppearanceSprite::SetRotationAngle(float) @ 0x0077C490; 1e-4f @ 0x0129BA3C, the frame rate (long 15)
//   @ 0x01375550, 1.0f @ 0x0122DED8.
// Layout (retail): CThing Dead bit 0 of +0x91, bitset +0x20 (0x5c = bit 28 of +0x28), map +0x44, PTCGraphicAppearance
//   +0x64; CThingPhysical::Scale +0x9c; CThingObject PDef +0xd0, DieOffTimer +0xd4, DiesOff +0xd8 (Ego_r.pdb +0xe8..
//   +0xf0); CThingObjectDef Graphic +0x70, RotationTime +0x80, DieOffTimer +0x94 (PDB 4 higher); CTCGraphicAppearance
//   graphic +0x44 (a 14-byte CEngineGraphic).
struct CTCBase;
struct CCharStringData { const char* m_pData; unsigned long m_length; };
long __fastcall Compare(const char* a, const char* b);  // 0x00411570
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
    bool SameText(const CCharString& other) const
    {
        if (PStringData->m_length != other.PStringData->m_length)
            return false;
        return Compare(PStringData->m_pData, other.PStringData->m_pData) == 0;
    }
    __forceinline bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData)
            return SameText(other);
        return false;
    }
};

#pragma pack(push, 2)
struct CEngineGraphic {                                 // 14 bytes (Ego_r.pdb)
    unsigned long BankIndex;                            // +0
    float         AnimStep;                             // +4
    float         RenderSizeX;                          // +8
    bool          AdditiveAlpha;                        // +0xc
    unsigned char Type;                                 // +0xd
};
#pragma pack(pop)
struct CTCGraphicAppearance {
    unsigned char  _pad00[0x44];
    CEngineGraphic Graphic;                             // +0x44
    void SetScale(float scale);                         // 0x004BFA50
};
struct CTCBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual CCharString GetClassName() const;          // slot 22 (+0x58)
};
struct CTCGraphicAppearanceSprite : CTCBase {
    void SetRotationAngle(float angle);                 // 0x0077C490
};

struct CThingObjectDef {
    unsigned char  _pad00[0x70];
    CEngineGraphic Graphic;                             // +0x70
    float          RotationTime;                        // +0x80
    long           Material;                            // +0x84
    long           FloorMaterial;                       // +0x88
    long           OnDeathObject;                       // +0x8c
    unsigned long  Property;                            // +0x90
    long           DieOffTimer;                         // +0x94
};

extern const float g_epsilon_0129ba3c;                  // 1e-4f
extern long g_framesPerSecond;                 // 15

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU stand-in for the CVectorMap LowerBound (0x0040F020, STLport lower_bound over the 8-byte entries): compiled above
// the caller so VC7.1 knows the key's address does not escape and packs the frame words as retail (the landed
// GiveHeroItemsFromContainer 0x00898C40 / chunk 242 idiom).
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* PTCGraphicAppearance;         // +0x64
    unsigned char  _pad68[0x91 - 0x68];
    unsigned char  Dead : 1;                            // +0x91 bit 0
    unsigned char  _bits91 : 7;
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};
struct CThingPhysical : CThing {
    unsigned char _pad94[0x9c - 0x94];                  // CThing rounds to 0x94
    float         Scale;                                // +0x9c
    bool ValidateInitialisation();                      // 0x006A4D60
};
struct CThingGameObject : CThingPhysical {
    bool OnCreate();                                    // 0x00838A40
};
struct CThingObject : CThingGameObject {
    unsigned char           _pada0[0xd0 - 0xa0];
    const CThingObjectDef*  PDef;                       // +0xd0 CDefPointer<const CThingObjectDef>
    long                    DieOffTimer;                // +0xd4
    bool                    DiesOff;                    // +0xd8
    bool OnCreate();
};

bool CThingObject::OnCreate()
{
    if (Dead)
        return false;
    DiesOff = false;
    CTCGraphicAppearance* appearance = PTCGraphicAppearance;
    if (appearance) {
        appearance->Graphic = PDef->Graphic;
        appearance->SetScale(Scale);
    }
    if (!CThingGameObject::OnCreate() || !ValidateInitialisation())
        return false;
    DieOffTimer = PDef->DieOffTimer;
    DiesOff = DieOffTimer > 0;
    if (PDef->RotationTime > g_epsilon_0129ba3c && HasTC(0x5c)) {
        CTCBase* component = GetTC(0x5c);
        if (component->GetClassName() == CCharString("CTCGraphicAppearanceSprite"))
            ((CTCGraphicAppearanceSprite*)component)->SetRotationAngle(1.0f / (g_framesPerSecond * PDef->RotationTime));
    }
    return true;
}
