// CCreatureAction_BodgeSideStrike::Initialise() -- pick the strike handedness from which side of the owner's facing the target stands (2D cross product > 0), animation "STRIKE_MEDIUM_SIDE", then CCombatActionBase::Initialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BF0F0, length 200 bytes (+ int3 padding), plain `ret` (virtual, no arguments).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (target +0xa8, owner +8), CThing::GetPosition 0x004C73D0 (twice), the
// owner physics body's (+0x60) virtual slot 70 (+0x118, GetFacing: C3DVector by hidden pointer),
// CThingCreatureBase::SetLastActionHandedness(ECombatAnimationHandedness) 0x006619F0, CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0
// ("STRIKE_MEDIUM_SIDE" @ 0x0127F304), SetAnimationName 0x008574D0, CCombatActionBase::Initialise 0x00857520. 0.0 @ 0x0122DEDC.
// Shape: the target is a named local fetched first; `target pos - owner pos` through an inline operator- evaluates the right
// operand first (the owner's position is fetched before the target's); the delta's Z is dead; the facing's Z is zeroed after the
// call (the escaped local keeps the store); the cross product's Z = d.X*f.Y - d.Y*f.X; the handedness is an if/else of two
// calls the compiler tail-merges into `push 1 / push 0` (a ternary argument gives `mov eax,1 / xor eax,eax`; it also swaps
// esi/edi); the facing comes through an inline creature forwarder (`mov ecx,[eax+60h] ; mov eax,[ecx]`; a direct
// `m_pBody->GetFacing()` loads the body into eax and the vptr into edx).
class CThing;

class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
};

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                             // 0x0099EAE0
};

struct CPhysicsBody {                           // the creature's physics body at +0x60
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
    virtual C3DVector GetFacing();              // slot 70 (+0x118), by value through the hidden pointer
};

enum ECombatAnimationHandedness { COMBAT_ANIM_LEFT_HANDED = 0, COMBAT_ANIM_RIGHT_HANDED = 1 };

class CThingCreatureBase { public:
    unsigned char _pad00[0x60];
    CPhysicsBody* m_pBody;                      // +0x60
    const C3DVector& GetPosition_004c73d0() const;       // 0x004C73D0 (CThing)
    C3DVector GetFacing() const { return m_pBody->GetFacing(); }   // inline forwarder
    void SetLastActionHandedness(ECombatAnimationHandedness handedness);   // 0x006619F0
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

class CCombatActionBase { public:
    virtual void v00();
    long _pad04;
    CIntelligentPointer<CThingCreatureBase> Owner;              // +0x08 (the owning creature)
    unsigned char _pad10[0xa8 - 0x10];
    CIntelligentPointer<CThingCreatureBase> Target;             // +0xa8 (the action's target)
    unsigned char _padb0[0x114 - 0xb0];
    virtual void Initialise();                  // 0x00857520
public:
    void SetCombatAnimation(const CCharString&);   // 0x008574D0
};

class CCreatureAction_BodgeSideStrike : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_BodgeSideStrike::Initialise()
{
    CThingCreatureBase* target = Target.GetPItem();
    C3DVector delta = target->GetPosition_004c73d0() - Owner.GetPItem()->GetPosition_004c73d0();
    C3DVector facing = Owner.GetPItem()->GetFacing();
    facing.Z = 0.0f;
    float side = delta.X * facing.Y - delta.Y * facing.X;
    if (side > 0.0f)
        Owner.GetPItem()->SetLastActionHandedness(COMBAT_ANIM_RIGHT_HANDED);
    else
        Owner.GetPItem()->SetLastActionHandedness(COMBAT_ANIM_LEFT_HANDED);
    {
        CCharString name("STRIKE_MEDIUM_SIDE");
        SetCombatAnimation(name);
    }
    CCombatActionBase::Initialise();
}