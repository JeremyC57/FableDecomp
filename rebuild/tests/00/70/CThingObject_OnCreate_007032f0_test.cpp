// Behaviour test for 007032f0 CThingObject::OnCreate: final.cpp body verbatim; the base creation / validation, the graphic
// appearance, the CCharString runtime and the sprite component are stubs recording their calls.
#ifndef _MSC_VER
#define __fastcall
#define __cdecl
#define __declspec(x)
#define __forceinline inline
#endif
extern "C" int __cdecl printf(const char* format, ...);
extern "C" int __cdecl strcmp(const char* a, const char* b);
extern "C" unsigned int __cdecl strlen(const char* s);
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
class CThing { public:
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
    if (PDef->RotationTime > g_epsilon_0129ba3c && HasTC(0x5c)) {
        CTCBase* component = GetTC(0x5c);
        bool isSprite;
        {
            const CCharString& spriteName = CCharString("CTCGraphicAppearanceSprite");
            isSprite = component->GetClassName() == spriteName;
        }
        if (isSprite)
            ((CTCGraphicAppearanceSprite*)component)->SetRotationAngle(1.0f / (g_framesPerSecond * PDef->RotationTime));
    }
    return true;
}

static int g_fail;
static void Check(bool ok, const char* what) { if (!ok) { printf("FAIL: %s\n", what); g_fail = 1; } }
const float g_epsilon_0129ba3c = 1e-4f;
long g_framesPerSecond = 15;
static int g_ctors, g_dtors, g_scaleCalls, g_angleCalls;
static float g_scale, g_angle;
static bool g_baseOk, g_validOk;
static CCharStringData g_strings[16];
static int g_nextString;
long __fastcall Compare(const char* a, const char* b) { return strcmp(a, b); }
CCharString::CCharString(const char* text, long length)
{
    ++g_ctors;
    CCharStringData* d = &g_strings[g_nextString++ & 15];
    d->m_pData = text;
    d->m_length = length < 0 ? strlen(text) : (unsigned long)length;
    PStringData = d;
}
CCharString::~CCharString() { ++g_dtors; }
void CTCGraphicAppearance::SetScale(float scale) { ++g_scaleCalls; g_scale = scale; }
bool CThingGameObject::OnCreate() { return g_baseOk; }
bool CThingPhysical::ValidateInitialisation() { return g_validOk; }
#define TCSTUB(n) void CTCBase::n() {}
TCSTUB(v00) TCSTUB(v01) TCSTUB(v02) TCSTUB(v03) TCSTUB(v04) TCSTUB(v05) TCSTUB(v06) TCSTUB(v07) TCSTUB(v08) TCSTUB(v09)
TCSTUB(v10) TCSTUB(v11) TCSTUB(v12) TCSTUB(v13) TCSTUB(v14) TCSTUB(v15) TCSTUB(v16) TCSTUB(v17) TCSTUB(v18) TCSTUB(v19)
TCSTUB(v20) TCSTUB(v21)
static const char* g_className = "CTCGraphicAppearanceSprite";
CCharString CTCBase::GetClassName() const { return CCharString(g_className); }
void CTCGraphicAppearanceSprite::SetRotationAngle(float angle) { ++g_angleCalls; g_angle = angle; }

static CThingObjectDef g_def;
static CTCGraphicAppearance g_appearance;
static CTCGraphicAppearanceSprite g_sprite;
static CTCBase g_other;
static CInterfaceMap::SEntry g_entries[3];
static CThingObject g_obj;
static void Reset()
{
    unsigned char* p = (unsigned char*)&g_obj;
    for (unsigned int i = 0; i < sizeof(g_obj); ++i) p[i] = 0;
    g_obj.PDef = &g_def;
    g_obj.PTCGraphicAppearance = &g_appearance;
    g_obj.Scale = 2.5f;
    g_obj.DieOffTimer = 77;
    g_obj.DiesOff = true;
    g_def.Graphic.BankIndex = 1234; g_def.Graphic.AnimStep = 0.5f; g_def.Graphic.RenderSizeX = 3.0f;
    g_def.Graphic.AdditiveAlpha = true; g_def.Graphic.Type = 7;
    g_def.DieOffTimer = 5; g_def.RotationTime = 0.0f;
    g_appearance.Graphic.BankIndex = 0; g_appearance.Graphic.Type = 0;
    g_entries[0].type = 0x10; g_entries[0].component = &g_other;
    g_entries[1].type = 0x5c; g_entries[1].component = &g_sprite;
    g_entries[2].type = 0x70; g_entries[2].component = &g_other;
    g_obj.m_interfaces.first = g_entries; g_obj.m_interfaces.last = g_entries + 3;
    g_ctors = g_dtors = g_scaleCalls = g_angleCalls = 0; g_angle = 0;
    g_baseOk = true; g_validOk = true; g_className = "CTCGraphicAppearanceSprite";
}

int main()
{
    Reset(); g_obj.Dead = 1;
    Check(!g_obj.OnCreate() && g_obj.DiesOff && g_scaleCalls == 0, "dead");
    Reset(); g_baseOk = false;
    Check(!g_obj.OnCreate() && !g_obj.DiesOff && g_obj.DieOffTimer == 77, "base fails");
    Check(g_scaleCalls == 1 && g_scale == 2.5f && g_appearance.Graphic.BankIndex == 1234 && g_appearance.Graphic.Type == 7 &&
          g_appearance.Graphic.AdditiveAlpha && g_appearance.Graphic.RenderSizeX == 3.0f, "graphic + scale");
    Reset(); g_validOk = false;
    Check(!g_obj.OnCreate() && g_obj.DieOffTimer == 77, "validate fails");
    Reset(); g_obj.PTCGraphicAppearance = 0;
    Check(g_obj.OnCreate() && g_scaleCalls == 0 && g_obj.DieOffTimer == 5 && g_obj.DiesOff && g_ctors == 0, "no appearance, dies off");
    Reset(); g_def.DieOffTimer = 0;
    Check(g_obj.OnCreate() && g_obj.DieOffTimer == 0 && !g_obj.DiesOff, "no die-off");
    // rotating sprite
    Reset(); g_def.RotationTime = 2.0f; g_obj.m_presentBits[2] = 1UL << 28;
    Check(g_obj.OnCreate() && g_angleCalls == 1 && g_angle == 1.0f / (15 * 2.0f) && g_ctors == 2 && g_dtors == 2, "sprite rotation");
    // rotating, but the component is another class
    Reset(); g_def.RotationTime = 2.0f; g_obj.m_presentBits[2] = 1UL << 28; g_className = "CTCGraphicAppearanceModel";
    Check(g_obj.OnCreate() && g_angleCalls == 0 && g_ctors == 2 && g_dtors == 2, "other class");
    // same length, different text
    Reset(); g_def.RotationTime = 2.0f; g_obj.m_presentBits[2] = 1UL << 28; g_className = "CTCGraphicAppearanceSpritX";
    Check(g_obj.OnCreate() && g_angleCalls == 0, "same length other text");
    // rotation time at epsilon / no component bit
    Reset(); g_def.RotationTime = 1e-4f; g_obj.m_presentBits[2] = 1UL << 28;
    Check(g_obj.OnCreate() && g_ctors == 0, "rotation at epsilon");
    Reset(); g_def.RotationTime = 2.0f;
    Check(g_obj.OnCreate() && g_ctors == 0 && g_angleCalls == 0, "no component bit");
    printf(g_fail ? "007032f0_TEST FAIL\n" : "007032f0_TEST PASS\n");
    return g_fail;
}