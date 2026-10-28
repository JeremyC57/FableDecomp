// CCreatureAction_NymphTeleportOutOfLimbo::Initialise() -- play the teleport effect, put the nymph at NewPos (reset and force-move there), name the "TELEPORT_OUTOF_LIMBO" animation, run the combat Initialise, then stop the CTCNymph flying around for the action's TotalTime
// Leaf: ?Initialise@CCreatureAction_NymphTeleportOutOfLimbo@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008CA170, 176 bytes, thiscall, plain `ret` (virtual).
// Identity (proven): RTTI vtable 0x01282A04 (.?AVCCreatureAction_NymphTeleportOutOfLimbo@@) slot 12 = Initialise; Ego_r.pdb
//   ?Initialise@CCreatureAction_NymphTeleportOutOfLimbo@@UAEXXZ (donor 0x00901840, after NymphTeleportIntoLimbo::PerformAction
//   0x009017D0 = retail 0x008CA110 and before NymphGetHit::Initialise 0x00901910 = retail 0x008CA220). Literal 0x012821E0
//   "TELEPORT_OUTOF_LIMBO".
// Callees: CCreatureAction_NymphTeleportLimboBase::DoEffect() 0x008C9FE0 (Ego_r: protected, IAE), CBaseIntelligentPointer::
//   GetPItem 0x00A01B50, CTCPhysicsBase::ResetPosition(const C3DVector&) (virtual slot 31, +0x7C; Ego_r CTCPhysicsBase /
//   CTCPhysicsNavigator) and SetForcedToMovePos(const C3DVector&, bool) (virtual slot 34, +0x88; the CTCPhysicsControlled /
//   CTCPhysicsNavigator slot-34 overrides are Ego_r SetForcedToMovePos), CCharString ctor/dtor 0x0099EBF0 / 0x0099EAE0,
//   CCombatActionBase::SetCombatAnimation(const CCharString&) 0x008574D0 (the only CCombatActionBase member of that
//   signature in Ego_r), CCombatActionBase::Initialise 0x00857520, CVectorMap LowerBound 0x0040F020 (inline GetInterface
//   0xF5 = CTCNymph), CTCNymph::StopFlyingAround(long) 0x0080F870.
// Shape: the NymphGetHit::Initialise model (0x008CA220): Ego_r inline GetPhysicsTC(), the named animation string in its own
//   block, the out-pointer GetInterface.
// Layout: NewPos +0x114 (Ego_r +0x12C; retail CCombatActionBase is 0x114 bytes); CCreatureActionBase TotalTime +0x10.
class CTCBase;

class CCharString {
public:
    CCharString(const char* text, long length = -1);         // 0x0099EBF0
    ~CCharString();                                          // 0x0099EAE0
    void* PStringData;
};

class C3DVector {
public:
    float X, Y, Z;
};

#define V10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); virtual void v##n##8(); \
    virtual void v##n##9();
class CTCPhysicsBase {
public:
    V10(0) V10(1) V10(2)                                     // slots 0..29
    virtual void v30();
    virtual void ResetPosition(const C3DVector& pos);        // slot 31 (+0x7C)
    virtual void v32(); virtual void v33();
    virtual void SetForcedToMovePos(const C3DVector& pos, bool forced);   // slot 34 (+0x88)
    virtual void v35();
    virtual void EnablePhysics(bool enable);                 // slot 36 (+0x90)
    virtual void v37(); virtual void v38(); virtual void v39();
    V10(4) V10(5) V10(6) V10(7)                              // slots 40..79
    virtual void ResetInterpolation();                       // slot 80 (+0x140)
};
#undef V10

class CTCNymph {
public:
    void StopFlyingAround(long frames);                      // 0x0080F870
};

struct CInterfaceMap {                                       // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
};

class CThing {
public:
    virtual ~CThing();
    unsigned char _pad_0x04[0x20 - 0x04];
    unsigned long UsingComponents[9];                        // +0x20
    CInterfaceMap Components;                                // +0x44
    unsigned char _pad_0x4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                               // +0x60
    const C3DVector& GetPos() const;                         // 0x004C73D0
    CTCPhysicsBase* GetPhysicsTC() { return PhysicsTC; }
    template <class T> __forceinline bool GetInterface(T** out, long type)
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
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
    long TotalTime;                                          // +0x10
    unsigned char _pad_0x14[0xa8 - 0x14];
};

class CCombatActionBase : public CCreatureActionBase {
public:
    virtual void Initialise();                               // 0x00857520
    void SetCombatAnimation(const CCharString& name);        // 0x008574D0
    unsigned char _pad_0xa8[0x114 - 0xa8];
};

class CCreatureAction_NymphTeleportLimboBase : public CCombatActionBase {
protected:
    void DoEffect();                                         // 0x008C9FE0
};

class CCreatureAction_NymphTeleportOutOfLimbo : public CCreatureAction_NymphTeleportLimboBase {
public:
    C3DVector NewPos;                                        // +0x114 (PDB +0x12C)
    virtual void Initialise();
};

void CCreatureAction_NymphTeleportOutOfLimbo::Initialise()
{
    DoEffect();
    PCreature->GetPhysicsTC()->ResetPosition(NewPos);
    PCreature->GetPhysicsTC()->SetForcedToMovePos(NewPos, false);
    {
        CCharString name("TELEPORT_OUTOF_LIMBO");
        SetCombatAnimation(name);
    }
    CCombatActionBase::Initialise();
    CTCNymph* nymph;
    if (PCreature->GetInterface(&nymph, 0xF5))
        nymph->StopFlyingAround(TotalTime);
}