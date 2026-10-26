// CCreatureAction_UndeadReturnToEarth::Initialise() -- the undead sinks back into the ground: spawn the creature definition's "SINK" special effect at its position (when its special-effects sub-def maps that name), suspend its animation, then the combat-action Initialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EC320, length 208 bytes (plain `ret`, virtual).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (PCreature +8, three times), CDefClassBase::GetSubDef<
// CSpecialEffectsDef>(CDefPointer<CSpecialEffectsDef>&) 0x00669580 on the thing's definition (+0x70), CCharString(const char*,
// long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CSpecialEffectsDef::GetEffectFromString(const CCharString&, long&) 0x00669D30
// (landed as CCRC::CalcAndLookup), CThing::GetPosition 0x004C73D0, __fastcall CreateEffectThing(long, const C3DVector*, bool)
// 0x006E0880, CThingAICreature::SetSuspendAnimation(bool, long) 0x00833A10, CCombatActionBase::Initialise 0x00857520 (direct
// base call, after the effect).
// Shape: the TeleportIn/TeleportOut lookup (0x008B9F20) without the CreateEffects gate: the def from the plain out-slot is
// copied into a named pointer (ebp) and released by hand at the very end; the creature is a named local; the `found && ...`
// condition with the implicitly converted string temporary keeps its construction flag in ebx (zeroed at entry) and the
// condition in a frame byte. The suspend call goes through a second named creature local so GetPItem runs before the
// two constant arguments are pushed.
class C3DVector { public: float X, Y, Z; };

class CCharString { public:
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
class CSpecialEffectsDef : public CDefBase { public:
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
template <class T> class CDefPointer { public:      // the out-slot GetSubDef fills (counted reference handed over)
    T* m_p;
    CDefPointer() : m_p(0) { }
};
template <class T> __forceinline void ReleaseDef(T* def)
{
    if (def && --def->m_refCount == 0)
        def->Destroy();
}
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};

class CThingAICreature { public:
public:
    void SuspendAI(bool, void*);   // 0x00833A10
};

class CThing : public CThingAICreature { public:
    unsigned char  _pad0[0x70];
    CDefClassBase* m_pDef;                      // +0x70
    const C3DVector& GetPosition() const;       // 0x004C73D0
};

CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

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
    virtual void Initialise();                  // 0x00857520
    unsigned char           _pad04[4];
    CIntelligentPointer<CThing> PCreature;          // +0x08
    unsigned char           _pad10[0x114 - 0x10];
};

class CCreatureAction_UndeadReturnToEarth : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_UndeadReturnToEarth::Initialise()
{
    CDefPointer<CSpecialEffectsDef> effectsSlot;
    long effectType = 0;
    CThing* creature = PCreature.GetPItem();
    CDefClassBase* def = creature->m_pDef;
    bool found = def->GetSubDef(effectsSlot);
    CSpecialEffectsDef* effects = effectsSlot.m_p;
    if (found && effects->GetEffectFromString("SINK", effectType))
        CreateEffectThing(effectType, &PCreature.GetPItem()->GetPosition(), false);
    CThing* sinking = PCreature.GetPItem();
    sinking->SuspendAI(true, 0);
    CCombatActionBase::Initialise();
    ReleaseDef(effects);
}