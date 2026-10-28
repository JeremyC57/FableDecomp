#include <cstdio>
class CThing { public:
    unsigned char  _pad00[0x8c];
    unsigned short DefIndex;                             // +0x8c
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                                       // +0x04
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                          // 0x00A01C10
    CThing* GetPItem() const;                            // 0x00A01B50
    void SetPItem(CThing* item);                         // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
};
class CCoopSpiritDef;
class CDefClassBase { public:
    virtual void v00();
    virtual void DestroySelf();                          // slot 1 (+4)
    long RefCount;                                       // +0x04
    bool GetSubDef(CDefPointer<const CCoopSpiritDef>* out);   // 0x00670B90
};
class CDefinitionManager { public:
    bool GetDef(long index, CDefPointer<CDefClassBase>* out); // 0x0042AF3C
};
void* FindDef_CDefClassBase_GetSubDef_1();   // stand-in callee of the stand-in body below
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<const CCoopSpiritDef>* out)
{
    void* def = FindDef_CDefClassBase_GetSubDef_1();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
void* FindDef_CDefinitionManager_GetDef_0();
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<CDefClassBase>* out)
{
    void* def = FindDef_CDefinitionManager_GetDef_0();
    if (!def)
        return false;
    *(void**)out = def;
    return true;
}
#pragma auto_inline(on)
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector& operator=(const C3DVector& o) { Z = o.Z; Y = o.Y; X = o.X; return *this; }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CPlayer;
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                 // +0x04
    bool    ToBeRemoved;                                 // +0x08
    bool    IsInGlobalUpdateTCs;                         // +0x09
    CDefinitionManager* GetDefinitionManager();          // 0x006869D0
    void OnCreate();                                     // 0x00686850
};
class CTCCoopSpirit : public CTCBase { public:
    long                              NoMovementTimer;   // +0x0c
    long                              Score;             // +0x10
    long                              OffscreenCounter;  // +0x14
    C3DVector                         MovementVector;    // +0x18
    CPlayer*                          Master;            // +0x24
    CDefPointer<const CCoopSpiritDef> PCoopSpiritDef;    // +0x28
    CIntelligentPointerThing          PParticleEmitter;  // +0x2c
    void OnCreate();
};

void CTCCoopSpirit::OnCreate()
{
    {
        PParticleEmitter = CIntelligentPointerThing(0);
    }
    CTCBase::OnCreate();
    MovementVector = C3DVector(0, 0, 0);
    NoMovementTimer = 0;
    Master = 0;
    OffscreenCounter = 0;
    Score = 0;
    {
        CDefPointer<CDefClassBase> def;
        GetDefinitionManager()->GetDef(ParentThing->DefIndex, &def);
        CDefClassBase* object = def.Object;
        object->GetSubDef(&PCoopSpiritDef);
        if (object != 0 && --object->RefCount == 0)
            object->DestroySelf();
    }
}

static int g_ctor, g_dtor, g_set, g_baseCreate, g_destroyed, g_dm;
static CThing* g_lastSet; static void* g_lastSetOn;
void CBaseIntelligentPointer::v00() {}
void CIntelligentPointerThing::v00() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() { PItem = (CThing*)0x77; ++g_ctor; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_dtor; }
CThing* CBaseIntelligentPointer::GetPItem() const { return PItem; }
void CBaseIntelligentPointer::SetPItem(CThing* item) { PItem = item; g_lastSet = item; g_lastSetOn = this; ++g_set; }
struct DefObj : CDefClassBase { };
void CDefClassBase::v00() {}
void CDefClassBase::DestroySelf() { ++g_destroyed; }
static DefObj g_def; static unsigned char g_coopDef[4];
void* FindDef_CDefinitionManager_GetDef_0() { return &g_def; }
void* FindDef_CDefClassBase_GetSubDef_1() { return g_coopDef; }
static CDefinitionManager g_mgr;
void CTCBase::v00() {}
CDefinitionManager* CTCBase::GetDefinitionManager() { ++g_dm; return &g_mgr; }
void CTCBase::OnCreate() { ++g_baseCreate; }
int main()
{
    static CThing parent; parent.DefIndex = 42;
    static unsigned char raw[sizeof(CTCCoopSpirit)];
    CTCCoopSpirit* tc = (CTCCoopSpirit*)raw;
    for (unsigned i = 0; i < sizeof(raw); ++i) raw[i] = 0x33;
    tc->ParentThing = &parent; tc->PCoopSpiritDef.Object = 0;
    g_def.RefCount = 2;
    tc->OnCreate();
    bool ok = g_ctor == 1 && g_dtor == 1 && g_set == 2 && g_lastSet == 0 && g_lastSetOn == &tc->PParticleEmitter
        && g_baseCreate == 1 && g_dm == 1
        && tc->NoMovementTimer == 0 && tc->Score == 0 && tc->OffscreenCounter == 0 && tc->Master == 0
        && tc->MovementVector.X == 0 && tc->MovementVector.Y == 0 && tc->MovementVector.Z == 0
        && (void*)tc->PCoopSpiritDef.Object == g_coopDef && g_def.RefCount == 1 && g_destroyed == 0;
    tc->OnCreate();                                     // second release reaches zero refs
    ok = ok && g_def.RefCount == 0 && g_destroyed == 1;
    std::printf(ok ? "006700f0_TEST PASS\n" : "006700f0_TEST FAIL\n");
    return ok ? 0 : 1;
}