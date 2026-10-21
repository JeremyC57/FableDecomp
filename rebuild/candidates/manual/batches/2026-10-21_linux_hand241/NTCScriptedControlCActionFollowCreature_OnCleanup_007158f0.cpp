// NTCScriptedControl::CActionFollowCreature::OnCleanup() -- restore the creature's original physics component (re-added at the current position/orientation, then the saved component deleted), stop sneaking, restore hero-pushability when dynamic obstacles were not avoided, and clear the navigation's ignored thing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007158F0, length 372 bytes (plain `ret`).
// Callees: the physics component's virtual slot 72 (+0x120, the current right-handed set), CBase::CBase @ 0x0099A2F0 /
//          ~CBase @ 0x0099A300 (CTCParamsPhysicsBase, vtable 0x0125C818, inline ctor), the saved component's virtual slot 22
//          (+0x58, its name by value) and deleting destructor (slot 0), CThing::AddTC @ 0x004C9D60, ~CCharString @ 0x0099EAE0,
//          CActionFollowCreature::SetAsSneaking @ 0x00713D50, the physics component's slot 84 (+0x150, pushable-by-hero
//          flag as a long), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, 7 = CTCCreatureNavigation, the masked
//          byte test `test cl,cl ; jns`), CTCCreatureNavigation::SetThingToIgnore @ 0x006CBE30.
// Shape: the landed CCreatureAction_UseObject::FinishAction (0x0069F2B0) body: position and orientation copied into named
//          locals (C3DVector user copy ctor) and again into the inline-constructed params; the creature is a named local so
//          it is fetched before the pushes; the name is a by-value temporary; `delete` through the virtual deleting
//          destructor, pointer cleared; the pushable flag is a bool parameter (widened through edx); the navigation lookup is
//          the out-parameter GetInterface (masked byte test `test cl,cl ; jns`, component through eax).
// Layout: Ego_r.pdb NTCScriptedControl::CActionBase {vptr, PTCScript +4 (CTCBase: ParentThing +4), Activated +8,
//          Terminated +9}; CActionFollowCreature PTargetThing +0xc, PTCOldPhysics +0x18, Sneaking +0x1c, FollowRange +0x20,
//          AddedStealthTC +0x24, AvoidDynamicObstacles +0x25, PreviousPushableByHero +0x26, Running +0x27. CThing +0x60 is
//          the physics component (position +0xc).
struct CTCBase;
struct CThing;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct CCharString {
    void* PStringData;
    ~CCharString();                                   // 0x0099EAE0
};
struct CBase {
    virtual void v00();
    CBase();                                          // 0x0099A2F0
    ~CBase();                                         // 0x0099A300
};
struct CTCParamsBase : CBase { };
struct CTCParamsPhysicsBase : CTCParamsBase {         // vtable 0x0125C818, 0x28 bytes
    C3DVector       Position;                         // +0x04
    CRightHandedSet Rhs;                              // +0x10
    CTCParamsPhysicsBase(const C3DVector& position, const CRightHandedSet& rhs) : Position(position), Rhs(rhs) { }
    virtual void v00();
};
#define V10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); virtual void v##n##4(); \
    virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); virtual void v##n##8(); virtual void v##n##9();
struct CPhysicsComponent {
    V10(0) V10(1) V10(2) V10(3) V10(4) V10(5) V10(6)  // slots 0..69
    virtual void v70(); virtual void v71();
    virtual const CRightHandedSet& GetRHS();          // slot 72 (+0x120)
    virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77(); virtual void v78();
    virtual void v79(); virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83();
    virtual void SetPushableByHero(bool pushable);    // slot 84 (+0x150)
    unsigned char _pad04[0x08];
    C3DVector Position;                               // +0x0c
};
struct CSavedPhysicsComponent {                       // the saved CTCBase
    virtual ~CSavedPhysicsComponent();                // slot 0 (deleting destructor)
    V10(0) V10(1)                                     // slots 1..20
    virtual void v21();
    virtual CCharString GetTCName();                  // slot 22 (+0x58)
};
#undef V10

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CTCCreatureNavigation {                  // interface 7
    void SetThingToIgnore(const CThing* thing); // 0x006CBE30
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x60 - 0x4c];
    CPhysicsComponent* m_pPhysics;              // +0x60
    CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
    CPhysicsComponent* GetPhysics() const { return m_pPhysics; }
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
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
struct CTCScriptedControl {                     // a CTCBase
    virtual void v00();
    CThing* ParentThing;                        // +0x04
};
struct CScriptThing { unsigned char _body[12]; };

namespace NTCScriptedControl {

struct CActionFollowCreature {
    virtual ~CActionFollowCreature();
    CTCScriptedControl* PTCScript;              // +0x04
    bool Activated;                             // +0x08
    bool Terminated;                            // +0x09
    CScriptThing PTargetThing;                  // +0x0c
    CSavedPhysicsComponent* PTCOldPhysics;      // +0x18
    bool Sneaking;                              // +0x1c
    float FollowRange;                          // +0x20
    bool AddedStealthTC;                        // +0x24
    bool AvoidDynamicObstacles;                 // +0x25
    bool PreviousPushableByHero;                // +0x26
    bool Running;                               // +0x27
    void SetAsSneaking(bool sneaking);          // 0x00713D50
    void OnCleanup();
};

void CActionFollowCreature::OnCleanup()
{
    if (PTCOldPhysics) {
        C3DVector position = PTCScript->ParentThing->m_pPhysics->Position;
        CRightHandedSet rhs = PTCScript->ParentThing->GetPhysics()->GetRHS();
        CTCParamsPhysicsBase params(position, rhs);
        CThing* creature = PTCScript->ParentThing;
        creature->AddTC(PTCOldPhysics->GetTCName(), true, &params);
        delete PTCOldPhysics;
        PTCOldPhysics = 0;
    }
    if (Sneaking)
        SetAsSneaking(false);
    if (!AvoidDynamicObstacles)
        PTCScript->ParentThing->m_pPhysics->SetPushableByHero(PreviousPushableByHero);
    CTCCreatureNavigation* navigation;
    if (PTCScript->ParentThing->GetInterface(&navigation, 7))
        navigation->SetThingToIgnore(0);
}

}
