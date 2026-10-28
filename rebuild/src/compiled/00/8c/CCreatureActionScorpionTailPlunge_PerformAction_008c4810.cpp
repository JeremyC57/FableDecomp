// CCreatureAction_ScorpionTailPlunge::PerformAction() -- spawn the "THING_EXPLOSION_SCORPION_KING_PLUNGE" thing at the scorpion's position and tell its explosion driver to spare the scorpion
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C4810, length 130 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, CGameDefinitionManager::Get @ 0x0044C6B0 (static __fastcall, returns
// the manager), CDefinitionManager::GetDefGlobalIndexFromName(const CCharString&) @ 0x009AD410, CCharString::~CCharString @ 0x0099EAE0,
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, twice), CThing::GetPos @ 0x004C73D0,
// CreateThing(long, const C3DVector&) @ 0x0080C460 (`__fastcall`), the explosion driver's exclusion add @ 0x0080CBF0 (wraps the thing
// in a CIntelligentPointer; manifest label "CCreatureActionBase::AddDependentThing" is propagated).
// Shape: the def name is a CCharString in its own block (the definition manager is fetched after the argument push: chained call),
// so the driver out pointer reuses its frame word: the inlined GetTCDriver (bit 1 of the flag byte at +0x28, cached pointer at +0x5c,
// the landed 0x00750DD0 template) is not tested and the absent arm reads that word (`mov edi,[esp+8]`); the add call is
// tail-duplicated into both arms.
class C3DVector { public: float x, y, z; };
class CThing;
struct CExplosionDriver {                   // the spawned thing's driver component
    void AddExcludedThing(CThing* thing);   // 0x0080CBF0
};
class CThing { public:
    unsigned char     _pad0[0x28];
    unsigned char     m_driverFlags;        // +0x28 bit 1: the driver component is cached
    unsigned char     _pad1[0x5c - 0x29];
    CExplosionDriver* m_pDriver;            // +0x5c
    const C3DVector& GetPos() const;   // 0x004C73D0
    bool GetTCDriver(CExplosionDriver** out)
    {
        if (m_driverFlags & 2) {
            *out = m_pDriver;
            return true;
        }
        return false;
    }
};
CThing* __fastcall CreateThing(long defIndex, const C3DVector& position);   // 0x0080C460

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};
class CGameDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();                 // 0x0044C6B0
    long GetDefGlobalIndexFromName(const CCharString& name);        // 0x009AD410
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
class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
};
class CCreatureAction_ScorpionTailPlunge : public CCombatActionBase { public:
    void PerformAction();
};

void CCreatureAction_ScorpionTailPlunge::PerformAction()
{
    long explosionIndex;
    {
        CCharString name("THING_EXPLOSION_SCORPION_KING_PLUNGE", -1);
        explosionIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
    }
    CThing* explosion = CreateThing(explosionIndex, PCreature.GetPItem()->GetPos());
    if (explosion) {
        CExplosionDriver* driver;
        explosion->GetTCDriver(&driver);
        driver->AddExcludedThing(PCreature.GetPItem());
    }
}