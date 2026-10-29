// CAIStateGroup_TavernKeeper::Update() -- close the keeper's shop stock and reset the 0x23 component, walk back inside the pub if outside, then loop: serve payment requests (old jug point + receive payment), refill empty jug slots on the bar (fetch the jug, walk to it, fill it), or idle at random (do nothing / lean on the bar); terminates when stopped or any step fails
// Leaf: ?Update@CAIStateGroup_TavernKeeper
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00969470, length 698 bytes (aligned `and esp,-8` frame, virtual slot 12).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface: 0x90 shop keeper, 0x23, 0xb9 tavern bar),
//   CTCShopKeeper::GetShopTC @ 0x00798F50 (x2), CTCShop::SetStockState @ 0x00773C00, the 0x23 component's 0x007EE540
//   (manifest IS_THING_ALIVE<...> label is propagated), CThingBuilding::IsPointInside @ 0x0082D000, CThing::GetPosition
//   @ 0x004C73D0 / GetPhysPos @ 0x004C7950, AIAction_MoveToPosProximity @ 0x0097E890, GetPaymentRequest 0x009683A0,
//   GetOldJugPoint 0x00968860, ReceivePayment 0x009692C0, the brain's slot 2, IsStopping 0x008FD010, Terminate
//   0x008FCF40, CTCTavernBar::GetEmptyJugSlot 0x007BA1D0 / GetJug 0x007B9F60 (returns the jug pointer by value),
//   WalkToPoint 0x00968230, FillJug 0x00968FA0, FreeBarPoint 0x00968950, GetPWorldSeed 0x008FCFD0 + GFROR13
//   0x00497890 (inline random step), AIAction_DoNothing 0x0097E200, LeanOnBar 0x009689C0, CBaseIntelligentPointer.
// Layout: engine/CAIStateGroup_TavernKeeper.h over the landed LeanOnBar 0x009689C0 model; the pub's physics body +0x60
//   holds the walk radius at +0x34.
// Shape: C3DVector has a user copy ctor so GetPhysPos builds straight into the by-value argument (`sub esp,0Ch ;
//   push esp`); the pub pointer (IsPointInside) and the pub thing for GetPhysPos are named locals fetched before the
//   argument pushes; the random idle is the landed inline seed step with `switch (r % 2)`.
class CTCBase;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class CRightHandedSet { public: float m[6]; };
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);       // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThingPhysicsBody;
class CThing { public:
    const C3DVector& GetPos() const;    // 0x004C73D0
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CThingPhysicsBody* PBody;                   // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            *ppOut = (T*)((it == map->last || interfaceType < it->first) ? map->last : it)->second;
            return true;
        }
        return false;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    CBaseIntelligentPointer& operator=(const CBaseIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
};
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
class CThingCreatureBase;
class CCreatureActionBase { public:
    virtual void v00();
    unsigned char _body[0xa4];
    ~CCreatureActionBase();                     // 0x00693EF0
};
class CCreatureAction_PlayIntoLoopOutOfAnimation : public CCreatureActionBase { public:
    unsigned char _anim[0x14];
    CCharString LoopAnimName;                   // +0xbc
    CCharString OutOfAnimName;                  // +0xc0
    CCreatureAction_PlayIntoLoopOutOfAnimation(CThingCreatureBase* creature, const CCharString& into,
        const CCharString& loop, const CCharString& outOf, float loopTime, bool b, long c, long d);   // 0x008427E0
};
class CAIBrain { public:
    virtual void v00(); virtual void v01();
    virtual void WaitForNextUpdate();           // slot 2 (+0x08)
    unsigned char _pad[0x18 - 4];
    CThing* PCreature;                          // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    void Terminate();                           // 0x008FCF40
    bool IsStopping();                          // 0x008FD010
    long AIAction_DoNothing();                  // 0x0097E200
    long AIAction_MoveToPosProximity(long repeat, C3DVector pos, float radius, bool* arrived, long movement, bool b1,
        bool b2, bool b3, CThing* faceThing, bool b4, float f);    // 0x0097E890
    CThing* GetCreature() const { return PBrain->PCreature; }
    CThingCreatureBase* GetCreatureBase() const { return (CThingCreatureBase*)PBrain->PCreature; }
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    void SetPushableByHero(bool pushable);      // 0x0097E330
    long AIAction_PerformAction(CCreatureActionBase* action);   // 0x0097D8B0
};
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890
class CTCShop { public:
    void SetStockState(long state);             // 0x00773C00
};
class CTCShopKeeper { public:                          // interface 0x90
    CTCShop* GetShopTC();                       // 0x00798F50
};
struct CTCScratch23 {                           // interface 0x23
    bool Reset();                               // 0x007EE540 (manifest IS_THING_ALIVE<...> label is propagated)
};
struct CThingPhysicsBody {
    unsigned char _pad[0x34];
    float Radius;                               // +0x34
};
struct CThingBuildingModel : CThing {
    bool IsPointInside(const C3DVector& pos);   // 0x0082D000
};
struct CThingPos : CThing {                     // no data of its own: PBody is CThing's +0x60
    C3DVector GetPhysPos() const;               // 0x004C7950
};
struct CIntelligentPointerObject : CBaseIntelligentPointer {     // CIntelligentPointer<CThingObject>
    virtual void v00();
};
class CTCTavernBar { public:                           // interface 0xb9
    bool GetEmptyJugSlot(CThing* creature, C3DVector* pos, CRightHandedSet* rh, long* actionPoint, long* slot);   // 0x007BA1D0
    CIntelligentPointerObject GetJug(long slot);   // 0x007B9F60
};
struct CAIStateGroup_TavernKeeper : CAIStateGroupBase {
    CBaseIntelligentPointer PJug;               // +0x14 CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer PBar;               // +0x1c CIntelligentPointer<CThingObject>
    CBaseIntelligentPointer PPub;               // +0x24 CIntelligentPointer<CThingBuilding>
    CBaseIntelligentPointer PPayee;             // +0x2c CIntelligentPointer<CThingAICreature>
    long JugActionPointIndex;                   // +0x34
    C3DVector JugPos;                           // +0x38
    CRightHandedSet JugRHSet;                   // +0x44
    long JugSlot;                               // +0x5c
    bool WalkToPoint(C3DVector* pos, CRightHandedSet* rh);   // 0x00968230
    bool GetPaymentRequest();                   // 0x009683A0
    bool GetOldJugPoint();                      // 0x00968860
    void FreeBarPoint();                        // 0x00968950
    bool LeanOnBar();                           // 0x009689C0
    bool FillJug();                             // 0x00968FA0
    bool ReceivePayment();                      // 0x009692C0
    virtual void Update();
};

void CAIStateGroup_TavernKeeper::Update()
{
    CTCShopKeeper* shopKeeper;
    if (GetCreature()->GetInterface(&shopKeeper, 0x90) && shopKeeper->GetShopTC())
        shopKeeper->GetShopTC()->SetStockState(0);
    CTCScratch23* scratch;
    GetCreature()->GetInterface(&scratch, 0x23);
    scratch->Reset();
    CThingPos* creature = (CThingPos*)GetCreature();
    CThingBuildingModel* pub = (CThingBuildingModel*)PPub.GetPItem();
    if (!pub->IsPointInside(creature->GetPos())) {
        bool arrived;
        CThingPhysicsBody* body = ((CThingPos*)PPub.GetPItem())->PBody;
        CThingPos* pubThing = (CThingPos*)PPub.GetPItem();
        if (AIAction_MoveToPosProximity(1, pubThing->GetPhysPos(), body->Radius, &arrived, 3, true,
                false, true, 0, true, 0.0f) != 2) {
            Terminate();
            return;
        }
    }
    CTCTavernBar* bar;
    PBar.GetPItem()->GetInterface(&bar, 0xb9);
    for (;;) {
        if (GetPaymentRequest()) {
            if (!GetOldJugPoint()) {
                PBrain->WaitForNextUpdate();
                if (IsStopping()) {
                    Terminate();
                    return;
                }
                continue;
            }
            if (!ReceivePayment()) {
                if (IsStopping()) {
                    Terminate();
                    return;
                }
                PBrain->WaitForNextUpdate();
                if (IsStopping()) {
                    Terminate();
                    return;
                }
                continue;
            }
        }
        if (bar->GetEmptyJugSlot(GetCreature(), &JugPos, &JugRHSet, &JugActionPointIndex, &JugSlot)) {
            PJug = bar->GetJug(JugSlot);
            if (!WalkToPoint(&JugPos, &JugRHSet)) {
                FreeBarPoint();
                Terminate();
                return;
            }
            if (!FillJug()) {
                FreeBarPoint();
                Terminate();
                return;
            }
            FreeBarPoint();
            continue;
        }
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        switch (rotated % 2) {
        case 0:
            if (AIAction_DoNothing() == 1) {
                Terminate();
                return;
            }
            break;
        case 1:
            if (!LeanOnBar()) {
                Terminate();
                return;
            }
            break;
        }
    }
}