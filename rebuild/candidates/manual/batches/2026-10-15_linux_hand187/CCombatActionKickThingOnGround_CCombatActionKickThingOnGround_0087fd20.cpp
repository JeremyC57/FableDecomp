// CCombatAction_KickThingOnGround::CCombatAction_KickThingOnGround(CThingCreatureBase* creature, CThing* target) -- base action (code 150, zeroed sound pair), the kick vector = sum of both things' (+0xc - +0x18) vectors, and the +0x12c float from the creature's virtual slot 81 on its +0x60 object's slot 70 result
// Leaf: ??0CCombatAction_KickThingOnGround
// Retail: Fable.exe (TLC, VC7.1) @ 0x0087FD20, 205 bytes (208 with padding), `ret 8`; vtable 0x01277D3C.
// Callees: CCombatActionBase::CCombatActionBase(owner, thing, code, sound) 0x00857F30; the creature's +0x60 object's virtual
//   slot 70 (+0x118, fills a C3DVector out-parameter and returns it), the creature's virtual slot 81 (+0x144, float).
// Layout: KickVector +0x120, KickValue +0x12c (retail); the +0x60 object of a thing has two C3DVectors at +0xc and +0x18.
// Shape: C3DVector operator-/operator+ inline, returning by value; the sum is assigned as a dword block. The target's
//   difference is the left operand (the right one, the creature's, is evaluated first); the position out-parameter is a
//   block-scoped local so it shares the frame with the vector temporaries (sub esp,24h).
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};

struct CThingPhysics {                          // the thing's +0x60 object
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual const C3DVector& GetPosition(C3DVector* out);   // slot 70 (+0x118)
    unsigned char _pad04[8];
    C3DVector m_current;                        // +0x0c
    C3DVector m_previous;                       // +0x18
};

struct CThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77();
    virtual void v78(); virtual void v79(); virtual void v80();
    virtual float GetDistanceTo(const C3DVector& position);   // slot 81 (+0x144)
    unsigned char _pad04[0x5c];
    CThingPhysics* m_pPhysics;                  // +0x60
};

struct CThingCreatureBase : CThing { };

struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

struct CCombatActionBase {
    virtual void v00();
    CCombatActionBase(CThingCreatureBase* owner, CThing* thing, long actionCode, const CSoundPair& sound);   // 0x00857F30
    unsigned char _body[0x120 - 4];
};

struct CCombatAction_KickThingOnGround : CCombatActionBase {
    C3DVector KickVector;                       // +0x120
    float KickValue;                            // +0x12c
    virtual void v00();
    CCombatAction_KickThingOnGround(CThingCreatureBase* creature, CThing* target);
};

CCombatAction_KickThingOnGround::CCombatAction_KickThingOnGround(CThingCreatureBase* creature, CThing* target)
    : CCombatActionBase(creature, target, 150, CSoundPair())
{
    KickVector = (target->m_pPhysics->m_current - target->m_pPhysics->m_previous)
               + (creature->m_pPhysics->m_current - creature->m_pPhysics->m_previous);
    {
        C3DVector position;
        KickValue = creature->GetDistanceTo(creature->m_pPhysics->GetPosition(&position));
    }
}
