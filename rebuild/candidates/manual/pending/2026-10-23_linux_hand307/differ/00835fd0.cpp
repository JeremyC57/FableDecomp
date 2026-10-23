// Grade: DIFFER(611v611) -- only the 24-byte CRightHandedSet copy from the virtual +0x120 result stores right after each load; retail lags each store one load behind (the position copy just before it matches)
// CThingDeadCreature::OnCreate() -- copy the def's corpse graphic and the thing's scale onto the appearance, run the game-object OnCreate, preload the dead animation, give the body standard physics at its current place and orientation, a discovery and a targeting component, clear the killer, set up the dead animation, set the decay life from the def, clear the persistence bits and create the body reorienter
// Leaf: ?OnCreate@CThingDeadCreature
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00835FD0, length 611 bytes (`ret`, bool in al).
// Callees: CThingDeadCreature::PeekDefinition @ 0x00835B90 / PreloadDeadAnimation @ 0x00835BD0 / SetupDeadAnimation
//   @ 0x00835E30, CTCGraphicAppearance::SetScale @ 0x004BFA50, CThingGameObject::OnCreate @ 0x00838A40, the physics TC's
//   virtual +0x120 (orientation) / +0xac, CTCParamsPhysicsBase(const C3DVector&, const CRightHandedSet&) @ 0x006A06E0 and
//   its root base dtor @ 0x0099A300, CCharString(const char*, long) @ 0x0099EBF0 / dtor @ 0x0099EAE0, CThing::AddTC
//   @ 0x004C9D60 ("CTCPhysicsStandard" @ 0x01239F38, "CTCDiscovery" @ 0x0123AD54, "CTCTargeted" @ 0x01239FC0),
//   CTCPhysicsBase::SetAsMoving @ 0x006B0130, LowerBound @ 0x0040F020, CTCTargeted flags setter @ 0x007EA270,
//   CBaseIntelligentPointer ctor @ 0x00A01B10 / GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10
//   (CIntelligentPointer<const CThingCreatureBase> vtable 0x0125AAEC), operator new @ 0x00BFEA1A (0x28 bytes),
//   CThingBodyReorienter ctor @ 0x00863750 / dtor @ 0x00863790, operator delete @ 0x00BFE9BC.
// Layout (retail; Ego_r.pdb in brackets): CThing PhysicsTC +0x60, PTCGraphicAppearance +0x64, bitfields LevelPersistent /
//   GameStatePersisted bits 4/5 of +0x92 [+0xa2]; CThingPhysical Scale +0x9c [+0xb4], MaxHealth +0xb0 [+0xc8], Health
//   +0xb4 [+0xcc]; CThingDeadCreature PKiller +0xd8 [+0xf0], BodyReorienter +0xe4 [+0xfc], StartedDecay +0xe8 [+0x100];
//   CThingCreatureDef dead-body graphic +0x100, NumSecondsToPersistOnDeath +0x88; CTCGraphicAppearance Graphic +0x44;
//   CTCPhysicsBase Position +0x0c; CTCTargeted Targetable +0x10.
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet(const CRightHandedSet& o) : Up(o.Up), Forward(o.Forward) { }
};
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CBaseClassNonCopyable {
    virtual void v00();
    ~CBaseClassNonCopyable();                           // 0x0099A300
};
struct CTCParamsBase : CBaseClassNonCopyable { };
struct CTCParamsPhysicsBase : CTCParamsBase {
    C3DVector       Position;                           // +0x04
    CRightHandedSet Rhs;                                // +0x10
    CTCParamsPhysicsBase(const C3DVector& position, const CRightHandedSet& rhs);   // 0x006A06E0
};
#pragma pack(push, 2)
struct CEngineGraphic {
    unsigned long BankIndex;
    float         AnimStep;
    float         RenderSizeX;
    bool          AdditiveAlpha;
    unsigned char Type;
};
#pragma pack(pop)
struct CTCGraphicAppearance {
    unsigned char  _pad00[0x44];
    CEngineGraphic Graphic;                             // +0x44
    void SetScale(float scale);                         // 0x004BFA50
};
struct CThingCreatureDef {
    unsigned char  _pad00[0x88];
    float          NumSecondsToPersistOnDeath;          // +0x88
    unsigned char  _pad8c[0x100 - 0x8c];
    CEngineGraphic DeadGraphic;                         // +0x100
};
struct CTCPhysicsBase {
    void* m_vtable;
    unsigned char _pad04[0x0c - 0x04];
    C3DVector Position;                                 // +0x0c
    void SetAsMoving(bool moving);                      // 0x006B0130
};
struct CTCPhysicsVirtual {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42();
    virtual void SetCollisionFlags(long flags);         // slot 43 (+0xac)
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55();
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61();
    virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67();
    virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual const CRightHandedSet& GetRHS();            // slot 72 (+0x120)
};
struct CTCVirtual4 {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void Activate();                            // slot 4 (+0x10)
};
struct CTCTargeted {
    unsigned char _pad00[0x10];
    bool Targetable;                                    // +0x10
    void SetFlags(unsigned long flags);                 // 0x007EA270
};
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing;
class CThingCreatureBase;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CThing* item);                        // 0x00A01B90
};
struct CIntelligentPointerCreature : CBaseIntelligentPointer {   // CIntelligentPointer<const CThingCreatureBase>, vtable 0x0125AAEC
    virtual void v00();
    CIntelligentPointerCreature(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerCreature& operator=(const CIntelligentPointerCreature& other) { SetPItem(other.GetPItem()); return *this; }
};
class CThingDeadCreature;
struct CThingBodyReorienter {
    unsigned char _body[0x28];
    CThingBodyReorienter(CThingDeadCreature* owner);    // 0x00863750
    ~CThingBodyReorienter();                            // 0x00863790
};
void* operator new(unsigned int size);                  // 0x00BFEA1A
void operator delete(void* p);                          // 0x00BFE9BC
template<class T> struct scoped_ptr {
    T* px;
    void reset(T* p = 0)
    {
        T* old = px;
        px = p;
        delete old;
    }
};

__forceinline CTCParamsPhysicsBase MakePhysicsParams(CTCPhysicsBase* physics)
{
    C3DVector position = physics->Position;
    CRightHandedSet rhs = ((CTCPhysicsVirtual*)physics)->GetRHS();
    return CTCParamsPhysicsBase(position, rhs);
}
extern long g_framesPerSecond;                          // 0x01375550

class CThing {
public:
    void* m_vtable;
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    CTCGraphicAppearance* PTCGraphicAppearance;         // +0x64
    unsigned char _pad68[0x92 - 0x68];
    unsigned char DeletedOnOwnerLevelOutOfScope : 1;    // +0x92 bit 0
    unsigned char Drawable : 1;
    unsigned char Dying : 1;
    unsigned char ForceFrameUpdate : 1;
    unsigned char LevelPersistent : 1;                  // bit 4
    unsigned char GameStatePersisted : 1;               // bit 5
    CTCBase* AddTC(const CCharString& name, bool a2, CTCParamsBase* params);   // 0x004C9D60
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
};
class CThingGameObject : public CThing {
public:
    unsigned char _pad94[0x9c - 0x94];
    float Scale;                                        // +0x9c
    unsigned char _pada0[0xb0 - 0xa0];
    float MaxHealth;                                    // +0xb0
    float Health;                                       // +0xb4
    unsigned char _padb8[0xd8 - 0xb8];
    bool OnCreate();                                    // 0x00838A40
};
class CThingDeadCreature : public CThingGameObject {
public:
    CIntelligentPointerCreature PKiller;                // +0xd8
    long BodyDieTimer;                                  // +0xe0
    scoped_ptr<CThingBodyReorienter> BodyReorienter;    // +0xe4
    bool StartedDecay;                                  // +0xe8
    const CThingCreatureDef& PeekDefinition() const;    // 0x00835B90
    void PreloadDeadAnimation();                        // 0x00835BD0
    void SetupDeadAnimation();                          // 0x00835E30
    bool OnCreate();
};

bool CThingDeadCreature::OnCreate()
{
    CTCGraphicAppearance* appearance = PTCGraphicAppearance;
    StartedDecay = false;
    if (appearance) {
        const CEngineGraphic& graphic = PeekDefinition().DeadGraphic;
        appearance->Graphic = graphic;
        appearance->SetScale(Scale);
        if (CThingGameObject::OnCreate()) {
            PreloadDeadAnimation();
            CTCParamsPhysicsBase params = MakePhysicsParams(PhysicsTC);
            AddTC("CTCPhysicsStandard", true, &params);
            PhysicsTC->SetAsMoving(false);
            ((CTCPhysicsVirtual*)PhysicsTC)->SetCollisionFlags(0x10a);
            AddTC("CTCDiscovery", false, 0);
            CTCVirtual4* discovery;
            if (HasTC(0x40))
                discovery = (CTCVirtual4*)GetTC(0x40);
            discovery->Activate();
            CTCTargeted* targeted = (CTCTargeted*)AddTC("CTCTargeted", false, 0);
            targeted->SetFlags(4);
            targeted->Targetable = true;
            PKiller = CIntelligentPointerCreature(0);
            SetupDeadAnimation();
            MaxHealth = Health = g_framesPerSecond * PeekDefinition().NumSecondsToPersistOnDeath;
            LevelPersistent = 0;
            GameStatePersisted = 0;
            BodyReorienter.reset(new CThingBodyReorienter(this));
            return true;
        }
    }
    return false;
}
