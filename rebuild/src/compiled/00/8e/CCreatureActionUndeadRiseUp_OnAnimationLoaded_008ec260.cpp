// CCreatureAction_UndeadRiseUp::OnAnimationLoaded() -- once the rise animation is loaded, spawn the creature definition's "RISE" special effect at the undead's position (when its special-effects sub-def maps that name), then the combat-action OnAnimationLoaded
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EC260, length 192 bytes (plain `ret`, virtual).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +8, twice), CDefClassBase::GetSubDef<
// CSpecialEffectsDef>(CDefPointer<CSpecialEffectsDef>&) 0x00669580 on the thing's definition (+0x70), CCharString(const char*,
// long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CSpecialEffectsDef::GetEffectFromString(const CCharString&, long&) 0x00669D30
// (landed as CCRC::CalcAndLookup), CThing::GetPosition 0x004C73D0, __fastcall CreateEffectThing(long, const C3DVector*, bool)
// 0x006E0880, CCombatActionBase::OnAnimationLoaded 0x00859EF0 (direct base
// call, after the effect).
// Shape: the TeleportIn/TeleportOut lookup (0x008B9F20) without the CreateEffects gate: the def from the plain out-slot is
// copied into a named pointer (ebp) and released by hand at the very end; the creature is a named local; the `found && ...`
// condition with the implicitly converted string temporary keeps its construction flag in ebx (zeroed at entry) and the
// condition in a frame byte. GetEffectFromString is a same-TU noinline stand-in (retry B's lever, FIXES.md) whose body
// only writes the effect type. VC7.1 then knows the effect-type word's address does not escape the call, which gives
// retail's registers (`this` in edi, &PCreature in esi) and keeps `push 0` ahead of the GetPItem/GetPosition calls.
struct C3DVector { float X, Y, Z; };

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                            // +0x04
};
struct CSpecialEffectsDef : CDefBase {
    bool GetEffectFromString(const CCharString& name, long& type) const;
};
long Find_CSpecialEffectsDef_GetEffectFromString(const CSpecialEffectsDef* def, const CCharString& name);   // 0x00669D30 body
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& type) const   // 0x00669D30 stand-in
{
    long found = Find_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    type = found;
    return true;
}
#pragma auto_inline(on)
template <class T> struct CDefPointer {      // the out-slot GetSubDef fills (counted reference handed over)
    T* m_p;
    CDefPointer() : m_p(0) { }
};
template <class T> __forceinline void ReleaseDef(T* def)
{
    if (def && --def->m_refCount == 0)
        def->Destroy();
}
struct CDefClassBase : CDefBase {
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};

struct CThingAnimatable {
    void SetSuspendAnimation(bool suspend, long reason);   // 0x00833A10
};

struct CThing : CThingAnimatable {
    unsigned char  _pad0[0x70];
    CDefClassBase* m_pDef;                      // +0x70
    const C3DVector& GetPosition() const;       // 0x004C73D0
};

CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

struct CBaseIntelligentPointer {
    void*   __vftable;
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

struct CCombatActionBase {
    virtual void v00();
    virtual void Initialise();                  // 0x00857520
    void OnAnimationLoaded();                   // 0x00859EF0
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0x114 - 0x10];
};

struct CCreatureAction_UndeadRiseUp : CCombatActionBase {
    void OnAnimationLoaded();
};

void CCreatureAction_UndeadRiseUp::OnAnimationLoaded()
{
    CDefPointer<CSpecialEffectsDef> effectsSlot;
    long effectType = 0;
    CThing* creature = PCreature.GetPItem();
    CDefClassBase* def = creature->m_pDef;
    bool found = def->GetSubDef(effectsSlot);
    CSpecialEffectsDef* effects = effectsSlot.m_p;
    if (found && effects->GetEffectFromString("RISE", effectType))
        CreateEffectThing(effectType, &PCreature.GetPItem()->GetPosition(), false);
    CCombatActionBase::OnAnimationLoaded();
    ReleaseDef(effects);
}