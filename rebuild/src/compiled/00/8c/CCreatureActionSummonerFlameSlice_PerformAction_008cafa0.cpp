// CCreatureAction_SummonerFlameSlice::PerformAction() -- kill the live obstruction, spawn THING_EXPLOSION_SUMMONER_FLAME_SLICE_SPHERICAL at the summoner and, when the explosion is alive, tell its driver to spare the summoner
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CAFA0, length 188 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::Kill(bool) @ 0x004C9B80, CCharString::CCharString(const char*,
// long) @ 0x0099EBF0, CGameDefinitionManager::Get @ 0x0044C6B0 (static __fastcall), CDefinitionManager::GetDefGlobalIndexFromName
// @ 0x009AD410, CCharString::~CCharString @ 0x0099EAE0, CThing::GetPosition @ 0x004C73D0, CreateThing(long, const C3DVector&)
// @ 0x0080C460 (`__fastcall`), the explosion driver's exclusion add @ 0x0080CBF0 (manifest "AddDependentThing" is propagated).
// Shape: the landed CCreatureAction_ScorpionTailPlunge::PerformAction (0x008C4810) preceded by the obstruction kill
// (present / not dead, thing byte +0x91 bit 0) and with the explosion's own dead test; the driver lookup is unchecked
// (bit 1 of +0x28, cached +0x5c) and its absent arm reads the string's frame word.
// Layout: Ego_r.pdb PObstruction +0x12c, retail +0x114 (CCombatActionBase is 0x114 in retail).
class C3DVector { public: float x, y, z; };
class CThing;
struct CExplosionDriver {
    void AddExcludedThing(CThing* thing);   // 0x0080CBF0
};
class CThing { public:
    unsigned char     _pad0[0x28];
    unsigned char     m_driverFlags;        // +0x28 bit 1: the driver component is cached
    unsigned char     _pad1[0x5c - 0x29];
    CExplosionDriver* m_pDriver;            // +0x5c
    unsigned char     _pad2[0x91 - 0x60];
    unsigned char     m_flags91;            // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;   // 0x004C73D0
    void Kill(bool silent);                 // 0x004C9B80
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
class CCreatureAction_SummonerFlameSlice : public CCombatActionBase { public:
    CIntelligentPointer<CThing> PObstruction;   // +0x114 (PDB +0x12c)
    void PerformAction();
};

void CCreatureAction_SummonerFlameSlice::PerformAction()
{
    if (PObstruction.GetPItem() && !(PObstruction.GetPItem()->m_flags91 & 1))
        PObstruction.GetPItem()->Kill(true);
    long explosionIndex;
    {
        CCharString name("THING_EXPLOSION_SUMMONER_FLAME_SLICE_SPHERICAL", -1);
        explosionIndex = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
    }
    CThing* explosion = CreateThing(explosionIndex, PCreature.GetPItem()->GetPosition());
    if (explosion && !(explosion->m_flags91 & 1)) {
        CExplosionDriver* driver;
        explosion->GetTCDriver(&driver);
        driver->AddExcludedThing(PCreature.GetPItem());
    }
}