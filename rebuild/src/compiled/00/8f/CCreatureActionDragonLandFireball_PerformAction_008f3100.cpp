// CCreatureAction_DragonLandFireball::PerformAction() -- fetch the dragon's weapon sub-def and its shot def, create the fireball shot thing at the dragon's DRAGON_MOUTH_DUMMY dummy, keep it in PShot, add its named shot TC and Init it (shooter = dragon, target = PTarget, the dragon's shot accuracy), then release both defs
// Leaf: ?PerformAction@CCreatureAction_DragonLandFireball
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F3100, length 513 bytes (+ int3 padding), plain `ret`. No call site or vtable
//   reference survives in retail (xref: none), so the class comes from the Ego_r.pdb symbol order: PerformAction of
//   CCreatureAction_DragonFlight (retail 0x008F3020), CCreatureAction_DragonLandFireball, CCreatureAction_DragonFireBlastBase
//   (retail 0x008F3310); the catalog label "CCreatureAction_BriarRoseShotFarOutof::PerformAction" is propagated from the
//   byte-identical twin 0x008C0900 (only the dummy-name literal differs: "DRAGON_MOUTH_DUMMY" @ 0x01285B4C).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CDefClassBase::GetSubDef<CWeaponDef> @ 0x0042B1A6 (same-TU out-only
//   stand-in here), CCreatureActionBase::GetPWorld @ 0x00692940, CDefinitionManager::GetDef<T> @ 0x0078B230 (same-TU out-only
//   stand-in), CThing::GetPosition @ 0x004C73D0, CCharString::CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString
//   @ 0x0099EAE0, CTCGraphicAppearance::GetDummyObject @ 0x004C08C0, the `__fastcall` thing factory Create @ 0x00703210,
//   CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10,
//   CThingCreatureBase::GetShotAccuracyPercentage @ 0x00663B10, CDefStringTable::GetString @ 0x009D49B0 on g_DefStringTable
//   0x013CA828, CThing::AddTC @ 0x004C9D60, CTCShotDefinitionBase::Init @ 0x007C0690.
// Shape: the 0x008C0900 body (see its notes): out-only def stand-ins, the shot def's out word in an inner block copied into a
//   register local, forceinline dummy/Create wrappers taking the creature, `accuracy` declared ahead of the PShot
//   assignment, the TC name a named local in its own block, the ability CDefPointer in a sibling block.
// Layout: PDB CCreatureAction_DragonLandFireball (0x134) has PShot at +0x12c; retail CCombatActionBase is 0x114 bytes, so
//   `this` is modelled locally with PShot at +0x114 (the twin's evidence).
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector First; C3DVector Second; };

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CDefStringTable {
    CCharString GetString(long index);                 // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;               // 0x013CA828

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                            // slot 1
    long RefCount;                                     // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CWeaponDef : CDefBase {
    unsigned char _pad08[0xa0 - 0x08];
    long Shot;                                         // +0xa0
};
struct CThingShotDef : CDefBase {
    unsigned char _pad08[0x60 - 0x08];
    long TCName;                                       // +0x60
    unsigned char _pad64[0x7c - 0x64];
    long Thing;                                        // +0x7c
};

template <class T> struct CDefPointerByValue {
    T* m_p;
    CDefPointerByValue(T* p) : m_p(p) { ++p->RefCount; }
    CDefPointerByValue(const CDefPointerByValue& o) : m_p(o.m_p) { ++m_p->RefCount; }
    ~CDefPointerByValue();
};

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();
};
struct CMeleeCombatAbilityDef : CDefVirtual, CRefCounted {
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
};

struct CDefClassBase {
    bool GetSubDef(CWeaponDef** out);                  // 0x0042B1A6
};
void* FindSubDef_CWeaponDef(CDefClassBase* def);
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CWeaponDef** out)
{
    void* found = FindSubDef_CWeaponDef(this);
    if (!found)
        return false;
    *out = (CWeaponDef*)found;
    return true;
}
#pragma auto_inline(on)

struct CDefinitionManager {
    bool GetDef(long index, CThingShotDef** out);      // 0x0078B230
};
void* FindDef_CThingShotDef(CDefinitionManager* manager, long index);
#pragma auto_inline(off)
__declspec(noinline) bool CDefinitionManager::GetDef(long index, CThingShotDef** out)
{
    void* found = FindDef_CThingShotDef(this, index);
    if (!found)
        return false;
    *out = (CThingShotDef*)found;
    return true;
}
#pragma auto_inline(on)

struct CWorld {
    unsigned char       _pad[0x10];
    CDefinitionManager* PDefinitionManager;            // +0x10
};

struct CTCBase;
struct CTCParamsBase;
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* position,
                        CRightHandedSet* orientation, float scale);   // 0x004C08C0
};
struct CThing {
    unsigned char         _pad0[0x64];
    CTCGraphicAppearance* PGraphicAppearance;          // +0x64
    unsigned char         _pad68[0x70 - 0x68];
    CDefClassBase*        PDef;                        // +0x70
    unsigned char         _pad74[0x90 - 0x74];
    signed char           PlayerNumber;                // +0x90
    const C3DVector& GetPosition() const;              // 0x004C73D0
    CTCBase* AddTC(const CCharString& name, bool init, CTCParamsBase* params);   // 0x004C9D60
};
__forceinline bool GetDummyObject(const CThing* thing, const CCharString& name, C3DVector* position, CRightHandedSet* orientation)
{
    CTCGraphicAppearance* appearance = thing->PGraphicAppearance;
    return appearance->GetDummyObject(name, 0, 0, position, orientation, 1.0f);
}
struct CThingCreatureBase : CThing {
    long GetShotAccuracyPercentage();                  // 0x00663B10
};
CThing* __fastcall Create(long defIndex, const C3DVector* position, long player, const CRightHandedSet* orientation,
                          unsigned long flags, const CCharString& scriptName);   // 0x00703210
__forceinline CThing* CreateThingObject(const long& defIndex, const C3DVector& position, const CThing* creature, const CCharString& name)
{
    return Create(defIndex, &position, creature->PlayerNumber, 0, 0, name);
}

struct CTCShotDefinitionBase {
    void Init(CDefPointerByValue<CThingShotDef> def, CThing* shooter, CThing* origin, CThing* target, float accuracy,
              float a, float b, CDefPointer<CMeleeCombatAbilityDef>* ability);   // 0x007C0690
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThingCreatureBase* m_pItem;
    CBaseIntelligentPointer();                         // 0x00A01B10
    ~CBaseIntelligentPointer();                        // 0x00A01C10
    CThingCreatureBase* GetPItem() const;              // 0x00A01B50
    void SetPItem(CThing* item);                       // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(CThing* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CCombatActionBase {
    virtual void PerformAction();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                 // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;                   // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
    CWorld* GetPWorld() const;                         // 0x00692940
};

struct CCreatureAction_DragonLandFireball : CCombatActionBase {
    CIntelligentPointer PShot;                         // +0x114 (PDB +0x12c)
    virtual void PerformAction();
};

void CCreatureAction_DragonLandFireball::PerformAction()
{
    CWeaponDef* weaponDef = 0;
    CDefClassBase* creatureDef = PCreature.GetPItem()->PDef;
    creatureDef->GetSubDef(&weaponDef);
    CThingShotDef* shotDef;
    {
        CThingShotDef* out = 0;
        CDefinitionManager* manager = GetPWorld()->PDefinitionManager;
        manager->GetDef(weaponDef->Shot, &out);
        shotDef = out;
    }
    C3DVector position = PCreature.GetPItem()->GetPosition();
    CRightHandedSet orientation;
    GetDummyObject(PCreature.GetPItem(), "DRAGON_MOUTH_DUMMY", &position, &orientation);
    CThing* thing = CreateThingObject(shotDef->Thing, position, PCreature.GetPItem(), "");
    if (thing) {
        long accuracy;
        PShot = CIntelligentPointer(thing);
        accuracy = PCreature.GetPItem()->GetShotAccuracyPercentage();
        CTCShotDefinitionBase* shot;
        {
            CCharString name = g_DefStringTable.GetString(shotDef->TCName);
            shot = (CTCShotDefinitionBase*)PShot.GetPItem()->AddTC(name, true, 0);
        }
        {
            CDefPointer<CMeleeCombatAbilityDef> ability;
            CThing* creature = PCreature.GetPItem();
            shot->Init(shotDef, creature, 0, PTarget.GetPItem(), (float)accuracy, 1.0f, 0.0f, &ability);
        }
    }
    shotDef->Release();
    weaponDef->Release();
}