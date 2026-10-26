// CCreatureAction_UseObject::FinishAction() -- when the action carries component params, add the named component to the creature placed at its current position/orientation, then delete the params; then the base FinishAction
// Leaf: ?FinishAction@CCreatureAction_UseObject
// Retail: Fable.exe (TLC, VC7.1) @ 0x0069F2B0, 303 bytes (304 with padding), virtual FinishAction.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +0x08), the physics component's virtual slot 72 (+0x120,
//   the current right-handed set; name unconfirmed), CBase::CBase 0x0099A2F0 / ~CBase 0x0099A300 (CTCParamsPhysicsBase,
//   vtable 0x0125C818, inline ctor), the params object's virtual slot 22 (+0x58, the component name by value) and its
//   deleting destructor (slot 0), CThing::AddTC(const CCharString&, bool, CTCParamsBase*) 0x004C9D60, ~CCharString
//   0x0099EAE0, CCreatureActionBase::FinishAction 0x00694430.
// Layout: retail CCreatureActionBase is 0xa8 bytes; this class's params pointer at +0xb0; CThing +0x60 the physics
//   component (its position at +0x0c).
// Shape: the position and orientation are named locals copied memberwise (C3DVector has a user copy ctor, as in the
//   landed CTCParamsPhysicsBase ctor 0x006A06E0) and copied again into the inline-constructed params object; the
//   creature (AddTC's object) is a named local, so it is fetched before the argument pushes (inline it is fetched last); the name is a by-value temporary; `delete` through
//   the virtual deleting destructor, then the pointer is cleared.
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };
class CCharString { public:
    void* PStringData;
    ~CCharString();                                   // 0x0099EAE0
};
namespace N_0099a300 { struct CBase {
    virtual void v00();
    CBase();                                          // 0x0099A2F0
    ~CBase();                                         // 0x0099A300
}; } using N_0099a300::CBase;
class CTCParamsBase : public CBase { public: };
class CTCParamsPhysicsBase : public CTCParamsBase { public:         // vtable 0x0125C818, 0x28 bytes
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
    unsigned char _pad04[0x08];
    C3DVector Position;                               // +0x0c
};
struct CUseObjectParams {
    virtual ~CUseObjectParams();                      // slot 0 (deleting destructor)
    V10(0) virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual CCharString GetTCName();                  // slot 22 (+0x58)
};
#undef V10
class CThing { public:
    unsigned char _pad00[0x60];
    CPhysicsComponent* m_pPhysics;                    // +0x60
    class CTCBase* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
    CPhysicsComponent* GetPhysics() const { return m_pPhysics; }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CThing* GetPItem() const;                         // 0x00A01B50
};
class CCreatureActionBase { public:                          // retail sizeof 0xa8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void FinishAction();                      // slot 4: 0x00694430
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0xa8 - 0x10];
};
class CCreatureAction_UseObject : public CCreatureActionBase { public:
    unsigned char _pada8[8];
    CUseObjectParams* m_pParams;                      // +0xb0
    virtual void FinishAction();
};

void CCreatureAction_UseObject::FinishAction()
{
    if (m_pParams) {
        C3DVector position = PCreature.GetPItem()->m_pPhysics->Position;
        CRightHandedSet rhs = PCreature.GetPItem()->GetPhysics()->GetRHS();
        CTCParamsPhysicsBase params(position, rhs);
        CThing* creature = PCreature.GetPItem();
        creature->AddTC(m_pParams->GetTCName(), true, &params);
        delete m_pParams;
        m_pParams = 0;
    }
    CCreatureActionBase::FinishAction();
}