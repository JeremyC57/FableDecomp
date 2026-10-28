// CCreatureAction_BriarRoseIntoInvulnerable::Initialise() -- animation "INVULNERABLE_INTO", base Initialise, then (mode manager present, enemy alive) push a new CModeBriarRoseTeleportInvulnerable(owner, enemy) onto the owner's mode manager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C1E80, length 225 bytes (+ int3 padding), plain `ret` (virtual, no arguments).
// Callees: CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("INVULNERABLE_INTO" @ 0x0128033C), SetCombatAnimation 0x008574D0,
// CCombatActionBase::Initialise 0x00857520 (direct), CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound 0x0040F020
// (inlined GetInterface 0x31 = CTCCreatureModeManager, bit 17 of +0x24), operator new 0x00BFEA1A (0x88 bytes),
// CModeBriarRoseTeleportInvulnerable::CModeBriarRoseTeleportInvulnerable(CThing*, CThingCreatureBase*) 0x008C35D0,
// CCountedPointer<CModeBase>::CCountedPointer(CModeBase*) 0x00718A00 (out of line here: built in the argument area, `sub esp,8 ;
// mov ecx,esp`), CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) 0x0073ACC0.
// Shape: as the landed CCombatAction_FallStunned::Initialise 0x008ED950; the owner argument goes through the intelligent pointer's
// inline `operator CThing*()` (evaluated before the plain enemy GetPItem, as the in-chunk 0x008F4320 / 0x008F41D0), which also
// spills the out-parameter manager to the frame; the `new` null test selects 0 for the counted pointer.
// Layout: PEnemy (CIntelligentPointer<CThingCreatureBase>) at retail +0x114 (PDB +0x12c); thing byte +0x91 bit 0 = dead.
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing { public: };

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    operator CThing*() const { return GetPItem(); }
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>
    CThingCreatureBase* GetPItem() const { return (CThingCreatureBase*)CBaseIntelligentPointer::GetPItem(); }
};

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

struct CModeBase { };
template <class T> class CCountedPointer { public:
    T*    m_pObject;                            // +0
    void* m_pCount;                             // +4
    CCountedPointer(T* object);                 // 0x00718A00
    CCountedPointer(const CCountedPointer& other);
};
struct CModeBriarRoseTeleportInvulnerable : CModeBase {
    unsigned char _pad[0x88];
    CModeBriarRoseTeleportInvulnerable(CThing* owner, CThingCreatureBase* enemy);   // 0x008C35D0
};
class CTCCreatureModeManager { public:
    bool AddMode(CCountedPointer<CModeBase> mode);   // 0x0073ACC0
};

class CCombatActionBase {
public:
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0x114 - 0x10];
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
};

class CCreatureAction_BriarRoseIntoInvulnerable : public CCombatActionBase {
public:
    CIntelligentPointerCreature PEnemy;         // +0x114 (PDB +0x12c)
    virtual void Initialise();
};

void CCreatureAction_BriarRoseIntoInvulnerable::Initialise()
{
    {
        CCharString name("INVULNERABLE_INTO");
        SetCombatAnimation(name);
    }
    CCombatActionBase::Initialise();
    CTCCreatureModeManager* modeManager;
    if (PCreature.GetPItem()->GetInterface(&modeManager, 0x31) && PEnemy.GetPItem() && !(PEnemy.GetPItem()->m_flags91 & 1))
        modeManager->AddMode(CCountedPointer<CModeBase>(new CModeBriarRoseTeleportInvulnerable(PCreature, PEnemy.GetPItem())));
}