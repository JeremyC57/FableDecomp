// CCreatureAction_FodderPrimaryAttack::Initialise() -- a wasp attacking a live flagged wasp uses WASP_VS_WASP, anything else STRIKE_MEDIUM_FRONT; then the base Initialise
// Leaf: Initialise
// Grade: RELOCATION_MATCH -- retry wave 2 (sec4): the +0x91 dead test through CThing::IsDead() const (Ego_r.pdb
//   ?IsDead@CThing@@QBE_NXZ, a bool accessor) instead of a direct bit test; the bool accessor keeps retail's register
//   choice / test-and-branch shape.
//   before the GetPItem object call (retail calls GetPItem first); named def-class locals fix the order but add a
//   `mov ecx,eax` (282v278). Needs the GetSubDef stand-in (else frame 0x10 vs 4).
// Retail: Fable.exe (TLC, VC7.1) @ 0x008B7350, 278 bytes (288 with padding), virtual Initialise.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CDefClassBase::GetSubDef<CCreatureDef>(CDefPointer<const
//   CCreatureDef>*) 0x00485815 (on the thing's def at +0x70), CCharString(const char*, long) 0x0099EBF0 ("WASP_VS_WASP"
//   0x0127A590 / "STRIKE_MEDIUM_FRONT" 0x012778B0), CCombatActionBase::SetCombatAnimation 0x008574D0, ~CCharString
//   0x0099EAE0, CCombatActionBase::Initialise 0x00857520.
// Layout: PCreature +0x08 and the combat target pointer +0xa8 (retail CCreatureActionBase is 0xa8 bytes); CThing +0x6c
//   flag bit 3, +0x70 the thing's def, +0x91 dead bit; CCreatureDef +0xac the creature type (0x100 = wasp, by these
//   strings); the def's RefCount +0x04, Destroy in vtable slot 1.
// Shape: two CDefPointer locals (the creature's at function scope, the target's in the if block, sharing its frame
//   word with the animation string); the inner choice is one SetCombatAnimation on a ternary of two CCharString
//   if/else implicit conversions merged by VC (`push -1` hoisted above the branch); the def releases are the DecRefToZero form (the null tests
//   fold away after the dereference).
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);  // 0x0099EBF0
    ~CCharString();                                   // 0x0099EAE0
};
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
class CCreatureDef { public:
    virtual void v00();
    virtual void Destroy();                           // slot 1
    long RefCount;                                    // +0x04
    unsigned char _pad08[0xac - 0x08];
    long CreatureType;                                // +0xac (0x100: wasp)
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (DecRefToZero(Object->RefCount)) Object->Destroy(); } }
    T* operator->() const { return Object; }
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                if (--Object->RefCount == 0)
                    Object->Destroy();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->RefCount;
        }
        return *this;
    }
};
class CDefinitionManager { public:
    CDefPointer<CCreatureDef> GetPDefFromGlobalIndex(long index);   // 0x009ADA10 (hidden result pointer)
};
class CDefClassBase { public:
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c(); virtual void v10();
    virtual void v14(); virtual void v18(); virtual void v1c(); virtual void v20(); virtual void v24();
    virtual void v28(); virtual void v2c(); virtual void v30(); virtual void v34();
    virtual long* GetSubDefIndex(const CCharString& name);   // slot 14 (+0x38)
    unsigned char _pad04[0x1c - 4];
    CDefinitionManager* m_pManager;                   // +0x1c
    bool GetSubDef(CDefPointer<CCreatureDef>* out);   // 0x00485815
};
#pragma auto_inline(off)
// Same-TU stand-in for GetSubDef<CCreatureDef> 0x00485815 (the landed body, CDefClassBase_GetSubDef00485815): compiled
// above the caller so VC7.1 knows the def pointer it fills does not escape (retail's LTCG knowledge) -- the def stays
// in a register and its frame word is reused by the target's def and the animation string.
bool CDefClassBase::GetSubDef(CDefPointer<CCreatureDef>* out)
{
    long* pIndex;
    {
        CCharString name("CCreatureDef");
        pIndex = GetSubDefIndex(name);
    }
    if (pIndex) {
        CDefPointer<CCreatureDef> result = m_pManager->GetPDefFromGlobalIndex(*pIndex);
        *out = result;
        return true;
    }
    return false;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char _pad00[0x6c];
    unsigned char m_flags6c;                          // +0x6c bit 3
    unsigned char _pad6d[3];
    CDefClassBase* m_pDef;                            // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char m_flags91;                          // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags91 & 1) != 0; }   // Ego_r.pdb ?IsDead@CThing@@QBE_NXZ
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                  // +4
    CThing* GetPItem() const;                         // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}
class CCombatActionBase { public:                            // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0xa8 - 0x10];
    CBaseIntelligentPointer PTarget;                  // +0xa8
    unsigned char _padb0[0x114 - 0xb0];
    void SetCombatAnimation(const CCharString& name); // 0x008574D0 (Ego_r.pdb ?SetCombatAnimation@CCombatActionBase@@QAEXABVCCharString@@@Z)
    void Initialise();                                // 0x00857520
};
class CCreatureAction_FodderPrimaryAttack : public CCombatActionBase { public:
    void Initialise();
};

void CCreatureAction_FodderPrimaryAttack::Initialise()
{
    CDefPointer<CCreatureDef> creatureDef;
    CThing* creature = PCreature.GetPItem();
    creature->m_pDef->GetSubDef(&creatureDef);
    if (creatureDef->CreatureType == 0x100 && IS_THING_ALIVE(PTarget) && (PTarget.GetPItem()->m_flags6c & 8)) {
        CDefPointer<CCreatureDef> targetDef;
        CThing* target = PTarget.GetPItem();
        target->m_pDef->GetSubDef(&targetDef);
        if (targetDef->CreatureType == 0x100)
            SetCombatAnimation("WASP_VS_WASP");
        else
            SetCombatAnimation("STRIKE_MEDIUM_FRONT");
    } else
        SetCombatAnimation("STRIKE_MEDIUM_FRONT");
    CCombatActionBase::Initialise();
}