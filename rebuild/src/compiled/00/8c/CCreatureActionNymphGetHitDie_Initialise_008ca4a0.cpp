// CCreatureAction_NymphGetHitDie::Initialise() -- FALL_DEAD animation, reconfigure the nymph's physics, stop its flying, record its height above the ground, then the base Initialise
// Leaf: ?Initialise@CCreatureAction_NymphGetHitDie
// Retail: Fable.exe (TLC, VC7.1) @ 0x008CA4A0, 251 bytes (256 with padding), virtual Initialise.
// Callees: CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0 ("FALL_DEAD" 0x012821F8), CCombatActionBase::SetCombatAnimation
//   0x008574D0, CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +0x08), the physics component's virtual slots 36
//   (+0x90, arg 1), 43 (+0xac, arg 2) and 80 (+0x140) (names unconfirmed), CVectorMap::LowerBound 0x0040F020 (inlined
//   GetInterface 0xF5 = CTCNymph, bit 21 of +0x3c), CTCNymph::StopFlyingAround(long) 0x0080F870, CThing::GetPos
//   0x004C73D0, CCreatureActionBase::GetWorldMap 0x00692C10, CWorldMap::GetHeightZBelowPos 0x0063E480,
//   CCombatActionBase::Initialise 0x00857520.
// Layout: as the landed sibling CCreatureAction_NymphGetHitDie::FrameUpdate 0x008C9F40: CThing +0x60 the physics
//   component, HeightOnInit at retail +0x148 (PDB +0x160).
// Shape: the animation name is a named local in its own block; the three physics calls each re-fetch the creature; the
//   physics component comes through the inline GetPhysics accessor (straight into ecx; the direct member read goes
//   through eax); the position is copied by value into the frame (three integer moves) and HeightOnInit = Z - ground height (`fsubr`).
class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
class CTCBase;
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
#define V10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); virtual void v##n##4(); \
    virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); virtual void v##n##8(); virtual void v##n##9();
struct CPhysicsComponent {
    V10(0) V10(1) V10(2)                              // slots 0..29
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void SetMovementType(long type);          // slot 36 (+0x90)
    virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42();
    virtual void SetCollisionType(long type);         // slot 43 (+0xac)
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    V10(5) V10(6) V10(7)                              // slots 50..79
    virtual void ResetMovement();                     // slot 80 (+0x140)
};
#undef V10
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CPhysicsComponent* m_pPhysics;                    // +0x60
    const C3DVector& GetPos() const;             // 0x004C73D0
    CPhysicsComponent* GetPhysics() const { return m_pPhysics; }
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
class CTCNymph { public:                                     // interface type 0xF5
    void StopFlyingAround(long mode);                 // 0x0080F870
};
class CWorldMap { public:
    float GetHeightZBelowPos(const C3DVector* position);   // 0x0063E480
};
class CCombatActionBase { public:                            // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;                // +0x08
    unsigned char _pad10[0x114 - 0x10];
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
    void Initialise();                                // 0x00857520
    CWorldMap* GetWorldMap();                         // 0x00692C10
};
class CCreatureAction_NymphGetHitDie : public CCombatActionBase { public:
    unsigned char _pad114[0x148 - 0x114];
    float HeightOnInit;                               // +0x148 (PDB +0x160)
    void Initialise();
};

void CCreatureAction_NymphGetHitDie::Initialise()
{
    {
        CCharString name("FALL_DEAD");
        SetCombatAnimation(name);
    }
    PCreature.GetPItem()->GetPhysics()->SetMovementType(1);
    PCreature.GetPItem()->GetPhysics()->SetCollisionType(2);
    PCreature.GetPItem()->GetPhysics()->ResetMovement();
    CTCNymph* nymph;
    if (PCreature.GetPItem()->GetInterface(&nymph, 0xF5))
        nymph->StopFlyingAround(-1);
    C3DVector position = PCreature.GetPItem()->GetPos();
    HeightOnInit = position.Z - GetWorldMap()->GetHeightZBelowPos(&position);
    CCombatActionBase::Initialise();
}