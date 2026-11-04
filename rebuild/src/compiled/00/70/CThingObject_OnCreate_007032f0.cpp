// CThingObject::OnCreate() -- a live object takes its def's graphic and its own scale into its graphic appearance, runs the game-object creation and the physical-thing validation, takes the def's die-off timer (dying off when it is positive) and, when the def gives a rotation time, sets a sprite appearance (component 0x5c) rotating once per that many seconds
// Leaf: ?OnCreate@CThingObject
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Naming wave 18: the shared const float 1e-4f at 0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (raw Ego_r public + typed data record; was EPSILON).
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
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
// Shape: the sprite test binds the literal CCharString to a block-scoped const reference and stores the comparison in a
//   bool, so both temporaries die before the test (retail destroys them ahead of `test bl,bl`) and the compare re-reads the
//   literal's data word from the frame instead of keeping the constructor's returned address.
class CTCBase;
class CCharStringData { public: const char* m_pData; unsigned long m_length; };
long __fastcall Compare(const char* a, const char* b);  // 0x00411570
class CCharString { public:
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
class CEngineGraphic { public:                                 // 14 bytes (Ego_r.pdb)
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
class CTCBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21();
    virtual CCharString GetClassName() const;          // slot 22 (+0x58)
};
struct CTCGraphicAppearanceSprite : CTCBase {
    void SetRotationAngle(float angle);                 // 0x0077C490
};

class CThingObjectDef { public:
    unsigned char  _pad00[0x70];
    CEngineGraphic Graphic;                             // +0x70
    float          RotationTime;                        // +0x80
    long           Material;                            // +0x84
    long           FloorMaterial;                       // +0x88
    long           OnDeathObject;                       // +0x8c
    unsigned long  Property;                            // +0x90
    long           DieOffTimer;                         // +0x94
};

extern const float DELTA_NOTIONAL_ZERO;           // 1e-4f
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                    // +0x20
    CInterfaceMap  m_interfaces;                        // +0x44
    unsigned char  _pad54[0x10];
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
        CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)key);
        CInterfaceMap::value_type* end = map->end();
        return (it == end || type < it->first ? end : it)->second;
    }
};
class CThingPhysical : public CThing { public:
    unsigned char _pad94[0x9c - 0x94];                  // CThing rounds to 0x94
    float         Scale;                                // +0x9c
    bool ValidateInitialisation();                      // 0x006A4D60
};
class CThingGameObject : public CThingPhysical { public:
    bool OnCreate();                                    // 0x00838A40
};
class CThingObject : public CThingGameObject { public:
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
    if (PDef->RotationTime > DELTA_NOTIONAL_ZERO && HasTC(0x5c)) {
        CTCBase* component = GetTC(0x5c);
        bool isSprite;
        {
            const CCharString& spriteName = CCharString("CTCGraphicAppearanceSprite");
            isSprite = component->GetClassName() == spriteName;
        }
        if (isSprite)
            ((CTCGraphicAppearanceSprite*)component)->SetRotationAngle(1.0f / (CMainGameComponent::GetConstantFPS() * PDef->RotationTime));
    }
    return true;
}