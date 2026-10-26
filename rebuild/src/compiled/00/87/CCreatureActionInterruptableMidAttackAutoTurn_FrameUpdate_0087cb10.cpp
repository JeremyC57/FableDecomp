// CCreatureAction_InterruptableMidAttackAutoTurn::FrameUpdate() -- once the strike has ended (first frame only): post the missed event when nothing was hit, and mark the melee flourish valid when the strike missed, the creature's 0x68 component's def allows it or the creature's stealth cannot flourish danger things; clear the combat component's +0x94 flag while a melee ability def is set; then the base combat frame update
// Leaf: ?FrameUpdate@CCreatureAction_InterruptableMidAttackAutoTurn
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087CB10, length 561 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: this's virtual slots 17 (+0x44) and 19 (+0x4c), CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//          CVectorMap::LowerBound @ 0x0040F020, operator new @ 0x00BFEA1A (the 8-byte event, vtable 0x01277874),
//          CTCEntityEvents::AddEvent @ 0x008113C0, CTCStealth::IsAbleToFlourishDangerThings @ 0x007471A0,
//          CTCCombat::SetMeleeFlourishValid(bool) @ 0x00733E10, CCombatActionBase::FrameUpdate @ 0x00859D80.
// Layout: Ego_r.pdb, retail offsets (CCreatureActionBase 0xa8, STLport list 4 / vector 12 bytes): CCombatActionBase
//   PMeleeAbilityDef +0xb8, HitCreature +0xe9; this class's PerformedStrikeEnd +0x128 (PDB +0x140). Def pointers are
//   intrusive (count +4, release through vtable slot 1).
// Shape: the 0x68 component's def is a by-value CDefPointer temporary inside an `&&` (its destruction flag is the frame
//   word zeroed on entry); the flourish flag is a bool initialised false at the top and set in the hit arm.
struct CEventBase {
    virtual void v00();
    long Data;                                               // +4
    CEventBase() : Data(0) { }
};
struct CEventMissedStrike : CEventBase {                     // vtable 0x01277874, 8 bytes
    virtual void v00();
};
class CTCEntityEvents { public:
    void AddEvent(CEventBase* event);                        // 0x008113C0
};
class CTCStealth { public:
    bool IsAbleToFlourishDangerThings();                     // 0x007471A0
};
class CTCCombat { public:
    unsigned char _pad00[0x94];
    bool          Flag94;                                    // +0x94
    void SetMeleeFlourishValid(bool valid);                  // 0x00733E10
};

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                                  // slot 1
    long IVRefCount;                                         // +4
};
class CWeaponDef : public CDefBase { public:
    unsigned char _pad08[0xe0 - 8];
    bool          AllowsFlourish;                            // +0xe0
};
template <class T> class CDefPointer { public:
    T* Object;
    __forceinline CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->IVRefCount; }
    __forceinline ~CDefPointer() { if (Object && --Object->IVRefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
    bool IsValid() const { return Object != 0; }
};
struct CTC68 {
    unsigned char           _pad00[0x58];
    CDefPointer<CWeaponDef> PDef;                            // +0x58
    CDefPointer<CWeaponDef> GetDef() const { return PDef; }
};

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                    // 0x0040F020
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                          // +0x20
    CInterfaceMap m_interfaces;                              // +0x44
    __forceinline bool HasTC(long interfaceType) const
    {
        return (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) != 0;
    }
    __forceinline CTCBase* FindTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            *ppOut = (T*)FindTC(interfaceType);
            return true;
        }
        return false;
    }
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
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16();
    virtual bool v17();                                      // slot 17 (+0x44)
    virtual void v18();
    virtual bool v19();                                      // slot 19 (+0x4c)
    virtual void FrameUpdate();                              // 0x00859D80
    unsigned char             _pad04[4];
    CIntelligentPointer<CThing>   PCreature;                     // +0x08
    unsigned char             _pad10[0xb8 - 0x10];
    CDefPointer<CWeaponDef>   PMeleeAbilityDef;              // +0xb8
    unsigned char             _padbc[0xe9 - 0xbc];
    bool                      HitCreature;                   // +0xe9
    unsigned char             _padea[0x114 - 0xea];
    CThing* GetCreature() const { return PCreature.GetPItem(); }
    CDefPointer<CWeaponDef> GetMeleeAbilityDef() const { return PMeleeAbilityDef; }
    bool HasMeleeAbilityDef() const { return GetMeleeAbilityDef().IsValid(); }
};
class CCreatureAction_InterruptableMidAttackAutoTurn : public CCombatActionBase { public:
    unsigned char _pad114[0x128 - 0x114];
    bool          PerformedStrikeEnd;                        // +0x128
    virtual void FrameUpdate();
};

#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* f = first;
    long count = last - first;
    while (count > 0) {
        long half = count >> 1;
        if (f[half].type < *type) { f += half + 1; count -= half + 1; } else count = half;
    }
    return f;
}
#pragma auto_inline(on)
void CCreatureAction_InterruptableMidAttackAutoTurn::FrameUpdate()
{
    if (v19() && v17() && !PerformedStrikeEnd) {
        PerformedStrikeEnd = true;
        bool flourish = false;
        bool valid;
        if (!HitCreature) {
            CTCEntityEvents* events;
            GetCreature()->GetInterface(&events, 0x42);
            events->AddEvent(new CEventMissedStrike);
            valid = true;
        } else {
            CTC68* tc68;
            if (GetCreature()->GetInterface(&tc68, 0x68) && tc68->GetDef()->AllowsFlourish)
                flourish = true;
            CTCStealth* stealth;
            valid = (GetCreature()->GetInterface(&stealth, 0x36) && !stealth->IsAbleToFlourishDangerThings()) || flourish;
        }
        CTCCombat* combat;
        if (valid && GetCreature()->GetInterface(&combat, 0x30))
            combat->SetMeleeFlourishValid(false);
        if (HasMeleeAbilityDef() && GetCreature()->GetInterface(&combat, 0x30))
            combat->Flag94 = false;
    }
    CCombatActionBase::FrameUpdate();
}