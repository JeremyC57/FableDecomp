// CCreatureAction_BalverineCorneredJumpIntoSky::FinishAction() -- base FinishAction; make the balverine willable again, flying flag on, full alpha; when the jump ran its full time: put it down at EndPos facing a living target, enter an intangible mode (the battle def's time, else 5s) that ends in a lunge at the target (or a drop-in from the sky), and detach its attached particles
// Leaf: ?FinishAction@CCreatureAction_BalverineCorneredJumpIntoSky
// Retail: Fable.exe (TLC, VC7.1) @ 0x008CCD70, 685 bytes (+ int3 padding), virtual FinishAction, plain `ret`.
// Callees: CCombatActionBase::FinishAction 0x00859A20 (direct), CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +8,
//   PTarget +0xa8), SetThingAsNotBloodyWillableAtAll 0x007E4040 (__fastcall, dl = false), the physics TC's slots 84 SetFlying
//   (+0x150), 31 SetPosition (+0x7c) and 75 SetRHSet (+0x12c), CTCGraphicAppearance::SetAlpha 0x004BFAC0, CThing::GetPos
//   0x004C73D0, C3DVector::AccurateNormalise @ 0x00A14440, CVectorMap::LowerBound 0x0040F020 (inlined GetInterface: 0x31
//   CTCCreatureModeManager bit 17 of +0x24, 0x82 attached particles bit 2 of +0x30), CDefClassBase::GetSubDef
//   <CBalverineBattleDef> 0x008CD0D0 ("CBalverineBattleDef" 0x01243B94; manifest "<CAICreatureWillPowerIndicatorDef>" is
//   propagated), operator new 0x00BFEA1A, CCreatureAction_BalverineLungeAttack(owner, target) 0x008CCC10 and
//   CCreatureAction_BalverineDropInFromSky(owner) 0x008CCBD0 (manifest "CCombatAction_GenericResponseToDrain/-Decapitation"
//   are propagated; their vtables 0x01283A54 / 0x0128390C), NCreatureMode::CModeIntangible(CThing*, float,
//   CCreatureActionBase*) 0x008E72D0, CCountedPointer<CModeBase>(CModeBase*) 0x00718A00, CTCCreatureModeManager::AddMode
//   0x0073ACC0, the particles' detach 0x007F8860 (manifest "Unwind@00fbdab0" is wrong). Constant 5.0f stored as an immediate.
// Layout: retail CCombatActionBase is 0x114 bytes (Ego_r.pdb 0x12c): PCreature +0x08, TotalTime +0x10, FramesRunning +0x1c,
//   PTarget +0xa8; EndPos (C3DVector*) +0x114 (PDB +0x12c), so `this` is modelled locally (the engine header carries PDB
//   offsets). CThing: interface bitset +0x20, map +0x44, PhysicsTC +0x60, GraphicAppearanceTC +0x64, def +0x70, dead bit
//   +0x91. CBalverineBattleDef's intangible time is the float at +0x44.
// Shape: the facing is `C3DVector(to.X - from.X, to.Y - from.Y, 0)` (Y first: ctor arguments right to left) normalised, then
//   copied into a CRightHandedSet temporary built with Up (0,0,1); the target thing is a named local fetched before the
//   creature's position. The def pointer is a zeroed CDefPointer released by its inline dtor; GetSubDef has a same-TU
//   out-only stand-in so VC7.1 keeps the def in ebp after the call (and the mode manager in its frame word). The lunge
//   action takes PCreature through an inline `operator CThingCreatureBase*()` -- the creature is fetched BEFORE the
//   target's GetPItem, which also gives retail's register map (direct GetPItem arguments put the mode manager in ebp).
class CTCBase;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector& AccurateNormalise();                                   // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
};
class CRightHandedSet { public:
    C3DVector Up;
    C3DVector Forward;
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) { }
};
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
#define VF(n) virtual void v##n();
#define VF10(p) VF(p##0) VF(p##1) VF(p##2) VF(p##3) VF(p##4) VF(p##5) VF(p##6) VF(p##7) VF(p##8) VF(p##9)
class CTCPhysicsBase { public:
    VF10(0) VF10(1) VF10(2) VF(30)
    virtual void SetPosition(const C3DVector& position);    // slot 31 (+0x7c)
    VF(32) VF(33) VF(34) VF(35) VF(36) VF(37) VF(38) VF(39) VF10(4) VF10(5) VF10(6)
    VF(70) VF(71) VF(72) VF(73) VF(74)
    virtual void SetRHSet(const CRightHandedSet& set);      // slot 75 (+0x12c)
    VF(76) VF(77) VF(78) VF(79) VF(80) VF(81) VF(82) VF(83)
    virtual void SetFlying(bool flying);                    // slot 84 (+0x150)
};
struct CTCGraphicAppearance {
    void SetAlpha(unsigned char alpha);                     // 0x004BFAC0
};
struct CDefBase {
    virtual void v00();
    virtual void Destroy();                                 // slot 1
    long m_count;                                           // +0x04
    void Release()
    {
        if (--m_count == 0)
            Destroy();
    }
};
class CBalverineBattleDef : public CDefBase { public:
    unsigned char _pad08[0x44 - 0x08];
    float IntangibleTime;                                   // +0x44
};
template<class T> class CDefPointer { public:
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        if (m_p)
            m_p->Release();
    }
    T* operator->() const { return m_p; }
};
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CBalverineBattleDef>& out);  // 0x008CD0D0
};
void* FindSubDef_CBalverineBattleDef(CDefClassBase* defClass);   // stand-in callee of the stand-in body below
// Stand-in body (out of line in retail @ 0x008CD0D0): it writes the out slot only, so VC7.1 knows the def pointer's
// address does not escape the call.
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CBalverineBattleDef>& out)
{
    void* def = FindSubDef_CBalverineBattleDef(this);
    if (!def)
        return false;
    out.m_p = (CBalverineBattleDef*)def;
    return true;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char   _pad0[0x20];
    unsigned long   m_presentBits[9];                   // +0x20
    CInterfaceMap   m_interfaces;                       // +0x44
    unsigned char   _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    CTCGraphicAppearance* GraphicAppearanceTC;          // +0x64
    unsigned char   _pad68[0x70 - 0x68];
    CDefClassBase*  m_pDef;                             // +0x70
    unsigned char   _pad74[0x91 - 0x74];
    unsigned char   m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPos() const;               // 0x004C73D0
    CTCPhysicsBase* GetPhysics() const { return PhysicsTC; }
    CTCGraphicAppearance* GetGraphicAppearance() const { return GraphicAppearanceTC; }
    CDefClassBase* GetDef() const { return m_pDef; }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(type);
            if (it == map->last || interfaceType < it->first)
                it = map->last;
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing { public: };
void __fastcall SetThingAsNotBloodyWillableAtAll(CThing* thing, bool notWillable);   // 0x007E4040

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                                    // +0x04
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct CCreaturePointer {
    void*               __vftable;
    CThingCreatureBase* m_pItem;
    CThingCreatureBase* GetPItem() const;               // 0x00A01B50
    operator CThingCreatureBase*() const { return GetPItem(); }
};

__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
struct CModeBase { };
template <class T> class CCountedPointer { public:
    T*    m_pObject;
    void* m_pCount;
    CCountedPointer(T* object);                         // 0x00718A00
    CCountedPointer(const CCountedPointer& other);
};
class CCreatureActionBase;
struct CModeIntangible : CModeBase {
    unsigned char _pad[0x44];
    CModeIntangible(CThing* thing, float time, CCreatureActionBase* nextAction);   // 0x008E72D0
};
class CTCCreatureModeManager { public:                         // interface 0x31
    bool AddMode(CCountedPointer<CModeBase> mode);      // 0x0073ACC0
};
struct CTCAttachedParticles {                           // interface 0x82
    void DetachParticles();                             // 0x007F8860
};

class CCreatureActionBase { public:
    virtual ~CCreatureActionBase();
};
class CCombatActionBase : public CCreatureActionBase { public:        // retail sizeof 0x114
    unsigned char           _pad04[4];
    CCreaturePointer        PCreature;                  // +0x08
    long                    TotalTime;                  // +0x10
    unsigned char           _pad14[0x1c - 0x14];
    long                    FramesRunning;              // +0x1c
    unsigned char           _pad20[0xa8 - 0x20];
    CBaseIntelligentPointer PTarget;                    // +0xa8
    unsigned char           _padb0[0x114 - 0xb0];
    virtual void FinishAction();                        // 0x00859A20
};
class CCreatureAction_BalverineDropInFromSky : public CCombatActionBase { public:    // vtable 0x0128390C
    CCreatureAction_BalverineDropInFromSky(CThingCreatureBase* owner); // 0x008CCBD0
};
namespace N_008ccc10 { struct CCreatureAction_BalverineLungeAttack : CCreatureAction_BalverineDropInFromSky {   // vtable 0x01283A54
    CCreatureAction_BalverineLungeAttack(CThingCreatureBase* owner, CThing* target);    // 0x008CCC10
}; } using N_008ccc10::CCreatureAction_BalverineLungeAttack;
class CCreatureAction_BalverineCorneredJumpIntoSky : public CCombatActionBase { public:
    C3DVector* EndPos;                                  // +0x114
    virtual void FinishAction();
};

void CCreatureAction_BalverineCorneredJumpIntoSky::FinishAction()
{
    CCombatActionBase::FinishAction();
    SetThingAsNotBloodyWillableAtAll(PCreature.GetPItem(), false);
    PCreature.GetPItem()->GetPhysics()->SetFlying(true);
    PCreature.GetPItem()->GetGraphicAppearance()->SetAlpha(0xff);
    if (TotalTime >= 0 && FramesRunning >= TotalTime) {
        if (IS_THING_ALIVE(PTarget)) {
            CTCPhysicsBase* physics = PCreature.GetPItem()->GetPhysics();
            physics->SetPosition(*EndPos);
            CThing* target = PTarget.GetPItem();
            const C3DVector& from = PCreature.GetPItem()->GetPos();
            const C3DVector& to = target->GetPos();
            C3DVector forward(to.X - from.X, to.Y - from.Y, 0.0f);
            forward.AccurateNormalise();
            physics->SetRHSet(CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), forward));
        }
        CTCCreatureModeManager* modeManager;
        if (PCreature.GetPItem()->GetInterface(&modeManager, 0x31)) {
            CDefPointer<CBalverineBattleDef> def;
            float time = 5.0f;
            if (PCreature.GetPItem()->GetDef()->GetSubDef(def))
                time = def->IntangibleTime;
            CCreatureActionBase* next;
            if (IS_THING_ALIVE(PTarget))
                next = new CCreatureAction_BalverineLungeAttack(PCreature, PTarget.GetPItem());
            else
                next = new CCreatureAction_BalverineDropInFromSky(PCreature.GetPItem());
            modeManager->AddMode(CCountedPointer<CModeBase>(new CModeIntangible(PCreature.GetPItem(), time, next)));
        }
        CTCAttachedParticles* particles;
        if (PCreature.GetPItem()->GetInterface(&particles, 0x82))
            particles->DetachParticles();
    }
}