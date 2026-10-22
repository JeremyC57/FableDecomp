// CCreatureAction_SummonerLightningOrbOutOf::PerformAction() -- stop the summoner's lightning-orb mode (0x5a); when the target is alive and the summoner's def has a CSummonerDef sub def: create the orb object (the Fireheart variant when the target is OBJECT_LPDD_FIREHEART_01) at the summoner's position, move it to the LIGHTNING_ORB_FOCUS dummy, copy the summoner's orientation, initialise its lightning-orb component from summoner to target and, for the normal orb, give it the def's lightning setting
// Grade: DIFFER(587v588) -- register allocation around the && temp: retail keeps the condition bool in bl and the CDefPointer temp flag in memory (ours the reverse), +0x31 key / frame word 0x20 vs 0x1c; Create args: retail fetches both creature pointers before the pushes; GetDummyObject loads +0x64 before the pushes; SetOrientation loads the orb physics after GetPItem
// Leaf: ?PerformAction@CCreatureAction_SummonerLightningOrbOutOf
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CB250, length 588 bytes, plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020, CTCCreatureModeManager::
//          RemoveMode @ 0x0073ABA0, CDefClassBase::GetSubDef<CSummonerDef> @ 0x008CBFF0 (same TU), CThing::GetDefName
//          @ 0x004C7CC0, CDefString::operator==(const char*) @ 0x005B3440, CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0,
//          CThing::GetPosition @ 0x004C73D0, CThingObject::Create @ 0x00703210 (__fastcall), CTCGraphicAppearance::
//          GetDummyObject @ 0x004C08C0, physics slots 31 (+0x7c) / 32 (+0x80) / 72 (+0x120), CTCLightningOrb::
//          Initialise(CThing*, CThing*) @ 0x007F26B0 and its setter @ 0x007F2100.
// Layout: owner CIntelligentPointer +8, target +0xa8 (the landed CCreatureAction_SummonerLightningOrbInto 0x008CB1A0); the
//   thing's def pointer +0x70, player byte +0x90, dead flag +0x91; CSummonerDef orb def indices +0x50/+0x54 and +0x58.
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
};
struct CDefString {
    bool operator==(const char* text) const;                 // 0x005B3440
};
struct CPhysicsBody {
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF(30)
    virtual void SetPosition(const C3DVector& position);     // slot 31 (+0x7c)
    virtual void SetOrientation(const CRightHandedSet& orientation);   // slot 32 (+0x80)
    VF(33) VF(34) VF(35) VF(36) VF(37) VF(38) VF(39) VF10(4) VF10(5) VF10(6) VF(70) VF(71)
    virtual const CRightHandedSet& GetOrientation() const;   // slot 72 (+0x120)
};
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* position,
                        CRightHandedSet* orientation, float scale);   // 0x004C08C0
};

struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                                  // slot 1
};
struct CDefBase : CDefVirtual, CRefCounted {
    void Release() { if (--m_count == 0) Destroy(); }
};
struct CSummonerDef : CDefBase {
    unsigned char _pad08[0x50 - 8];
    long          OrbDefIndex;                               // +0x50
    long          FireheartOrbDefIndex;                      // +0x54
    long          OrbLightningType;                          // +0x58
};
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    CDefPointer(const CDefPointer& o) : m_p(o.m_p) { if (m_p) ++m_p->m_count; }
    ~CDefPointer() { if (m_p) m_p->Release(); }
    T* operator->() const { return m_p; }
};
struct CDefClassBase : CDefBase {
    bool GetSubDef(CDefPointer<CSummonerDef>* out);          // 0x008CBFF0
};
CSummonerDef* FindSummonerDef(const CDefClassBase* def);     // stand-in: the sub def lookup (not graded here)
#pragma auto_inline(off)
// Same-TU body of GetSubDef<CSummonerDef> (retail instantiates it in this TU): writes only its out pointer.
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CSummonerDef>* out)
{
    CSummonerDef* def = FindSummonerDef(this);
    out->m_p = def;
    return def != 0;
}
#pragma auto_inline(on)

struct CTCBase;
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
    unsigned char  _pad4c[0x60 - 0x4c];
    CPhysicsBody*  PhysicsBody;                              // +0x60
    CTCGraphicAppearance* GraphicAppearance;                 // +0x64
    unsigned char  _pad68[0x70 - 0x68];
    CDefPointer<CDefClassBase> PDef;                         // +0x70
    unsigned char  _pad74[0x90 - 0x74];
    signed char    PlayerNumber;                             // +0x90
    unsigned char  ThingFlags;                               // +0x91 (bit 0: dead)
    const C3DVector& GetPosition() const;                    // 0x004C73D0
    const CDefString& GetDefName() const;                    // 0x004C7CC0
    CDefPointer<CDefClassBase> GetDef() const { return PDef; }
    bool GetDummyObject(const CCharString& name, C3DVector* position, CRightHandedSet* orientation)
    {
        return GraphicAppearance->GetDummyObject(name, 0, 0, position, orientation, 1.0f);
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
struct CRightHandedSet;
struct CThingObject : CThing {
    static CThingObject* __fastcall Create(long defIndex, const C3DVector* position, long player,
                                           CRightHandedSet* orientation, unsigned long flags,
                                           const CCharString& name);   // 0x00703210
};
struct CTCCreatureModeManager {
    void RemoveMode(unsigned long mode);                     // 0x0073ABA0
};
struct CTCLightningOrb {
    void Initialise(CThing* source, CThing* target);         // 0x007F26B0
    void SetLightningType(long type);                        // 0x007F2100
};
struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                                         // +0x04
    CThing* GetPItem() const;                                // 0x00A01B50
    operator CThing*() const { return GetPItem(); }
};
class CCombatActionBase {
public:
    virtual void PerformAction();
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                       // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;                         // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
};
class CCreatureAction_SummonerLightningOrbOutOf : public CCombatActionBase {
public:
    virtual void PerformAction();
};

void CCreatureAction_SummonerLightningOrbOutOf::PerformAction()
{
    CTCCreatureModeManager* modeManager;
    if (PCreature.GetPItem()->GetInterface(&modeManager, 0x31))
        modeManager->RemoveMode(0x5a);
    CDefPointer<CSummonerDef> summonerDef;
    if (PTarget.GetPItem() && !(PTarget.GetPItem()->ThingFlags & 1) && PCreature.GetPItem()->GetDef()->GetSubDef(&summonerDef)) {
        bool fireheart = false;
        long orbDef = summonerDef->OrbDefIndex;
        if (PTarget.GetPItem()->GetDefName() == "OBJECT_LPDD_FIREHEART_01") {
            orbDef = summonerDef->FireheartOrbDefIndex;
            fireheart = true;
        }
        CThingObject* orb = CThingObject::Create(orbDef, &PCreature.GetPItem()->GetPosition(), PCreature.GetPItem()->PlayerNumber, 0, 0, "");
        C3DVector position;
        CRightHandedSet orientation;
        bool found;
        {
            CCharString name("LIGHTNING_ORB_FOCUS");
            found = PCreature.GetPItem()->GetDummyObject(name, &position, &orientation);
        }
        if (found)
            orb->PhysicsBody->SetPosition(position);
        orb->PhysicsBody->SetOrientation(PCreature.GetPItem()->PhysicsBody->GetOrientation());
        CTCLightningOrb* lightningOrb;
        orb->GetInterface(&lightningOrb, 0x10a);
        lightningOrb->Initialise(PCreature, PTarget.GetPItem());
        if (!fireheart)
            lightningOrb->SetLightningType(summonerDef->OrbLightningType);
    }
}
