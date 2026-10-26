// CCreatureAction_TeleportIn::Initialise() -- the combat-action Initialise, then (when the action creates effects) look up the creature definition's special-effects sub-def and, if it maps "TELEPORT_IN" to an effect type, spawn that effect at the creature's position
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B9F20, length 208 bytes (plain `ret`, virtual).
// Callees: CCombatActionBase::Initialise 0x00857520 (direct base call, first), CBaseIntelligentPointer::GetPItem 0x00A01B50
// (PCreature +8, twice), CDefClassBase::GetSubDef<CSpecialEffectsDef>(CDefPointer<CSpecialEffectsDef>&) 0x00669580 on the
// thing's definition (+0x70), CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0,
// CSpecialEffectsDef::GetEffectFromString(const CCharString&, long&) 0x00669D30 (the name's CRC looked up in the def's
// SpecialEffectsMap at +0x28; landed as CCRC::CalcAndLookup), CThing::GetPosition 0x004C73D0, __fastcall
// CreateEffectThing(long, const C3DVector*, bool) 0x006E0880.
// Layout: retail CreateEffects +0x114 (PDB header +0x12C: the debug base is 0x18 larger), so a local model.
// Shape: the GetSubDef out-slot is a plain counted-pointer slot (no destructor); the caller copies the def into a named
// pointer right after the call and releases it by hand at the end (`test edi,edi ; dec [edi+4] ; call [edx+4]`), which
// is what keeps it in edi across the string and lookup calls. The creature is a named local (so GetPItem runs before the
// argument push, and the base-relative &PCreature takes over esi). The lookup's string comes from the implicit
// const char* conversion inside a `found && ...` condition, so VC keeps a "temporary constructed" flag (zeroed at
// entry, set with the string) and destroys the string under it before branching on the condition saved in bl.
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

class CThing { public:
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

class CCreatureAction_TeleportIn : public CCombatActionBase { public:
    bool CreateEffects;                         // +0x114
    virtual void Initialise();
};

void CCreatureAction_TeleportIn::Initialise()
{
    CCombatActionBase::Initialise();
    if (CreateEffects) {
        CDefPointer<CSpecialEffectsDef> effectsSlot;
        long effectType = 0;
        CThing* creature = PCreature.GetPItem();
        CDefClassBase* def = creature->m_pDef;
        bool found = def->GetSubDef(effectsSlot);
        CSpecialEffectsDef* effects = effectsSlot.m_p;
        if (found && effects->GetEffectFromString("TELEPORT_IN", effectType))
            CreateEffectThing(effectType, &PCreature.GetPItem()->GetPosition(), false);
        ReleaseDef(effects);
    }
}