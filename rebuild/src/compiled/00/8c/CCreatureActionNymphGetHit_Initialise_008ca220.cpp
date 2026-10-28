// CCreatureAction_NymphGetHit::Initialise() -- reset the nymph's physics interpolation and disable its physics, record where its "DUMMY_CENTRE" helper is (its position when it has none), flag the CTCNymph as hit, then the base strike-response Initialise
// Leaf: ?Initialise@CCreatureAction_NymphGetHit@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x008CA220, 238 bytes, thiscall, plain `ret` (virtual).
// Identity (proven): RTTI vtable 0x01263F84 (.?AVCCreatureAction_NymphGetHit@@) slot 12 = Initialise (slot 12 of the
//   CCreatureActionBase family, e.g. CCombatActionBase::Initialise 0x00857520 in 0x01259E44); Ego_r.pdb
//   ?Initialise@CCreatureAction_NymphGetHit@@UAEXXZ (donor 0x00901910, between NymphTeleportOutOfLimbo::Initialise 0x00901840
//   = retail 0x008CA170 and NymphGetHit::PerformAction 0x00901A20 = retail 0x008CA310, landed). Literal 0x0126F110 "DUMMY_CENTRE".
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CTCPhysicsBase::ResetInterpolation (virtual slot 80, +0x140; landed
//   0x006AF790) and EnablePhysics(bool) (virtual slot 36, +0x90: CTCPhysicsStandard's slot-36 override 0x00723630 stores
//   PhysicsEnabled = Ego_r ?EnablePhysics@CTCPhysicsStandard@@UAEX_N@Z; the base slot is an empty `ret 4`), CCharString
//   ctor/dtor 0x0099EBF0 / 0x0099EAE0, NGraphicAppearance::CTCGraphicAppearance::GetDummyObject 0x004C08C0, CThing::GetPos
//   0x004C73D0, CVectorMap LowerBound 0x0040F020 (inline GetInterface 0xF5 = CTCNymph), CCombatAction_GenericStrikeResponseBase::
//   Initialise 0x00865F40.
// Shape: `if (!GetDummyObject(...))` with the helper-name temporary gives retail's `neg ; sbb ; inc` bool kept across the
//   string's destructor; the Ego_r inline accessors GetPhysicsTC() / GetTCGraphicAppearance() (as 0x008C9E60); the CTCNymph
//   out-pointer GetInterface of the landed sibling 0x008CA4A0 (component left in eax).
// Layout: InitDummyPos +0x148 (engine/CCreatureAction_NymphGetHit.h); CTCNymph BeenHit +0x5D (Ego_r +0x61: the
//   CCardinalSpline's CArray is 4 bytes shorter in retail).
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

class CRightHandedSet {
public:
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

#define V10(n) virtual void v##n##0(); virtual void v##n##1(); virtual void v##n##2(); virtual void v##n##3(); \
    virtual void v##n##4(); virtual void v##n##5(); virtual void v##n##6(); virtual void v##n##7(); virtual void v##n##8(); \
    virtual void v##n##9();
class CTCPhysicsBase {
public:
    V10(0) V10(1) V10(2)                                     // slots 0..29
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void EnablePhysics(bool enable);                 // slot 36 (+0x90)
    virtual void v37(); virtual void v38(); virtual void v39();
    V10(4) V10(5) V10(6) V10(7)                              // slots 40..79
    virtual void ResetInterpolation();                       // slot 80 (+0x140)
};
#undef V10

class CTCNymph {
public:
    unsigned char _pad_0x00[0x5d];
    bool BeenHit;                                            // +0x5D
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
    NGraphicAppearance::CTCGraphicAppearance* PTCGraphicAppearance;   // +0x64
    const C3DVector& GetPos() const;                         // 0x004C73D0
    CTCPhysicsBase* GetPhysicsTC() { return PhysicsTC; }
    NGraphicAppearance::CTCGraphicAppearance* GetTCGraphicAppearance() { return PTCGraphicAppearance; }
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
    unsigned char _pad_0x10[0xa8 - 0x10];
};

class CCombatAction_GenericStrikeResponseBase : public CCreatureActionBase {
public:
    virtual void Initialise();                               // 0x00865F40
    unsigned char _pad_0xa8[0x148 - 0xa8];
};

class CCreatureAction_NymphGetHit : public CCombatAction_GenericStrikeResponseBase {
public:
    C3DVector InitDummyPos;                                  // +0x148
    virtual void Initialise();
};

void CCreatureAction_NymphGetHit::Initialise()
{
    PCreature->GetPhysicsTC()->ResetInterpolation();
    PCreature->GetPhysicsTC()->EnablePhysics(false);
    CRightHandedSet dummy_set;
    if (!PCreature->GetTCGraphicAppearance()->GetDummyObject("DUMMY_CENTRE", 0, 0, InitDummyPos, dummy_set, 1.0f))
        InitDummyPos = PCreature->GetPos();
    CTCNymph* nymph;
    if (PCreature->GetInterface(&nymph, 0xF5))
        nymph->BeenHit = true;
    CCombatAction_GenericStrikeResponseBase::Initialise();
}