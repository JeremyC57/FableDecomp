// CCreatureAction_SummonerDead::Initialise() -- play FALL_TO_KNEES_OUT_OF, base Initialise, stop mode 0x5c and kill the summoner; when its CSummonerDef names a death object, create it at the summoner and (when it lives and the world is not loading) make the hero its owner; then spawn the summoner's MESH_EXPLODE effect at its position
// Leaf: ?Initialise@CCreatureAction_SummonerDead
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CB940, length 580 bytes, plain `ret` (virtual).
// Callees: CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0, CCombatActionBase::SetAnimationName @ 0x008574D0 / Initialise
//          @ 0x00857520, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020,
//          CTCCreatureModeManager::RemoveMode @ 0x0073ABA0, CThing::Kill(bool) @ 0x004C9B80, CDefClassBase::GetSubDef
//          <CSummonerDef> @ 0x008CBFF0 (same TU) / <CSpecialEffectsDef> @ 0x00669580, CThing::GetPos @ 0x004C73D0,
//          CreateThing(long, const C3DVector&) @ 0x0080C460 (__fastcall), CCreatureActionBase::GetWorld @ 0x00692950,
//          CWorld::GetMainPlayer @ 0x0049D9D0, CPlayer::GetPControlledCreature @ 0x00487DC0, the driver's SetOwner @ 0x0080CBF0,
//          CSpecialEffectsDef::GetEffectFromString @ 0x00669D30, CreateEffectThing @ 0x006E0880 (__fastcall).
// Layout: thing interface bitset +0x20, map +0x44, PTCDriver +0x5c, def +0x70, flags +0x91; CSummonerDef death object
//   +0x4c; the world's loading flag +0xe1 (name descriptive). Models: the landed CCreatureAction_JackFloatUp::PerformAction
//   0x008C0FB0 (effects lookup) and CCreatureAction_SummonerLightningOrbOutOf (CSummonerDef).
// Shape: the animation name is passed as const char* (its temporary shares the def pointer's frame word); same-TU
//   noinline GetSubDef / GetEffectFromString stand-ins; the def references are counted temporaries inside the &&
//   chains (the second chain's flags live in ebx: 2 = def temporary, 4 = name temporary).
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                                  // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                                         // +0x04
    void Release() { if (--m_refCount == 0) Destroy(); }
};
class CSummonerDef : public CDefBase { public:
    unsigned char _pad08[0x4c - 8];
    long          DeathObjectDefIndex;                       // +0x4c
};
class CSpecialEffectsDef : public CDefBase { public:
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& type) const
{
    long found = Lookup_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    type = found;
    return true;
}
#pragma auto_inline(on)
template <class T> class CDefPointer { public:
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (m_p) m_p->Release(); }
    T* operator->() const { return m_p; }
};
class CDefClassBase : public CDefBase { public:
    bool GetSubDef(CDefPointer<CSummonerDef>* out);          // 0x008CBFF0
    bool GetSubDef(CDefPointer<CSpecialEffectsDef>* out);    // 0x00669580
};
void* Find_CDefClassBase_GetSubDef(const void* owner);       // stand-in: the sub def lookup (not graded here)
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CSummonerDef>* out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    out->m_p = (CSummonerDef*)def;
    return def != 0;
}
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CSpecialEffectsDef>* out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    out->m_p = (CSpecialEffectsDef*)def;
    return def != 0;
}
#pragma auto_inline(on)
struct CDefClassReference {                                  // a counted CDefClassBase reference
    CDefClassBase* m_p;
    CDefClassReference(CDefClassBase* p) : m_p(p) { if (m_p) ++m_p->m_refCount; }
    ~CDefClassReference() { if (m_p) m_p->Release(); }
    CDefClassBase* operator->() const { return m_p; }
};

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
};
class CThing {
public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                         // +0x20
    CInterfaceMap  m_interfaces;                             // +0x44
    unsigned char  _pad4c[0x5c - 0x4c];
    CTCBase*       PTCDriver;                                // +0x5c
    unsigned char  _pad60[0x70 - 0x60];
    CDefClassBase* m_pDef;                                   // +0x70
    unsigned char  _pad74[0x91 - 0x74];
    unsigned char  m_flags91;                                // +0x91 bit 0: dead
    const C3DVector& GetPos() const;                    // 0x004C73D0
    void Kill(bool immediately);                             // 0x004C9B80
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
class CThingPlayerCreature : public CThing { };
CThing* __fastcall CreateThing(long defIndex, const C3DVector& position);              // 0x0080C460
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880
struct CTCDriver41 {
    void SetOwner(CThing* owner);                            // 0x0080CBF0
};
class CTCCreatureModeManager { public:
    void RemoveMode(unsigned long mode);                     // 0x0073ABA0
};
class CPlayer { public:
    CThingPlayerCreature* GetPControlledCreature() const;                                  // 0x00487DC0
};
class CWorld { public:
    unsigned char _pad00[0xe1];
    bool          Loading;                                   // +0xe1 (name descriptive)
    CPlayer* GetMainPlayer();                                // 0x0049D9D0
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CCombatActionBase { public:                                   // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();                               // 0x00857520
    unsigned char           _pad04[4];
    CIntelligentPointer<CThing> PCreature;                       // +0x08
    unsigned char           _pad10[0x114 - 0x10];
    CThing* GetCreature() const { return PCreature.GetPItem(); }
    CWorld* GetWorld() const;                                // 0x00692950
    void SetAnimationName(const CCharString& name);          // 0x008574D0
};
class CCreatureAction_SummonerDead : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_SummonerDead::Initialise()
{
    SetAnimationName("FALL_TO_KNEES_OUT_OF");
    CCombatActionBase::Initialise();
    CTCCreatureModeManager* modeManager;
    GetCreature()->GetInterface(&modeManager, 0x31);
    modeManager->RemoveMode(0x5c);
    GetCreature()->Kill(true);
    CDefPointer<CSummonerDef> summonerDef;
    if (CDefClassReference(GetCreature()->m_pDef)->GetSubDef(&summonerDef) && summonerDef->DeathObjectDefIndex > 0) {
        CThing* deathObject = CreateThing(summonerDef->DeathObjectDefIndex, GetCreature()->GetPos());
        if (deathObject && !(deathObject->m_flags91 & 1) && !GetWorld()->Loading) {
            CTCDriver41* driver;
            if (deathObject->HasTC(0x41))
                driver = (CTCDriver41*)deathObject->PTCDriver;
            CThing* hero = GetWorld()->GetMainPlayer()->GetPControlledCreature();
            if (hero && !(hero->m_flags91 & 1))
                driver->SetOwner(GetWorld()->GetMainPlayer()->GetPControlledCreature());
        }
    }
    CDefPointer<CSpecialEffectsDef> effects;
    long effectType = 0;
    if (CDefClassReference(GetCreature()->m_pDef)->GetSubDef(&effects) && effects->GetEffectFromString("MESH_EXPLODE", effectType))
        CreateEffectThing(effectType, &GetCreature()->GetPos(), false);
}