// CCreatureAction_DragonFlyInto::Initialise() -- animation FLY_INTO, base Initialise, restart the running time; when the
// action has a single animation set, start it at the fraction of the delay-to-total window already run; when the dragon
// has a creature-mode manager, add a CModeDragonFlying over MovementPoints from PreviousPoint (or the first point) at Speed
// Leaf: ?Initialise@CCreatureAction_DragonFlyInto
struct CTCBase;
struct C3DVector { float X, Y, Z; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
struct CPhysicsBody {                                 // the thing's physics body at +0x60
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
    VF10(0) VF10(1) VF10(2) VF(30)
    virtual void SetPosition(const C3DVector& position);   // slot 31 (+0x7c)
    VF(32) VF(33) VF(34) VF(35)
    virtual void SetFlying(bool flying);              // slot 36 (+0x90)
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CPhysicsBody* PhysicsBody;                        // +0x60
    CPhysicsBody* GetPhysics() const { return PhysicsBody; }
    const C3DVector& GetPosition() const;             // 0x004C73D0
    void RemoveTC(long interfaceType);                // 0x004C9840
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
struct CAnimSet {
    unsigned char _pad[0x1c];
    float StartFraction;                              // +0x1c
    float CurrentFraction;                            // +0x20
};
CAnimSet* __fastcall GetSingleActionAnimSet(CThing* creature, long action);   // 0x008F4AA0
struct CAnimationEntry { long BankIndex; };
extern long g_framesPerSecond;                        // 0x01375550
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};

struct C3DVectorVector {                              // STLport vector<C3DVector>, 12 bytes in retail
    C3DVector* _M_start;
    C3DVector* _M_finish;
    C3DVector* _M_end_of_storage;
    C3DVector* begin() { return _M_start; }
    C3DVector& operator[](long i) { return _M_start[i]; }
    C3DVector* insert(C3DVector* where, const C3DVector& value);   // 0x007653B0
};

struct SCountBlock {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
struct CCreatureActionBase;
struct CCountedActionPointer {                        // CCountedPointer<CCreatureActionBase>
    CCreatureActionBase* m_pObject;
    SCountBlock* m_pCount;
    CCountedActionPointer(const CCountedActionPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
    ~CCountedActionPointer();
};
struct CModeBase { virtual void v00(); };
struct CCountedModePointer {
    CModeBase* m_pObject;
    SCountBlock* m_pCount;
    CCountedModePointer(CModeBase* object);           // 0x00718A00
    CCountedModePointer(const CCountedModePointer& other);
    ~CCountedModePointer();
};
struct CModeDragonFlying : CModeBase {
    unsigned char _body[0x90];                        // sizeof = 0x94
    CModeDragonFlying(CThing* creature, const C3DVectorVector& path, const C3DVector& pos, float speed,
                      CCountedActionPointer action);  // 0x008E1150
};
struct CTCCreatureModeManager {                       // interface 0x31
    bool AddMode(CCountedModePointer mode);           // 0x0073ACC0
};

struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                         // 0x00A01B50
};
struct CCombatActionBase {                            // retail sizeof 0x114
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual long GetAnimDelayTime();                  // slot 6 (+0x18)
    VF(07) VF(08) VF(09) VF10(1) VF10(2) VF10(3) VF10(4) VF10(5) VF(60) VF(61)
    virtual float GetStartRunningTimeSeconds();       // slot 62 (+0xf8)
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    long TotalTime;                                   // +0x10
    unsigned char _pad14[0x40 - 0x14];
    float RunningTimeSeconds;                         // +0x40
    unsigned char _pad44[0x74 - 0x44];
    CAnimationEntry* PAnimEntry;                      // +0x74
    unsigned char _pad78[0x114 - 0x78];
    void SetAnimationName(const CCharString& name);   // 0x008574D0
    void Initialise();                                // 0x00857520
    CThing* GetCreature() const { return PCreature.GetPItem(); }
};
struct CCreatureAction_DragonFlyInto : CCombatActionBase {
    C3DVector             Dest;                       // +0x114 (PDB +0x12C)
    C3DVector             StartPos;                   // +0x120
    C3DVector             PreviousPoint;              // +0x12C
    C3DVectorVector       MovementPoints;             // +0x138
    float                 Speed;                      // +0x144
    bool                  UsePreviousPoint;           // +0x148
    CCountedActionPointer PEndAction;                 // +0x14C
    void Initialise();
};

void CCreatureAction_DragonFlyInto::Initialise()
{
    {
        CCharString name("FLY_INTO");
        SetAnimationName(name);
    }
    CCombatActionBase::Initialise();
    RunningTimeSeconds = GetStartRunningTimeSeconds();
    CAnimSet* set = GetSingleActionAnimSet(PCreature.GetPItem(), PAnimEntry->BankIndex);
    float fps = (float)g_framesPerSecond;
    if (set) {
        float delay = GetAnimDelayTime() / fps;
        float fraction = (RunningTimeSeconds - delay) / (TotalTime / fps - delay);
        if (fraction < 0.0f)
            fraction = 0.0f;
        else if (fraction > 1.0f)
            fraction = 1.0f;
        set->StartFraction = fraction;
        set->CurrentFraction = fraction;
    }
    CTCCreatureModeManager* modes;
    if (PCreature.GetPItem()->GetInterface(&modes, 0x31)) {
        if (!UsePreviousPoint)
            PreviousPoint = MovementPoints[0];
        modes->AddMode(CCountedModePointer(new CModeDragonFlying(GetCreature(), MovementPoints, PreviousPoint, Speed, PEndAction)));
    }
}