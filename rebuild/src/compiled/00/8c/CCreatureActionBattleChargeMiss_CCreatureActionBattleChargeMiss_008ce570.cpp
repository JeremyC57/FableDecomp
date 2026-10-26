// CCreatureAction_BattleChargeMiss::CCreatureAction_BattleChargeMiss(CThingCreatureBase*, CIntelligentPointer<CThing>, CIntelligentPointer<CThing>) -- base CCombatActionBase(owner, 145, zeroed sound), copy the two by-value thing pointers into the members, animation "ST_BATTLE_CHARGE_MISS"
// Leaf: ??0CCreatureAction_BattleChargeMiss
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008CE570, length 182 bytes (+ int3 padding), `ret 14h` (owner + two 8-byte by-value pointers).
// Vtable 0x012847c4 (own class); CIntelligentPointer<CThing> vtable 0x01238C6C.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) 0x00858030, CBaseIntelligentPointer
// ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10, CCharString ctor 0x0099EBF0 / dtor 0x0099EAE0,
// SetAnimationName 0x008574D0 (manifest label "CTCInGameMenu::SetPDef" is propagated; named as in the landed 0x008CE630 family).
// Shape: the two members are copy-constructed from the by-value parameters (inline copy ctor: base ctor, derived vtable, the
// source's GetPItem, SetPItem); the scoped named animation string; the by-value parameters are destroyed by the callee at the end.
// Layout: retail CCombatActionBase is 0x114 bytes (the PDB one is 0xc larger), so the two pointers sit at +0x114 / +0x11c.
class CThingCreatureBase;
class CBaseObject;

class CCharString { public:                        // engine string (4 bytes: data pointer)
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

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

class CThing;
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
public:
    void SetCombatAnimation(const CCharString&);   // 0x008574D0
};

class CCreatureAction_BattleChargeMiss : public CCombatActionBase { public:   // vtable 0x012847c4
    CIntelligentPointer<CThing> Left;    // +0x114 (PDB +0x12c)
    CIntelligentPointer<CThing> Right;   // +0x11c (PDB +0x134)
    virtual void v00();
    CCreatureAction_BattleChargeMiss(CThingCreatureBase* owner, CIntelligentPointer<CThing> left, CIntelligentPointer<CThing> right);
};

CCreatureAction_BattleChargeMiss::CCreatureAction_BattleChargeMiss(CThingCreatureBase* owner, CIntelligentPointer<CThing> left, CIntelligentPointer<CThing> right)
    : CCombatActionBase(owner, 145, CSoundPair())
    , Left(left)
    , Right(right)
{
    {
        CCharString name("ST_BATTLE_CHARGE_MISS");
        SetCombatAnimation(name);
    }
}