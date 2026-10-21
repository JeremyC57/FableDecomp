// CCreatureAction_DragonStrafeInto::Initialise() -- animation "STRAFE_INTO", base Initialise, then (mode manager present) push a new CModeDragonStrafe(owner, Timer, Speed, Timer * 0.125, Timer * 0.75) onto the owner's mode manager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F4480, length 233 bytes (+ int3 padding), plain `ret` (virtual, no arguments).
// Callees: CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("STRAFE_INTO" @ 0x01288514), SetAnimationName 0x008574D0,
// CCombatActionBase::Initialise 0x00857520 (direct), CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound 0x0040F020
// (inlined GetInterface 0x31 = CTCCreatureModeManager), operator new 0x00BFEA1A (0x58 bytes),
// CModeDragonStrafe::CModeDragonStrafe(CThing*, float, float, float, float) 0x008DDFB0, CCountedPointer<CModeBase>::CCountedPointer(CModeBase*)
// 0x00718A00, CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) 0x0073ACC0.
// Shape: as CCreatureAction_DragonBurninateInto::Initialise 0x008F4320 (in-chunk): the owner argument goes through the inline
// `operator CThing*()`, so its GetPItem call runs before the members are loaded and pushed; the two Timer fractions (0.125 @
// 0x0123095C, 0.75 @ 0x01238174) are named float locals computed before the allocation; the manager stays in ebx.
// Layout: Timer / Speed at retail +0x114 / +0x118 (PDB +0x12c / +0x130).
// GetInterface: the inline out-parameter lookup picks the entry with the end/hit ternary (`*out = (it == last || k <
//   it->type ? last : it)->component`), which loads the component straight into the register retail uses.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
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
            *ppOut = (T*)(it == map->last || interfaceType < it->type ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingCreatureBase : CThing { };

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    operator CThing*() const { return GetPItem(); }
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<CThingCreatureBase>
    CThingCreatureBase* GetPItem() const { return (CThingCreatureBase*)CBaseIntelligentPointer::GetPItem(); }
};

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

struct CModeBase { };
template <class T> struct CCountedPointer {
    T*    m_pObject;                            // +0
    void* m_pCount;                             // +4
    CCountedPointer(T* object);                 // 0x00718A00
    CCountedPointer(const CCountedPointer& other);
};
struct CModeDragonStrafe : CModeBase {
    unsigned char _pad[0x58];
    CModeDragonStrafe(CThing* owner, float timer, float speed, float rampUp, float rampDown);   // 0x008DDFB0
};
struct CTCCreatureModeManager {
    bool AddMode(CCountedPointer<CModeBase> mode);   // 0x0073ACC0
};

class CCombatActionBase {
public:
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;            // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

class CCreatureAction_DragonStrafeInto : public CCombatActionBase {
public:
    float Timer;                                // +0x114 (PDB +0x12c)
    float Speed;                                // +0x118 (PDB +0x130)
    virtual void Initialise();
};

void CCreatureAction_DragonStrafeInto::Initialise()
{
    {
        CCharString name("STRAFE_INTO");
        SetAnimationName(name);
    }
    CCombatActionBase::Initialise();
    CTCCreatureModeManager* modeManager;
    if (PCreature.GetPItem()->GetInterface(&modeManager, 0x31)) {
        float rampUp = Timer * 0.125f;
        float rampDown = Timer * 0.75f;
        modeManager->AddMode(CCountedPointer<CModeBase>(new CModeDragonStrafe(PCreature, Timer, Speed, rampUp, rampDown)));
    }
}