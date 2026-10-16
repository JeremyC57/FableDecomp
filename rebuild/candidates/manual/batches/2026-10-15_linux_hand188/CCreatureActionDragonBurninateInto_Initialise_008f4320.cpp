// CCreatureAction_DragonBurninateInto::Initialise() -- animation "INTO_BURNINATE", base Initialise, then push a new CModeDragonStrafeBurninate(owner, the movement points, previous point, burn start/stop, flame start/end) onto the owner's mode manager
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F4320, length 218 bytes, plain `ret` (virtual, no arguments).
// Callees: CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("INTO_BURNINATE" @ 0x012884F0), SetAnimationName 0x008574D0,
// CCombatActionBase::Initialise 0x00857520 (direct), CBaseIntelligentPointer::GetPItem 0x00A01B50, CVectorMap::LowerBound 0x0040F020
// (inlined GetInterface 0x31 = CTCCreatureModeManager), operator new 0x00BFEA1A (0xc8 bytes),
// CModeDragonStrafeBurninate::CModeDragonStrafeBurninate(CThing*, const vector<C3DVector>&, const C3DVector&, float, float,
// const C3DVector&, const C3DVector&) 0x008E1580, CCountedPointer<CModeBase>::CCountedPointer(CModeBase*) 0x00718A00,
// CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) 0x0073ACC0.
// Shape: the owner argument goes through the intelligent pointer's inline `operator CThing*()` (an inline call argument is
// evaluated before the plain member arguments: `call GetPItem` precedes the pushes; a direct `.GetPItem()` is fetched last);
// the mode-manager GetInterface out-parameter form;
// (tested here, as in 0x008C1E80).
// Layout: retail PreviousPoint +0x114, MovementPoints (std::vector<C3DVector>, 12 bytes) +0x120, BurnStart +0x12c, BurnStop
// +0x130, FlameStartPos +0x134, FlameEndPos +0x140 (PDB +0x12c.. with a 16-byte vector).
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
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
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

struct C3DVector {
    float X, Y, Z;
};
struct CVectorC3DVector {                       // std::vector<C3DVector> (retail 12 bytes)
    C3DVector* _First;
    C3DVector* _Last;
    C3DVector* _End;
};

struct CModeBase { };
template <class T> struct CCountedPointer {
    T*    m_pObject;                            // +0
    void* m_pCount;                             // +4
    CCountedPointer(T* object);                 // 0x00718A00
    CCountedPointer(const CCountedPointer& other);
};
struct CModeDragonStrafeBurninate : CModeBase {
    unsigned char _pad[0xc8];
    CModeDragonStrafeBurninate(CThing* owner, const CVectorC3DVector& points, const C3DVector& previous, float burnStart, float burnStop,
                               const C3DVector& flameStart, const C3DVector& flameEnd);   // 0x008E1580
};
struct CTCCreatureModeManager {
    bool AddMode(CCountedPointer<CModeBase> mode);   // 0x0073ACC0
};

class CCombatActionBase {
public:
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0x114 - 0x10];
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

class CCreatureAction_DragonBurninateInto : public CCombatActionBase {
public:
    C3DVector        PreviousPoint;             // +0x114 (PDB +0x12c)
    CVectorC3DVector MovementPoints;            // +0x120 (PDB +0x138)
    float            BurnStart;                 // +0x12c (PDB +0x148)
    float            BurnStop;                  // +0x130 (PDB +0x14c)
    C3DVector        FlameStartPos;             // +0x134 (PDB +0x150)
    C3DVector        FlameEndPos;               // +0x140 (PDB +0x15c)
    virtual void Initialise();
};

void CCreatureAction_DragonBurninateInto::Initialise()
{
    {
        CCharString name("INTO_BURNINATE");
        SetAnimationName(name);
    }
    CCombatActionBase::Initialise();
    CTCCreatureModeManager* modeManager;
    if (PCreature.GetPItem()->GetInterface(&modeManager, 0x31))
        modeManager->AddMode(CCountedPointer<CModeBase>(new CModeDragonStrafeBurninate(PCreature, MovementPoints, PreviousPoint,
                                                                                       BurnStart, BurnStop, FlameStartPos, FlameEndPos)));
}
