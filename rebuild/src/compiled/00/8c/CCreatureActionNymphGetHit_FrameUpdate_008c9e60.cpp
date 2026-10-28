// CCreatureAction_NymphGetHit::FrameUpdate() -- base strike-response FrameUpdate, then keep the nymph's "DUMMY_CENTRE" helper where it was when the action started: move the nymph by (InitDummyPos - current helper position) and mark it as not moving
// Leaf: ?FrameUpdate@CCreatureAction_NymphGetHit@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C9E60, 222 bytes, thiscall, plain `ret` (virtual).
// Identity (proven): RTTI vtable 0x01263F84 (.?AVCCreatureAction_NymphGetHit@@) slot 56 = FrameUpdate (slot 56 of the
//   CCreatureActionBase family, e.g. landed CCreatureAction_FireMissileWeapon::FrameUpdate 0x008D5000); Ego_r.pdb
//   ?FrameUpdate@CCreatureAction_NymphGetHit@@UAEXXZ (donor 0x009014D0, between NymphSummonOutOf::FinishAction 0x009014A0 =
//   retail 0x008C9E40 and NymphGetHitDie::FrameUpdate 0x009015D0 = retail 0x008C9F40, both landed). Literal 0x0126F110
//   "DUMMY_CENTRE".
// Callees: CCombatAction_GenericStrikeResponseBase::FrameUpdate 0x00865C20 (landed), CCharString ctor/dtor 0x0099EBF0 /
//   0x0099EAE0, CBaseIntelligentPointer::GetPItem 0x00A01B50, NGraphicAppearance::CTCGraphicAppearance::GetDummyObject
//   (const CCharString&, long, unsigned long, C3DVector&, CRightHandedSet&, float) const 0x004C08C0, CThing::GetPos
//   0x004C73D0, CTCPhysicsBase::SetPosition (virtual slot 30, +0x78; RTTI 0x0125D2C4 slot 30), CTCPhysicsBase::
//   SetAsMoving(bool) 0x006B0130.
// Shape: the Ego_r inline accessors GetTCGraphicAppearance() / GetPhysicsTC() (not the raw members) give retail's order: the
//   helper string and the component are evaluated before the arguments are pushed, and `mov ecx,[eax+60h]` precedes
//   `push 0`.
// Layout: retail CThing PhysicsTC +0x60, PTCGraphicAppearance +0x64; InitDummyPos +0x148 (engine/CCreatureAction_NymphGetHit.h).
class CCharString {
public:
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
    void* PStringData;
};

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
    C3DVector operator-(const C3DVector& v) const { return C3DVector(X - v.X, Y - v.Y, Z - v.Z); }
};

class CRightHandedSet {
public:
    CRightHandedSet() { }
    C3DVector Up;
    C3DVector Forward;
};

namespace NGraphicAppearance {
class CTCGraphicAppearance {
public:
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector& pos, CRightHandedSet& set,
                        float scale) const;                  // 0x004C08C0
};
}

class CTCPhysicsBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void SetPosition(const C3DVector& pos);          // slot 30 (+0x78)
    void SetAsMoving(bool moving);                           // 0x006B0130
};

class CThing {
public:
    virtual ~CThing();
    unsigned char _pad_0x04[0x60 - 0x04];
    CTCPhysicsBase* PhysicsTC;                               // +0x60
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
    const C3DVector& GetPos() const;                         // 0x004C73D0
    CTCPhysicsBase* GetPhysicsTC() { return PhysicsTC; }     // Ego_r ?GetPhysicsTC@CThing@@QAEPAVCTCPhysicsBase@@XZ (inline)
    NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance() { return PTCGraphicAppearance; }   // Ego_r (inline)
};

class CThingCreatureBase : public CThing { };

class CBaseObject;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    void* PData;
};

template <class T>
class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    virtual ~CIntelligentPointer();
    T* operator->() const { return (T*)GetPItem(); }
};

class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();
    unsigned char _pad_0x04[4];
    CIntelligentPointer<CThingCreatureBase> PCreature;       // +0x08
    unsigned char _pad_0x10[0xa8 - 0x10];
};

class CCombatAction_GenericStrikeResponseBase : public CCreatureActionBase {
public:
    virtual void FrameUpdate();                              // 0x00865C20
    unsigned char _pad_0xa8[0x148 - 0xa8];
};

class CCreatureAction_NymphGetHit : public CCombatAction_GenericStrikeResponseBase {
public:
    C3DVector InitDummyPos;                                  // +0x148
    virtual void FrameUpdate();
};

void CCreatureAction_NymphGetHit::FrameUpdate()
{
    CCombatAction_GenericStrikeResponseBase::FrameUpdate();
    C3DVector dummy_pos;
    CRightHandedSet dummy_set;
    if (PCreature->GetTCGraphicAppearance()->GetDummyObject("DUMMY_CENTRE", 0, 0, dummy_pos, dummy_set, 1.0f)) {
        PCreature->GetPhysicsTC()->SetPosition((InitDummyPos - dummy_pos) + PCreature->GetPos());
        PCreature->GetPhysicsTC()->SetAsMoving(false);
    }
}